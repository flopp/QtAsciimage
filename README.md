# QtAsciimage
QtAsciimage - Asciimage for Qt Applications

This is a C++/Qt version of (Charles Parnot)[https://twitter.com/cparnot]'s (ASCIImage)[http://asciimage.org/] format. ASCIImage let's you specify simple images directly in your source code using 'ASCII art'.

## Format Extension
QtAsciimage uses an extended format allowing for a direct styling of the individual ASCIImage shapes. The styling rules are appended to the ASCIImage, separated by a line containing three dashes `---` only:

    . . . . 5 . . . .
    . . . . # . . . .
    . . . . # . . . .
    . . . . # . . . .
    . . 1 . # . 3 . .
    . . . # 5 # . . .
    6 . . . 2 . . . 9
    # . . . . . . . #
    7 # # # # # # # 8
    ---
    1 open color=#FF0000
    5 color=#FF0000
    6 open 

A styling rule contains of a shape identifier (the character that is used in the ASCIImage to start the corresponding shape) and a set of styling options:
    
    ID [color=#RRGGBB] [open] [empty] [cutting]

*   `color=#RRGGBB`
Applicable to all shape types.
The shape is drawn in the given hex-color instead of the default color (black).

*   `open`
Applicable to polygons.
The shape is not automatically closed, and thus drawn as a polyline instead of a polygon. 

*   `empty`
Applicable to ellipses and polygons.
The shape is not filled; only the edge is drawn.

*   `cutting`
Applicable to all shape types.
Instead of drawing the shape, it is cut out of the previsously drawn shapes, leaving a 'hole'.


## Usage 
To include QtAsciimage into your QMake-based project, copy the QtAsciimage directory to your project tree and `include()` QtAsciimage's `pri`-file in you main QMake-file:

    include(QtAsciimage/QtAsciimage.pri)

Of course you need to `include()` QtAsciimage's header files (`QtAsciimage/image.h` should normally suffice; you also need `QtAsciimage/iconengine.h` if you want to use the QIconEngine) into your C++ sources. 

### Rendering to QImage

    asciimage::Image asciiSun({
      ". . . . 8 . . . .",
      ". 7 . . 8 . . 9 .",
      ". . 7 . . . 9 . .",
      ". . . . 1 . . . .",
      "6 6 . 1 # 1 . 2 2",
      ". . . . 1 . . . .",
      ". . 5 . . . 3 . .",
      ". 5 . . 4 . . 3 .",
      ". . . . 4 . . . ."
    });
    
    int scale = 4;
    QImage result = asciiSun.render(scale);

### Rendering via QIconEngine

    asciimage::Image asciiSaveIcon({
      ". . . . 5 . . . .",
      ". . . . # . . . .",
      ". . . . # . . . .",
      ". . . . # . . . .",
      ". . 1 . # . 3 . .",
      ". . . # 5 # . . .",
      "6 . . . 2 . . . 9",
      "# . . . . . . . #",
      "7 # # # # # # # 8",
      "---",
      "1 open",
      "6 open"
    });
    QIconEngine* iconEngine = new asciimage::IconEngine(asciiSaveIcon);
    QAction* saveAction = new QAction("Save", this);
    saveAction->setIcon(QIcon(iconEngine));
    // add saveAction to a toolbar/menu to display the icon

## Demo Program / QtAsciimageEditor

QtAsciimageEditor let's you edit Asciimages visually and, furthermore, demos how to use the QtAsciimage library.
 
![Screenshot of QtAsciimageEditor](http://flopp.github.io/QtAsciimage/editor-screenshot.png)
