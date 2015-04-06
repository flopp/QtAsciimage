/*******************************************************************************
* Copyright (c) 2015 Florian Pigorsch <mail@florian-pigorsch.de>               *
*                                                                              *
* Permission is hereby granted, free of charge, to any person obtaining a copy *
* of this software and associated documentation files (the "Software"), to deal*
* in the Software without restriction, including without limitation the rights *
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell    *
* copies of the Software, and to permit persons to whom the Software is        *
* furnished to do so, subject to the following conditions:                     *
*                                                                              *
* The above copyright notice and this permission notice shall be included in   *
* all copies or substantial portions of the Software.                          *
*                                                                              *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR   *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,     *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE  *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER       *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,*
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN    *
* THE SOFTWARE.                                                                *
*******************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../QtAsciimage/image.h"
#include <QMainWindow>
class QAction;
namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void initializeImage(int width, int height);

  protected:
    virtual void closeEvent(QCloseEvent* event) override;

  private:
    void setupActions();
    void setupMenus();
    void setupToolbars();

    void updateTitle();
    bool save(bool silent = false);
    bool saveAs();
    bool open(bool silent = false);


  private slots:
    void slotNew();
    void slotOpen();
    void slotSave();
    void slotSaveAs();
    void slotClose();

    void slotChanged();

    void parseImage();
    void renderImage();
    void toggleOverwriteMode();

  private:
    Ui::MainWindow* m_ui;
    asciimage::Image m_image;
    bool m_modified;
    QString m_filename;

    QAction* m_newAction;
    QAction* m_openAction;
    QAction* m_saveAction;
    QAction* m_saveAsAction;
    QAction* m_closeAction;
};

#endif
