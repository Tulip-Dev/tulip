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
#include "PerspectiveItemWidget.h"

#include "ui_PerspectiveItem.h"

#include <QMouseEvent>

#include <tulip/PluginLister.h>
#include "TulipMainWindow.h"

using namespace tlp;

PerspectiveItemWidget::PerspectiveItemWidget(const QString& perspectiveName,QWidget *parent)
  : QWidget(parent), _perspectiveName(perspectiveName), _ui(new Ui::PerspectiveItemData) {
  _ui->setupUi(this);
  _ui->name->setText(_perspectiveName);
  const tlp::Plugin& info = PluginLister::instance()->pluginInformation(_perspectiveName.toStdString());
  _ui->description->setText(info.info().c_str());
  _ui->icon->setPixmap(QPixmap(info.icon().c_str()));
}

void PerspectiveItemWidget::run() {
  TulipMainWindow::instance()->createPerspective(_perspectiveName);
  emit selected();
}

void PerspectiveItemWidget::mouseDoubleClickEvent(QMouseEvent *) {
  run();
}

void PerspectiveItemWidget::focusInEvent(QFocusEvent *) {
  _ui->frame->setProperty("highlighted",true);
  _ui->frame->setStyleSheet(_ui->frame->styleSheet());
}

void PerspectiveItemWidget::focusOutEvent(QFocusEvent *) {
  _ui->frame->setProperty("highlighted",false);
  _ui->frame->setStyleSheet(_ui->frame->styleSheet());
}

