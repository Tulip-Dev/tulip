#include "PluginsCenter.h"

PluginsCenter::PluginsCenter(QWidget *parent) :
    QWidget(parent), _ui(new Ui::PluginsCenterData()) {
  _ui->setupUi(this);
}
