/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
#include <QMenu>
#include "PreferencesDialog.h"

#include "ui_PreferencesDialog.h"

#include <tulip/Perspective.h>
#include <tulip/TlpTools.h>
#include <tulip/TulipSettings.h>
#include <tulip/TulipItemDelegate.h>
#include <tulip/TulipMetaTypes.h>

#include <QMessageBox>

using namespace tlp;

PreferencesDialog::PreferencesDialog(QWidget *parent)
    : QDialog(parent), _ui(new Ui::PreferencesDialog) {
  _ui->setupUi(this);
  _ui->graphDefaultsTable->setItemDelegate(new tlp::TulipItemDelegate(_ui->graphDefaultsTable));
  connect(_ui->graphDefaultsTable, SIGNAL(cellChanged(int, int)), this,
          SLOT(cellChanged(int, int)));
  _ui->graphDefaultsTable->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(_ui->graphDefaultsTable, SIGNAL(customContextMenuRequested(const QPoint &)), this,
          SLOT(showGraphDefaultsContextMenu(const QPoint &)));
  connect(_ui->randomSeedCheck, SIGNAL(stateChanged(int)), this, SLOT(randomSeedCheckChanged(int)));
  connect(_ui->resetAllDrawingDefaultsButton, SIGNAL(released()), this,
          SLOT(resetToTulipDefaults()));

  // disable edition for title items (in column 0)
  for (int i = 0; i < _ui->graphDefaultsTable->rowCount(); ++i) {
    _ui->graphDefaultsTable->item(i, 0)->setFlags(Qt::ItemIsEnabled);
    QTableWidgetItem *item = _ui->graphDefaultsTable->item(i, 0);
    item->setToolTip(QString("Click mouse right button to display a contextual menu allowing to "
                             "reset the default values of <b>") +
                     item->text() + "</b>.");
  }

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
  _ui->graphDefaultsTable->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
#else
  _ui->graphDefaultsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
#endif
}

PreferencesDialog::~PreferencesDialog() {
  delete _ui;
}

void PreferencesDialog::usetlpbformat(int state) {
  if (state == Qt::Checked) {
    QMessageBox::warning(this, "Use tlpb file format", "Be careful: using the tlpb file format "
                                                       "means faster Tulip project loading/saving "
                                                       "but you will lose compatibility with "
                                                       "previous versions of Tulip (<4.10).");
  }
}

template <typename PROP, typename TYPE>
inline void setDefaultNodeValueInProperty(const std::string &propertyName, const TYPE &value,
                                          bool &graphPush) {
  Graph *root = NULL;
  forEach(root, getRootGraphs()) {
    if (root->existLocalProperty(propertyName)) {
      PROP *prop = dynamic_cast<PROP *>(root->getProperty(propertyName));

      if (prop) {
        if (graphPush)
          root->push();

        prop->setAllNodeValue(value);
      }
    }

    Graph *sg = NULL;
    forEach(sg, root->getDescendantGraphs()) {
      if (sg->existLocalProperty(propertyName)) {
        PROP *prop = dynamic_cast<PROP *>(sg->getProperty(propertyName));

        if (prop) {
          prop->setAllNodeValue(value);
        }
      }
    }
  }
  graphPush = false;
}

template <typename PROP, typename TYPE>
inline void setDefaultEdgeValueInProperty(const std::string &propertyName, const TYPE &value,
                                          bool &graphPush) {
  Graph *root = NULL;
  forEach(root, getRootGraphs()) {
    if (root->existLocalProperty(propertyName)) {
      PROP *prop = dynamic_cast<PROP *>(root->getProperty(propertyName));

      if (prop) {
        if (graphPush)
          root->push();

        prop->setAllEdgeValue(value);
      }
    }

    Graph *sg = NULL;
    forEach(sg, root->getDescendantGraphs()) {
      if (sg->existLocalProperty(propertyName)) {
        PROP *prop = dynamic_cast<PROP *>(sg->getProperty(propertyName));

        if (prop) {
          prop->setAllEdgeValue(value);
        }
      }
    }
  }
  graphPush = false;
}

void PreferencesDialog::writeSettings() {
  TulipSettings::instance().setProxyEnabled(_ui->proxyCheck->isChecked());

  switch (_ui->proxyType->currentIndex()) {
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

  QAbstractItemModel *model = _ui->graphDefaultsTable->model();
  bool applyDrawingDefaults = _ui->applyDrawingDefaultsCheck->isChecked();
  bool graphPush = true;

  if (TulipSettings::instance().defaultColor(tlp::NODE) !=
      model->data(model->index(0, 1)).value<tlp::Color>()) {
    TulipSettings::instance().setDefaultColor(tlp::NODE,
                                              model->data(model->index(0, 1)).value<tlp::Color>());

    if (applyDrawingDefaults)
      setDefaultNodeValueInProperty<ColorProperty>(
          "viewColor", TulipSettings::instance().defaultColor(tlp::NODE), graphPush);
  }

  if (TulipSettings::instance().defaultColor(tlp::EDGE) !=
      model->data(model->index(0, 2)).value<tlp::Color>()) {
    TulipSettings::instance().setDefaultColor(tlp::EDGE,
                                              model->data(model->index(0, 2)).value<tlp::Color>());

    if (applyDrawingDefaults)
      setDefaultEdgeValueInProperty<ColorProperty>(
          "viewColor", TulipSettings::instance().defaultColor(tlp::EDGE), graphPush);
  }

  if (TulipSettings::instance().defaultSize(tlp::NODE) !=
      model->data(model->index(1, 1)).value<tlp::Size>()) {
    TulipSettings::instance().setDefaultSize(tlp::NODE,
                                             model->data(model->index(1, 1)).value<tlp::Size>());
    setDefaultNodeValueInProperty<SizeProperty>(
        "viewSize", TulipSettings::instance().defaultSize(tlp::NODE), graphPush);
  }

  if (TulipSettings::instance().defaultSize(tlp::EDGE) !=
      model->data(model->index(1, 2)).value<tlp::Size>()) {
    TulipSettings::instance().setDefaultSize(tlp::EDGE,
                                             model->data(model->index(1, 2)).value<tlp::Size>());

    if (applyDrawingDefaults)
      setDefaultEdgeValueInProperty<SizeProperty>(
          "viewSize", TulipSettings::instance().defaultSize(tlp::EDGE), graphPush);
  }

  if (TulipSettings::instance().defaultShape(tlp::NODE) !=
      model->data(model->index(2, 1)).value<NodeShape::NodeShapes>()) {
    TulipSettings::instance().setDefaultShape(
        tlp::NODE, model->data(model->index(2, 1)).value<NodeShape::NodeShapes>());

    if (applyDrawingDefaults)
      setDefaultNodeValueInProperty<IntegerProperty>(
          "viewShape", TulipSettings::instance().defaultShape(tlp::NODE), graphPush);
  }

  if (TulipSettings::instance().defaultShape(tlp::EDGE) !=
      int(model->data(model->index(2, 2)).value<EdgeShape::EdgeShapes>())) {
    TulipSettings::instance().setDefaultShape(
        tlp::EDGE, int(model->data(model->index(2, 2)).value<EdgeShape::EdgeShapes>()));
    setDefaultEdgeValueInProperty<IntegerProperty>(
        "viewShape", TulipSettings::instance().defaultShape(tlp::EDGE), graphPush);
  }

  if (TulipSettings::instance().defaultLabelColor() !=
      model->data(model->index(4, 1)).value<tlp::Color>()) {
    TulipSettings::instance().setDefaultLabelColor(
        model->data(model->index(4, 1)).value<tlp::Color>());

    if (applyDrawingDefaults) {
      setDefaultNodeValueInProperty<ColorProperty>(
          "viewLabelColor", TulipSettings::instance().defaultLabelColor(), graphPush);
      setDefaultEdgeValueInProperty<ColorProperty>(
          "viewLabelColor", TulipSettings::instance().defaultLabelColor(), graphPush);
    }
  }

  TulipSettings::instance().setDefaultSelectionColor(
      model->data(model->index(3, 1)).value<tlp::Color>());

  TulipSettings::instance().applyProxySettings();

  TulipSettings::instance().setDisplayDefaultViews(_ui->displayDefaultViews->isChecked());
  TulipSettings::instance().setAutomaticMapMetric(_ui->colorMappingCheck->isChecked());
  TulipSettings::instance().setAutomaticRatio(_ui->aspectRatioCheck->isChecked());
  TulipSettings::instance().setAutomaticCentering(_ui->centerViewCheck->isChecked());
  TulipSettings::instance().setViewOrtho(_ui->viewOrthoCheck->isChecked());
  TulipSettings::instance().setResultPropertyStored(_ui->resultPropertyStoredCheck->isChecked());
  TulipSettings::instance().setLogPluginCall(_ui->logCombo->currentIndex());
  TulipSettings::instance().setUseTlpFileFormat(_ui->usetlpbformat->isChecked());

  if (_ui->randomSeedCheck->isChecked()) {
    bool ok = true;
    unsigned int seed = _ui->randomSeedEdit->text().toUInt(&ok);
    tlp::setSeedOfRandomSequence(seed);
  } else
    tlp::setSeedOfRandomSequence();

  TulipSettings::instance().setSeedOfRandomSequence(tlp::getSeedOfRandomSequence());
}

void PreferencesDialog::readSettings() {
  _ui->proxyCheck->setChecked(TulipSettings::instance().isProxyEnabled());

  if (TulipSettings::instance().isProxyEnabled()) {
    _ui->networkFrame1->setEnabled(true);
    _ui->networkFrame2->setEnabled(true);
  }

  switch (TulipSettings::instance().proxyType()) {
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

  if (TulipSettings::instance().isUseProxyAuthentification()) {
    _ui->proxyUser->setEnabled(true);
    _ui->proxyPassword->setEnabled(true);
  }

  _ui->proxyUser->setText(TulipSettings::instance().proxyUsername());
  _ui->proxyPassword->setText(TulipSettings::instance().proxyPassword());

  QAbstractItemModel *model = _ui->graphDefaultsTable->model();
  model->setData(model->index(0, 1), QVariant::fromValue<tlp::Color>(
                                         TulipSettings::instance().defaultColor(tlp::NODE)));
  model->setData(model->index(0, 2), QVariant::fromValue<tlp::Color>(
                                         TulipSettings::instance().defaultColor(tlp::EDGE)));
  model->setData(model->index(1, 1),
                 QVariant::fromValue<tlp::Size>(TulipSettings::instance().defaultSize(tlp::NODE)));
  model->setData(model->index(1, 2),
                 QVariant::fromValue<tlp::Size>(TulipSettings::instance().defaultSize(tlp::EDGE)));
  model->setData(model->index(2, 1),
                 QVariant::fromValue<NodeShape::NodeShapes>(static_cast<NodeShape::NodeShapes>(
                     TulipSettings::instance().defaultShape(tlp::NODE))));
  model->setData(model->index(2, 2),
                 QVariant::fromValue<EdgeShape::EdgeShapes>(static_cast<EdgeShape::EdgeShapes>(
                     TulipSettings::instance().defaultShape(tlp::EDGE))));
  model->setData(model->index(3, 1), QVariant::fromValue<tlp::Color>(
                                         TulipSettings::instance().defaultSelectionColor()));
  model->setData(model->index(3, 2), QVariant::fromValue<tlp::Color>(
                                         TulipSettings::instance().defaultSelectionColor()));
  model->setData(model->index(4, 1),
                 QVariant::fromValue<tlp::Color>(TulipSettings::instance().defaultLabelColor()));
  model->setData(model->index(4, 2),
                 QVariant::fromValue<tlp::Color>(TulipSettings::instance().defaultLabelColor()));
  // edges selection color is not editable
  //_ui->graphDefaultsTable->item(3,2)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
  _ui->applyDrawingDefaultsCheck->setChecked(false);
  Iterator<Graph *> *itr = tlp::getRootGraphs();
  _ui->applyDrawingDefaultsCheck->setEnabled(itr->hasNext());
  delete itr;

  _ui->displayDefaultViews->setChecked(TulipSettings::instance().displayDefaultViews());
  _ui->aspectRatioCheck->setChecked(TulipSettings::instance().isAutomaticRatio());
  _ui->centerViewCheck->setChecked(TulipSettings::instance().isAutomaticCentering());
  _ui->viewOrthoCheck->setChecked(TulipSettings::instance().isViewOrtho());
  _ui->resultPropertyStoredCheck->setChecked(TulipSettings::instance().isResultPropertyStored());
  _ui->colorMappingCheck->setChecked(TulipSettings::instance().isAutomaticMapMetric());
  _ui->logCombo->setCurrentIndex(TulipSettings::instance().logPluginCall());

  if (TulipSettings::instance().isUseTlpbFileFormat()) {
    _ui->usetlpbformat->setChecked(true);
  } else
    connect(_ui->usetlpbformat, SIGNAL(stateChanged(int)), this, SLOT(usetlpbformat(int)));

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
  if (row >= 3) {
    // force selection color to be the same for nodes & edges
    QAbstractItemModel *model = _ui->graphDefaultsTable->model();
    model->setData(model->index(row, column == 1 ? 2 : 1), model->data(model->index(row, column)));
  }
}

void PreferencesDialog::randomSeedCheckChanged(int state) {
  if (state == Qt::Checked && _ui->randomSeedEdit->text().isEmpty())
    _ui->randomSeedEdit->setText("1");
}

#define RESET_NODE 0
#define RESET_EDGE 1
#define RESET_BOTH 2
void PreferencesDialog::resetToTulipDefaults(int row, int updateMode) {
  if (updateMode == RESET_BOTH) {
    resetToTulipDefaults(row, RESET_NODE);
    resetToTulipDefaults(row, RESET_EDGE);
    return;
  }

  if (row == -1) {
    for (row = 0; row < _ui->graphDefaultsTable->rowCount(); ++row)
      resetToTulipDefaults(row, RESET_BOTH);

    return;
  }

  QAbstractItemModel *model = _ui->graphDefaultsTable->model();
  model->setData(model->index(4, 1),
                 QVariant::fromValue<tlp::Color>(TulipSettings::instance().defaultLabelColor()));
  model->setData(model->index(4, 2),
                 QVariant::fromValue<tlp::Color>(TulipSettings::instance().defaultLabelColor()));

  switch (row) {
  case 0: // default color
    if (updateMode == RESET_NODE)
      model->setData(
          model->index(0, 1),
          QVariant::fromValue<tlp::Color>(TulipSettings::instance().defaultColor(tlp::NODE, true)));
    else
      model->setData(
          model->index(0, 2),
          QVariant::fromValue<tlp::Color>(TulipSettings::instance().defaultColor(tlp::EDGE, true)));

    break;

  case 1: // default size
    if (updateMode == RESET_NODE)
      model->setData(
          model->index(1, 1),
          QVariant::fromValue<tlp::Size>(TulipSettings::instance().defaultSize(tlp::NODE, true)));
    else
      model->setData(
          model->index(1, 2),
          QVariant::fromValue<tlp::Size>(TulipSettings::instance().defaultSize(tlp::EDGE, true)));

  case 2: // default shape
    if (updateMode == RESET_NODE)
      model->setData(model->index(2, 1),
                     QVariant::fromValue<NodeShape::NodeShapes>(static_cast<NodeShape::NodeShapes>(
                         TulipSettings::instance().defaultShape(tlp::NODE, true))));
    else
      model->setData(model->index(2, 2),
                     QVariant::fromValue<EdgeShape::EdgeShapes>(static_cast<EdgeShape::EdgeShapes>(
                         TulipSettings::instance().defaultShape(tlp::EDGE, true))));

  case 3: // default selection color
    if (updateMode == RESET_NODE)
      model->setData(
          model->index(3, 1),
          QVariant::fromValue<tlp::Color>(TulipSettings::instance().defaultSelectionColor(true)));
    else
      model->setData(
          model->index(3, 2),
          QVariant::fromValue<tlp::Color>(TulipSettings::instance().defaultSelectionColor(true)));

  case 4: // default label color
    if (updateMode == RESET_NODE)
      model->setData(model->index(4, 1), QVariant::fromValue<tlp::Color>(
                                             TulipSettings::instance().defaultLabelColor(true)));
    else
      model->setData(model->index(4, 2), QVariant::fromValue<tlp::Color>(
                                             TulipSettings::instance().defaultLabelColor(true)));

  default:
    break;
  }
}

void PreferencesDialog::showGraphDefaultsContextMenu(const QPoint &p) {
  QModelIndex idx = _ui->graphDefaultsTable->indexAt(p);

  if (idx.column() == 0) {
    QMenu contextMenu;
    // the style sheet below allows to display disabled items
    // as "title" items in the "mainMenu"
    contextMenu.setStyleSheet("QMenu[mainMenu = \"true\"]::item:disabled {color: white; "
                              "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:, "
                              "y2:1, stop:0 rgb(75,75,75), stop:1 rgb(60, 60, 60))}");
    // so it is the "mainMenu"
    contextMenu.setProperty("mainMenu", true);
    Perspective::redirectStatusTipOfMenu(&contextMenu);

    int row = idx.row();
    QString defaultProp = _ui->graphDefaultsTable->item(row, idx.column())->text();
    QAction *action = contextMenu.addAction(defaultProp);
    action->setEnabled(false);
    contextMenu.addSeparator();

    if (row < 3) {
      QMenu *subMenu = contextMenu.addMenu(QString("Reset to Tulip predefined"));
      subMenu->setToolTip(
          QString("Choose the type of elements for which the default value will be reset"));
      action = subMenu->addAction(QString("Node default value"));
      action->setToolTip(QString("Reset the node ") + defaultProp +
                         " to the Tulip predefined value");
      action->setData(QVariant(int(RESET_NODE)));
      action = subMenu->addAction(QString("Edge default value"));
      action->setToolTip(QString("Reset the edge ") + defaultProp +
                         " to the Tulip predefined value");
      action->setData(QVariant(int(RESET_EDGE)));
      action = subMenu->addAction(QString("Node/Edge default values"));
      action->setToolTip(QString("Reset the node/edge ") + defaultProp +
                         " to the Tulip predefined value");
      action->setData(QVariant(int(RESET_BOTH)));
    } else {
      action = contextMenu.addAction(QString("Reset to Tulip predefined value"));
      action->setData(QVariant(int(RESET_BOTH)));
      action->setToolTip(QString("Reset ") + defaultProp + " to the Tulip predefined value");
    }

    action = contextMenu.exec(QCursor::pos() - QPoint(5, 5));

    if (action)
      resetToTulipDefaults(row, action->data().toInt());
  }
}
