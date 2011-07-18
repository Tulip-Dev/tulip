#include "AlgorithmRunner.h"

#include "ui_AlgorithmRunner.h"

QMap<QString,QString> AlgorithmRunner::ALGORITHMS_DISPLAY_NAMES;

AlgorithmRunner::AlgorithmRunner(QWidget *parent): QWidget(parent), _ui(new Ui::AlgorithmRunnerData) {
  _ui->setupUi(this);
  _ui->header->setMenus(QStringList() );
}

void AlgorithmRunner::staticInit() {
}
