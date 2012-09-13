#ifndef NAVIGABLETABLEVIEW_H
#define NAVIGABLETABLEVIEW_H

#include <QtGui/QTableView>

class QKeyEvent;

class NavigableTableView : public QTableView {
  Q_OBJECT
public:
  explicit NavigableTableView(QWidget *parent = 0);
  void keyPressEvent(QKeyEvent *event);
signals:

public slots:

};

#endif // NAVIGABLETABLEVIEW_H
