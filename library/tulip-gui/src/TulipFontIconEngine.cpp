/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#include "tulip/TulipIconicFont.h"
#include "tulip/TulipFontIconEngine.h"
#include "tulip/TlpQtTools.h"

#include <QFile>
#include <QFontDatabase>
#include <QPainter>

#include <unordered_map>

using namespace tlp;

std::unordered_map<std::string, QString> qFontNames;
void TulipFontIconEngine::init(const std::string &iconName) {
  codePoint = TulipIconicFont::getIconCodePoint(iconName);
  auto fontFile = TulipIconicFont::getTTFLocation(iconName);
  if (qFontNames.find(fontFile) == qFontNames.end()) {
    // load the font file
    QFile res(tlpStringToQString(fontFile));
    if (!res.open(QIODevice::ReadOnly)) {
      qDebug() << "Error when opening file " << tlpStringToQString(fontFile);
      return;
    }
    QByteArray fontData(res.readAll());
    res.close();

    // fetch the given font
    auto fontId = QFontDatabase::addApplicationFontFromData(fontData);

    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    if (!fontFamilies.empty() ) {
      qFontNames[fontFile] = fontFamilies.at(0);
    } else {
      qDebug() << "No data found when loading file" << tlpStringToQString(fontFile);
      return;
    }
  }
  fontName = qFontNames[fontFile];
}

TulipFontIconEngine::TulipFontIconEngine(const std::string& iconName) {
  init(iconName);
}

TulipFontIconEngine::TulipFontIconEngine(const QString& iconName) {
  init(QStringToTlpString(iconName));
}

void TulipFontIconEngine::paint(QPainter* painter, const QRect& rect,
				QIcon::Mode mode, QIcon::State) {
  painter->save();

  // set the correct color
  QColor color(50,50,50);

  if ((mode == QIcon::Active) || (mode == QIcon::Selected))
    color.setRgb(10,10,10);
  else if(mode == QIcon::Disabled)
    color.setRgb(70,70,70,60);
  painter->setPen(color);

  // set the font
  QFont font(fontName);
  // add some 'padding' around the icon
  font.setPixelSize(qRound(rect.height() * 0.9));
  painter->setFont(font);
  
  painter->drawText(rect, QString(QChar(static_cast<int>(codePoint))),
		    QTextOption(Qt::AlignCenter|Qt::AlignVCenter));
  
  painter->restore();
}

QPixmap TulipFontIconEngine::pixmap(const QSize& size, QIcon::Mode mode, QIcon::State state) {
  QPixmap pm(size);
  pm.fill(Qt::transparent); // we need transparency
  QPainter painter(&pm);
  paint(&painter, QRect(QPoint(0,0), size), mode, state);

  return pm;
}
