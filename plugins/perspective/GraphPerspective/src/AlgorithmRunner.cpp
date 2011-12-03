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

#include <tulip/ColorProperty.h>
#include <tulip/PropertyAlgorithm.h>
#include <tulip/Algorithm.h>
#include <tulip/PluginLister.h>
#include <tulip/ParameterListModel.h>
#include <tulip/TulipItemDelegate.h>

#include "ui_AlgorithmRunner.h"
#include "ui_AlgorithmRunnerItem.h"
#include "ExpandableGroupBox.h"
#include "GraphPerspective.h"

#include <QtCore/QDebug>

#include <QtGui/QLabel>
#include <QtGui/QToolButton>
#include <QtGui/QMessageBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>

using namespace tlp;

// **********************************************
// Helper classes
// **********************************************
AlgorithmRunnerItem::AlgorithmRunnerItem(const QString &group, const QString &name, const tlp::ParameterList& params, QWidget *parent): QWidget(parent), _ui(new Ui::AlgorithmRunnerItemData), _group(group), _params(params) {
  _ui->setupUi(this);
  setEnabled(false);
  _ui->algNameLabel->setText(name);
  _ui->algNameLabel->setToolTip(_ui->algNameLabel->text());
  _ui->settingsButton->setToolTip(trUtf8("Set up ") + name);
  _ui->playButton->setToolTip(trUtf8("Run ") + name);
  _ui->parameters->hide();
  _ui->settingsButton->setVisible(params.size()>0);
  _ui->parameters->setItemDelegate(new TulipItemDelegate);
  setObjectName(name);
  connect(_ui->playButton,SIGNAL(clicked()),this,SIGNAL(run()));
}

QString AlgorithmRunnerItem::name() const {
  return objectName();
}

QString AlgorithmRunnerItem::group() const {
  return _group;
}

void AlgorithmRunnerItem::setGraph(tlp::Graph* graph) {
  setEnabled(graph != NULL);

  if (graph != NULL)
    _ui->parameters->setModel(new ParameterListModel(_params,graph));
  else {
    _ui->parameters->setModel(NULL);
    return;
  }

  int h=0;

  for (int i=0; i<_ui->parameters->model()->rowCount(); ++i)
    h += _ui->parameters->rowHeight(i);

  _ui->parameters->setMaximumHeight(h);
  _ui->parameters->setMinimumHeight(h);
}

AlgorithmRunnerItem::~AlgorithmRunnerItem() {
  delete _ui;
}

void AlgorithmRunnerItem::toggleParameters(bool f) {
  _ui->settingsButton->setChecked(f);
}

void AlgorithmRunnerItem::settingsButtonToggled(bool f) {
  _ui->parameters->setVisible(f);
  emit settingsToggled(f);
}

tlp::DataSet AlgorithmRunnerItem::params() const {
  if (_ui->parameters->model() == NULL)
    return tlp::DataSet();

  return static_cast<tlp::ParameterListModel*>(_ui->parameters->model())->parametersValues();
}

// **********************************************
template<typename ALG,typename PROPTYPE>
class PropertyPluginFacade: public PluginFacade {
  typedef tlp::StaticPluginLister<ALG,tlp::PropertyContext> Lister;
  std::string _defaultPropName;
  PropertyInterface* _lastComputedProperty;
public:
  PropertyPluginFacade(const std::string& defaultPropertyName): _defaultPropName(defaultPropertyName), _lastComputedProperty(NULL) {
  }
  PropertyInterface* lastComputedProperty() const {
    return _lastComputedProperty;
  }
  PROPTYPE* getProperty(tlp::Graph* g, const std::string& name) {
    PROPTYPE* result = NULL;

    if (g->existProperty(name)) {
      PropertyInterface* interface = g->getProperty(name);
      result = dynamic_cast<PROPTYPE*>(interface);
    }
    else {
      result = g->getProperty<PROPTYPE>(name);
    }

    return result;
  }
  PROPTYPE* getLocalProperty(tlp::Graph* g, const std::string& name) {
    PROPTYPE* result = NULL;

    if (g->existProperty(name)) {
      PropertyInterface* interface = g->getProperty(name);

      if (dynamic_cast<PROPTYPE*>(interface) != NULL)
        result = g->getLocalProperty<PROPTYPE>(name);
    }
    else {
      result = g->getProperty<PROPTYPE>(name);
    }

    return result;
  }
  QMap<QString,QStringList> algorithms() {
    QMap<QString,QStringList> result;
    std::string name;
    tlp::Iterator<std::string> *it = Lister::availablePlugins();

    while (it->hasNext()) {
      name = it->next();
      QString group = Lister::pluginInformations(name).getGroup().c_str();
      QStringList lst = result[group];
      lst << name.c_str();
      result[group] = lst;
    }

    delete it;
    return result;
  }
  bool computeProperty(tlp::Graph* g, const QString &alg, QString &msg, tlp::PluginProgress *progress, tlp::DataSet *data, bool isLocal=false) {
    tlp::Observable::holdObservers();
    PROPTYPE* defaultProperty = (isLocal ? getLocalProperty(g,_defaultPropName) : getProperty(g,_defaultPropName));
    QString name = (alg + " " + data->toString().c_str());
    PROPTYPE* namedProperty = (isLocal ? getLocalProperty(g,name.toStdString()) : getProperty(g,name.toStdString()));

    if (defaultProperty == NULL && namedProperty == NULL) {
      msg = trUtf8("Cannot find suitable output property");
      return false;
    }

    PROPTYPE* out = (defaultProperty != NULL ? defaultProperty : namedProperty);
    _lastComputedProperty = out;
    std::string errorMsg;
    bool result = g->computeProperty<PROPTYPE>(alg.toStdString(),out,errorMsg,progress,data);
    msg = errorMsg.c_str();

    if (result && out != namedProperty && namedProperty != NULL) {
      *namedProperty = *out;
    }

    tlp::Observable::unholdObservers();
    return result;
  }
  tlp::ParameterList parameters(const QString &alg) {
    return Lister::getPluginParameters(alg.toStdString());
  }
};


class GeneralPluginFacade: public PluginFacade {
public:
  virtual QMap<QString,QStringList> algorithms() {
    QMap<QString,QStringList> result;
    Iterator<std::string>* it = AlgorithmLister::availablePlugins();

    while (it->hasNext()) {
      QString name = it->next().c_str();
      QString group = AlgorithmLister::pluginInformations(name.toStdString()).getGroup().c_str();
      QStringList nameList = result[group];
      nameList << name;
      result[group] = nameList;
    }

    delete it;
    return result;
  }
  virtual bool computeProperty(tlp::Graph* g,const QString& alg, QString& msg, tlp::PluginProgress* progress=0, tlp::DataSet *data=0,bool isLocal=false) {
    std::string errorMsg;
    bool result = g->applyAlgorithm(errorMsg,data,alg.toStdString(),progress);
    msg = errorMsg.c_str();
    return result;
  }
  virtual tlp::ParameterList parameters(const QString& alg) {
    return AlgorithmLister::getPluginParameters(alg.toStdString());
  }
  PropertyInterface* lastComputedProperty() const {
    return NULL;
  }
};

// **********************************************

QMap<QString,PluginFacade *> AlgorithmRunner::FACADES_UI_NAMES;

void AlgorithmRunner::staticInit() {
  if (FACADES_UI_NAMES.empty()) {
    FACADES_UI_NAMES[trUtf8("Coloring algorithms")] = new PropertyPluginFacade<tlp::ColorAlgorithm,tlp::ColorProperty>("viewColor");
    FACADES_UI_NAMES[trUtf8("Filtering algorithms")] = new PropertyPluginFacade<tlp::BooleanAlgorithm,tlp::BooleanProperty>("viewSelection");
    FACADES_UI_NAMES[trUtf8("Metric algorithms (double)")] = new PropertyPluginFacade<tlp::DoubleAlgorithm,tlp::DoubleProperty>("viewMetric");
    FACADES_UI_NAMES[trUtf8("Metric algorithms (integer)")] = new PropertyPluginFacade<tlp::IntegerAlgorithm,tlp::IntegerProperty>("viewInteger");
    FACADES_UI_NAMES[trUtf8("Labeling algorithms")] = new PropertyPluginFacade<tlp::StringAlgorithm,tlp::StringProperty>("viewLabel");
    FACADES_UI_NAMES[trUtf8("Layout algorithms")] = new PropertyPluginFacade<tlp::LayoutAlgorithm,tlp::LayoutProperty>("viewLayout");
    FACADES_UI_NAMES[trUtf8("Resizing algorithms")] = new PropertyPluginFacade<tlp::SizeAlgorithm,tlp::SizeProperty>("viewSize");
    FACADES_UI_NAMES[trUtf8("General algorithms")] = new GeneralPluginFacade();
  }
}

AlgorithmRunner::AlgorithmRunner(QWidget *parent): QWidget(parent), _ui(new Ui::AlgorithmRunnerData), _pluginsListMgr(0), _model(NULL) {
  staticInit();
  _ui->setupUi(this);
  // Manually setup local mode button (can't do that with designer)
  QToolButton* localModeButton = new QToolButton(_ui->header);
  localModeButton->setMaximumSize(25,25);
  localModeButton->setMinimumSize(25,25);
  localModeButton->setCheckable(true);
  localModeButton->setChecked(false);
  localModeButton->setIcon(QIcon(":/tulip/graphperspective/icons/hierarchy_add.png"));
  localModeButton->setIconSize(QSize(22,22));
  localModeButton->setToolTip(trUtf8("Always store result in local property"));
  localModeButton->setObjectName("localModeButton");
  _localModeButton = localModeButton;

  _ui->header->layout()->addWidget(localModeButton);
  connect(_ui->header,SIGNAL(menuChanged(QString)),this,SLOT(algorithmTypeChanged(QString)));
  connect(_ui->searchBox,SIGNAL(textEdited(QString)),this,SLOT(setFilter(QString)));
  _ui->header->setMenus(FACADES_UI_NAMES.keys());
}

void AlgorithmRunner::algorithmTypeChanged(const QString &type) {
  _pluginsListMgr = FACADES_UI_NAMES[type];
  assert(_pluginsListMgr);
  buildListWidget();
  _ui->searchBox->setText("");
}

void AlgorithmRunner::buildListWidget() {
  _currentAlgorithmsList = _pluginsListMgr->algorithms();

  QWidget *listWidget = new QWidget;
  listWidget->setObjectName("algorithmListContents");
  QVBoxLayout *layout = new QVBoxLayout;
  layout->setObjectName("algorithmListContentsLayout");
  layout->setSpacing(10);

  _ui->searchBox->setEnabled(!_currentAlgorithmsList.empty());

  if (_currentAlgorithmsList.empty()) {
    QLabel *errorLabel = new QLabel("<span style=\"color:#626262;\">" + AlgorithmRunner::trUtf8("There is no plugin to display.") + "</span>");
    errorLabel->setObjectName("errorLabel");
    errorLabel->setWordWrap(true);
    layout->addWidget(errorLabel);
  }
  else {
    layout->setContentsMargins(3,6,3,6);
    QString group;
    foreach(group,_currentAlgorithmsList.keys()) {
      QWidget *groupWidget = new QWidget;
      groupWidget->setObjectName(group + "_contents");

      QVBoxLayout *groupLayout = new QVBoxLayout;
      groupLayout->setObjectName(group + "_layout");
      groupLayout->setSpacing(4);
      groupWidget->setLayout(groupLayout);

      QString algName;
      foreach(algName,_currentAlgorithmsList[group]) {
        AlgorithmRunnerItem *item = new AlgorithmRunnerItem(group,algName,_pluginsListMgr->parameters(algName));
        connect(item,SIGNAL(settingsToggled(bool)),this,SLOT(itemSettingsToggled(bool)));
        connect(item,SIGNAL(run()),this,SLOT(runAlgorithm()));
        groupLayout->addWidget(item);
      }

      if (group == "") {
        groupLayout->setContentsMargins(0,0,0,0);
        layout->addWidget(groupWidget);
      }
      else {
        groupLayout->setContentsMargins(0,15,0,6);
        ExpandableGroupBox *groupBox = new ExpandableGroupBox(group);
        groupBox->setObjectName(group);
        groupBox->setWidget(groupWidget);
        layout->addWidget(groupBox);
      }
    }
    layout->addItem(new QSpacerItem(2,2,QSizePolicy::Maximum,QSizePolicy::Expanding));
  }

  listWidget->setLayout(layout);

  _ui->algorithmList->setWidget(listWidget);

  if (_model)
    currentGraphChanged(_model->currentGraph());
}

void AlgorithmRunner::setFilter(const QString &filter) {
  ExpandableGroupBox *gi;
  foreach(gi, findChildren<ExpandableGroupBox *>())
  gi->setVisible(gi->objectName().contains(filter,Qt::CaseInsensitive));
  foreach(AlgorithmRunnerItem* it, findChildren<AlgorithmRunnerItem *>()) {
    if (!it->parentWidget()->isVisible())
      it->hide();
  }
  foreach(AlgorithmRunnerItem* it, findChildren<AlgorithmRunnerItem *>()) {
    if (it->objectName().contains(filter,Qt::CaseInsensitive)) {
      it->show();
      it->parentWidget()->parentWidget()->show();
    }
  }
}

void AlgorithmRunner::currentGraphChanged(tlp::Graph* g) {
  foreach(AlgorithmRunnerItem* it, findChildren<AlgorithmRunnerItem *>()) {
    it->setGraph(g);
  }
}

void AlgorithmRunner::setModel(GraphHierarchiesModel *model) {
  _model = model;
  connect(_model,SIGNAL(currentGraphChanged(tlp::Graph*)),this,SLOT(currentGraphChanged(tlp::Graph*)));
  currentGraphChanged(_model->currentGraph());
}

void AlgorithmRunner::itemSettingsToggled(bool f) {
  if (!f)
    return;

  foreach(AlgorithmRunnerItem* it, findChildren<AlgorithmRunnerItem *>()) {
    if (it != sender())
      it->toggleParameters(false);
  }
}


void AlgorithmRunner::runAlgorithm() {
  if (_model->currentGraph() == NULL)
    return;

  AlgorithmRunnerItem* item = static_cast<AlgorithmRunnerItem*>(sender());
  QString msg;
  DataSet data = item->params();
  bool result = _pluginsListMgr->computeProperty(_model->currentGraph(),item->name(),msg,0,&data,isLocalMode());

  if (!result) {
    QMessageBox::critical(this,trUtf8("Plugin error"),trUtf8("Error while running ") + item->name() + trUtf8(": ") + msg);
  }
  else if (_pluginsListMgr->lastComputedProperty() != NULL) {
    // Asks the perspective to check if any view needs to be centered after computing this algorithm
    static_cast<GraphPerspective*>(Perspective::instance())->centerPanels(_pluginsListMgr->lastComputedProperty());
  }
}

bool AlgorithmRunner::isLocalMode() const {
  return _localModeButton->isChecked();
}
