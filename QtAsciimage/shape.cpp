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

#include "shape.h"
#include "style.h"
#include <QPainter>

QRect asciimage::Shape::boundingRect() const
{
  if (m_points.isEmpty())
  {
    return QRect();
  }

  QPoint min = m_points.first();
  QPoint max = min;

  for (const QPoint& p : points())
  {
    if (p.x() < min.x()) { min.setX(p.x()); }
    if (p.y() < min.y()) { min.setY(p.y()); }
    if (p.x() > max.x()) { max.setX(p.x()); }
    if (p.y() > max.y()) { max.setY(p.y()); }
  }

  return QRect(min, max);
}


void asciimage::Shape::render(QPainter* painter, int scale, const Style& style) const
{
  painter->save();

  if (style.isCutting())
  {
    painter->setCompositionMode(QPainter::CompositionMode_Clear);
  }

  switch (type())
  {
  case Type::EMPTY:
    // nothing to do
    break;

  case Type::POINT:
    renderPoint(painter, scale, style);
    break;

  case Type::LINE:
    renderLine(painter, scale, style);
    break;

  case Type::POLYGON:
    renderPolygon(painter, scale, style);
    break;

  case Type::ELLIPSE:
    renderEllipse(painter, scale, style);
    break;
  }

  painter->restore();
}


void asciimage::Shape::renderPoint(QPainter* painter, int scale, const Style& style) const
{
  Q_ASSERT(type() == Type::POINT);
  Q_ASSERT(points().size() == 1);

  if (scale == 1)
  {
    painter->setPen(style.color());
    painter->drawPoint(points().first());
  }
  else
  {
    painter->setPen(style.color());
    painter->setBrush(style.color());
    painter->drawRect(scaledOuterRect(boundingRect(), scale));
  }
}


bool asciimage::Shape::isStraightLine() const
{
  if (type() != Type::LINE)
  {
    return false;
  }

  if (points().size() != 2)
  {
    return false;
  }

  const QPoint& p0 = points()[0];
  const QPoint& p1 = points()[1];

  return (p0.x() == p1.x() || p0.y() == p1.y());
}


void asciimage::Shape::renderLine(QPainter* painter, int scale, const Style& style) const
{
  Q_ASSERT(type() == Type::LINE);
  Q_ASSERT(points().size() == 2);

  if (scale == 1)
  {
    if (!isStraightLine())
    {
      painter->setRenderHint(QPainter::Antialiasing, true);
    }
    painter->setPen(style.color());
    painter->drawLine(points()[0], points()[1]);
  }
  else
  {
    if (isStraightLine())
    {
      const QRect b = boundingRect();
      painter->setPen(style.color());
      painter->setBrush(style.color());
      painter->drawRect(scaledOuterRect(b, scale));
    }
    else
    {
      painter->setRenderHint(QPainter::Antialiasing, true);
      painter->setPen(QPen(style.color(), scale, Qt::SolidLine, Qt::SquareCap));
      painter->drawLine(p(points()[0], scale), p(points()[1], scale));
    }
  }
}


bool asciimage::Shape::isRectangle() const
{
  if (type() != Type::POLYGON)
  {
    return false;
  }

  if (points().size() != 4)
  {
    return false;
  }

  const QPoint& p0 = points()[0];
  const QPoint& p1 = points()[1];
  const QPoint& p2 = points()[2];
  const QPoint& p3 = points()[3];

  return
    (p0.x() == p1.x() && p1.y() == p2.y() && p2.x() == p3.x() && p3.y() == p0.y()) ||
    (p0.y() == p1.y() && p1.x() == p2.x() && p2.y() == p3.y() && p3.x() == p0.x());
}


void asciimage::Shape::renderPolygon(QPainter* painter, int scale, const Style& style) const
{
  Q_ASSERT(type() == Type::POLYGON);

  if (!style.isClosed())
  {
    renderLines(painter, scale, style);
    return;
  }

  Q_ASSERT(points().size() >= 3);

  if (isRectangle() && style.isFilled())
  {
    const QRect b = boundingRect();
    painter->setPen(style.color());
    painter->setBrush(style.color());
    painter->drawRect(scaledOuterRect(b, scale));
  }
  else
  {
    QPolygonF poly;
    for (const QPoint& point : points())
    {
      poly << p(point, scale);
    }
    poly << p(points().first(), scale);

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(QPen(style.color(), scale, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));
    if (style.isFilled())
    {
      painter->setBrush(style.color());
    }
    painter->drawPolygon(poly);
  }
}


void asciimage::Shape::renderLines(QPainter* painter, int scale, const Style& style) const
{
  Q_ASSERT(type() == Type::POLYGON);
  Q_ASSERT(points().size() >= 2);
  Q_ASSERT(!style.isClosed());

  QPolygonF lines;
  for (const QPoint& point : points())
  {
    lines << p(point, scale);
  }

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(QPen(style.color(), scale, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));
  painter->drawPolyline(lines);
}


void asciimage::Shape::renderEllipse(QPainter* painter, int scale, const Style& style) const
{
  Q_ASSERT(type() == Type::ELLIPSE);
  Q_ASSERT(points().size() >= 3);

  painter->setRenderHint(QPainter::Antialiasing, true);

  const QRect rect = boundingRect();

  if (style.isFilled())
  {
    painter->setPen(style.color());
    painter->setBrush(style.color());
    painter->drawEllipse(scaledOuterRect(rect, scale).adjusted(0,0,1,1));
  }
  else
  {
    painter->setPen(QPen(style.color(), scale));
    painter->drawEllipse(QRectF(scaledOuterRect(rect, scale).adjusted(0, 0, 1, 1)).adjusted(0.5 * scale, 0.5 * scale, -0.5 * scale, -0.5 * scale));
  }
}


QRect asciimage::Shape::scaledOuterRect(const QRect& r, int scale) const
{
  return QRect(scale * r.left(), scale * r.top(), scale * r.width() - 1, scale * r.height() - 1);
}
