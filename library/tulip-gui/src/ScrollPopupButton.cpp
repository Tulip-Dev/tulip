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

#include "tulip/ScrollPopupButton.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QSlider>
#include <QCursor>
#include <QDebug>

ScrollPopupButton::ScrollPopupButton(QWidget *parent): QPushButton(parent) {
  _slider = new QSlider();
  _slider->setOrientation(Qt::Vertical);
  _slider->setWindowFlags(Qt::Popup);
  _slider->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Preferred);
  _slider->adjustSize();
  _slider->setStyleSheet("QSlider { background-color: white; border: 1px solid #C9C9C9 }");
  _slider->setFocusPolicy(Qt::StrongFocus);
  _slider->installEventFilter(this);
  connect(this,SIGNAL(clicked()),this,SLOT(showPopup()));
  connect(_slider,SIGNAL(valueChanged(int)),this,SIGNAL(valueChanged(int)));
}

void ScrollPopupButton::showPopup() {
  QPoint globalPos = QCursor::pos();
  _slider->setGeometry(globalPos.x()-5,globalPos.y()-290,width(),300);
  _slider->show();
  _slider->setFocus();
}

void ScrollPopupButton::hidePopup() {
  _slider->hide();
}

void ScrollPopupButton::setPopupVisible(bool f) {
  if (f)
    showPopup();
  else
    hidePopup();
}

void ScrollPopupButton::setValue(int v) {
  _slider->setValue(v);
}
int ScrollPopupButton::value() const {
  return _slider->value();
}
int ScrollPopupButton::maximum() const {
  return _slider->maximum();
}
void ScrollPopupButton::setMaximum(int v) {
  _slider->setMaximum(v);
}
int ScrollPopupButton::minimum() const {
  return _slider->minimum();
}
void ScrollPopupButton::setMinimum(int v) {
  _slider->setMinimum(v);
}
bool ScrollPopupButton::eventFilter(QObject *, QEvent *ev) {
  if (ev->type() == QEvent::KeyPress && static_cast<QKeyEvent*>(ev)->key() == Qt::Key_Escape) {
    hidePopup();
  }
  else if (ev->type() == QEvent::MouseButtonPress && !geometry().contains(static_cast<QMouseEvent*>(ev)->pos())) {
    hidePopup();
  }

  return false;
}
