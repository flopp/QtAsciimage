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

#ifndef QT_ASCIIMAGE_STYLE_H
#define QT_ASCIIMAGE_STYLE_H

#include <QColor>

namespace asciimage
{
  class Style
  {
    public:
      Style() : m_color(), m_filled(true), m_closed(true), m_cutting(false) {}
      void setColor(const QColor& color) { m_color = color; }
      void setFilled(bool filled) { m_filled = filled; }
      void setClosed(bool closed) { m_closed = closed; }
      void setCutting(bool cutting) { m_cutting = cutting; }

      const QColor& color() const { return m_color; }
      bool isFilled() const { return m_filled; }
      bool isClosed() const { return m_closed; }
      bool isCutting() const { return m_cutting; }

    private:
      QColor m_color;
      bool m_filled;
      bool m_closed;
      bool m_cutting;
  };
}

#endif
