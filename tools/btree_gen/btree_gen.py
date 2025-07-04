import os
import sys
import yaml
import argparse

def emit_tree_structure(root_label, nodes):
    def walk(label, prefix="", is_last=True):
        lines = []
        node_type = nodes[label]["type"]
        connector = "└── " if is_last else "├── "
        lines.append(f"{prefix}{connector}[{node_type}] {label}")
        node = nodes.get(label, {})
        children = node.get("children", [])

        for i, child in enumerate(children):
            is_last_child = (i == len(children) - 1)
            child_prefix = prefix + ("    " if is_last else "│   ")
            lines.extend(walk(child, child_prefix, is_last_child))
        return lines

    result = []
    node_type = nodes[root_label]["type"]
    result.append(f"[{node_type}] {root_label}")
    for i, child in enumerate(nodes[root_label].get("children", [])):
        is_last = (i == len(nodes[root_label]["children"]) - 1)
        result.extend([line for line in walk(child, "", is_last)])
    return result

def load_btree_yaml(filepath):
    with open(filepath, 'r', encoding='utf-8') as file:
        try:
            return yaml.safe_load(file)
        except yaml.YAMLError as e:
            print("YAML parse error:", e)
            sys.exit(1)

def sanitize_label(label):
    return label.replace("/", "_")

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Generate C code for behavior tree.')
    parser.add_argument('scriptfile', help='YAML file describing the behavior tree')
    parser.add_argument('-d', '--dir', help='Output directory')
    args = parser.parse_args()

    if args.dir:
        os.makedirs(args.dir, exist_ok=True)

    filename = os.path.splitext(os.path.basename(args.scriptfile))[0]
    data = load_btree_yaml(args.scriptfile)
    btree = data["btree"]

    label_to_index = {}
    index_to_label = {}
    flat_nodes = []
    index = 0

    def flatten_tree(label, subtree, parent_idx):
        global index
        current_idx = index
        label_to_index[label] = current_idx
        index_to_label[current_idx] = label
        flat_nodes.append((label, subtree["type"], subtree, parent_idx, subtree["description"]))
        index += 1
        if subtree["type"] in ("selector", "sequencer"):
            for child in subtree["children"]:
                flatten_tree(child, btree[child], current_idx)

    flatten_tree("root", btree["root"], -1)

    node_defs = []
    leaf_defs = []
    parent_table = []

    for idx, (label, node_type, node_data, parent_idx, description) in enumerate(flat_nodes):
        if parent_idx == -1:
            parent_table.append("NULL")
        else:
            parent_label = index_to_label[parent_idx]
            parent_table.append(f"&node_{parent_idx}_{parent_label}")
        if node_type == "leaf":
            leaf_id = node_data["id"]
            leaf_defs.append(
                f'// {label}\n'
                f'// {description}\n'
                f'static const mgc_btree_leaf_t leaf_{idx}_{label} = {{ .id = "{leaf_id}" }};'
            )
            node_defs.append(
                f'// {label}\n'
                f'// {description}\n'
                f'static const mgc_btree_node_t node_{idx}_{label} = {{\n'
                f'    .tag = {idx},\n'
                f'    .parent_index = {idx},\n'
                f'    .next_sibling = NULL,\n'
                f'    .type = MGC_BTREE_NODE_TYPE_LEAF,\n'
                f'    .body.leaf = &leaf_{idx}_{label}\n'
                f'}};'
            )
        else:
            first_child_idx = label_to_index[node_data["children"][0]]
            first_child_label =  node_data["children"][0]
            node_defs.append(
                f'// {label}\n'
                f'// {description}\n'
                f'static const mgc_btree_node_t node_{idx}_{label} = {{\n'
                f'    .tag = {idx},\n'
                f'    .parent_index = {idx},\n'
                f'    .next_sibling = NULL,\n'
                f'    .type = MGC_BTREE_NODE_TYPE_{"SELECTOR" if node_type == "selector" else "SEQUENCER"},\n'
                f'    .body.first_child = &node_{first_child_idx}_{first_child_label}\n'
                f'}};'
            )

    siblings_map = {}
    for idx, (_, _, node_data, parent_idx, description) in enumerate(flat_nodes):
        if parent_idx == -1:
            continue
        siblings_map.setdefault(parent_idx, []).append(idx)

    for siblings in siblings_map.values():
        for i in range(len(siblings) - 1):
            src = siblings[i]
            dst = siblings[i + 1]
            label = index_to_label[dst]
            node_defs[src] = node_defs[src].replace(
                ".next_sibling = NULL",
                f".next_sibling = &node_{dst}_{label}"
            )

    h_path = os.path.join(args.dir or "", f"{filename}.h")
    c_path = os.path.join(args.dir or "", f"{filename}.c")

    with open(h_path, "w", encoding="utf-8") as f:
        guard = f"{filename.upper()}_H"
        f.write(f"#ifndef {guard}\n#define {guard}\n\n")
        f.write("#ifdef __cplusplus\n")
        f.write('extern "C" {\n')
        f.write("#endif/*__cplusplus*/\n\n")
        f.write('#include "mgc/sequencer/btree.h"\n')
        
        tree_comment = emit_tree_structure("root", btree)
        f.write("\n")
        f.write("// Behavior Tree Structure:\n")
        for line in tree_comment:
            f.write(f"// {line}\n")
        f.write("\n")
        
        f.write(f"extern const mgc_btree_t {filename};\n\n")
        f.write("#ifdef __cplusplus\n}\n#endif/*__cplusplus*/\n\n")
        f.write(f"#endif /* {guard} */\n")

    with open(c_path, "w", encoding="utf-8") as f:
        f.write(f'#include "{filename}.h"\n\n')

        for line in leaf_defs:
            f.write(line + "\n\n")
        for line in reversed(node_defs):
            f.write(line + "\n\n")
        f.write("static const mgc_btree_node_t* parent_table[] = {\n")
        for parent in parent_table:
            f.write(f"    {parent},\n")
        f.write("};\n\n")
        f.write(f"const mgc_btree_t {filename} = {{\n")
        f.write("    .root = &node_0_root,\n")
        f.write("    .parents = parent_table,\n")
        f.write(f"    .parents_count = {len(parent_table)}\n")
        f.write("};\n")
