/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */

#include "PreferencesDialog.h"

#include "ui_PreferencesDialog.h"

#include <tulip/TlpTools.h>
#include <tulip/TulipSettings.h>
#include <tulip/TulipItemDelegate.h>
#include <tulip/TulipMetaTypes.h>

using namespace tlp;

PreferencesDialog::PreferencesDialog(QWidget *parent): QDialog(parent), _ui(new Ui::PreferencesDialog) {
  _ui->setupUi(this);
  _ui->graphDefaultsTable->setItemDelegate(new tlp::TulipItemDelegate(_ui->graphDefaultsTable));
  connect(_ui->graphDefaultsTable, SIGNAL(cellChanged(int, int)), this, SLOT(cellChanged(int, int)));
  connect(_ui->randomSeedCheck, SIGNAL(stateChanged(int)), this, SLOT(randomSeedCheckChanged(int)));

  // disable edition for title items (in column 0)
  for (int i = 0; i < 4; ++i)
    _ui->graphDefaultsTable->item(i, 0)->setFlags(Qt::ItemIsEnabled);
}

PreferencesDialog::~PreferencesDialog() {
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
  TulipSettings::instance().setDefaultShape(tlp::NODE,model->data(model->index(2,1)).value<NodeShape::NodeShapes>());
  TulipSettings::instance().setDefaultShape(tlp::EDGE,(int)(model->data(model->index(2,2)).value<EdgeShape::EdgeShapes>()));
  TulipSettings::instance().setDefaultSelectionColor(model->data(model->index(3,1)).value<tlp::Color>());
  TulipSettings::instance().setDefaultSelectionColor(model->data(model->index(3,2)).value<tlp::Color>());

  TulipSettings::instance().applyProxySettings();

  TulipSettings::instance().setDisplayDefaultViews(_ui->displayDefaultViews->isChecked());
  TulipSettings::instance().setAutomaticMapMetric(_ui->colorMappingCheck->isChecked());
  TulipSettings::instance().setAutomaticRatio(_ui->aspectRatioCheck->isChecked());
  TulipSettings::instance().setViewOrtho(_ui->viewOrthoCheck->isChecked());
  TulipSettings::instance().setResultPropertyStored(_ui->resultPropertyStoredCheck->isChecked());
  TulipSettings::instance().setRunningTimeComputed(_ui->runningTimeComputedCheck->isChecked());

  if (_ui->randomSeedCheck->isChecked()) {
    bool ok = true;
    unsigned int seed = _ui->randomSeedEdit->text().toUInt(&ok);
    tlp::setSeedOfRandomSequence(seed);
  }
  else
    tlp::setSeedOfRandomSequence();

  TulipSettings::instance().setSeedOfRandomSequence(tlp::getSeedOfRandomSequence());
}

void PreferencesDialog::readSettings() {
  _ui->proxyCheck->setChecked(TulipSettings::instance().isProxyEnabled());

  if(TulipSettings::instance().isProxyEnabled()) {
    _ui->networkFrame1->setEnabled(true);
    _ui->networkFrame2->setEnabled(true);
  }

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

  if(TulipSettings::instance().isUseProxyAuthentification()) {
    _ui->proxyUser->setEnabled(true);
    _ui->proxyPassword->setEnabled(true);
  }

  _ui->proxyUser->setText(TulipSettings::instance().proxyUsername());
  _ui->proxyPassword->setText(TulipSettings::instance().proxyPassword());


  QAbstractItemModel* model = _ui->graphDefaultsTable->model();
  model->setData(model->index(0,1),QVariant::fromValue<tlp::Color>(TulipSettings::instance().defaultColor(tlp::NODE)));
  model->setData(model->index(0,2),QVariant::fromValue<tlp::Color>(TulipSettings::instance().defaultColor(tlp::EDGE)));
  model->setData(model->index(1,1),QVariant::fromValue<tlp::Size>(TulipSettings::instance().defaultSize(tlp::NODE)));
  model->setData(model->index(1,2),QVariant::fromValue<tlp::Size>(TulipSettings::instance().defaultSize(tlp::EDGE)));
  model->setData(model->index(2,1),QVariant::fromValue<NodeShape::NodeShapes>(static_cast<NodeShape::NodeShapes>(TulipSettings::instance().defaultShape(tlp::NODE))));
  model->setData(model->index(2,2),QVariant::fromValue<EdgeShape::EdgeShapes>(static_cast<EdgeShape::EdgeShapes>(TulipSettings::instance().defaultShape(tlp::EDGE))));
  model->setData(model->index(3,1),QVariant::fromValue<tlp::Color>(TulipSettings::instance().defaultSelectionColor()));
  model->setData(model->index(3,2),QVariant::fromValue<tlp::Color>(TulipSettings::instance().defaultSelectionColor()));
  // edges selection color is not editable
  //_ui->graphDefaultsTable->item(3,2)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

  _ui->displayDefaultViews->setChecked(TulipSettings::instance().displayDefaultViews());
  _ui->aspectRatioCheck->setChecked(TulipSettings::instance().isAutomaticRatio());
  _ui->viewOrthoCheck->setChecked(TulipSettings::instance().isViewOrtho());
  _ui->colorMappingCheck->setChecked(TulipSettings::instance().isAutomaticMapMetric());
  _ui->runningTimeComputedCheck->setChecked(TulipSettings::instance().isRunningTimeComputed());

  // initialize seed according to settings
  unsigned int seed;
  tlp::setSeedOfRandomSequence(seed = TulipSettings::instance().seedOfRandomSequence());
  // UINT_MAX seed value means the seed is random
  bool isSeedRandom = (seed == UINT_MAX);
  _ui->randomSeedCheck->setChecked(!isSeedRandom);
  _ui->randomSeedEdit->setEnabled(!isSeedRandom);
  _ui->randomSeedEdit->setText(isSeedRandom ? QString() : QString::number(seed));
}

void PreferencesDialog::cellChanged(int row, int column) {
  if (row == 3) {
    // force selection color to be the same for nodes & edges
    QAbstractItemModel* model = _ui->graphDefaultsTable->model();
    model->setData(model->index(3, column == 1 ? 2 : 1), model->data(model->index(3,column)));
  }
}

void PreferencesDialog::randomSeedCheckChanged(int state) {
  if (state == Qt::Checked && _ui->randomSeedEdit->text().isEmpty())
    _ui->randomSeedEdit->setText("1");
}
