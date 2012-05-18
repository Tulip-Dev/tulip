#ifndef GRAPHPERSPECTIVELOGGER_H
#define GRAPHPERSPECTIVELOGGER_H

#include <QtGui/QPixmap>
#include <QtCore/QDebug>
#include <QtGui/QFrame>

namespace Ui {
class GraphPerspectiveLogger;
}

class GraphPerspectiveLogger: public QFrame {
  Q_OBJECT

  QtMsgType _logSeverity;
  unsigned int _logCount;

  Ui::GraphPerspectiveLogger* _ui;

  QString iconForType(QtMsgType);
public:
  GraphPerspectiveLogger(QWidget* parent = NULL);

  QPixmap icon();
  unsigned int count() const;
  void log(QtMsgType,const char*);

public slots:
  void clear();

signals:
  void cleared();
};

#endif // GRAPHPERSPECTIVELOGGER_H
