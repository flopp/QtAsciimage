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

#include "image.h"
#include "shape.h"
#include <qmath.h>
#include <QPainter>
#include <QVariantMap>

QString asciimage::Image::m_identifiers =
  "123456789"                  // '0' is missing intentionally
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
  "abcdefghijklmnpqrstuvwxyz"; // 'o' is missing intentionally


asciimage::Image::Image() :
    m_isValid(false),
    m_width(0),
    m_height(0)
{}


asciimage::Image::Image(const QString& data) :
    m_isValid(false),
    m_width(0),
    m_height(0)
{
  parse(data);
}


asciimage::Image::Image(const QList<QString>& data) :
    m_isValid(false),
    m_width(0),
    m_height(0)
{
  parse(data);
}


asciimage::Image::~Image()
{
}


bool asciimage::Image::parse(const QList<QString>& data)
{
  m_shapes.clear();
  m_styles.clear();
  m_isValid = false;
  m_width = 0;
  m_height = 0;

  QList<QString> imageData;
  QList<QString> stylesData;
  bool inStyles = false;
  for (const QString& row : data)
  {
    if (!inStyles)
    {
      if (row.indexOf("---") != 0)
      {
        imageData << row;
      }
      else
      {
        inStyles = true;
      }
    }
    else
    {
      stylesData << row;
    }
  }

  parseImage(imageData);
  parseStyles(stylesData);

  return isValid();
}


void asciimage::Image::paint(QPainter* painter, int scale, const QColor& defaultColor) const
{
  if (!isValid() || m_width == 0 || m_height == 0 || scale <= 0)
  {
    return;
  }

  for (const QSharedPointer<Shape> shape : m_shapes)
  {
    Style style;
    if (m_styles.contains(shape->identifier()))
    {
      style = m_styles.value(shape->identifier());
    }
    if (!style.color().isValid())
    {
      style.setColor(defaultColor);
    }
    shape->render(painter, scale, style);
  }
}


QImage asciimage::Image::render(int scale, const QColor& defaultColor) const
{
  if (!isValid() || m_width == 0 || m_height == 0 || scale <= 0)
  {
    return QImage();
  }

  QImage image(scale * m_width, scale * m_height, QImage::Format_ARGB32);
  image.fill(Qt::transparent);

  QPainter painter(&image);

  paint(&painter, scale, defaultColor);

  return image;
}


QHash<QChar, QList<QPoint>> asciimage::Image::getXyData(const QList<QString>& data)
{
  QHash<QChar, QList<QPoint>> xyData;

  for (int y = 0; y != data.size(); ++y)
  {
    const QString& row = data[y];
    for (int x = 0; x != row.size(); ++x)
    {
      xyData[row[x]] << QPoint(x, y);
    }
  }
  return xyData;
}


void asciimage::Image::parseImage(const QList<QString>& data)
{
  m_isValid = false;
  m_shapes.clear();
  m_width = 0;
  m_height = 0;

  QList<QString> rows;
  for (const QString& row : data)
  {
    QString sanitized;
    for (const QChar& c : row)
    {
      if (!c.isSpace()) sanitized += c;
    }
    if (sanitized.isEmpty())
    {
      continue;
    }

    if (m_width == 0)
    {
      m_width = sanitized.size();
    }
    else if (sanitized.size() != m_width)
    {
      m_width = 0;
      m_height = 0;
      m_isValid = false;
      return;
    }

    ++m_height;
    rows << sanitized;
  }

  const QHash<QChar, QList<QPoint>> xyData = getXyData(rows);
  QSharedPointer<Shape> shape;
  for (QChar identifier : m_identifiers)
  {
    const QList<QPoint> coordinates = xyData.value(identifier);
    if (coordinates.isEmpty())
    {
      shape.clear();
    }
    else if (coordinates.size() == 1)
    {
      if (!shape)
      {
        shape = QSharedPointer<Shape>(new Shape);
        shape->setType(Shape::Type::POINT);
        shape->setIdentifier(identifier);
        shape->appendPoint(coordinates.first());
        m_shapes << shape;
      }
      else
      {
        shape->appendPoint(coordinates.first());
        if (shape->points().size() == 2)
        {
          shape->setType(Shape::Type::LINE);
        }
        else
        {
          shape->setType(Shape::Type::POLYGON);
        }
      }
    }
    else if (coordinates.size() == 2)
    {
      shape = QSharedPointer<Shape>(new Shape);
      shape->setType(Shape::Type::LINE);
      shape->setIdentifier(identifier);
      shape->setPoints(coordinates);
      m_shapes << shape;

      shape.clear();
    }
    else /*if (coordinates.size() >= 3)*/
    {
      shape = QSharedPointer<Shape>(new Shape);
      shape->setType(Shape::Type::ELLIPSE);
      shape->setIdentifier(identifier);
      shape->setPoints(coordinates);
      m_shapes << shape;

      shape.clear();
    }
  }

  m_isValid = true;
}


QColor asciimage::Image::parseColor(const QString& s) const
{
  QRegExp r("^#[0-9a-f]{6}$", Qt::CaseInsensitive);
  if (r.indexIn(s) == 0)
  {
    return QColor(s);
  }

  return QColor();
}


void asciimage::Image::parseStyles(const QList<QString>& data)
{
  m_styles.clear();
  for (const QString& row : data)
  {
    // X [color=#XXXXXX] [open] [empty] [cutting]
    QStringList items = row.split(' ', QString::SkipEmptyParts);
    if (items.size() <= 1)
    {
      continue;
    }

    if (items[0].size() != 1)
    {
      continue;
    }
    const QChar id = items[0][0];
    if (m_identifiers.indexOf(id) < 0)
    {
      continue;
    }
    items.pop_front();

    Style style;
    for (const QString& item : items)
    {
      if (item.indexOf("color=", 0, Qt::CaseInsensitive) == 0)
      {
        QStringList x = item.split('=');
        if (x.size() != 2)
        {
          continue;
        }
        style.setColor(parseColor(x[1]));
      }
      else if (item.compare("open", Qt::CaseInsensitive) == 0)
      {
        style.setClosed(false);
      }
      else if (item.compare("empty", Qt::CaseInsensitive) == 0)
      {
        style.setFilled(false);
      }
      else if (item.compare("cutting", Qt::CaseInsensitive) == 0)
      {
        style.setCutting(true);
      }
    }

    m_styles.insert(id, style);
  }
}
