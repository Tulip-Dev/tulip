#ifndef VECTOREDITIONWIDGET_H
#define VECTOREDITIONWIDGET_H

#include <QtGui/QWidget>

#include <tulip/tulipconf.h>

namespace Ui {
class VectorEditionWidget;
}

class TLP_QT_SCOPE VectorEditionWidget:public QWidget {
  Q_OBJECT
  Ui::VectorEditionWidget* _ui;

  int _userType;

public:
  explicit VectorEditionWidget(QWidget* parent=0);
  void setVector(const QVector<QVariant>& d, int userType);
  QVector<QVariant> vector() const;

public slots:
  void add();
  void remove();
};

#endif // VECTOREDITIONWIDGET_H
