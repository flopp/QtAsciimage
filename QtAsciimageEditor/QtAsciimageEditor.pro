TARGET = QtAsciimageEditor
TEMPLATE = app
CONFIG += c++11
QT += core gui widgets

HEADERS += mainWindow.h newImageDialog.h
SOURCES += main.cpp mainWindow.cpp newImageDialog.cpp
FORMS += mainWindow.ui newImageDialog.ui

include(../QtAsciimage/QtAsciimage.pri)
