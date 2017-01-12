/**
 *
 * This file is part of Tulip (www.tulip-software.org)
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

#include "tulip/PythonInterpreter.h"
#include "tulip/PythonTabWidget.h"
#include "tulip/FontIconManager.h"

#include <QPainter>
#include <QTabBar>
#include <QLinearGradient>

using namespace tlp;

PythonTabWidget::PythonTabWidget(QWidget *parent) : QTabWidget(parent), _drawGradient(false), _textColor(QColor(100, 100, 100)) {
}

void PythonTabWidget::paintEvent(QPaintEvent *event) {
  QTabWidget::paintEvent(event);
  static QString pythonVersion(PythonInterpreter::getInstance()->getPythonVersionStr());
  QPainter painter(this);
  painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

  painter.fillRect(x(), tabBar()->pos().y(), width(), tabBar()->height(), QColor("#404244"));

  painter.setPen(_textColor);
#ifndef __APPLE__
  painter.setFont(QFont("Arial", static_cast<int>(12 * tabBar()->height() / 27.0)));
#else
  painter.setFont(QFont("Arial", static_cast<int>(18 * tabBar()->height() / 27.0)));
#endif
  int firstLabelWidth = static_cast<int>(120 * tabBar()->height() / 27.0);
  int imageWidth = static_cast<int>(30 * tabBar()->height() / 27.0);
  int secondLabelWidth = static_cast<int>(120 * tabBar()->height() / 27.0);
  QRectF rect(width() - (firstLabelWidth + imageWidth + secondLabelWidth), tabBar()->pos().y(), firstLabelWidth, tabBar()->height());
  QRect rect2(width() - (imageWidth + secondLabelWidth), tabBar()->pos().y(), imageWidth, tabBar()->height());
  QRectF rect3(width() - secondLabelWidth, tabBar()->pos().y(), secondLabelWidth, tabBar()->height());
  painter.drawText(rect, Qt::AlignCenter, "Powered by ");
  painter.drawPixmap(rect2, FontIconManager::instance()->getMaterialDesignIcon(md::languagepython, _textColor, 0.9).pixmap(QSize(32, 32)));
  painter.drawText(rect3, Qt::AlignCenter, QString("Python ") + pythonVersion);
}
