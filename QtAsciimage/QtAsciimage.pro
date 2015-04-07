TARGET = QtAsciimage
TEMPLATE = lib
CONFIG += staticlib

greaterThan(QT_MAJOR_VERSION, 4) {
    CONFIG += c++11
} else {
    QMAKE_CXXFLAGS += -std=c++0x
}

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
