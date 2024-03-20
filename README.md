# mgc

mgc is a library under development intended for creating small games or gadgets.
This library processes images in units of 16x16 pixels.Threfore, the size of the 
graphic RAM needs to be at least this size.

## Demonstration

<div style="display: flex;">
  <img src="samples/platformer/img/lcd+controller.jpg" style="width: 35%;">
  <img src="samples/platformer/img/platformer.gif" style="width: 100%;">
</div>

Source code: [samples/platformer](samples/platformer)

## Specification
### Image format
 - Color format: RGB565
 - Color mode: indexed color

### Font
 - Font size: maximum 16 pixels
 - Font file format: BDF

## How to use

There is currently no manual for this library, but samples and examples are gradually being created.
These samples and examples use the k8x12S font. Therefore, when building, the font files must be in the following path
```
mgc/fonts/k8x12_bdf/k8x12S.bdf
```
This font can be downloaded from [the creator's website](https://littlelimit.net/k8x12.htm).
See [SETUP.md](SETUP.md) for information on setting up the development environment.

## License

mgc is released under the MIT licence. For more information, see the LICENSE file.
However, for automatically generated code such as fonts, maps and tilesets, 
follow the licence of the original data.

