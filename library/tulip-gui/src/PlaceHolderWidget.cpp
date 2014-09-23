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
#include "tulip/PlaceHolderWidget.h"

#include <QCloseEvent>
#include <QVBoxLayout>
#include <QWidget>
#include <QDebug>

PlaceHolderWidget::PlaceHolderWidget(QWidget *parent): QWidget(parent), _widget(NULL) {
  setLayout(new QVBoxLayout);
  layout()->setMargin(0);
  layout()->setSpacing(0);
}

void PlaceHolderWidget::setWidget(QWidget *widget) {
  if (_widget != NULL) {
    _widget->hide();
    layout()->removeWidget(_widget);
    _widget->setParent(NULL);
    _widget = NULL;
  }

  _widget = widget;

  if (_widget != NULL) {
    _widget->installEventFilter(this);
    layout()->addWidget(_widget);
    _widget->show();
  }
}

QWidget* PlaceHolderWidget::widget() const {
  return _widget;
}

void PlaceHolderWidget::resetWidget() {
  _widget = NULL;
}
