TARGET = QtAsciimageEditor
TEMPLATE = app
QT += core gui

greaterThan(QT_MAJOR_VERSION, 4) {
    CONFIG += c++11
    QT += widgets
} else {
    QMAKE_CXXFLAGS += -std=c++0x
}

HEADERS += mainWindow.h newImageDialog.h
SOURCES += main.cpp mainWindow.cpp newImageDialog.cpp
FORMS += mainWindow.ui newImageDialog.ui

include(../QtAsciimage/QtAsciimage.pri)
