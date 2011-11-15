#include "ImportWizard.h"

#include <QtGui/QAbstractButton>
#include <tulip/TulipItemDelegate.h>
#include <tulip/ParameterListModel.h>
#include <tulip/ImportModule.h>
#include <tulip/ForEach.h>
#include "ui_ImportWizard.h"

//FIXME: remove me
#include <QtCore/QDebug>

using namespace tlp;
using namespace std;

ImportWizard::ImportWizard(QWidget *parent): QWizard(parent), _ui(new Ui::ImportWizard) {
  _ui->setupUi(this);
  QSet<QString> groups;
  string algName;
  forEach(algName,ImportModuleLister::availablePlugins()) {
    groups.insert(ImportModuleLister::pluginInformations(algName).getGroup().c_str());
  }
  _ui->categoryList->addItems(groups.toList());
  _ui->parametersList->setItemDelegate(new TulipItemDelegate);
  _ui->parametersFrame->hide();
  updateFinishButton();
}

ImportWizard::~ImportWizard() {
  delete _ui->parametersList->model();
  delete _ui;
}

void ImportWizard::algorithmSelected(const QString& alg) {
  _ui->parametersFrame->setVisible(!alg.isEmpty());
  QAbstractItemModel* oldModel = _ui->parametersList->model();
  QAbstractItemModel* newModel = NULL;
  if (ImportModuleLister::pluginExists(alg.toStdString())) {
    newModel = new ParameterListModel(ImportModuleLister::getPluginParameters(alg.toStdString()));
  }
  _ui->parametersList->setModel(newModel);

  delete oldModel;
  updateFinishButton();
}

void ImportWizard::groupSelected(const QString& group) {
  _ui->algorithmList->clear();

  string algName;
  forEach(algName,ImportModuleLister::availablePlugins()) {
    if (group == ImportModuleLister::pluginInformations(algName).getGroup().c_str())
      _ui->algorithmList->addItem(algName.c_str());
  }

  _ui->algorithmFrame->setEnabled(_ui->algorithmList->count() > 0);
  updateFinishButton();
}

QString ImportWizard::algorithm() const {
  if (_ui->algorithmList->currentItem() != NULL)
    return _ui->algorithmList->currentItem()->text();
  return QString::null;
}

QString ImportWizard::group() const {
  if (_ui->categoryList->currentItem() != NULL)
    return _ui->categoryList->currentItem()->text();
  return QString::null;
}

tlp::DataSet ImportWizard::parameters() const {
  ParameterListModel* model = dynamic_cast<ParameterListModel*>(_ui->parametersList->model());
  if (model == NULL)
    return DataSet();
  return model->parametersValues();
}

void ImportWizard::updateFinishButton() {
  button(QWizard::FinishButton)->setEnabled(!algorithm().isNull() || (!_ui->algorithmFrame->isEnabled() && !group().isNull()));
}
