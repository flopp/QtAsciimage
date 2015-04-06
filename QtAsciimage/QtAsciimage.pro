TARGET = QtAsciimage
TEMPLATE = lib
CONFIG += staticlib c++11

SOURCES += iconengine.cpp image.cpp shape.cpp
HEADERS += iconengine.h image.h shape.h style.h

isEmpty(PREFIX) {
    unix {
        PREFIX = /usr
    } else {
        PREFIX = $$[QT_INSTALL_PREFIX]
    }
}

install_headers.files = $$HEADERS
install_headers.path = $$PREFIX/include/QtAsciimage
target.path = $$PREFIX/lib
INSTALLS += install_headers target
