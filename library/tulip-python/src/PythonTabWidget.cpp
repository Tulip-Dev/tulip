/**
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

#include "tulip/PythonInterpreter.h"
#include "tulip/PythonTabWidget.h"

#include <QPainter>
#include <QTabBar>
#include <QLinearGradient>

using namespace tlp;

PythonTabWidget::PythonTabWidget(QWidget *parent)
    : QTabWidget(parent), _drawGradient(false), _textColor(QColor(100, 100, 100)) {}

void PythonTabWidget::paintEvent(QPaintEvent *event) {
  QTabWidget::paintEvent(event);
  static QImage pythonLogoImg(":/tulip/gui/icons/python.png");
  static QString pythonVersion(PythonInterpreter::getInstance()->getPythonFullVersionStr());
  QPainter painter(this);
  painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing |
                         QPainter::SmoothPixmapTransform);

  if (_drawGradient) {
    QLinearGradient gradient(width() / 2, tabBar()->pos().y(), width() / 2,
                             tabBar()->pos().y() + tabBar()->height());
    gradient.setColorAt(0.0, "#838383");
    gradient.setColorAt(0.4, "#707070");
    gradient.setColorAt(0.401, "#636363");
    gradient.setColorAt(1.0, "#4a4a4a");
    painter.fillRect(x(), tabBar()->pos().y(), width(), tabBar()->height(), gradient);
  }

  painter.setPen(_textColor);

  int imageWidth = int(25 * tabBar()->height() / 27.0);
  auto pyVersion = QString("Python ") + pythonVersion;
  int labelWidth = QFontMetrics(painter.font()).boundingRect(pyVersion).width();
  int offset = tabBar()->height() - imageWidth;
  QRectF rect(width() - (imageWidth + labelWidth + 10),
              tabBar()->pos().y() + offset / 2, imageWidth, imageWidth);
  QRectF rect2(width() - labelWidth - 5, tabBar()->pos().y(), labelWidth,
               tabBar()->height());
  painter.drawImage(rect, pythonLogoImg);
  painter.drawText(rect2, Qt::AlignCenter, pyVersion);
}
