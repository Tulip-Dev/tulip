#include "tulip/PlaceHolderWidget.h"

#include <QtGui/QVBoxLayout>

PlaceHolderWidget::PlaceHolderWidget(QWidget *parent): QWidget(parent), _widget(NULL) {
  setLayout(new QVBoxLayout);
  layout()->setMargin(0);
  layout()->setSpacing(0);
}

void PlaceHolderWidget::setWidget(QWidget *widget) {
  if (_widget != NULL) {
    delete _widget;
  }
  _widget = widget;
  if (_widget != NULL) {
    layout()->addWidget(_widget);
  }
}

QWidget* PlaceHolderWidget::widget() const {
  return _widget;
}

QWidget* PlaceHolderWidget::takeWidget() {
  layout()->removeWidget(_widget);
  QWidget* result = _widget;
  _widget = NULL;
  return result;
}
