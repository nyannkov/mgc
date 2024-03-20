# font_gen.py

This script is a utility for handling font data in mgc.
It converts font files in BDF format into mgc_font_t structure constants and generates C source code.
It can also subset font data using strings described in a given text file.

## Example usage

```bash
python3 font_gen.py k8x12S.bdf --dir ./font --subsettext subset.txt
```

In this example, the font data k8x12S.bdf is subset with the string described in subset.txt, and the generated mgc_font_t structure constant is placed in the directory ./font as C source code.

## Caution

The encoding of the font data and strings described in subset.txt currently supports only UTF-8.
