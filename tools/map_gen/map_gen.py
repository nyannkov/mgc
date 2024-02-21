import os
import sys
import csv
import argparse

def gen_tilemap_h_file(args):

    filename = os.path.basename(args.mapfile).split('.')[0]
    entity_name = filename
    include_guard = 'MGC_'+filename.upper()+'_H'
    output_path = filename + '.h';
    if args.dir:
        output_path = args.dir + '/' + output_path

    with open(output_path, mode='w') as f:
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
        f.write('#include "mgc/components/map.h"\n')
        f.write('\n')

        # Extern tile instance
        f.write('extern const mgc_map_t {};\n'.format(entity_name))
        f.write('\n')

        # End of extern "C"
        f.write('#ifdef __cplusplus\n')
        f.write('}/*extern "C"*/\n')
        f.write('#endif\n')
        f.write('\n')

        # End of include guard
        f.write('#endif/*{}*/\n'.format(include_guard))


def gen_tilemap_c_file(args):

    filename = os.path.basename(args.mapfile).split('.')[0]
    entity_name = filename
    map_csv_file = args.mapfile
    output_path = filename + '.c';
    if args.dir:
        output_path = args.dir + '/' + output_path

    map_matrix = []
    with open(map_csv_file) as f:
        header = next(csv.reader(f))
        reader = csv.reader(f)
        for row in reader:
            map_matrix.append(list(map(lambda x:0 if not x else int(x, 0), row)))

    map_width = len(map_matrix[0])
    map_height = len(map_matrix)
    map_id = header[1]

    with open(output_path, mode='w') as f:
        # Include statement
        f.write('#include "{}"\n'.format(filename+'.h'))
        f.write('\n')

        # Declare tile map
        if args.compression == 'runlength':
            map_runlength = []
            cell_value = map_matrix[0][0]
            cell_value_count = 1;
            for next_cell_value in map_matrix[0][1:]:
                if cell_value != next_cell_value:
                    map_runlength.append(cell_value)
                    map_runlength.append(cell_value_count)
                    cell_value = next_cell_value;
                    cell_value_count = 1;
                else:
                    if cell_value_count >= 255:
                        map_runlength.append(cell_value)
                        map_runlength.append(cell_value_count)
                        cell_value_count = 1;
                    else:
                        cell_value_count += 1;

            for r in range(1, map_height):
                for next_cell_value in map_matrix[r]:
                    if cell_value != next_cell_value:
                        map_runlength.append(cell_value)
                        map_runlength.append(cell_value_count)
                        cell_value = next_cell_value;
                        cell_value_count = 1;
                    else:
                        if cell_value_count >= 255:
                            map_runlength.append(cell_value)
                            map_runlength.append(cell_value_count)
                            cell_value_count = 1;
                        else:
                            cell_value_count += 1;


            if cell_value_count > 0:
                map_runlength.append(cell_value)
                map_runlength.append(cell_value_count)

            f.write('static const uint8_t map[{}] = {{\n'.format(len(map_runlength)))
            for i in range(0, len(map_runlength), 2):
                f.write('    0x{:02X}, 0x{:02X},\n'.format(map_runlength[i], map_runlength[i+1]))
            f.write('};\n')
            f.write('\n')

        else:
            # compression: none
            f.write('static const uint8_t map[{}] = {{\n'.format(map_width*map_height))
            map_matrix = list(map(lambda row: list(map('0x{:02X}'.format, row)), map_matrix))
            map_list = []
            for r in range(0, map_height):
                map_col = ', '.join(map_matrix[r])
                map_list.append('    {}'.format(map_col))
            f.write(',\n'.join(map_list));
            f.write('\n};\n')
            f.write('\n')

        # Declare tileset instance
        comp_type = 'MGC_MAP_COMPRESSION_RUN_LENGTH' if args.compression == 'runlength' else 'MGC_MAP_COMPRESSION_NONE'
        text = '\n'.join([
            'const mgc_map_t {} = {{'.format(entity_name),
            '    .map = map,',
            '    .cell_count = sizeof(map)/sizeof(map[0]),',
            '    .map_width = {},'.format(map_width),
            '    .map_height = {},'.format(map_height),
            '    .comp_type = {},'.format(comp_type),
            '};'
        ])
        f.write(text)

if __name__ == '__main__':

    parser = argparse.ArgumentParser(description='Convert CSV map file to C source code for mgc.')
    parser.add_argument('mapfile', help='Specify the path to the CSV file containing map data.')
    parser.add_argument('-d', '--dir',  help='Specify the destination directory to save the generated files.')
    parser.add_argument('-c', '--compression',  choices=['none', 'runlength'], default='none', help='Specify the compression method for the generated map array. Choose between "none" (no compression) or "runlength" (run-length compression). Default is "none".')
    args = parser.parse_args()

    if args.dir:
        os.makedirs(args.dir, exist_ok=True)

    gen_tilemap_h_file(args)
    gen_tilemap_c_file(args)

