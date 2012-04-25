#include <tulip/DragHandle.h>
#include <tulip/TulipMimes.h>
#include <tulip/WorkspacePanel.h>
#include <tulip/View.h>

#include <assert.h>

#include <QtCore/QMimeData>
#include <QtGui/QDrag>
#include <QtGui/QMouseEvent>
#include <QtGui/QApplication>

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
  Qt::DropAction dropaction = drag->exec(Qt::MoveAction);
}

void DragHandle::setPanel(tlp::WorkspacePanel* panel) {
  _panel = panel;
}

