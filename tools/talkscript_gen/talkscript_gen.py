import os
import sys
import yaml
import argparse

def load_talkscript(filepath):
    with open(filepath, 'r', encoding='utf-8') as file:
        try:
            data = yaml.safe_load(file)
            return data
        except yaml.YAMLError as e:
            print("YAML parse error:", e)
            sys.exit(1)

if __name__ == '__main__':

    parser = argparse.ArgumentParser(description='''\
    This script reads a talkscript.yaml file and generates corresponding C source code.
    It converts the dialogue data into C structure definitions for use with mgc_talkflow.
    ''', formatter_class=argparse.RawTextHelpFormatter)

    parser.add_argument('scriptfile', help='Set the path to the talk-script.yaml.')
    parser.add_argument('-d', '--dir',  help='Set the output directory.')
    args = parser.parse_args()

    if args.dir:
        os.makedirs(args.dir, exist_ok=True)

    filename = os.path.basename(args.scriptfile).split('.')[0]
    scriptname = filename

    data = load_talkscript(args.scriptfile);

    talkscript = data["talkscript"]
    subject = talkscript["subject"]
    description = talkscript["description"]
    nodes = talkscript["talknodes"]

    node_list = []
    constants = []
    node_index = 0
    label_macros = {}
    c_lines = []

    #### h_file
    h_file_path = scriptname + '.h';
    if args.dir:
        h_file_path = args.dir + '/' + h_file_path

    with open(h_file_path, "w", encoding="utf-8") as f:
        if subject:
            f.write('/*\n')
            f.write(f'Subject: {subject}\n')
            f.write(' */\n')

        if description:
            f.write('/*\n')
            f.write(f'Description: {description}\n')
            f.write(' */\n')

        include_guard = '{}_H'.format(scriptname.upper())
        entity_name = scriptname

        # Start to include guard 
        f.write('#ifndef {}\n'.format(include_guard))
        f.write('#define {}\n'.format(include_guard))
        f.write('\n')

        # Start to extern "C"
        f.write('#ifdef __cplusplus\n')
        f.write('extern "C" {\n')
        f.write('#endif\n')
        f.write('\n')

        # Include statement
        f.write('#include "mgc/sequencer/talkscript.h"\n')
        f.write('\n')

        # Define macros
        for label, node_group in nodes.items():
            label_macro = f"MGC_{scriptname.upper()}_{label.upper()}"
            label_macros[label] = label_macro
            f.write(f"// {label}\n")
            f.write(f"#define {label_macro} {node_index}\n")
            for entry in node_group:
                if "message" in entry:
                    if "tag" in entry:
                        message_tag_value = entry['tag']
                        f.write(f"#define MGC_{scriptname.upper()}_TAG_MESSAGE__{message_tag_value} {node_index}\n")

                elif "choice" in entry:
                    choice = entry["choice"]
                    choice_tag_value = f"{node_index}"
                    if "tag" in choice:
                        choice_tag_value = choice['tag']

                    f.write(f"#define MGC_{scriptname.upper()}_TAG_CHOICE__{choice_tag_value} {node_index}\n")

                    if "items" in choice:
                        item_index = 0
                        for item in choice["items"]:
                            if "tag" in item:
                                f.write(f"#define MGC_{scriptname.upper()}_TAG_CHOICE__{choice_tag_value}__ITEM__{item['tag']} {item_index}\n")
                            item_index += 1
                elif "decision" in entry:
                    decision = entry["decision"]
                    if "tag" in decision:
                        decision_tag_value = decision['tag']
                        f.write(f"#define MGC_{scriptname.upper()}_TAG_DECISION__{decision_tag_value} {node_index}\n")

                node_index += 1

        f.write('\n')
        f.write('\n')

        # Extern tile instance
        f.write('extern const mgc_talkscript_t {};\n'.format(entity_name))
        f.write('\n')

        # End of extern "C"
        f.write('#ifdef __cplusplus\n')
        f.write('}/*extern "C"*/\n')
        f.write('#endif\n')
        f.write('\n')

        # End of include guard 
        f.write('#endif/*{}*/\n'.format(include_guard))

    #### c_file
    node_index = 0
    for label, node_group in nodes.items():
        node_list.append(f"// {label}")
        for entry in node_group:
            if "message" in entry:
                text = entry["message"]
                format_flag = entry.get("format", False)
                next_index = node_index + 1
                end = entry.get("end", False)
                if "next" in entry:
                    next_index = label_macros[entry["next"]]
                c_lines.append(f'static const mgc_node_message_t node_{node_index}_message = {{')
                c_lines.append(f'    .text = "{text}",')
                c_lines.append(f'    .format = {"true" if format_flag else "false"},')
                c_lines.append(f'    .next = {next_index}')
                c_lines.append('};\n')
                node_list.append(f'{{ .content.message = &node_{node_index}_message, .type = MGC_TALKNODE_TYPE_MESSAGE, .end = {"true" if end else "false"} }},')
            elif "choice" in entry:
                items = entry["choice"]["items"]
                choice_tag = entry["choice"].get("tag")
                item_lines = []
                for i, item in enumerate(items):
                    text = item["item"]
                    value = item.get("value", 0)
                    next_label = item.get("next")
                    next_idx = label_macros[next_label] if next_label else node_index + 1
                    item_lines.append(f'    [{i}] = {{ .text = "{text}", .value = {value}, .next = {next_idx} }},')
                c_lines.append(f'static const mgc_node_choice_item_t node_{node_index}_items[{len(items)}] = {{')
                c_lines.extend(item_lines)
                c_lines.append('};')
                c_lines.append(f'static const mgc_node_choice_t node_{node_index}_choice = {{')
                c_lines.append(f'    .items = node_{node_index}_items,')
                c_lines.append(f'    .item_count = {len(items)}')
                c_lines.append('};\n')
                node_list.append(f'{{ .content.choice = &node_{node_index}_choice, .type = MGC_TALKNODE_TYPE_CHOICE, .end = false }},')
            elif "decision" in entry:
                branches = entry["decision"]["branches"]
                next_if_true = node_index + 1
                next_if_false = next_if_true
                if True in branches:
                    next_if_true = label_macros[branches[True]]
                if False in branches:
                    next_if_false = label_macros[branches[False]]

                c_lines.append(f'static const mgc_node_decision_t node_{node_index}_decision = {{')
                c_lines.append(f'    .next_if_true = {next_if_true},')
                c_lines.append(f'    .next_if_false = {next_if_false}')
                c_lines.append('};\n')
                node_list.append(f'{{ .content.decision = &node_{node_index}_decision, .type = MGC_TALKNODE_TYPE_DECISION, .end = false }},')
            else:
                pass

            node_index += 1

    c_lines.append('static const mgc_talknode_t nodes[] = {')
    c_lines.extend(['    ' + n for n in node_list])
    c_lines.append('};\n')

    c_lines.append(f'const mgc_talkscript_t {scriptname} = {{')
    c_lines.append('    .nodes = nodes,')
    c_lines.append('    .node_count = countof(nodes)')
    c_lines.append('};')

    c_file_path = scriptname + '.c';
    if args.dir:
        c_file_path = args.dir + '/' + c_file_path


    with open(c_file_path, "w", encoding="utf-8") as f:
        f.write(f"#include \"{scriptname}.h\"\n\n")
        f.write('\n'.join(c_lines))


