# Dynamic Binary Visualization

CLI to help find the file type of a binary and visualize it.
Might never be useful, but great excuse to go back into C after 3 years.

## Example
Bitmap file:

<img title="Bitmap" alt="Bitmap output" src="/examples/bmp.png">

x86 file:

<img title="x86" alt="x86 output" src="/examples/x86.png">

PNF file:

<img title="PNG" alt="png output" src="/examples/png.png">

## Usage
```bash
bustelo <file>
```

Dev - Win x86:
1. Install Raylib & W64DevKit system wise: [Guide](https://github.com/raysan5/raylib/wiki/Working-on-Windows)
2. Don't forget to lauch the Shell
```bash
C:\w64devkit\w64devkit.exe
```
3. Compile the project & run it
```bash
make br
```

## Ressources:
- This repo contain my own implementation of Christopher Domas Talk: [Youtube](https://www.youtube.com/watch?v=C8--cXwuuFQ)
