#ifndef EXPANDABLEGROUPBOX_H
#define EXPANDABLEGROUPBOX_H

#include <QtGui/QGroupBox>

class ExpandableGroupBox : public QGroupBox {
  Q_OBJECT
public:
  explicit ExpandableGroupBox(const QString &title=QString(),QWidget *parent = 0);
  virtual ~ExpandableGroupBox();

signals:

public slots:

};

#endif // EXPANDABLEGROUPBOX_H
