# QtAsciimage
QtAsciimage - Asciimage for Qt Applications


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
