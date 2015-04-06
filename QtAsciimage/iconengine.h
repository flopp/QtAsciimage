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

#ifndef QT_ASCIIMAGE_ICON_ENGINE_H
#define QT_ASCIIMAGE_ICON_ENGINE_H

#include <QIconEngine>
#include "image.h"

namespace asciimage
{
  class IconEngine : public QIconEngine
  {
    public:
      IconEngine(const asciimage::Image& image);
      virtual ~IconEngine();

      virtual QSize actualSize(const QSize& size, QIcon::Mode mode, QIcon::State state) override;
      virtual QIconEngine* clone() const override;
      virtual void paint(QPainter* painter, const QRect& rect, QIcon::Mode mode, QIcon::State state) override;
      virtual QPixmap pixmap(const QSize& size, QIcon::Mode mode, QIcon::State state) override;

    private:
      asciimage::Image m_image;
  };
}

#endif
