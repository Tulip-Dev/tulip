#include "ExpandableGroupBox.h"

#include <QtGui/QVBoxLayout>

ExpandableGroupBox::ExpandableGroupBox(const QString &title,QWidget *parent): QGroupBox(title,parent), _expanded(true) {
  setCheckable(true);
  setChecked(true);
  setExpanded(_expanded);
  connect(this,SIGNAL(toggled(bool)),this,SLOT(setExpanded(bool)));
}

ExpandableGroupBox::~ExpandableGroupBox() {
}

void ExpandableGroupBox::setExpanded(bool e) {
  if (layout()) {
    if (!e) {
      _oldMargins = layout()->contentsMargins();
      layout()->setContentsMargins(0,0,0,0);
    }
    else
      layout()->setContentsMargins(_oldMargins);
  }

  _expanded=e;
  QWidget *w;
  foreach(w,findChildren<QWidget *>())
    w->setVisible(e);
}
