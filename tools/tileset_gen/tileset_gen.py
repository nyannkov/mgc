import os
import sys
import csv
import argparse
from PIL import Image

def get_formed_palette(palette, max_color_index, pixel_format):
    palette = \
    [[palette[i], palette[i+1], palette[i+2]] for i in range(0, 3*(max_color_index+1), 3)]

    formed_palette = []
    if  pixel_format == 'RGB565':
        for row in palette:
            r = row[0]>>3
            g = row[1]>>2
            b = row[2]>>3
            pixel_color = (r<<11)|(g<<5)|(b)
            formed_palette.append('0x{:04X}'.format(pixel_color))
        return formed_palette 
    else:
        return None

def gen_tileset_h_file(args):

    pixel_format = args.pixelformat
    filename = os.path.basename(args.bitmapfile).split('.')[0]
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
        f.write('#include "mgc/components/tileset.h"\n')
        f.write('\n')

        # Extern tile instance
        f.write('extern const struct mgc_tileset {};\n'.format(entity_name))
        f.write('\n')
        
        # End of extern "C"
        f.write('#ifdef __cplusplus\n')
        f.write('}/*extern "C"*/\n')
        f.write('#endif\n')
        f.write('\n')
        
        # End of include guard 
        f.write('#endif/*{}*/\n'.format(include_guard))

def gen_tileset_c_file(args):

    pixel_format = args.pixelformat
    filename = os.path.basename(args.bitmapfile).split('.')[0]
    entity_name = filename
    palette = image.getpalette()
    bitmap_width, bitmap_height  = image.size
    max_color_index = max(image.getdata())
    tile_count = args.count

    if tile_count == 1:
        tile_width = bitmap_width
        tile_height = bitmap_height
    else:
        tile_width = args.width
        tile_height = args.height

    if tile_count == 0:
        tile_count = (bitmap_width//tile_width)*(bitmap_height//tile_height)

    tile_elem_num = tile_width*tile_height

    output_path = filename + '.c';
    if args.dir:
        output_path = args.dir + '/' + output_path

    with open(output_path, mode='w') as f:
        # Include statement
        f.write('#include "{}"\n'.format(filename+'.h'))
        f.write('\n')
        
        # Define macro
        f.write('#define countof(x)    (sizeof((x))/sizeof((x)[0]))\n')
        f.write('\n')

        # Declare palette
        index = 0
        formed_palette = get_formed_palette(palette, max_color_index, pixel_format)
        f.write('static const mgc_color_t palette_array[{}] = {{\n'.format(max_color_index+1))
        palette_array = []
        for plt in formed_palette:
            palette_array.append('    {}/*0x{:02X}*/'.format(plt, index))
            index += 1
        f.write(',\n'.join(palette_array));
        f.write('\n};\n')

        # Declare tiles
        bitmap_image = image.getdata()
        bitmap_matrix = []
        for h in range(0, bitmap_height):
            bitmap_row = []
            for w in range(0, bitmap_width):
                bitmap_row.append(bitmap_image[w+bitmap_width*h])
            bitmap_matrix.append(bitmap_row)

        for image_pos in range(0, tile_count*tile_width, tile_width):
            tile_id = image_pos//tile_width
            w = image_pos % bitmap_width
            h = (image_pos // bitmap_width)*tile_height
            
            tile_matrix = [row[w:w+tile_width] for row in bitmap_matrix[h:h+tile_height]]
            f.write('static const uint8_t tile_{:04d}[{}] = {{\n'.format(tile_id, tile_elem_num))
            tile_matrix = list(map(lambda row: list(map('0x{:02X}'.format, row)), tile_matrix))
            tile_list = []
            for r in range(0, tile_height):
                tile_col = ', '.join(tile_matrix[r])
                tile_list.append('    {}'.format(tile_col))
            f.write(',\n'.join(tile_list));
            f.write('\n};\n')

        # Declare tile array
        f.write('static const uint8_t *tile_array[{}] = {{\n'.format(tile_count))
        tile_list = []
        for t in range(0, tile_count):
            tile_list.append('    tile_{:04d}'.format(t))
        f.write(',\n'.join(tile_list));
        f.write('\n};\n')

        # Declare tileset instance
        text = '\n'.join([
            'const struct mgc_tileset {} = {{'.format(entity_name),
            '    .pixel_format = MGC_{},'.format(pixel_format),
            '    .tile_width = {},'.format(tile_width),
            '    .tile_height = {},'.format(tile_height),
            '    .palette_array = palette_array,',
            '    .palette_count = countof(palette_array),',
            '    .tile_array = tile_array,',
            '    .tile_count = countof(tile_array)',
            '};'
        ])
        f.write(text)

if __name__ == '__main__':

    parser = argparse.ArgumentParser()
    parser.add_argument('bitmapfile', help='Set bitmap file path.')
    parser.add_argument('-w', '--width', default=16, type=int, help='Set tile width.')
    parser.add_argument('-H', '--height', default=16, type=int, help='Set tile height.')
    parser.add_argument('-c', '--count', default=0, type=int, help='Set tile count.')
    parser.add_argument('-p', '--pixelformat',default='RGB565',  help='Set pixel format.')
    parser.add_argument('-d', '--dir',  help='Destination directory.')

    args = parser.parse_args()
    image = Image.open(args.bitmapfile)

    if image.mode != 'P':
        print('Error: Only supports indexed color.')
        sys.exit(1)

    if (args.width <= 0) or (args.height <=0):
        print('Error: Tile width and height must be greater than 0.')
        sys.exit(2)

    if args.dir:
        os.makedirs(args.dir, exist_ok=True)

    # Create tileset header file.
    gen_tileset_h_file(args)

    # Create tileset source file.
    gen_tileset_c_file(args)

    sys.exit(0)

