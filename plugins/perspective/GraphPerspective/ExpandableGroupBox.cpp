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
  QObject *o;
  foreach(o,children()) {
    QWidget *w = dynamic_cast<QWidget *>(o);

    if (w)
      w->setVisible(e);
  }
}
