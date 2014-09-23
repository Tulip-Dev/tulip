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

#include "OutputPanelButton.h"

#include <QPainter>
#include <QApplication>

OutputPanelButton::OutputPanelButton(QWidget *parent): QPushButton(parent), _number(0) {}

QSize OutputPanelButton::sizeHint() const {
  ensurePolished();

  QSize s = fontMetrics().size(Qt::TextSingleLine, _title);

  // Expand to account for border image set by stylesheet above
  s.rwidth() += 19 + 5 + 2;
  s.rheight() += 2 + 2;

  return s.expandedTo(QApplication::globalStrut());
}

#include <iostream>

void OutputPanelButton::paintEvent(QPaintEvent* event) {
  QPushButton::paintEvent(event);

  const QFontMetrics fm = fontMetrics();
  int baseLine = (height() - fm.height() + 1) / 2 + fm.ascent();
  const int numberWidth = fm.width(QString::number(_number));

  QPainter p(this);
  p.setFont(font());
  p.setPen(Qt::white);
  p.drawText((20 - numberWidth) / 2, baseLine, QString::number(_number));

  if (!isChecked())
    p.setPen(Qt::black);

#ifdef __APPLE__ // On MacOS: force the baseLine to 2 to get the text drawn in the button
  baseLine = 2;
#endif
  int leftPart = 22;

  p.drawText(leftPart, baseLine, width()-leftPart, height()-baseLine*2, Qt::AlignCenter | Qt::AlignVCenter, fm.elidedText(_title, Qt::ElideMiddle, width() - leftPart - 1));
}

void OutputPanelButton::setGlowColor(const QColor &color) {
  QString colorStr="rgba(";
  QString str;

  str.setNum(color.red());
  colorStr.append(str+",");

  str.setNum(color.green());
  colorStr.append(str+",");

  str.setNum(color.blue());
  colorStr.append(str+",");

  str.setNum(color.alpha());
  colorStr.append(str+")");

  setStyleSheet(QString("OutputPanelButton { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0,")
                +"stop:0 rgba(255,255,255, 0), stop:0.25 "+colorStr+", stop:0.9 " +colorStr
                +", stop:1 rgba(255,255,255, 0)); }");
}

void OutputPanelButton::resetBackgroundColor() {
  setStyleSheet("OutputPanelButton { background-color: transparent; }");
}
