#include "tulip/ModalPopup.h"

#include <QtCore/QEvent>
#include <QtGui/QKeyEvent>

ModalPopup::ModalPopup(QWidget *parent): PlaceHolderWidget(parent), _deleteWidgetOnClose(true) {
  setWindowFlags(Qt::Popup | Qt::WindowStaysOnBottomHint);
  setWindowModality(Qt::ApplicationModal);
  setAttribute(Qt::WA_DeleteOnClose,true);
}

ModalPopup::~ModalPopup() {
  if (!_deleteWidgetOnClose)
    takeWidget();
}

bool ModalPopup::event(QEvent* ev) {
  if (ev->type() == QEvent::KeyPress) {
    QKeyEvent* keyEv = static_cast<QKeyEvent*>(ev);
    if (keyEv->key() == Qt::Key_Escape) {
      close();
      return true;
    }
  }
  else if (ev->type() == QEvent::MouseButtonPress) {
    QMouseEvent* mouseEv = static_cast<QMouseEvent*>(ev);
    if (!geometry().contains(mouseEv->pos())) {
      close();
      return true;
    }
  }
  return false;
}

bool ModalPopup::deleteWidgetOnClose() const {
  return _deleteWidgetOnClose;
}

void ModalPopup::setDeleteWidgetOnClose(bool f) {
  _deleteWidgetOnClose = f;
}
