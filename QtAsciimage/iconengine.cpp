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

#include "iconengine.h"
#include <QPainter>

asciimage::IconEngine::IconEngine(const asciimage::Image& image) :
  m_image(image)
{}


asciimage::IconEngine::~IconEngine()
{}


QSize asciimage::IconEngine::actualSize(const QSize& size, QIcon::Mode mode, QIcon::State state)
{
  Q_UNUSED(mode);
  Q_UNUSED(state);

  if (!m_image.isValid() || m_image.width() == 0 || m_image.height() == 0)
  {
    return QSize();
  }

  int scale = qMin(size.width() / m_image.width(), size.height() / m_image.height());
  return QSize(scale * m_image.width(), scale * m_image.height());
}


QIconEngine* asciimage::IconEngine::clone() const
{
  return new IconEngine(m_image);
}


void asciimage::IconEngine::paint(QPainter* painter, const QRect& rect, QIcon::Mode mode, QIcon::State state)
{
  Q_UNUSED(state);

  if (!m_image.isValid() || m_image.width() == 0 || m_image.height() == 0)
  {
    return;
  }

  int scale = qMin(rect.width() / m_image.width(), rect.height() / m_image.height());
  if (scale == 0)
  {
    return;
  }

  QColor color;
  switch (mode)
  {
  case QIcon::Normal:
    color = QColor(50, 50, 50);
    break;
  case QIcon::Disabled:
    color = QColor(70, 70, 70);
    break;
  case QIcon::Active:
  case QIcon::Selected:
    color = QColor(10, 10, 10);
    break;
  }

  painter->save();
  painter->translate(0.5 * (rect.width() - scale * m_image.width()), 0.5 * (rect.height() - scale * m_image.height()));
  m_image.paint(painter, scale, color);
  painter->restore();
}

QPixmap asciimage::IconEngine::pixmap(const QSize& size, QIcon::Mode mode, QIcon::State state)
{
  QImage image(size, QImage::Format_ARGB32);
  image.fill(qRgba(0,0,0,0));
  QPixmap pix = QPixmap::fromImage(image, Qt::NoFormatConversion);
  {
     QPainter painter(&pix);
     QRect r(QPoint(0.0, 0.0), size);
     this->paint(&painter, r, mode, state);
  }
  return pix;
}
