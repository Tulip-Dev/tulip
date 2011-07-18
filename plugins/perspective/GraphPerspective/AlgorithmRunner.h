#ifndef ALGORITHMRUNNER_H
#define ALGORITHMRUNNER_H

#include <QtGui/QWidget>

namespace Ui {
class AlgorithmRunnerData;
}

class AlgorithmRunner : public QWidget {
  Q_OBJECT

  static QMap<QString,QString> ALGORITHMS_DISPLAY_NAMES;
  static void staticInit();

  Ui::AlgorithmRunnerData *_ui;
public:
  explicit AlgorithmRunner(QWidget *parent = 0);

signals:

public slots:

};

#endif // ALGORITHMRUNNER_H
