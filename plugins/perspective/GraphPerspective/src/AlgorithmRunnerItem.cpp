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

#include <QMouseEvent>
#include <QMessageBox>
#include <QDrag>
#include <QtCore/QDateTime>

#include "GraphPerspective.h"
#include "AlgorithmRunnerItem.h"
#include "ui_AlgorithmRunnerItem.h"

#include <tulip/GraphTest.h>
#include <tulip/TulipMimes.h>
#include <tulip/TulipItemDelegate.h>
#include <tulip/ParameterListModel.h>
#include <tulip/TulipSettings.h>
#include <tulip/TlpQtTools.h>
#include <tulip/BooleanProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/GraphProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/TulipMetaTypes.h>

using namespace tlp;

AlgorithmRunnerItem::AlgorithmRunnerItem(QString pluginName, QWidget *parent): QWidget(parent), _ui(new Ui::AlgorithmRunnerItem), _pluginName(pluginName), _graph(NULL), _storeResultAsLocal(true) {
  _ui->setupUi(this);
  connect(_ui->favoriteCheck,SIGNAL(toggled(bool)),this,SIGNAL(favorized(bool)));
  const Plugin& plugin = PluginLister::instance()->pluginInformation(pluginName.toStdString());
  // split pluginName after the second word if needed
  QStringList words = pluginName.split(' ');

  if (words.size() > 3) {
    QString name = pluginName;
    name.replace(words[1] + ' ', words[1] + '\n');
    _ui->playButton->setText(name);
  }
  else
    _ui->playButton->setText(pluginName);

  _ui->playButton->setStyleSheet ("text-align: left");
  QString tooltip(QString("Apply '") + pluginName + "'");
  // initialize parameters only if needed
  _ui->parameters->setVisible(false);

  if (plugin.inputRequired()) {
    tooltip +=  " with current settings";
    _ui->parameters->setItemDelegate(new TulipItemDelegate);
  }
  else {
    _ui->settingsButton->setVisible(false);
  }

  std::string infos = plugin.info();

  // show infos in tooltip only if it contains more than one word
  if (infos.find(' ') != std::string::npos)
    _ui->playButton->setToolTip(QString("<table><tr><td>%1:</td></tr><tr><td><i>%2</i></td></tr></table>").arg(tooltip).arg(tlp::tlpStringToQString(infos)));
  else
    _ui->playButton->setToolTip(tooltip);

  setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Maximum);

  static QPixmap cppPix(":/tulip/graphperspective/icons/16/cpp.png");
  static QPixmap pythonPix(":/tulip/graphperspective/icons/16/python.png");

  if (plugin.programmingLanguage() == "Python") {
    _ui->languageLabel->setPixmap(pythonPix);
    _ui->languageLabel->setToolTip("Plugin written in Python");
  }
  else {
    _ui->languageLabel->setPixmap(cppPix);
    _ui->languageLabel->setToolTip("Plugin written in C++");
  }
}

AlgorithmRunnerItem::~AlgorithmRunnerItem() {
  delete _ui;
}

#define ISPROP(T) t->getTypeName().compare(typeid(T).name()) == 0

void AlgorithmRunnerItem::setGraph(Graph* g) {
  _graph = g;

  if (_ui->parameters->model() != NULL) {
    ParameterListModel* model = static_cast<ParameterListModel*>(_ui->parameters->model());
    DataSet dataSet = model->parametersValues();
    std::pair<std::string,tlp::DataType*> it;
    stableForEach(it,dataSet.getValues()) {
      DataType* t = it.second;

      if (ISPROP(tlp::BooleanProperty*) ||
          ISPROP(tlp::BooleanVectorProperty*) ||
          ISPROP(tlp::DoubleProperty*) ||
          ISPROP(tlp::DoubleVectorProperty*) ||
          ISPROP(tlp::LayoutProperty*) ||
          ISPROP(tlp::CoordVectorProperty*) ||
          ISPROP(tlp::StringProperty*) ||
          ISPROP(tlp::StringVectorProperty*) ||
          ISPROP(tlp::IntegerProperty*) ||
          ISPROP(tlp::IntegerVectorProperty*) ||
          ISPROP(tlp::SizeProperty*) ||
          ISPROP(tlp::SizeVectorProperty*) ||
          ISPROP(tlp::ColorProperty*) ||
          ISPROP(tlp::ColorVectorProperty*) ||
          ISPROP(tlp::NumericProperty*) ||
          ISPROP(tlp::PropertyInterface*) ||
          ISPROP(tlp::GraphProperty*)) {
        dataSet.remove(it.first);
      }
    }
    _initData = dataSet;

    _ui->parameters->setModel(NULL);
  }

  if (_ui->parameters->isVisible())
    initModel();
}

void AlgorithmRunnerItem::setData(const DataSet &data) {
  initModel();
  ParameterListModel* model = static_cast<ParameterListModel*>(_ui->parameters->model());
  model->setParametersValues(data);
}

QString AlgorithmRunnerItem::name() const {
  return _pluginName;
}

tlp::Graph *AlgorithmRunnerItem::graph() const {
  return _graph;
}

template<typename PROP>
void asLocal(QVariant var, DataSet& data, Graph* g) {
  if (var.userType() == qMetaTypeId<PROP*>()) {
    PROP* local = g->getLocalProperty<PROP>(var.value<PROP*>()->getName());
    data.set("result",local);
  }
}

static void copyToLocal(DataSet& data, Graph* g) {
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

// simple structure to hold an output property parameter
// while running an algorithm
struct OutPropertyParam {
  std::string name; // the name of the parameter
  PropertyInterface* dest; // the destination property
  PropertyInterface* tmp;  // the temporary property

  OutPropertyParam(const std::string& pName)
    : name(pName), dest(NULL), tmp(NULL) {}
};

class AlgorithmPreviewHandler: public ProgressPreviewHandler {
  Graph* graph;
  const std::vector<OutPropertyParam>& outPropParams;
  bool inited;
  std::map<std::string, tlp::PropertyInterface*> outPropsMap;
public:
  AlgorithmPreviewHandler(Graph* g, std::vector<OutPropertyParam>& opp):
    graph(g), outPropParams(opp), inited(false) {
  }

  ~AlgorithmPreviewHandler() {
    if (!outPropsMap.empty()) {
      // build outPropsMap with initial properties
      std::vector<OutPropertyParam>::const_iterator it = outPropParams.begin();

      for (; it != outPropParams.end(); ++it) {
        const std::string& outPropName = it->dest->getName();

        if (it->tmp && !outPropName.empty())
          outPropsMap[outPropName] = it->dest;
      }

      // restore initial properties
      Perspective::typedInstance<GraphPerspective>()->setGlMainViewPropertiesForGraph(graph, outPropsMap);
    }
  }

  void progressStateChanged(int, int) {
    if (!inited) {
      // build outPropsMap with temporary properties
      std::vector<OutPropertyParam>::const_iterator it = outPropParams.begin();

      for (; it != outPropParams.end(); ++it) {
        const std::string& outPropName = it->dest->getName();

        if (it->tmp && !outPropName.empty())
          outPropsMap[outPropName] = it->tmp;
      }

      inited = true;

      if (!outPropsMap.empty() &&
          // set temporary properties as drawing properties
          (Perspective::typedInstance<GraphPerspective>()->setGlMainViewPropertiesForGraph(graph, outPropsMap) == false))
        // clear map if there is nothing to do
        outPropsMap.clear();
    }

    // draw with temporary computed properties
    if (!outPropsMap.empty()) {
      Perspective::typedInstance<GraphPerspective>()->centerPanelsForGraph(graph, true, true);
    }
  }
};

#define TN( T )   typeid(T).name()

void AlgorithmRunnerItem::run(Graph *g) {
  initModel();

  if (g == NULL)
    g=_graph;

  if (g == NULL) {
    qCritical() << QStringToTlpString(name()) << trUtf8(": No graph selected");
    return;
  }

  Observable::holdObservers();
  DataSet originalDataSet = static_cast<ParameterListModel*>(_ui->parameters->model())->parametersValues();

  DataSet dataSet(originalDataSet);

  g->push();

  if (_storeResultAsLocal)
    copyToLocal(dataSet, g);

  std::string algorithm = _pluginName.toStdString();
  std::string algoAndParams = algorithm + " - " + dataSet.toString();
  std::vector<std::string> outNonPropertyParams;
  // use temporary output properties
  // to ease the undo in case of failure
  std::vector<OutPropertyParam> outPropertyParams;
  ParameterDescriptionList paramList = PluginLister::getPluginParameters(algorithm);
  ParameterDescription desc;
  forEach(desc, paramList.getParameters()) {
    std::string typeName(desc.getTypeName());

    // forget non property out param
    if (typeName != TN(PropertyInterface*)
        && typeName != TN(NumericProperty*)
        && typeName != TN(BooleanProperty)
        && typeName != TN(DoubleProperty)
        && typeName != TN(LayoutProperty)
        && typeName != TN(StringProperty)
        && typeName != TN(IntegerProperty)
        && typeName != TN(SizeProperty)
        && typeName != TN(ColorProperty)
        && typeName != TN(BooleanVectorProperty)
        && typeName != TN(DoubleVectorProperty)
        && typeName != TN(CoordVectorProperty)
        && typeName != TN(StringVectorProperty)
        && typeName != TN(IntegerVectorProperty)
        && typeName != TN(SizeVectorProperty)
        && typeName != TN(ColorVectorProperty)) {
      if (desc.getDirection() != IN_PARAM)
        outNonPropertyParams.push_back(desc.getName());

      continue;
    }

    if (desc.getDirection() == IN_PARAM) {
      if (desc.isMandatory()) {
        // if it is a mandatory input property
        // check it is not null
        PropertyInterface* prop = NULL;
        dataSet.get(desc.getName(), prop);

        if (prop == NULL) {
          g->pop();
          Observable::holdObservers();
          QString message("Mandatory property parameter '");
          message += tlp::tlpStringToQString(desc.getName());
          message += "'<br/> cannot be null";
          qCritical() << message;
          QMessageBox::critical(parentWidget(), name(), message);
          return;
        }
      }

      continue;
    }

    OutPropertyParam outPropParam(desc.getName());
    // get destination property
    dataSet.get(desc.getName(), outPropParam.dest);
    // clone it in a not registered (because unnamed)
    // temporary property
    outPropParam.tmp = outPropParam.dest ?
                       outPropParam.dest->clonePrototype(outPropParam.dest->getGraph(), "") :
                       NULL;
    // set the temporary as the destination property
    dataSet.set(desc.getName(), outPropParam.tmp);

    if (outPropParam.tmp) {
      outPropertyParams.push_back(outPropParam);

      if (desc.getDirection() == OUT_PARAM) {
        outPropParam.tmp->setAllNodeDataMemValue(outPropParam.dest->getNodeDefaultDataMemValue());
        outPropParam.tmp->setAllEdgeDataMemValue(outPropParam.dest->getEdgeDefaultDataMemValue());
      }
      else
        // inout property
        outPropParam.tmp->copy(outPropParam.dest);
    }
  }

  //Perspective::typedInstance<GraphPerspective>()->setAutoCenterPanelsOnDraw(true);
  std::string errorMessage;
  PluginProgress* progress = Perspective::instance()->progress();
  progress->setTitle(algorithm);

  // set preview handler if needed
  if (!outPropertyParams.empty())
    progress->setPreviewHandler(new AlgorithmPreviewHandler(g, outPropertyParams));
  else
    progress->showPreview(false);

  // take time before run
  QDateTime start = QDateTime::currentDateTime();
  bool result = g->applyAlgorithm(algorithm,errorMessage,&dataSet,progress);

  if (!outPropertyParams.empty())
    progress->setPreviewHandler(NULL);

  // display spent time
  if (TulipSettings::instance().isRunningTimeComputed())
    qDebug() << algoAndParams << ": " << start.msecsTo(QDateTime::currentDateTime()) << "ms";

  //Perspective::typedInstance<GraphPerspective>()->setAutoCenterPanelsOnDraw(false);

  if (!result) {
    g->pop();

    if (progress->state() == TLP_CANCEL && errorMessage.empty()) {
      errorMessage = QStringToTlpString(trUtf8("Cancelled by user"));
      qWarning() << QStringToTlpString(name()).c_str() << ": " << errorMessage.c_str();
      QMessageBox::warning(parentWidget(), name(), errorMessage.c_str());
    }
    else {
      qCritical() << QStringToTlpString(name()).c_str() << ": " << errorMessage.c_str();
      QMessageBox::critical(parentWidget(), name(), errorMessage.c_str());
    }
  }
  else {
    if (progress->state() == TLP_STOP) {
      errorMessage = QStringToTlpString(trUtf8("Stopped by user"));
      qWarning() << QStringToTlpString(name()).c_str() << ": " << errorMessage.c_str();
      QMessageBox::warning(parentWidget(), name(), errorMessage.c_str());
    }
  }

  delete progress;

  if (result) {
    // copy or cleanup out properties
    std::vector<OutPropertyParam>::const_iterator it = outPropertyParams.begin();

    for (; it != outPropertyParams.end(); ++it) {
      // copy computed property in the original output property
      it->dest->copy(it->tmp);
      // restore it in the dataset
      dataSet.set(it->name, it->dest);

      if (it->name == "result" &&
          TulipSettings::instance().isResultPropertyStored()) {
        // store the result property values in an automatically named property
        std::string storedResultName = algoAndParams
                                       + "(" + it->dest->getName() + ")";
        PropertyInterface* storedResultProp =
          it->dest->clonePrototype(it->dest->getGraph(),
                                   storedResultName);
        storedResultProp->copy(it->tmp);
      }

      delete it->tmp;
    }
  }

  afterRun(g, dataSet);

  if (result && !outNonPropertyParams.empty()) {
    // only show computed value of non property output parameters.
    // output property params are not taken into account
    // because they may have been created on the fly
    // (local properties see copyToLocal function above)
    // and thus they may be deleted further in case of undo
    for (unsigned int i = 0; i < outNonPropertyParams.size(); ++i) {
      tlp::DataType *dataType = dataSet.getData(outNonPropertyParams[i]);
      originalDataSet.setData(outNonPropertyParams[i], dataType);
    }

    ParameterListModel* model = static_cast<ParameterListModel*>(_ui->parameters->model());
    model->setParametersValues(originalDataSet);
  }

  while (Observable::observersHoldCounter() > 0)
    Observable::unholdObservers();
}

void AlgorithmRunnerItem::setStoreResultAsLocal(bool m) {
  _storeResultAsLocal = m;
}

void AlgorithmRunnerItem::mousePressEvent(QMouseEvent *ev) {
  if (ev->button() == Qt::LeftButton)
    _dragStartPosition = ev->pos();
}

void AlgorithmRunnerItem::mouseMoveEvent(QMouseEvent *ev) {
  if (!(ev->buttons() & Qt::LeftButton) ||
      (ev->pos() - _dragStartPosition).manhattanLength() < QApplication::startDragDistance()) {
    QWidget::mouseMoveEvent(ev);
    return;
  }

  QDrag *drag = new QDrag(this);
  const Plugin& p = PluginLister::pluginInformation(_pluginName.toStdString().c_str());
  QPixmap icon(QPixmap(p.icon().c_str()).scaled(64,64));
  QFont f;
  f.setBold(true);
  QFontMetrics metrics(f);
  int textHeight = metrics.boundingRect(0,0,icon.width(),INT_MAX,Qt::AlignTop | Qt::AlignHCenter | Qt::TextWordWrap,_pluginName).height();
  QPixmap pix(icon.width()+textHeight,icon.height()+textHeight);
  pix.fill(Qt::white);
  QPainter painter(&pix);
  painter.drawPixmap(pix.width()/2 - icon.width()/2,0,icon.width(),icon.height(),icon);
  painter.setFont(f);
  painter.drawText(0,icon.height(),pix.width(),pix.height()-icon.height(),Qt::AlignCenter | Qt::AlignHCenter | Qt::TextWordWrap,_pluginName);
  painter.setBrush(Qt::transparent);
  painter.setPen(QColor(169, 169, 169));
  painter.drawRect(0,0,pix.width()-1,pix.height()-1);
  drag->setPixmap(pix);

  initModel();
  AlgorithmMimeType* mimeData = new AlgorithmMimeType(name(),static_cast<ParameterListModel*>(_ui->parameters->model())->parametersValues());
  connect(mimeData,SIGNAL(mimeRun(tlp::Graph*)),this,SLOT(run(tlp::Graph*)));
  drag->setMimeData(mimeData);
  drag->exec(Qt::CopyAction | Qt::MoveAction);
}

void AlgorithmRunnerItem::afterRun(Graph* g, const tlp::DataSet& dataSet) {
  PluginLister* pluginLister = PluginLister::instance();
  std::string stdName = name().toStdString();

  if (pluginLister->pluginExists<LayoutAlgorithm>(stdName)) {
    if (TulipSettings::instance().isAutomaticRatio()) {
      LayoutProperty* prop = NULL;
      dataSet.get<LayoutProperty*>("result",prop);

      if (prop)
        prop->perfectAspectRatio();
    }

    Perspective::typedInstance<GraphPerspective>()->centerPanelsForGraph(g);
  }
  else if (pluginLister->pluginExists<Algorithm>(stdName) &&
           !pluginLister->pluginExists<PropertyAlgorithm>(stdName) &&
           !pluginLister->pluginExists<GraphTest>(stdName)) {
    Perspective::typedInstance<GraphPerspective>()->centerPanelsForGraph(g);
  }
  else if (pluginLister->pluginExists<DoubleAlgorithm>(stdName) &&
           TulipSettings::instance().isAutomaticMapMetric()) {
    DoubleProperty* prop = NULL;
    dataSet.get<DoubleProperty*>("result",prop);

    if (prop != NULL && prop->getName().compare("viewMetric") == 0) {
      std::string errMsg;
      ColorProperty* color;

      if (g->existLocalProperty("viewColor"))
        color = g->getLocalProperty<ColorProperty>("viewColor");
      else {
        color = g->getLocalProperty<ColorProperty>("viewColor");
        ColorProperty* ancestorColor =
          g->getSuperGraph()->getProperty<ColorProperty>("viewColor");
        // same default values as ancestor property default values
        color->setAllNodeDataMemValue(ancestorColor->getNodeDefaultDataMemValue());
        color->setAllEdgeDataMemValue(ancestorColor->getEdgeDefaultDataMemValue());
      }

      g->applyPropertyAlgorithm("Color Mapping", color, errMsg);
    }
  }
  else if (pluginLister->pluginExists<GraphTest>(stdName)) {
    bool result = true;
    dataSet.get<bool>("result",result);
    std::string gname;
    g->getAttribute<std::string>("name",gname);
    std::stringstream sstr;
    sstr << stdName << (result ? " test succeed" : " test failed") << "\n on " <<  gname;

    if (result) {
      qDebug() << sstr.str().c_str();
      QMessageBox::information(parentWidget(), "Tulip test result",
                               tlp::tlpStringToQString(sstr.str()));
    }
    else {
      qWarning() << sstr.str().c_str();
      QMessageBox::warning(parentWidget(), "Tulip test result",
                           tlp::tlpStringToQString(sstr.str()));
    }
  }
}

void AlgorithmRunnerItem::setFavorite(bool f) {
  _ui->favoriteCheck->setChecked(f);
}

tlp::DataSet AlgorithmRunnerItem::data() const {
  if (_ui->parameters->model() == NULL)
    return DataSet();

  return static_cast<ParameterListModel*>(_ui->parameters->model())->parametersValues();
}


void AlgorithmRunnerItem::initModel() {
  if (_ui->parameters->model() != NULL)
    return;

  ParameterListModel* model = new ParameterListModel(PluginLister::getPluginParameters(_pluginName.toStdString()),_graph,_ui->parameters);
  _ui->parameters->setModel(model);
  int h = 10;

  for (int i=0; i<model->rowCount(); ++i)
    h += _ui->parameters->rowHeight(i);

  _ui->parameters->setMinimumSize(_ui->parameters->minimumSize().width(),h);
  _ui->parameters->setMaximumSize(_ui->parameters->maximumSize().width(),h);

  if (!_initData.empty()) {
    DataSet dataSet = model->parametersValues();
    std::pair<std::string,tlp::DataType*> it;
    forEach(it,_initData.getValues()) {
      dataSet.setData(it.first,it.second);
    }
    model->setParametersValues(dataSet);
  }
}
