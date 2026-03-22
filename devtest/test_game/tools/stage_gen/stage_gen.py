import xml.etree.ElementTree as ET
import os
import argparse


def parse_tmx(tmx_path: str):
    tree = ET.parse(tmx_path)
    root = tree.getroot()

    layers = []
    for layer in root.findall("layer"):
        name = layer.attrib.get("name", "noname")
        data = layer.find("data")
        if data is None or data.text is None:
            continue

        rows = []
        for line in data.text.strip().splitlines():
            nums = [int(x) for x in line.split(",") if x.strip() != ""]
            rows.append(nums)

        layers.append((name, rows))

    return layers


def name_matches(layer_name: str, pattern_list):
    lname = layer_name.lower()
    for p in pattern_list:
        if p.lower() in lname:
            return True
    return False


def convert_tile_value(value: int, apply_or128: bool) -> int:
    if value == 0:
        return 0
    return (value | 128) if apply_or128 else value


def apply_border_or(rows, border_flag: bool):
    if not border_flag:
        return rows

    height = len(rows)
    width = len(rows[0]) if height > 0 else 0

    for y in range(height):
        for x in range(width):
            if (
                y == 0 or y == height - 1 or
                x == 0 or x == width - 1
            ):
                rows[y][x] |= 128

    return rows


def write_csv(base_name: str, layer_name: str, rows, out_dir: str,
              tile_or_list, border_list):

    os.makedirs(out_dir, exist_ok=True)

    out_name = f"{base_name}_{layer_name}.csv"
    out_path = os.path.join(out_dir, out_name)

    apply_or128 = name_matches(layer_name, tile_or_list)
    apply_border = name_matches(layer_name, border_list)

    converted_rows = [
        [convert_tile_value(v, apply_or128) for v in row]
        for row in rows
    ]

    converted_rows = apply_border_or(converted_rows, apply_border)

    with open(out_path, "w", encoding="utf-8") as f:
        f.write("VERSION,1,\n")
        for row in converted_rows:
            f.write(",".join(str(v) for v in row) + ",\n")

    print(f"Generated: {out_path}")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("tmx_file", help="input TMX file")
    parser.add_argument(
        "-d", "--dir",
        help="Specify the destination directory to save the generated files.",
        default="."
    )
    parser.add_argument(
        "--tile-or128",
        nargs="+",
        help="Apply OR-128 to non-zero tile values for specified layer names.",
        default=[]
    )
    parser.add_argument(
        "--border",
        nargs="+",
        help="Apply OR-128 to border for specified layer names.",
        default=[]
    )
    parser.add_argument(
        "--list-outputs",
        action="store_true",
        help="Only list the paths of files that would be generated."
    )

    args = parser.parse_args()

    tmx_file = args.tmx_file
    out_dir = args.dir

    base = os.path.splitext(os.path.basename(tmx_file))[0]
    layers = parse_tmx(tmx_file)

    if args.list_outputs:
        output_paths = []
        for name, _ in layers:
            out_name = f"{base}_{name}.csv"
            out_path = os.path.abspath(os.path.join(args.dir, out_name))
            output_paths.append(out_path)
        print(";".join(output_paths))
        return

    for name, rows in layers:
        write_csv(base, name, rows, out_dir,
                  args.tile_or128, args.border)


if __name__ == "__main__":
    main()

