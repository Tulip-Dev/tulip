#include "PreferencesDialog.h"

#include "ui_PreferencesDialog.h"

#include <tulip/TulipSettings.h>
#include <tulip/TulipItemDelegate.h>

PreferencesDialog::PreferencesDialog(QWidget *parent): QDialog(parent), _ui(new Ui::PreferencesDialog) {
  _ui->setupUi(this);
  _ui->graphDefaultsTable->setItemDelegate(new tlp::TulipItemDelegate(_ui->graphDefaultsTable));
}

PreferencesDialog::~PreferencesDialog(){
    delete _ui;
}

void PreferencesDialog::writeSettings() {
  TulipSettings::instance().setProxyEnabled(_ui->proxyCheck->isChecked());

  switch(_ui->proxyType->currentIndex()) {
  case 0:
    TulipSettings::instance().setProxyType(QNetworkProxy::Socks5Proxy);
    break;

  case 1:
    TulipSettings::instance().setProxyType(QNetworkProxy::HttpProxy);
    break;

  case 2:
    TulipSettings::instance().setProxyType(QNetworkProxy::HttpCachingProxy);
    break;

  case 3:
    TulipSettings::instance().setProxyType(QNetworkProxy::FtpCachingProxy);
    break;

  default:
    break;
  }

  TulipSettings::instance().setProxyHost(_ui->proxyAddr->text());
  TulipSettings::instance().setProxyPort(_ui->proxyPort->value());
  TulipSettings::instance().setUseProxyAuthentification(_ui->proxyAuthCheck->isChecked());
  TulipSettings::instance().setProxyUsername(_ui->proxyUser->text());
  TulipSettings::instance().setProxyPassword(_ui->proxyPassword->text());

  QAbstractItemModel* model = _ui->graphDefaultsTable->model();

  TulipSettings::instance().setDefaultColor(tlp::NODE,model->data(model->index(0,1)).value<tlp::Color>());
  TulipSettings::instance().setDefaultColor(tlp::EDGE,model->data(model->index(0,2)).value<tlp::Color>());
  TulipSettings::instance().setDefaultSize(tlp::NODE,model->data(model->index(1,1)).value<tlp::Size>());
  TulipSettings::instance().setDefaultSize(tlp::EDGE,model->data(model->index(1,2)).value<tlp::Size>());
  TulipSettings::instance().setDefaultShape(tlp::NODE,model->data(model->index(2,1)).value<NodeShape>().nodeShapeId);
  TulipSettings::instance().setDefaultShape(tlp::EDGE,(int)(model->data(model->index(2,2)).value<tlp::EdgeShape>()));
  TulipSettings::instance().setDefaultSelectionColor(model->data(model->index(3,1)).value<tlp::Color>());

  TulipSettings::instance().applyProxySettings();

  TulipSettings::instance().setAutomaticMapMetric(_ui->colorMappingCheck->isChecked());
  TulipSettings::instance().setAutomaticRatio(_ui->aspectRatioCheck->isChecked());
  TulipSettings::instance().setResultPropertyStored(_ui->resultPropertyStoredCheck->isChecked());
  TulipSettings::instance().setRunningTimeComputed(_ui->runningTimeComputedCheck->isChecked());
}

void PreferencesDialog::readSettings() {
  _ui->proxyCheck->setChecked(TulipSettings::instance().isProxyEnabled());

  switch(TulipSettings::instance().proxyType()) {
  case QNetworkProxy::Socks5Proxy:
    _ui->proxyType->setCurrentIndex(0);
    break;

  case QNetworkProxy::HttpProxy:
    _ui->proxyType->setCurrentIndex(1);
    break;

  case QNetworkProxy::HttpCachingProxy:
    _ui->proxyType->setCurrentIndex(2);
    break;

  case QNetworkProxy::FtpCachingProxy:
    _ui->proxyType->setCurrentIndex(3);
    break;

  default:
    break;
  }

  _ui->proxyAddr->setText(TulipSettings::instance().proxyHost());
  _ui->proxyPort->setValue(TulipSettings::instance().proxyPort());
  _ui->proxyAuthCheck->setChecked(TulipSettings::instance().isUseProxyAuthentification());
  _ui->proxyUser->setText(TulipSettings::instance().proxyUsername());
  _ui->proxyPassword->setText(TulipSettings::instance().proxyPassword());


  QAbstractItemModel* model = _ui->graphDefaultsTable->model();
  model->setData(model->index(0,1),QVariant::fromValue<tlp::Color>(TulipSettings::instance().defaultColor(tlp::NODE)));
  model->setData(model->index(0,2),QVariant::fromValue<tlp::Color>(TulipSettings::instance().defaultColor(tlp::EDGE)));
  model->setData(model->index(1,1),QVariant::fromValue<tlp::Size>(TulipSettings::instance().defaultSize(tlp::NODE)));
  model->setData(model->index(1,2),QVariant::fromValue<tlp::Size>(TulipSettings::instance().defaultSize(tlp::EDGE)));
  model->setData(model->index(2,1),QVariant::fromValue<NodeShape>(NodeShape(TulipSettings::instance().defaultShape(tlp::NODE))));
  model->setData(model->index(2,2),QVariant::fromValue<tlp::EdgeShape>((tlp::EdgeShape)(TulipSettings::instance().defaultShape(tlp::EDGE))));
  model->setData(model->index(3,1),QVariant::fromValue<tlp::Color>(TulipSettings::instance().defaultSelectionColor()));

  _ui->aspectRatioCheck->setChecked(TulipSettings::instance().isAutomaticRatio());
  _ui->colorMappingCheck->setChecked(TulipSettings::instance().isAutomaticMapMetric());
  _ui->runningTimeComputedCheck->setChecked(TulipSettings::instance().isRunningTimeComputed());
}
