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

std::unordered_map<std::string, QFont> qFonts;
QFont nullFont;

QFont &TulipFontIconEngine::init(const std::string &iconName) {
  // first get iconQString
  iconQString = QString(TulipIconicFont::getIconUtf8String(iconName).c_str());
  // then get font
  auto &&fontFile = TulipIconicFont::getTTFLocation(iconName);
  if (qFonts.find(fontFile) == qFonts.end()) {
    // load the font file
    auto fontId = QFontDatabase::addApplicationFont(tlpStringToQString(fontFile));
    if (fontId == -1) {
      qDebug() << "Error when loading font file " << tlpStringToQString(fontFile);
      return nullFont;
    }

    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    if (!fontFamilies.empty()) {
      qFonts.emplace(fontFile, fontFamilies.at(0));
    } else {
      qDebug() << "No data found when loading file" << tlpStringToQString(fontFile);
      return nullFont;
    }
  }
  return qFonts[fontFile];
}

TulipFontIconEngine::TulipFontIconEngine(const std::string &iconName, bool dm)
    : font(init(iconName)), darkMode(dm) {}

TulipFontIconEngine::TulipFontIconEngine(const QString &iconName, bool dm)
    : font(init(QStringToTlpString(iconName))), darkMode(dm) {}

void TulipFontIconEngine::paint(QPainter *painter, const QRect &rect, QIcon::Mode mode,
                                QIcon::State) {
  painter->save();

  // set the correct color
  QColor color(50, 50, 50);

  if ((mode == QIcon::Active) || (mode == QIcon::Selected))
    color.setRgb(10, 10, 10);
  else if (mode == QIcon::Disabled)
    color.setRgb(70, 70, 70, 60);
  // invert color if in dark mode
  if (darkMode)
    color.setRgb(255 - color.red(), 255 - color.green(), 255 - color.blue());
  painter->setPen(color);

  // add some 'padding' around the icon
  font.setPixelSize(qRound(rect.height() * 0.9));
  // set the font
  painter->setFont(font);

  painter->drawText(rect, iconQString, QTextOption(Qt::AlignCenter | Qt::AlignVCenter));

  painter->restore();
}

QPixmap TulipFontIconEngine::pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state) {
  QPixmap pm(size);
  pm.fill(Qt::transparent); // we need transparency
  QPainter painter(&pm);
  paint(&painter, QRect(QPoint(0, 0), size), mode, state);

  return pm;
}
