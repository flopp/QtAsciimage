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

#ifndef QT_ASCIIMAGE_SHAPE_H
#define QT_ASCIIMAGE_SHAPE_H

#include <QChar>
#include <QList>
#include <QPoint>
#include <QRect>
class QPainter;

namespace asciimage
{
  class Style;

  class Shape
  {
    public:
      enum class Type
      {
        EMPTY,
        POINT,
        LINE,
        POLYGON,
        ELLIPSE
      };

      Shape() : m_type(Type::EMPTY) {}

      Type type() const { return m_type; }
      void setType(Type type) { m_type = type; }

      const QChar& identifier() const { return m_identifier; }
      void setIdentifier(const QChar& id) { m_identifier = id; }

      const QList<QPoint>& points() const { return m_points; }
      void setPoints(const QList<QPoint>& p) { m_points = p; }
      void appendPoint(const QPoint& p) { m_points << p; }

      QRect boundingRect() const;

      void render(QPainter* painter, int scale, const Style& style) const;

    private:
      bool isStraightLine() const;
      bool isRectangle() const;
      static QPointF p(QPoint point, int scale) { return QPointF((point.x() + 0.5) * scale, (point.y() + 0.5) * scale); }
      void renderPoint(QPainter* painter, int scale, const Style& style) const;
      void renderLine(QPainter* painter, int scale, const Style& style) const;
      void renderPolygon(QPainter* painter, int scale, const Style& style) const;
      void renderLines(QPainter* painter, int scale, const Style& style) const;
      void renderEllipse(QPainter* painter, int scale, const Style& style) const;

      QRect scaledOuterRect(const QRect& r, int scale) const;
      QRect scaledInnerRect(const QRect& r, int scale) const;

    private:
      Type m_type;
      QChar m_identifier;
      QList<QPoint> m_points;
  };
}

#endif
