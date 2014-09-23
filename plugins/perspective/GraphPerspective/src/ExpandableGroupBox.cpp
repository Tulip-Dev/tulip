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
#include "ExpandableGroupBox.h"

#include <QVBoxLayout>
#include <QDebug>

ExpandableGroupBox::ExpandableGroupBox(QWidget *parent, const QString &title): QGroupBox(title,parent), _expanded(true), _widget(0) {
  setCheckable(true);
  setChecked(true);
  setExpanded(_expanded);
  connect(this,SIGNAL(toggled(bool)),this,SLOT(setExpanded(bool)));

  QVBoxLayout *lyt = new QVBoxLayout;
  lyt->setContentsMargins(0,0,0,0);
  lyt->setMargin(0);
  lyt->setSpacing(0);
  setLayout(lyt);
}

ExpandableGroupBox::~ExpandableGroupBox() {
}

void ExpandableGroupBox::setExpanded(bool e) {
  _expanded=e;
  // As told in Qt stylesheet reference, this force recomputation of stylesheet based on dynamic properties
  setStyleSheet(styleSheet());

  if (_widget) {
    _widget->setVisible(e);
  }
}

void ExpandableGroupBox::setWidget(QWidget *w) {
  delete _widget;
  _widget = w;

  if (_widget)
    layout()->addWidget(_widget);
}

QWidget *ExpandableGroupBox::takeWidget() {
  QWidget *result = _widget;
  _widget = NULL;

  if (result)
    layout()->removeWidget(result);

  return result;
}
