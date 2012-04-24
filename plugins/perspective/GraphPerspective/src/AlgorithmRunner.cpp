/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include "AlgorithmRunner.h"

#include <QtGui/QToolButton>

#include <tulip/PluginModel.h>
#include <tulip/ParameterListModel.h>
#include <tulip/TulipItemDelegate.h>
#include <tulip/Perspective.h>
#include "ExpandableGroupBox.h"

#include "ui_AlgorithmRunner.h"
#include "ui_AlgorithmRunnerItem.h"

using namespace tlp;

void buildTreeUi(QWidget* w, PluginModel<tlp::Algorithm>* model, const QModelIndex& parent, QToolButton* localModeButton, bool root = false) {
  for (int i=0; i<model->rowCount(parent); ++i) {
    QModelIndex index = model->index(i,0,parent);
    QString name = model->data(index).toString();

    if (model->rowCount(index) > 0) {
      ExpandableGroupBox* groupBox = new ExpandableGroupBox(NULL,name);
      groupBox->setProperty("root",root);
      w->layout()->addWidget(groupBox);
      QWidget* groupWidget = new QWidget();
      groupWidget->setLayout(new QVBoxLayout);
      groupWidget->layout()->setContentsMargins(0,15,0,6);
      groupWidget->layout()->setSpacing(0);
      groupBox->setWidget(groupWidget);
      buildTreeUi(groupWidget,model,index,localModeButton);
    }
    else {
      AlgorithmRunnerItem* item = new AlgorithmRunnerItem(name);
      QObject::connect(localModeButton,SIGNAL(clicked(bool)),item,SLOT(setLocalMode(bool)));
      w->layout()->addWidget(item);
    }
  }
}

#include <QtGui/QToolButton>
AlgorithmRunner::AlgorithmRunner(QWidget* parent): QWidget(parent), _ui(new Ui::AlgorithmRunner) {
  _ui->setupUi(this);
  QToolButton* localModeButton = new QToolButton(_ui->header);
  localModeButton->setMaximumSize(25,25);
  localModeButton->setMinimumSize(25,25);
  localModeButton->setCheckable(true);
  localModeButton->setChecked(true);
  localModeButton->setIcon(QIcon(":/tulip/graphperspective/icons/16/hierarchy_add.png"));
  localModeButton->setIconSize(QSize(22,22));
  localModeButton->setToolTip(trUtf8("Always store result in local property"));
  _ui->header->insertWidget(localModeButton);
  PluginModel<tlp::Algorithm> model;
  buildTreeUi(_ui->contents, &model, QModelIndex(), localModeButton, true);
}

AlgorithmRunner::~AlgorithmRunner() {
  delete _ui;
}

void AlgorithmRunner::setGraph(Graph* g) {
  foreach(AlgorithmRunnerItem* item, findChildren<AlgorithmRunnerItem*>())
  item->setGraph(g);
}

// A not recursive implementation of QObject::findChildren
template<typename T>
QList<T> childrenObj(QObject* obj) {
  QList<T> result;
  foreach(QObject* o, obj->children()) {
    T var = dynamic_cast<T>(o);

    if (var != NULL)
      result+=var;
  }
  return result;
}
bool filterGroup(ExpandableGroupBox* group, QString filter) {
  QList<ExpandableGroupBox*> subGroups = childrenObj<ExpandableGroupBox*>(group->widget());
  QList<AlgorithmRunnerItem*> subItems = childrenObj<AlgorithmRunnerItem*>(group->widget());

  if (group->title().contains(filter,Qt::CaseInsensitive)) {
    group->show();
    foreach(ExpandableGroupBox* g, subGroups) {
      g->show();
      subItems+=childrenObj<AlgorithmRunnerItem*>(g->widget());
    }
    foreach(AlgorithmRunnerItem* i, subItems)
    i->show();
    return true;
  }

  bool groupVisible = false;
  foreach(ExpandableGroupBox* g, subGroups) {
    groupVisible |= filterGroup(g,filter);
  }
  foreach(AlgorithmRunnerItem* i, subItems) {
    bool itemVisible = i->name().contains(filter,Qt::CaseInsensitive);
    i->setVisible(itemVisible);
    groupVisible |= itemVisible;
  }
  group->setVisible(groupVisible);
  return groupVisible;
}

void AlgorithmRunner::setFilter(QString filter) {
  foreach(ExpandableGroupBox* group, childrenObj<ExpandableGroupBox*>(_ui->contents)) {
    filterGroup(group,filter);
  }
}

AlgorithmRunnerItem::AlgorithmRunnerItem(QString pluginName, QWidget *parent): QWidget(parent), _ui(new Ui::AlgorithmRunnerItem), _pluginName(pluginName), _localMode(true) {
  _ui->setupUi(this);
  _ui->algorithmName->setText(pluginName);
  _ui->parameters->setVisible(false);
  _ui->parameters->setItemDelegate(new TulipItemDelegate);
}

AlgorithmRunnerItem::~AlgorithmRunnerItem() {
  delete _ui;
}

void AlgorithmRunnerItem::setGraph(Graph* g) {
  _graph = g;
  _ui->parameters->setModel(new ParameterListModel(PluginLister::getPluginParameters(_pluginName.toStdString()),g));
}

QString AlgorithmRunnerItem::name() const {
  return _pluginName;
}


template<typename PROP>
void asLocal(QVariant var, DataSet& data, Graph* g) {
  if (var.userType() == qMetaTypeId<PROP*>()) {
    PROP* local = g->getLocalProperty<PROP>(var.value<PROP*>()->getName());
    data.set("result",local);
  }
}
void copyToLocal(DataSet& data, Graph* g) {
  if (!data.exist("result"))
    return;

  DataType* d = data.getData("result");
  QVariant var = TulipMetaTypes::dataTypeToQvariant(d,"");
  asLocal<DoubleProperty>(var,data,g);
  asLocal<IntegerProperty>(var,data,g);
  asLocal<LayoutProperty>(var,data,g);
  asLocal<SizeProperty>(var,data,g);
  asLocal<ColorProperty>(var,data,g);
  asLocal<BooleanProperty>(var,data,g);
  asLocal<StringProperty>(var,data,g);
}
void AlgorithmRunnerItem::run(Graph *g) {
  if (g == NULL)
    g=_graph;

  if (g == NULL) {
    qCritical() << name() << trUtf8(": No graph selected");
    return;
  }

  Observable::holdObservers();
  DataSet dataSet = static_cast<ParameterListModel*>(_ui->parameters->model())->parametersValues();

  if (_localMode)
    copyToLocal(dataSet, g);

  std::string errorMessage;
  PluginProgress* progress = Perspective::instance()->progress();
  bool result = g->applyAlgorithm(_pluginName.toStdString(),errorMessage,&dataSet,progress);
  delete progress;

  if (!result)
    qCritical() << name() << ": " << errorMessage.c_str();

  Observable::unholdObservers();
}

void AlgorithmRunnerItem::setLocalMode(bool m) {
  _localMode = m;
}
