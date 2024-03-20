# map_gen.py

This script is a utility for handling map information created in CSV files in mgc.
It converts the map information created in the CSV file into mgc_map_t structure constants and generates them as C source code.

## Map information file

The first line of the CSV file describes the format version. The current version is fixed and is '1'.
The cells after the second line describe the specific map information.


| VERSION | 1 |   |   |   |
|---------|---|---|---|---|
| 0x81    | 0x81 | 0x81 | 0x81 | 0x81 |
| 0x81    |     |     |     | 0x03 |
| 0x81    |     |     |     | 0x03 |
| 0x81    |     | 0x81 |     | 0x03 |

Each cell specifies one byte of data. The data content follows the following bit fields.

| Bit | Description |
|--------|----------------------------|
| 0-6 | Tile number to be drawn (0x00-0x7F) |
| 7 | Collision detection enabled/disabled (0: disabled, 1: enabled) |


The tile number to be drawn corresponds to the tileset index. Also, when collision detection is enabled,
collision between that cell and the sprite can be detected by the maphit module.

## Examples of use

```bash
python3 map_gen.py ./map/map_example.csv --dir ./generates --compression none
```

This example converts the map information described in map_example.csv into mgc_map_t structure constants, which are then stored in ./generates as C source code.
The compression option specifies the compression method of the generated map array.
In the case of this example, the contents of the CSV file are simply converted to an array. On the other hand, if runlength is specified for compression, the map information is compressed into a run-length array.
In the case of map information with the same consecutive tile number, the latter will reduce the data size.
However, the drawing speed will be slower than the former due to the decompression process.

```bash
python3 map_gen.py ./map/map_example.csv --dir ./generates --compression runlength
```

## Notes

 - The value of a blank cell is assumed to be 0.
 - Tile number 0 is treated as a transparent tile.
 - Each cell is treated as a tile with a size of 16x16 pixels. Therefore, tilesets must also be created with 16x16 pixels.
 
 
