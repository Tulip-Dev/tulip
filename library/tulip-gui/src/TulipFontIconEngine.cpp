/*
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#include <QFontMetrics>
#include <QPainter>

#include <tulip/tuliphash.h>
#include <unordered_set>

using namespace tlp;

tlp_hash_map<std::string, QFont> qFonts;
std::unordered_set<std::string> unknownIcons;
QFont nullFont;

void TulipFontIconEngine::init(const std::string &iconName) {
  // get iconQString
  iconQString = QString(TulipIconicFont::getIconUtf8String(iconName).c_str());
  if (iconQString.isEmpty()) {
    if (unknownIcons.find(iconName) == unknownIcons.end()) {
      unknownIcons.insert(iconName);
      tlp::warning() << "Warning: icon \"" << iconName.c_str() << "\" does not exist" << std::endl;
    }
    // use fas-question instead
    init("fas-question");
  } else {
    // then get font
    auto &&fontFile = TulipIconicFont::getTTFLocation(iconName);
    if (qFonts.find(fontFile) == qFonts.end()) {
      // load the font file
      auto fontId = QFontDatabase::addApplicationFont(tlpStringToQString(fontFile));
      if (fontId == -1) {
        tlp::warning() << "Error when loading font file " << fontFile << std::endl;
        font = nullFont;
        return;
      }

      QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
      if (!fontFamilies.empty()) {
        qFonts.emplace(fontFile, fontFamilies.at(0));
      } else {
        tlp::warning() << "No data found when loading file " << fontFile << std::endl;
        font = nullFont;
        return;
      }
    }
    font = qFonts[fontFile];
    font.setStyleName(tlpStringToQString(TulipIconicFont::getIconStyle(iconName)));
  }
}

TulipFontIconEngine::TulipFontIconEngine(const std::string &iconName, bool dm) : darkMode(dm) {
  init(iconName);
}

TulipFontIconEngine::TulipFontIconEngine(const QString &iconName, bool dm) : darkMode(dm) {
  init(QStringToTlpString(iconName));
}

void TulipFontIconEngine::paint(QPainter *painter, const QRect &rect, QIcon::Mode mode,
                                QIcon::State) {
  painter->save();

  // compute a convenient pen color
  QColor color(50, 50, 50);

  if ((mode == QIcon::Active) || (mode == QIcon::Selected))
    color.setRgb(10, 10, 10);
  else if (mode == QIcon::Disabled)
    color.setRgb(70, 70, 70, 60);
  // invert color if in dark mode
  if (darkMode)
    color.setRgb(255 - color.red(), 255 - color.green(), 255 - color.blue());
  painter->setPen(color);

  // ensure icon full display in the given rect
  // so compute a sub rect with same h/w ratio as icon bounding rect
  QFontMetrics fm(font);
  auto br = fm.boundingRect(iconQString);
  auto maxH = (br.height() * rect.width()) / br.width();
  auto srect(rect);
  if (srect.height() > maxH)
    // reduce height
    srect.setHeight(maxH);
  // add some 'padding' around the icon
  font.setPixelSize(qRound(srect.height() * 0.9));

  // set the font
  painter->setFont(font);

  painter->drawText(srect, iconQString, QTextOption(Qt::AlignCenter | Qt::AlignVCenter));

  painter->restore();
}

QPixmap TulipFontIconEngine::pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state) {
  // ensure icon full display, so compute an enlarged pixmap size
  // with same h/w ratio as icon bounding rect
  QFontMetrics fm(font);
  auto br = fm.boundingRect(iconQString);
  auto sz(size);
  auto minW = (sz.height() * br.width()) / br.height();
  if (sz.width() < minW)
    // enlarge width
    sz.setWidth(minW);

  QPixmap pm(sz);
  pm.fill(Qt::transparent); // we need transparency
  QPainter painter(&pm);
  paint(&painter, QRect(QPoint(0, 0), sz), mode, state);

  return pm;
}
