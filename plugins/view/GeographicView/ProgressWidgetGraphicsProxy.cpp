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

#include <QPainter>

#include "ProgressWidgetGraphicsProxy.h"
#include "ui_ProgressWidget.h"

using namespace tlp;

ProgressWidget::ProgressWidget(QWidget *parent) : QWidget(parent), _ui(new Ui::ProgressWidgetData), cancelClicked(true) {
  _ui->setupUi(this);
  connect(_ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelButtonClicked()));
}

ProgressWidget::~ProgressWidget() {
  delete _ui;
}

void ProgressWidget::setComment(const QString &comment) {
  _ui->commentLabel->setText(comment);
}

void ProgressWidget::setProgress(int value, int max) {
  _ui->progressBar->setMaximum(max);
  _ui->progressBar->setValue(value);
}

void ProgressWidget::showEvent(QShowEvent * event) {
  cancelClicked = false;
  QWidget::showEvent(event);
}

void ProgressWidget::cancelButtonClicked() {
  cancelClicked = true;
}

ProgressWidgetGraphicsProxy::ProgressWidgetGraphicsProxy() : frameColor(Qt::green) {
  progressWidget = new ProgressWidget();
  setWidget(progressWidget);
  setWindowFlags(Qt::Window);
  setWindowFrameMargins(0,0,0,0);
#if (QT_VERSION >= QT_VERSION_CHECK(4, 5, 0))
  setOpacity(0.8);
#endif
}

void ProgressWidgetGraphicsProxy::setComment(const QString &comment) {
  progressWidget->setComment(comment);
}

void ProgressWidgetGraphicsProxy::setProgress(int value, int max) {
  progressWidget->setProgress(value, max);
}

const float offset = 10.;

void ProgressWidgetGraphicsProxy::paintWindowFrame (QPainter * painter, const QStyleOptionGraphicsItem *, QWidget *) {
  QRectF widgetGeometry = windowFrameRect();
  QPainterPath path;
  path.addRect(widgetGeometry);
  path.moveTo(widgetGeometry.topLeft() + QPointF(0, -offset));
  path.quadTo(widgetGeometry.topLeft() + QPointF(-offset, -offset), widgetGeometry.topLeft() + QPointF(-offset, 0));
  path.lineTo(widgetGeometry.bottomLeft() + QPointF(-offset, 0));
  path.quadTo(widgetGeometry.bottomLeft() + QPointF(-offset, offset), widgetGeometry.bottomLeft() + QPointF(0, offset));
  path.lineTo(widgetGeometry.bottomRight() + QPointF(0, offset));
  path.quadTo(widgetGeometry.bottomRight() + QPointF(offset, offset), widgetGeometry.bottomRight() + QPointF(offset, 0));
  path.lineTo(widgetGeometry.topRight() + QPointF(offset, 0));
  path.quadTo(widgetGeometry.topRight() + QPointF(offset, -offset), widgetGeometry.topRight() + QPointF(0, -offset));
  path.lineTo(widgetGeometry.topLeft() + QPointF(0, -offset));

  painter->setPen(Qt::black);
  painter->setBrush(frameColor);
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->drawPath(path);
  painter->setRenderHint(QPainter::Antialiasing, false);
}

bool ProgressWidgetGraphicsProxy::cancelRequested() const {
  return progressWidget->cancelRequested();
}
