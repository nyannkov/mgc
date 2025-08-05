import os
import sys
import yaml
import argparse
from PIL import Image
import subprocess

def load_yaml(path):
    with open(path, "r", encoding="utf-8") as f:
        return yaml.safe_load(f)

def normalize_paths(yaml_data, yaml_dir):
    """Normalize frame file paths with yaml_dir"""
    for anim in yaml_data["animations"]:
        for frame in anim["frames"]:
            frame["norm_file"] = os.path.normpath(
                os.path.join(yaml_dir, frame["file"])
            )

def build_tile_index_map(yaml_data):
    """Build unique frame file list and mapping to tile index"""
    all_files = []
    for anim in yaml_data["animations"]:
        for frame in anim["frames"]:
            all_files.append(frame["norm_file"])
    # Deduplicate preserving order
    seen = set()
    unique_files = []
    for f in all_files:
        if f not in seen:
            seen.add(f)
            unique_files.append(f)
    # Mapping file -> tile index
    tile_index_map = {f: idx for idx, f in enumerate(unique_files)}
    return unique_files, tile_index_map

def generate_merged_bmp(unique_files, output_bmp_path, columns=8):
    """Merge all unique frame BMPs into one tileset BMP"""
    if not unique_files:
        print("No frames to merge.", file=sys.stderr)
        sys.exit(1)

    # Validate and collect images
    imgs = []
    first_palette = None
    first_size = None
    for path in unique_files:
        img = Image.open(path)
        if img.mode != "P":
            print(f"Error: {path} is not indexed color (mode {img.mode})")
            sys.exit(2)
        if first_palette is None:
            first_palette = img.getpalette()
            first_size = img.size
        else:
            if img.size != first_size:
                print(f"Error: {path} size {img.size} != {first_size}")
                sys.exit(2)
            if img.getpalette() != first_palette:
                print(f"Error: {path} palette differs from first image.")
                sys.exit(2)
        imgs.append(img)

    tile_w, tile_h = first_size
    rows = (len(imgs) + columns - 1) // columns
    merged = Image.new("P", (tile_w * columns, tile_h * rows))
    merged.putpalette(first_palette)

    for idx, img in enumerate(imgs):
        x = (idx % columns) * tile_w
        y = (idx // columns) * tile_h
        merged.paste(img, (x, y))

    os.makedirs(os.path.dirname(output_bmp_path), exist_ok=True)
    merged.save(output_bmp_path)
    print(f"Generated merged BMP: {output_bmp_path} ({len(imgs)} tiles)")

    return tile_w, tile_h

def generate_anim_files(yaml_data, yaml_base, output_dir, tile_index_map):
    """Generate .c/.h per animation and a combined <yamlbase>.h"""
    combined_h_path = os.path.join(output_dir, f"{yaml_base}.h")
    with open(combined_h_path, "w") as combined_h:
        include_guard = f"ANIM_{yaml_base.upper()}_H"
        combined_h.write(f"#ifndef {include_guard}\n#define {include_guard}\n\n")
        combined_h.write("#ifdef __cplusplus\nextern \"C\" {\n#endif\n\n")
        combined_h.write("#include \"mgc/sequencer/anim_frames.h\"\n\n")

        for anim in yaml_data["animations"]:
            anim_name = f"{yaml_base}_{anim["name"]}"
            combined_h.write(f"extern const mgc_anim_frames_t {anim_name};\n")

        combined_h.write("\n#ifdef __cplusplus\n}\n#endif\n")
        combined_h.write(f"#endif /* {include_guard} */\n")

    c_files = []
    for anim in yaml_data["animations"]:
        anim_name = f"{yaml_base}_{anim["name"]}"
        h_path = os.path.join(output_dir, f"{anim_name}.h")
        c_path = os.path.join(output_dir, f"{anim_name}.c")
        c_files.append(c_path)

        with open(h_path, "w") as hf:
            include_guard = f"ANIM_{anim_name.upper()}_H"
            hf.write(f"#ifndef {include_guard}\n#define {include_guard}\n\n")
            hf.write("#ifdef __cplusplus\nextern \"C\" {\n#endif\n\n")
            hf.write("#include \"mgc/sequencer/anim_frames.h\"\n\n")

            hf.write("enum {\n")
            for idx, frame in enumerate(anim["frames"]):
                user_name = frame.get("name")
                if user_name:
                    enum_name = f"{anim_name}_{user_name}".upper()
                else:
                    enum_name = f"{anim_name}_FRAME{idx}".upper()
                hf.write(f"    {enum_name} = {idx},\n")
            hf.write("};\n\n")

            hf.write(f"extern const mgc_anim_frames_t {anim_name};\n\n")
            hf.write("#ifdef __cplusplus\n}\n#endif\n")
            hf.write(f"#endif /* {include_guard} */\n")

        with open(c_path, "w") as cf:
            cf.write(f"#include \"{anim_name}.h\"\n")
            cf.write(f"#include \"tileset_{yaml_base}.h\"\n\n")
            description = anim.get("description", "")
            if description:
                cf.write(f"/* {description} */\n")

            frames_var = f"{anim_name}_frames"
            cf.write(f"static const mgc_anim_frame_t {frames_var}[] = {{\n")
            for frame in anim["frames"]:
                file = frame["file"]
                norm_file = frame["norm_file"]
                time_ms = frame["time_ms"]
                tile_index = tile_index_map[norm_file]
                cf.write(f"    {{ {tile_index}, {time_ms} }}, "
                         f"/* {os.path.basename(file)} (tile {tile_index}) */\n")
            cf.write("};\n\n")

            cf.write(
                f"const mgc_anim_frames_t {anim_name} = {{\n"
                f"    .tileset = &tileset_{yaml_base},\n"
                f"    .frame_array = {frames_var},\n"
                f"    .frame_count = countof({frames_var})\n"
                f"}};\n\n"
            )

    print(f"Generated animation files in {output_dir}")
    return c_files

def generate_cmakelists(output_dir, yaml_base, c_files):
    """Generate a CMakeLists.txt that builds all generated C files into a static lib"""
    cmake_path = os.path.join(output_dir, "CMakeLists.txt")
    libname = f"{yaml_base}"
    with open(cmake_path, "w") as cf:
        cf.write("cmake_minimum_required(VERSION 3.12)\n")
        cf.write(f"project({libname} C)\n\n")
        cf.write("add_library(${PROJECT_NAME} STATIC\n")
        for cfile in c_files:
            cf.write(f"    {os.path.basename(cfile)}\n")
        cf.write(")\n\n")
        cf.write("# Link against mgc (must be added via add_subdirectory in parent project)\n")
        cf.write("target_link_libraries(${PROJECT_NAME} PUBLIC mgc)\n")
        cf.write("target_include_directories(${PROJECT_NAME} PUBLIC\n")
        cf.write("    ${CMAKE_CURRENT_SOURCE_DIR}\n")
        cf.write(")\n")
    print(f"Generated CMakeLists.txt: {cmake_path}")

def main():
    parser = argparse.ArgumentParser(
        description="Generate merged tileset BMP and animation C/H files"
    )
    parser.add_argument("yamlfile", help="Animation definition YAML")
    parser.add_argument("-d", "--dir", default=".", help="Output directory")
    parser.add_argument("--columns", type=int, default=8, help="Tiles per row in merged BMP")
    parser.add_argument("--no-gen-tileset", action="store_true", help="Do not call tileset_gen.py")
    args = parser.parse_args()

    yaml_path = os.path.abspath(args.yamlfile)
    yaml_dir = os.path.dirname(yaml_path)
    yaml_base = os.path.splitext(os.path.basename(yaml_path))[0]
    output_dir = os.path.abspath(args.dir)

    yaml_data = load_yaml(yaml_path)
    normalize_paths(yaml_data, yaml_dir)
    unique_files, tile_index_map = build_tile_index_map(yaml_data)

    # Merge BMP
    merged_bmp_name = f"tileset_{yaml_base}.bmp"
    merged_bmp_path = os.path.join(output_dir, merged_bmp_name)
    tile_w, tile_h = generate_merged_bmp(unique_files, merged_bmp_path, args.columns)

    # Generate anim C/H files
    c_files = generate_anim_files(yaml_data, yaml_base, output_dir, tile_index_map)
    c_files.append(merged_bmp_name.replace(".bmp", ".c"))

    # Generate CMakeLists
    generate_cmakelists(output_dir, yaml_base, c_files)

    # Optionally call tileset_gen.py
    if not args.no_gen_tileset:
        cmd = [
            sys.executable,
            os.path.join(os.path.dirname(__file__), "..", "tileset_gen", "tileset_gen.py"),
            merged_bmp_path,
            "-w", str(tile_w),
            "-H", str(tile_h),
            "-c", str(len(unique_files)),
            "-d", output_dir
        ]
        print("Calling tileset_gen.py:", " ".join(cmd))
        subprocess.check_call(cmd)

if __name__ == "__main__":
    main()
