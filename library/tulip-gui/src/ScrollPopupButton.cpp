#include "tulip/ScrollPopupButton.h"

#include <QtGui/QVBoxLayout>
#include <QtGui/QScrollBar>
#include <QtGui/QCursor>

ScrollPopupButton::ScrollPopupButton(QWidget *parent): QPushButton(parent), _popup(new QWidget(NULL)) {
  _popup->setWindowFlags(Qt::Popup);
  _popup->setLayout(new QVBoxLayout);
  _scrollBar = new QScrollBar();
  _scrollBar->setOrientation(Qt::Vertical);
  _popup->layout()->addWidget(_scrollBar);
  connect(this,SIGNAL(clicked()),this,SLOT(showPopup()));
  connect(_scrollBar,SIGNAL(valueChanged(int)),this,SIGNAL(valueChanged(int)));
}

void ScrollPopupButton::showPopup() {
  QPoint globalPos = QCursor::pos();
  _popup->setGeometry(globalPos.x()-5,globalPos.y()-290,width(),300);
  _popup->show();
}

void ScrollPopupButton::hidePopup() {
  _popup->hide();
}

void ScrollPopupButton::setPopupVisible(bool f) {
  if (f)
    showPopup();
  else
    hidePopup();
}

void ScrollPopupButton::setValue(int v) {
  _scrollBar->setValue(v);
}
int ScrollPopupButton::value() const {
  return _scrollBar->value();
}
int ScrollPopupButton::maximum() const {
  return _scrollBar->maximum();
}
void ScrollPopupButton::setMaximum(int v) {
  _scrollBar->setMaximum(v);
}
int ScrollPopupButton::minimum() const {
  return _scrollBar->minimum();
}
void ScrollPopupButton::setMinimum(int v) {
  _scrollBar->setMinimum(v);
}

