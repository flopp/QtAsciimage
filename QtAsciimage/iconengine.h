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

#include "image.h"

// QIconEngineV2 is replaced by QIconEngine in Qt >= 5.0.0
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    #include <QIconEngine>
    #define ICON_ENGINE_BASE_CLASS QIconEngine
#else
    #include <QIconEngineV2>
    #define ICON_ENGINE_BASE_CLASS QIconEngineV2
#endif

namespace asciimage
{
  class IconEngine : public ICON_ENGINE_BASE_CLASS
  {
    public:
      IconEngine(const asciimage::Image& image);
      virtual ~IconEngine();

      virtual QSize actualSize(const QSize& size, QIcon::Mode mode, QIcon::State state) override;
      virtual ICON_ENGINE_BASE_CLASS* clone() const override { return new IconEngine(m_image); }
      virtual void paint(QPainter* painter, const QRect& rect, QIcon::Mode mode, QIcon::State state) override;
      virtual QPixmap pixmap(const QSize& size, QIcon::Mode mode, QIcon::State state) override;

    private:
      asciimage::Image m_image;
  };
}

#endif
