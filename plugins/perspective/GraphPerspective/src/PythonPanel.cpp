#include "PythonPanel.h"
#include "ui_PythonPanel.h"
#include "OutputPanelButton.h"

#include <tulip/GraphHierarchiesModel.h>
#include <tulip/PythonInterpreter.h>

#include <QtCore/QPropertyAnimation>

static const QString setCurrentGraphFunction =
  "graph = None\n"
  "def setCurrentGraph(g):\n"
  "	global graph\n"
  "	graph = g\n"
  ;

PythonPanel::PythonPanel(QWidget *parent) : QWidget(parent), _ui(new Ui::PythonPanel), _button(NULL), _animation(NULL) {
    _ui->setupUi(this);
    connect(_ui->clearButton, SIGNAL(clicked()), this, SLOT(clearConsole()));
    connect(_ui->consoleWidget, SIGNAL(textChanged()), this, SLOT(newOutputInConsole()));
    tlp::PythonInterpreter::getInstance()->runString(setCurrentGraphFunction);
}

QAbstractScrollArea *PythonPanel::consoleWidget() {
    return _ui->consoleWidget;
}

void PythonPanel::setModel(tlp::GraphHierarchiesModel* model) {
    _ui->graphCombo->setGraphsModel(model);
    connect(_ui->graphCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(graphComboIndexChanged()));
}

void PythonPanel::graphComboIndexChanged() {
    tlp::Graph* g = _ui->graphCombo->model()->data(_ui->graphCombo->selectedIndex(),tlp::TulipModel::GraphRole).value<tlp::Graph*>();
    tlp::PythonInterpreter::getInstance()->runGraphScript("__main__", "setCurrentGraph", g);
    _ui->pythonShellWidget->getAutoCompletionDb()->setGraph(g);
}

void PythonPanel::clearConsole() {
    _ui->consoleWidget->clear();
}

void PythonPanel::setPanelButton(OutputPanelButton *button) {
    _button = button;
    connect(_button, SIGNAL(toggled(bool)), this, SLOT(stopButtonAnimation()));
}

void PythonPanel::newOutputInConsole() {
    _ui->tabWidget->setCurrentIndex(1);
    if (!_button->isChecked()) {
        startButtonAnimation();
    }
}

void PythonPanel::startButtonAnimation() {
    _animation = new QPropertyAnimation(_button, "glowColor");
    _animation->setKeyValueAt(0, QColor(255,255,255,0));
    _animation->setKeyValueAt(0.25, QColor(255,0,0));
    _animation->setKeyValueAt(0.75, QColor(255,0,0));
    _animation->setKeyValueAt(1, QColor(255,255,255,0));
    _animation->setDuration(1000);
    _animation->setLoopCount(-1);
    _animation->start();
}

void PythonPanel::stopButtonAnimation() {
    if (_animation) {
        _animation->stop();
        delete _animation;
        _animation = NULL;
        _button->resetBackgroundColor();
    }
}
