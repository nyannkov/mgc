# anim_gen.py

This script is a utility for generating animation data in mgc from a YAML animation definition and indexed-color BMP frame images.  
It merges individual frame BMPs into a single tileset BMP and produces corresponding C source/header files describing animation frames referencing the tileset.

## How to generate animations

Animations are defined in a YAML file specifying animation names and frame sequences, where each frame references a BMP image file and its display duration.

The script normalizes frame paths relative to the YAML file location, deduplicates frames, and merges all unique frame images into one tileset BMP arranged in rows and columns.

From this data, it generates:
- A merged tileset BMP (`tileset_<basename>.bmp`)
- Per-animation C source/header files describing the frame sequences
- A combined header declaring all animations
- A `CMakeLists.txt` file to build all generated sources as a static library

Optionally, it calls `tileset_gen.py` to convert the merged BMP into C tileset data.

## Example of use

```bash
python3 anim_gen.py ./animations/character.yaml -d ./generated --columns 8
```

This command processes `character.yaml`, merges frame BMPs into `tileset_character.bmp` with 8 tiles per row,  
and outputs animation C files and `CMakeLists.txt` into the `generated` directory.

## Example YAML animation file

```yaml
animations:
  - name: walk_left
    description: "Walk left"
    frames:
      - { file: frame_001.bmp, time_ms: 50, name: WALK_LEFT_1 }
      - { file: frame_002.bmp, time_ms: 50, name: WALK_LEFT_2 }
  - name: walk_right
    description: "Walk right"
    frames:
      - { file: frame_003.bmp, time_ms: 50 }
      - { file: frame_004.bmp, time_ms: 50, name: WALK_RIGHT_2 }
```

- Each animation has a `name` and optional `description`.
- Each frame specifies:
  - `file`: relative path to an indexed-color BMP frame image.
  - `time_ms`: duration to display the frame in milliseconds.
  - Optional `name` to generate frame enum identifiers.

## Command line options

- `yamlfile`: Path to the YAML animation definition file.
- `-d`, `--dir`: Output directory for generated files (default: current directory).
- `--columns`: Number of tiles per row in the merged tileset BMP (default: 8).
- `--no-gen-tileset`: Skip calling `tileset_gen.py` after generating the merged BMP.

## Notes

- All frame BMPs must be indexed color (`mode 'P'`), share identical palette and size.
- Frame images are deduplicated: identical files appear once in the tileset.
- The merged tileset arranges tiles row-wise according to the specified `--columns` option.
- Generated `.c` and `.h` files rely on `mgc/sequencer/anim_frames.h` and link against the `mgc` library.
- The script expects `tileset_gen.py` to be located at `../tileset_gen/tileset_gen.py` relative to this script, unless `--no-gen-tileset` is specified.
