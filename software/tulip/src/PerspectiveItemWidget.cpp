/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include "PerspectiveItemWidget.h"

#include <QtGui/QMouseEvent>
#include <tulip/PluginInformations.h>
#include "ui_PerspectiveItem.h"

PerspectiveItemWidget::PerspectiveItemWidget(tlp::PluginInformations *infos,QWidget *parent)
  : QWidget(parent), _perspectiveId(infos->identifier()), _ui(new Ui::PerspectiveItemData) {

  _ui->setupUi(this);
  _ui->icon->setPixmap(infos->iconPath());
  _ui->name->setText("<p><span style=\"font-size:large;\"><b>"
                     +
                     infos->name()
                     +
                     "</b></span></p>");
  _ui->description->setText("<p><span style=\"color:#626262;\">"
                            +
                            infos->shortDescription()
                            +
                            "</span></p>");
  connect(_ui->runButton,SIGNAL(clicked()),this,SIGNAL(selected()));
}

void PerspectiveItemWidget::focusInEvent(QFocusEvent *) {
  _ui->frame->setStyleSheet("QFrame#frame { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(243, 249, 255, 255), stop:1 rgba(232, 238, 244, 255));  border-left: 1px solid \"#C9C9C9\";  border-right: 1px solid \"#C9C9C9\";  border-top: 1px solid \"#C9C9C9\";  border-bottom: 1px solid \"#C9C9C9\";}");
}

void PerspectiveItemWidget::focusOutEvent(QFocusEvent *) {
  _ui->frame->setStyleSheet("QFrame#frame { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(245, 245, 245, 255), stop:1 rgba(225,225,225, 255));  border-left: 1px solid \"#C9C9C9\";  border-right: 1px solid \"#C9C9C9\";  border-top: 1px solid \"#C9C9C9\";  border-bottom: 1px solid \"#C9C9C9\";}");
}

void PerspectiveItemWidget::mouseDoubleClickEvent(QMouseEvent *e) {
  e->accept();
  emit selected();
}

