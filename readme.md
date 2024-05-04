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
bustelo <file>  # CLI mode
busoto --gui    # GUI mode
```

## Dev - Setup
**Win x86**
Install Raylib & W64DevKit system wise: [Guide](https://github.com/raysan5/raylib/wiki/Working-on-Windows)
```bash
# Launch the Shell with the devkit
C:\w64devkit\w64devkit.exe

# Compile the project & run it (on git-bash - Not in the same shell)
mingw32-make
```

Dev - Linux:
Insall Raylib system wise
```bash
make
```

## Ressources:
- This repo contain my own implementation of Christopher Domas Talk: [Youtube](https://www.youtube.com/watch?v=C8--cXwuuFQ)
- Project Template: [Misha's Makefile magic](https://github.com/KRMisha/Makefile)
- Adapted for C for my own need
