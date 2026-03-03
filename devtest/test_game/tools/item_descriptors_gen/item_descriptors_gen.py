import os
import sys
import yaml
import argparse
from PIL import Image

def load_yaml(path):
    with open(path, "r", encoding="utf-8") as f:
        return yaml.safe_load(f)

def generate_c_source(yaml_data, yaml_base, output_dir):

    combined_c_path = os.path.join(output_dir, f'{yaml_base}.c')
    tileset_path = yaml_data['item_descriptors']['tileset_path']
    tileset_base = os.path.splitext(os.path.basename(tileset_path))[0]

    with open(combined_c_path, "w") as f:
        f.write(f'#include \"{yaml_base}.h\"\n')
        f.write(f'#include \"{tileset_path}\"\n')
        f.write('\n')
        f.write(f'static const item_descriptor_t descs[] = {{\n')

        for desc in yaml_data['item_descriptors']['descriptors']:
            name = desc['name']
            description = desc['description']
            tile_index = desc['tile_index']
            f.write(f'    {{"{name}", "{description}", {tile_index}}},\n')
            
        f.write('};\n')
        f.write('\n')

        title = yaml_data['item_descriptors']['title']


        f.write(f'const item_descriptors_t {yaml_base} = {{\n')
        f.write(f'    .title = "{title}",\n')
        f.write(f'    .tileset = &{tileset_base},\n')
        f.write(f'    .descriptors = descs,\n')
        f.write(f'    .descriptor_count = sizeof(descs)/sizeof(descs[0])\n')
        f.write('};\n')
        f.write('\n')

def generate_c_header(yaml_base, output_dir):
    
    combined_c_path = os.path.join(output_dir, f'{yaml_base}.h')
    include_guard = f'MGC_{yaml_base.upper()}_H'

    with open(combined_c_path, "w") as f:
        f.write(f'#ifndef {include_guard}\n')
        f.write(f'#define {include_guard}\n')
        f.write('#ifdef __cplusplus\n')
        f.write('extern "C" {\n')
        f.write('#endif\n')

        f.write('\n')
        f.write('#include \"menu/item_descriptors.h\"\n')

        f.write('\n')
        f.write(f'extern const item_descriptors_t {yaml_base};\n')
        f.write('\n')

        f.write('#ifdef __cplusplus\n')
        f.write('}\n')
        f.write('#endif\n')
        f.write(f'#endif/*{include_guard}*/\n')

def generate_cpp_header(yaml_data, yaml_base, output_dir):
    combined_c_path = os.path.join(output_dir, f'{yaml_base}.hpp')
    include_guard = f'MGC_{yaml_base.upper()}_HPP'
    id_group_name = yaml_data['item_descriptors']['id_group_name']

    with open(combined_c_path, "w") as f:
        f.write(f'#ifndef {include_guard}\n')
        f.write(f'#define {include_guard}\n')
        f.write('\n')

        f.write(f'enum class {id_group_name} : uint32_t {{\n')
        first_flag = True
        for desc in yaml_data['item_descriptors']['descriptors']:
            id_name = desc['id']
            if ( first_flag ):
                first_flag = False
                f.write(f'    {id_name} = 0,\n')
            else:
                f.write(f'    {id_name},\n')

        
        f.write(f'    Count,\n')
        f.write(f'    None\n')
        f.write('};\n')
        f.write('\n')

        f.write(f'#endif/*{include_guard}*/\n')

def main():
    parser = argparse.ArgumentParser(
        description="Generate item_descriptors C/H files"
    )
    parser.add_argument("yamlfile", help="Item descriptors definition YAML")
    parser.add_argument("-d", "--dir", default=".", help="Output directory")

    args = parser.parse_args()

    yaml_path = os.path.abspath(args.yamlfile)
    yaml_dir = os.path.dirname(yaml_path)
    yaml_base = os.path.splitext(os.path.basename(yaml_path))[0]
    output_dir = os.path.abspath(args.dir)
    os.makedirs(output_dir, exist_ok=True)

    yaml_data = load_yaml(yaml_path)
    generate_c_source(yaml_data, yaml_base, output_dir)
    generate_c_header(yaml_base, output_dir)
    generate_cpp_header(yaml_data, yaml_base, output_dir)

    # Generate anim C/H files
#    c_files = generate_anim_files(yaml_data, yaml_base, output_dir, tile_index_map)
#    c_files.append(merged_bmp_name.replace(".bmp", ".c"))

if __name__ == "__main__":
    main()
