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

#include "../QtAsciimage/iconengine.h"
#include "../QtAsciimage/image.h"
#include "mainWindow.h"
#include "newImageDialog.h"
#include "ui_mainWindow.h"
#include <QAction>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QShortcut>
#include <QToolBar>

MainWindow::MainWindow(QWidget* parent) :
QMainWindow(parent),
m_modified(false)
{
  m_ui = new Ui::MainWindow;
  m_ui->setupUi(this);

  m_ui->splitter->setSizes({1, 1});

  connect(m_ui->textEditor, SIGNAL(textChanged()), this, SLOT(parseImage()));
  connect(m_ui->textEditor, SIGNAL(textChanged()), this, SLOT(slotChanged()));
  connect(m_ui->scaleEditor, SIGNAL(valueChanged(int)), this, SLOT(renderImage()));

  QShortcut *s = new QShortcut(Qt::Key_Insert, this);
  s->setContext(Qt::ApplicationShortcut);
  connect(s, SIGNAL(activated()), SLOT(toggleOverwriteMode()));

  parseImage();
  updateTitle();

  setupActions();
  setupMenus();
  setupToolbars();
}


MainWindow::~MainWindow()
{
  delete m_ui;
}


void MainWindow::parseImage()
{
  m_image.parse(m_ui->textEditor->toPlainText());
  renderImage();
}


void MainWindow::renderImage()
{
  if (m_image.isValid())
  {
    m_ui->displayLabel->setText(QString());
    m_ui->displayLabel->setPixmap(QPixmap::fromImage(m_image.render(m_ui->scaleEditor->value())));
  }
  else
  {
    m_ui->displayLabel->setPixmap(QPixmap());
    m_ui->displayLabel->setText("bad image");
  }
}


void MainWindow::updateTitle()
{
  QString title;
  if (m_modified) title += "*";

  QString fileName = m_filename;
  if (fileName.isEmpty())
  {
    title += "unnamed";
  }
  else
  {
    title += fileName;
  }

  title += " - QtAsciimageEditor";

  setWindowTitle(title);
}


void MainWindow::slotClose()
{
  close();
}


void MainWindow::slotNew()
{
  if (m_modified)
  {
    const QMessageBox::StandardButton answer =
      QMessageBox::question(
        this,
        "New Asciimage",
        "The current asciimage has unsaved changes.\nSave them before creating a new asciimage?",
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    if (answer == QMessageBox::Yes)
    {
      if (!save()) return;
    }
    else if (answer == QMessageBox::No)
    {
      // nothing to do
    }
    else // Cancel
    {
      return;
    }
  }

  m_ui->textEditor->clear();
  m_filename.clear();
  m_modified = true;
  updateTitle();

  NewImageDialog dialog;
  if (!dialog.exec())
  {
    return;
  }

  QString newDoc;
  if (dialog.doFillGrid())
  {
    QSize s = dialog.gridSize();
    for (int y = 0; y != s.height(); ++y)
    {
      for (int x = 0; x != s.width(); ++x)
      {
        newDoc += ". ";
      }
      newDoc += "\n";
    }
  }

  m_ui->textEditor->setPlainText(newDoc);
}


void MainWindow::setupActions()
{
  asciimage::Image newIcon({
      "1 # # # # # # # 2",
      "# . . . . . . . #",
      "# . a . c . e . #",
      "# . . . . . . . #",
      "# . g . i . k . #",
      "# . . . . . . . #",
      "# . m . p . r . #",
      "# . . . . . . . #",
      "4 # # # # # # # 3",
      "---",
      "1 empty"
    });
    m_newAction = new QAction(QIcon(new asciimage::IconEngine(newIcon)), "New...", this);
    connect(m_newAction, SIGNAL(triggered()), this, SLOT(slotNew()));

    asciimage::Image openIcon({
      ". . . . 2 . . . .",
      ". . . # 5 # . . .",
      ". . 1 . # . 3 . .",
      ". . . . # . . . .",
      ". . . . # . . . .",
      ". . . . # . . . .",
      "6 . . . 5 . . . 9",
      "# . . . . . . . #",
      "7 # # # # # # # 8",
      "---",
      "1 open",
      "6 open"
    });
    m_openAction = new QAction(QIcon(new asciimage::IconEngine(openIcon)), "Open...", this);
    connect(m_openAction, SIGNAL(triggered()), this, SLOT(slotOpen()));

    asciimage::Image saveIcon({
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
    m_saveAction = new QAction(QIcon(new asciimage::IconEngine(saveIcon)), "Save", this);
    connect(m_saveAction, SIGNAL(triggered()), this, SLOT(slotSave()));

    asciimage::Image saveAsIcon({
      ". . . . 5 . . . .",
      ". . . . # . . . .",
      ". . 1 . # . 3 . .",
      ". . . # 5 # . . .",
      ". . . . 2 . . . .",
      ". . . . . . . . .",
      "6 . a . c . e . 9",
      "# . . . . . . . #",
      "7 # # # # # # # 8",
      "---",
      "1 open",
      "6 open"
    });
    m_saveAsAction = new QAction(QIcon(new asciimage::IconEngine(saveAsIcon)), "Save As", this);
    connect(m_saveAsAction, SIGNAL(triggered()), this, SLOT(slotSaveAs()));

    asciimage::Image closeIcon({
      ". . . 2 A 3 . . .",
      ". . . . # . . . .",
      ". . . # 1 # . . .",
      ". . # . # . # . .",
      ". # . 5 A 4 . # .",
      ". 1 . . . . . 1 .",
      ". # . . . . . # .",
      ". . # . . . # . .",
      ". . . # 1 # . . .",
      "---",
      "1 empty",
      "2 cutting"
    });
    m_closeAction = new QAction(QIcon(new asciimage::IconEngine(closeIcon)), "close", this);
    connect(m_closeAction, SIGNAL(triggered()), this, SLOT(slotClose()));
}


void MainWindow::setupMenus()
{
  QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(m_newAction);
  fileMenu->addSeparator();
  fileMenu->addAction(m_openAction);
  fileMenu->addSeparator();
  fileMenu->addAction(m_saveAction);
  fileMenu->addAction(m_saveAsAction);
  fileMenu->addSeparator();
  fileMenu->addAction(m_closeAction);
}


void MainWindow::setupToolbars()
{
  QToolBar* fileToolBar = new QToolBar(this);
  addToolBar(fileToolBar);

  fileToolBar->addAction(m_newAction);
  fileToolBar->addAction(m_openAction);
  fileToolBar->addAction(m_saveAction);
  fileToolBar->addAction(m_saveAsAction);
  fileToolBar->addAction(m_closeAction);
}


void MainWindow::closeEvent(QCloseEvent* event)
{
  if (m_modified)
  {
    const QMessageBox::StandardButton answer =
      QMessageBox::question(
        this,
        "Quit app",
        "The current asciimage has unsaved changes.\nSave them before quitting?",
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    if (answer == QMessageBox::Yes)
    {
      if (!save())
      {
        event->ignore();
        return;
      }
    }
    else if (answer == QMessageBox::No)
    {
      // nothing to do
    }
    else // Cancel
    {
      event->ignore();
      return;
    }
  }

  m_modified = false;
  event->accept();
}


void MainWindow::toggleOverwriteMode()
{
  m_ui->textEditor->setOverwriteMode(!m_ui->textEditor->overwriteMode());
}


bool MainWindow::save(bool silent)
{
  if (m_filename.isEmpty())
  {
    if (!silent)
    {
      return saveAs();
    }
    else
    {
      return false;
    }
  }
  else
  {
    QFile saveFile(m_filename);
    if (!saveFile.open(QIODevice::WriteOnly))
    {
      if (!silent)
      {
        QMessageBox::warning(
          this,
          "Save Asciimage",
          QString("Cannot save asciimage to file '%1'").arg(m_filename),
          QMessageBox::Ok);
      }
      return false;
    }

    saveFile.write(m_ui->textEditor->toPlainText().toLatin1());

    m_modified = false;
    updateTitle();

    return true;
  }
}


bool MainWindow::saveAs()
{
  QString filename = QFileDialog::getSaveFileName(this, "Save Asciimage", m_filename, "Text Files (*.txt)");

  if (filename.isEmpty())
  {
    return false;
  }

  QFile saveFile(filename);
  if (!saveFile.open(QIODevice::WriteOnly))
  {
    QMessageBox::warning(
      this,
      "Save Asciimage",
      QString("Cannot save asciimage to file '%1'").arg(filename),
      QMessageBox::Ok);
    return false;
  }

  saveFile.write(m_ui->textEditor->toPlainText().toLatin1());

  m_filename  = filename;
  m_modified = false;
  updateTitle();

  return true;
}


bool MainWindow::open(bool silent)
{
  QString fileName = QFileDialog::getOpenFileName(this, "Load Asciimage", m_filename, "Text Files (*.txt)");

  if (fileName.isEmpty())
  {
    return false;
  }

  QFile loadFile(fileName);

  if (!loadFile.open(QIODevice::ReadOnly))
  {
    if (!silent)
    {
      QMessageBox::warning(
        this,
        "Load Asciimage",
        QString("Cannot read asciimage from file '%1'").arg(fileName),
        QMessageBox::Ok);
    }
    return false;
  }

  QByteArray data = loadFile.readAll();
  m_ui->textEditor->setPlainText(data);
  m_modified = false;
  m_filename = fileName;
  updateTitle();

  return true;
}


void MainWindow::slotOpen()
{
  if (m_modified)
  {
    const QMessageBox::StandardButton answer =
      QMessageBox::question(
        this,
        "Open Asciimage",
        "The current asciimage has unsaved changes.\nSave them before opening a asciimage?",
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    if (answer == QMessageBox::Yes)
    {
      if (!save()) return;
    }
    else if (answer == QMessageBox::No)
    {
      // nothing to do
    }
    else // Cancel
    {
      return;
    }
  }

  open();
}


void MainWindow::slotSave()
{
  save();
}


void MainWindow::slotSaveAs()
{
  saveAs();
}


void MainWindow::slotChanged()
{
  m_modified = true;
  updateTitle();
}
