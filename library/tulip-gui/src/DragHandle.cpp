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

#include <QMimeData>
#include <QDrag>
#include <QMouseEvent>
#include <QApplication>

#include <tulip/DragHandle.h>
#include <tulip/TulipMimes.h>
#include <tulip/WorkspacePanel.h>
#include <tulip/View.h>

#include <cassert>

using namespace tlp;

DragHandle::DragHandle(QWidget* parent, Qt::WindowFlags f) : QLabel(parent, f), _panel(NULL), _pressed(false) {
}

void DragHandle::mousePressEvent(QMouseEvent* ev) {
  _pressed = true;
  _clickPosition = ev->pos();
}

void DragHandle::mouseReleaseEvent(QMouseEvent*) {
  _pressed = false;
}

void DragHandle::mouseMoveEvent(QMouseEvent* ev) {
  assert(_panel != NULL);

  if(!_panel || !_pressed || (ev->pos() - _clickPosition).manhattanLength() < QApplication::startDragDistance())
    return;

  QDrag* drag = new QDrag(_panel);
  PanelMimeType* mimedata = new PanelMimeType();
  mimedata->setPanel(_panel);
  drag->setMimeData(mimedata);
  drag->setPixmap(_panel->view()->snapshot(QSize(100,100)));
  drag->exec(Qt::MoveAction);
}

void DragHandle::setPanel(tlp::WorkspacePanel* panel) {
  _panel = panel;
}

