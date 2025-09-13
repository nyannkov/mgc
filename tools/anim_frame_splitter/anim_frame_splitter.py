import os
import sys
import argparse
from PIL import Image

def split_bitmap(args):
    image = Image.open(args.bitmapfile)

    if image.mode != 'P':
        print('Error: Only supports indexed color BMP (palette mode).')
        sys.exit(1)

    bitmap_width, bitmap_height = image.size

    tile_width = args.width
    tile_height = args.height

    if tile_width <= 0 or tile_height <= 0:
        print('Error: Tile width and height must be greater than 0.')
        sys.exit(2)

    cols = bitmap_width // tile_width
    rows = bitmap_height // tile_height
    tile_count = cols * rows

    print(f'Bitmap size: {bitmap_width}x{bitmap_height}')
    print(f'Tile size: {tile_width}x{tile_height}')
    print(f'Total tiles: {tile_count} ({cols} x {rows})')

    os.makedirs(args.dir, exist_ok=True)

    index = 0
    for row in range(rows):
        for col in range(cols):
            x0 = col * tile_width
            y0 = row * tile_height
            tile = image.crop((x0, y0, x0 + tile_width, y0 + tile_height))

            filename = f'frame_{index:03d}.bmp'
            filepath = os.path.join(args.dir, filename)
            tile.save(filepath)
            index += 1

    print(f'Exported {tile_count} tiles to {args.dir}')

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Split indexed BMP into tile images.')
    parser.add_argument('bitmapfile', help='Input BMP file path')
    parser.add_argument('-w', '--width', type=int, default=16, help='Tile width (default: 16)')
    parser.add_argument('-H', '--height', type=int, default=16, help='Tile height (default: 16)')
    parser.add_argument('-d', '--dir', default='tiles_out', help='Output directory (default: tiles_out)')
    args = parser.parse_args()

    split_bitmap(args)

