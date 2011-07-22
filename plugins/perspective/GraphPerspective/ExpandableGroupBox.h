#ifndef EXPANDABLEGROUPBOX_H
#define EXPANDABLEGROUPBOX_H

#include <QtGui/QGroupBox>
#include <QtCore/QMap>

class ExpandableGroupBox : public QGroupBox {
  Q_OBJECT

  Q_PROPERTY(bool expanded READ expanded WRITE setExpanded)
  bool _expanded;

  Q_PROPERTY(QWidget * widget READ widget WRITE setWidget)
  QWidget *_widget;

public:
  explicit ExpandableGroupBox(const QString &title=QString(),QWidget *parent = 0);
  virtual ~ExpandableGroupBox();
  bool expanded() const {
    return _expanded;
  }

  QWidget *widget() const { return _widget; }
  QWidget *takeWidget();

signals:

public slots:
  void setExpanded(bool e);
  void setWidget(QWidget *);
};

#endif // EXPANDABLEGROUPBOX_H
