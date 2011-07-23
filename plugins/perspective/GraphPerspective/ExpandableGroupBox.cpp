#include "ExpandableGroupBox.h"

#include <QtGui/QVBoxLayout>

#include <QtCore/QDebug>

ExpandableGroupBox::ExpandableGroupBox(const QString &title,QWidget *parent): QGroupBox(title,parent), _expanded(true), _widget(0) {
  setCheckable(true);
  setChecked(true);
  setExpanded(_expanded);
  connect(this,SIGNAL(toggled(bool)),this,SLOT(setExpanded(bool)));

  QVBoxLayout *lyt = new QVBoxLayout;
  lyt->setContentsMargins(0,0,0,0);
  lyt->setSpacing(0);
  setLayout(lyt);
}

ExpandableGroupBox::~ExpandableGroupBox() {
}

void ExpandableGroupBox::setExpanded(bool e) {
  _expanded=e;

  if (_widget)
    _widget->setVisible(e);
}

void ExpandableGroupBox::setWidget(QWidget *w) {
  delete _widget;
  _widget = w;

  if (_widget)
    layout()->addWidget(_widget);
}

QWidget *ExpandableGroupBox::takeWidget() {
  QWidget *result = _widget;
  _widget = 0;

  if (result)
    layout()->removeWidget(result);

  return result;
}
