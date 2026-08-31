# Photoeditor

A lightweight BMP image editing tool with a graphical interface, built in C using the [IUP](https://www.tecgraf.puc-rio.br/iup/) toolkit.

## Features

- Open and save 24-bit uncompressed `.bmp` images
- Grayscale conversion
- Brightness adjustment (3 preset levels)
- Color invert
- Horizontal and vertical flip
- Rotate (90° / 180° / 270°, clockwise or counterclockwise)
- Crop to a custom region
- Blur (3x3 box blur)
- Sharpen (3x3 kernel)
- Undo (up to 10 steps)

## Requirements

- GCC and `make`
- GTK3 development headers (`libgtk-3-dev`) and `pkg-config`

The [IUP](https://www.tecgraf.puc-rio.br/iup/) library itself is **already bundled** in this repo under `main/third_party/iup/`, so you do **not** need to download or install IUP separately.

## Building and running (Linux)

```bash
sudo apt update
sudo apt install -y build-essential libgtk-3-dev pkg-config git

git clone https://github.com/heckerabid/Photoeditor.git
cd Photoeditor/main
make
make run
```

This produces a `bmpedit` binary in `main/`. Run it directly with:

```bash
./bmpedit
```

