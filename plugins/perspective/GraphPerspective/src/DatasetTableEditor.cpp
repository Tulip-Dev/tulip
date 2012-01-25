#include "DatasetTableEditor.h"
#include "ui_DatasetTableEditor.h"

using namespace tlp;

DatasetTableEditor::DatasetTableEditor(QWidget *parent) :
  QWidget(parent),
  _ui(new Ui::DatasetTableEditor) {
  _ui->setupUi(this);
  connect(_ui->headerFrameWidget,SIGNAL(menuChanged(QString)),SLOT(menuChanged(QString)));

}

DatasetTableEditor::~DatasetTableEditor() {
  delete _ui;
}

ElementType DatasetTableEditor::displayedType()const {
  return _ui->headerFrameWidget->currentMenuIndex()==0?NODE:EDGE;
}

void DatasetTableEditor::setTulipTableWidgetColumnSelectionModel(TulipTableWidgetColumnSelectionModel* model) {
  _ui->columnConfigurationWidget->setColumnSelectionModel(model);
}

void  DatasetTableEditor::menuChanged(const QString&) {
  emit tableChanged(_ui->headerFrameWidget->currentMenuIndex()==0?NODE:EDGE);
}
