#include "FiltersManagerItem.h"

#include "ui_FiltersManagerItem.h"

#include <tulip/PluginLister.h>
#include <tulip/PropertyAlgorithm.h>
#include <tulip/Algorithm.h>

FiltersManagerItem::FiltersManagerItem(QWidget *parent): QWidget(parent), _ui(new Ui::FiltersManagerItemData), _removable(true) {
  _ui->setupUi(this);

  _ui->settingsFrame->hide();
  _ui->modeSelector->hide();

  connect(_ui->addButton,SIGNAL(clicked()),this,SLOT(expand()));

  tlp::Iterator<std::string> *it = tlp::StaticPluginLister<tlp::DoubleAlgorithm,tlp::PropertyContext>::availablePlugins();
  while (it->hasNext()) {
    std::string name = it->next();
    _ui->firstMember->addItem(name.c_str());
    _ui->secondMember->addItem(name.c_str());
  }

  delete it;
}

FiltersManagerItem::~FiltersManagerItem() {
  delete _ui;
}

void FiltersManagerItem::setRemovable(bool f) {
  _removable = f;
}

void FiltersManagerItem::expand() {
  _ui->settingsFrame->show();
  _ui->addButton->hide();
  _ui->modeSelector->show();
}
