import os
import sys
import argparse
from enum import Enum

def gen_fontset_c_file(args):
    class ParseState(Enum):
        INIT =0 
        GLOBAL_FONT_INFO = 1
        PROPERTIES = 2
        GLYPH_INFO = 3
        BITMAP = 4
        END = 5
    parse_state = ParseState.INIT

    bdf_version = ''
    bdf_global_font_info = {}
    bdf_properties = {}
    bdf_glyph_info_list = []
    bdf_glyph_info = {}

    bdf_bitmap_list = []

    with open(args.fontfile, mode='r') as f:
        for line in f:
            data = line.split()
            command = data[0]
            if parse_state == ParseState.INIT:
                if command == 'STARTFONT':
                    bdf_version = data[1]
                    parse_state = ParseState.GLOBAL_FONT_INFO

            elif parse_state == ParseState.GLOBAL_FONT_INFO:
                if command == 'STARTPROPERTIES':
                    bdf_global_font_info[command] = data[1]
                    parse_state = ParseState.PROPERTIES
                else:
                    bdf_global_font_info[command] = data[1:]
                    
            elif parse_state == ParseState.PROPERTIES:
                if command == 'ENDPROPERTIES':
                    parse_state = ParseState.GLYPH_INFO
                else:
                    bdf_properties[command] = data[1:]

            elif parse_state == ParseState.GLYPH_INFO:
                if command == 'STARTCHAR':
                    bdf_glyph_info = {}
                    bdf_glyph_info[command] = data[1]
                elif command == 'BITMAP':
                    bdf_bitmap_list = []
                    parse_state = ParseState.BITMAP
                elif command == 'ENDFONT':
                    parse_state = ParseState.END
                elif command == 'CHARS':
                    pass
                else:
                    bdf_glyph_info[command] = data[1:]

            elif parse_state == ParseState.BITMAP:
                if command == 'ENDCHAR':
                    bdf_glyph_info['BITMAP'] = bdf_bitmap_list;
                    bdf_glyph_info_list.append(bdf_glyph_info)
                    parse_state = ParseState.GLYPH_INFO
                else:
                    bdf_bitmap_list.append(command)
            else:
                break

    if parse_state != ParseState.END:
        # Parse error
        exit(-1)

    filename = os.path.basename(args.fontfile).split('.')[0]
    ofs_addr = 0
    bitmap_len = 0
    output_path = filename + '.c';
    if args.dir:
        output_path = args.dir + '/' + output_path

    if args.subsettext:
        subset_unicode_list = []
        with open(args.subsettext, "r", encoding="utf-8") as f:
            for line in f:
                for char in line:
                    subset_unicode_list.append(ord(char))

        bdf_glyph_info_list = [glyph for glyph in bdf_glyph_info_list if int(glyph['ENCODING'][0]) in subset_unicode_list]

    with open(output_path, mode='w') as f:
        # Include statement
        f.write('#include "{}"\n'.format(filename+'.h'))
        f.write('\n')

        # Define macro
        f.write('#define countof(x)    (sizeof((x))/sizeof((x)[0]))\n')
        f.write('\n')
        
        # Declare bitmap
        f.write('static const uint8_t bitmap[] = {\n')
        for glyph in bdf_glyph_info_list:
            f.write('    /* [{:08X}] Encoding {:04X} ({}) */'.format(ofs_addr, int(glyph['ENCODING'][0]), glyph['STARTCHAR']))
            f.write('\n')
            bitmap_len = len(glyph['BITMAP'])
            if bitmap_len != 0:
                if int(glyph['BBX'][0]) <= 8:
                    f.write('    {},\n'.format(', '.join(list(map(lambda x:'0x'+x, glyph['BITMAP'])))))
                    ofs_addr += bitmap_len
                elif int(glyph['BBX'][0]) <= 16:
                    f.write('    {},\n'.format(', '.join(list(map(lambda x:'0x'+x[0:2] +', '+'0x'+x[2:4], glyph['BITMAP'])))))
                    ofs_addr += (bitmap_len*2)
                else:
                    # error
                    sys.exit("Error: font size must be less equal to 16 px.")
            else:
                f.write('    /* None */\n')
        f.write('};\n')
        f.write('\n')
                
        ofs_addr = 0
        f.write('static const struct mgc_glyph glyph[] = {\n')
        for glyph in bdf_glyph_info_list:
            f.write('    /* {} */ '.format(glyph['STARTCHAR']))
            f.write('{')
            f.write('.encoding = {}, '.format(glyph['ENCODING'][0]))
#            f.write('.swx0 = {}, '.format(glyph['SWIDTH'][0]))
#            f.write('.swy0 = {}, '.format(glyph['SWIDTH'][1]))
            f.write('.dwx0 = {}, '.format(glyph['DWIDTH'][0]))
            f.write('.dwy0 = {}, '.format(glyph['DWIDTH'][1]))
            f.write('.bb_w = {}, '.format(glyph['BBX'][0]))
            f.write('.bb_h = {}, '.format(glyph['BBX'][1]))
            f.write('.bb_xoff0x = {}, '.format(glyph['BBX'][2]))
            f.write('.bb_yoff0y = {}, '.format(glyph['BBX'][3]))
            f.write('.bitmap = &bitmap[0x{:08X}] '.format(ofs_addr))
            f.write('},\n')
            if int(glyph['BBX'][0]) <= 8:
                ofs_addr += len(glyph['BITMAP'])
            else:
                ofs_addr += len(glyph['BITMAP'])*2

        f.write('};')
        f.write('\n')
        f.write('\n')
        f.write('const struct mgc_font {} = {{\n'.format(filename))
        f.write('    .name = "{}",\n'.format(bdf_global_font_info['FONT'][0]))
        f.write('    .fbb_x = {},\n'.format(bdf_global_font_info['FONTBOUNDINGBOX'][0]))
        f.write('    .fbb_y = {},\n'.format(bdf_global_font_info['FONTBOUNDINGBOX'][1]))
        f.write('    .x_off = {},\n'.format(bdf_global_font_info['FONTBOUNDINGBOX'][2]))
        f.write('    .y_off = {},\n'.format(bdf_global_font_info['FONTBOUNDINGBOX'][3]))
        f.write('    .pixel_size = {},\n'.format(bdf_properties['PIXEL_SIZE'][0]))
        f.write('    .fontname_registry = {},\n'.format(bdf_properties['FONTNAME_REGISTRY'][0]))
        f.write('    .foundry  = {},\n'.format(bdf_properties['FOUNDRY'][0]))
        f.write('    .family_name = {},\n'.format(bdf_properties['FAMILY_NAME'][0]))
        f.write('    .weight_name = {},\n'.format(bdf_properties['WEIGHT_NAME'][0]))
        f.write('    .slant = {},\n'.format(bdf_properties['SLANT'][0]))
        f.write('    .charset_registry = {},\n'.format(bdf_properties['CHARSET_REGISTRY'][0]))
        f.write('    .charset_encoding = {},\n'.format(bdf_properties['CHARSET_ENCODING'][0]))
        f.write('    .copyright = {},\n'.format(' '.join(bdf_properties['COPYRIGHT'])))
        f.write('    .glyph_array = glyph,\n')
        f.write('    .glyph_count = countof(glyph)\n')
        f.write('};')
        f.write('\n')
    pass

def gen_fontset_h_file(args):

    filename = os.path.basename(args.fontfile).split('.')[0]
    output_path = filename + '.h';
    if args.dir:
        output_path = args.dir + '/' + output_path

    with open(output_path, mode='w') as f:
        include_guard = '{}_H'.format(filename.upper())
        entity_name = filename

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
        f.write('#include "mgc/font/font.h"\n')
        f.write('\n')

        # Extern tile instance
        f.write('extern const struct mgc_font {};\n'.format(entity_name))
        f.write('\n')
        
        # End of extern "C"
        f.write('#ifdef __cplusplus\n')
        f.write('}/*extern "C"*/\n')
        f.write('#endif\n')
        f.write('\n')
        
        # End of include guard 
        f.write('#endif/*{}*/\n'.format(include_guard))
    
if __name__ == '__main__':

    parser = argparse.ArgumentParser()
    parser.add_argument('fontfile', help='Set bitmap file path.')
    parser.add_argument('-d', '--dir',  help='Destination directory.')
    parser.add_argument('-s', '--subsettext', help='font subset file path.')
    args = parser.parse_args()

    if args.dir:
        os.makedirs(args.dir, exist_ok=True)

    gen_fontset_h_file(args)
    gen_fontset_c_file(args)

