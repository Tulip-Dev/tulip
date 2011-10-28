#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QtGui/QWidget>

namespace Ui {
class Workspace;
}

class Workspace: public QWidget {
  Q_OBJECT

  Ui::Workspace* _ui;
public:
  explicit Workspace(QWidget *parent = 0);

signals:

public slots:

};

#endif // WORKSPACE_H
