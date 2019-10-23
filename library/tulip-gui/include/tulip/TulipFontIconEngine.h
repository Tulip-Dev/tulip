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
///@cond DOXYGEN_HIDDEN

#ifndef TULIPFONTICONENGINE_H
#define TULIPFONTICONENGINE_H

#include <QIconEngine>
#include <QPixmap>
#include <QFont>
#include <tulip/tulipconf.h>

class TLP_QT_SCOPE TulipFontIconEngine : public QIconEngine {
  QString iconQString;
  QFont &font;

  QFont &init(const std::string &iconName);

public:
  TulipFontIconEngine(const std::string &iconName);
  TulipFontIconEngine(const QString &iconName);
  TulipFontIconEngine(const TulipFontIconEngine &engine)
      : QIconEngine(), iconQString(engine.iconQString), font(engine.font) {}

  TulipFontIconEngine *clone() const override {
    return new TulipFontIconEngine(*this);
  }

  void paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state) override;

  QPixmap pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state) override;

  static inline QIcon icon(const char *iconName) {
    return QIcon(new TulipFontIconEngine(std::string(iconName)));
  }

  static inline QIcon icon(const std::string &iconName) {
    return QIcon(new TulipFontIconEngine(iconName));
  }

  static inline QIcon icon(const QString &iconName) {
    return QIcon(new TulipFontIconEngine(iconName));
  }

  static inline QPixmap pixmap(const std::string &iconName, unsigned int height,
                               QIcon::Mode mode = QIcon::Normal, QIcon::State state = QIcon::On) {
    return TulipFontIconEngine(iconName).pixmap(QSize(height, height), mode, state);
  }

  static inline QPixmap pixmap(const QString &iconName, unsigned int height,
                               QIcon::Mode mode = QIcon::Normal, QIcon::State state = QIcon::On) {
    return TulipFontIconEngine(iconName).pixmap(QSize(height, height), mode, state);
  }
};

#endif
///@endcond
