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

#ifndef QT_ASCIIMAGE_IMAGE_H
#define QT_ASCIIMAGE_IMAGE_H

#include "style.h"
#include <QHash>
#include <QImage>
#include <QList>
#include <QPoint>
#include <QString>
class QPainter;

namespace asciimage
{
  class Shape;

  class Image {
    public:
      Image();
      explicit Image(const QList<QString>& data);
      explicit Image(const QString& data);
      ~Image();

      bool parse(const QList<QString>& data);
      bool parse(const QString& data) { return parse(data.split('\n')); }

      bool isValid() const { return m_isValid; }
      int width() const { return m_width; }
      int height() const { return m_height; }

      void paint(QPainter* painter, int scale, const QColor& defaultColor = Qt::black) const;
      QImage render(int scale, const QColor& defaultColor = Qt::black) const;

      static QString identifiers() { return m_identifiers; }

    private:
      void parseImage(const QList<QString>& data);
      void parseStyles(const QList<QString>& data);
      static QHash<QChar, QList<QPoint>> getXyData(const QList<QString>& data);

      QColor parseColor(const QString& s) const;

    private:
      static QString m_identifiers;
      bool m_isValid;
      int m_width;
      int m_height;

      QList<QSharedPointer<Shape>> m_shapes;
      QHash<QChar, Style> m_styles;
  };
}

#endif
