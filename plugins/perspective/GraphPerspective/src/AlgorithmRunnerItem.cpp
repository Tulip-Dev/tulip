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

#include <QMouseEvent>
#include <QMessageBox>
#include <QDrag>
#include <QtCore/QTime>

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
#include <tulip/ColorScalesManager.h>
#include <tulip/StableIterator.h>
#include <tulip/PluginLister.h>

using namespace tlp;

AlgorithmRunnerItem::AlgorithmRunnerItem(QString pluginName, bool darkBackground, QWidget *parent)
    : QWidget(parent), _ui(new Ui::AlgorithmRunnerItem), _pluginName(pluginName), _graph(nullptr),
      _storeResultAsLocal(true) {
  setProperty("algorithRunnerItem", true);
  _ui->setupUi(this);
  connect(_ui->favoriteCheck, SIGNAL(toggled(bool)), this, SIGNAL(favorized(bool)));
  const Plugin &plugin = PluginLister::pluginInformation(QStringToTlpString(pluginName));
  // split pluginName after the second word if needed
  QStringList words = pluginName.split(' ');

  if (words.size() > 3) {
    QString name = pluginName;
    name.replace(words[1] + ' ', words[1] + '\n');
    _ui->playButton->setText(name);
  } else
    _ui->playButton->setText(pluginName);

  _ui->playButton->setStyleSheet("text-align: left");
  QString tooltip(QString("<b>%1</b> <small>(%2 plugin)</small>")
                      .arg(pluginName)
                      .arg(plugin.programmingLanguage().c_str()));
  // initialize parameters only if needed
  _ui->parameters->setVisible(false);
  // set foreground colors according to contents background color
  if (darkBackground) {
    _ui->parameters->setStyleSheet("QHeaderView::section { color: white }");
    _ui->playButton->setStyleSheet("QPushButton { color: white; text-align: left; } ");
  }

  if (!plugin.getParameters().empty()) {
    _ui->parameters->setItemDelegate(new TulipItemDelegate(_ui->parameters));
  } else {
    _ui->settingsButton->setVisible(false);
  }

  std::string &&info = plugin.info();

  // show info in tooltip only if it contains more than one word
  if (info.find(' ') != std::string::npos)
    _ui->playButton->setToolTip(
        QString("%1:<br/><i>%2</i>").arg(tooltip).arg(tlp::tlpStringToQString(info)));
  else
    _ui->playButton->setToolTip(tooltip);

  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

  connect(_ui->favoriteCheck, SIGNAL(stateChanged(int)), this, SLOT(favoriteChanged(int)));
  // used to display plugin doc in a QMessageBox
  _ui->settingsButton->installEventFilter(this);
}

AlgorithmRunnerItem::~AlgorithmRunnerItem() {
  delete _ui->parameters->itemDelegate();
  delete _ui;
}

void AlgorithmRunnerItem::setGraph(Graph *g) {
  _graph = g;

  if (_ui->parameters->model() != nullptr) {
    ParameterListModel *model = static_cast<ParameterListModel *>(_ui->parameters->model());
    DataSet dataSet = model->parametersValues();
    for (const std::pair<std::string, tlp::DataType *> &it : stableIterator(dataSet.getValues())) {
      if (it.second->isTulipProperty()) {
        dataSet.remove(it.first);
      }
    }
    _initData = dataSet;

    _ui->parameters->setModel(nullptr);
  }

  if (_ui->parameters->isVisible())
    initModel();
}

void AlgorithmRunnerItem::setData(const DataSet &data) {
  initModel();
  ParameterListModel *model = static_cast<ParameterListModel *>(_ui->parameters->model());
  model->setParametersValues(data);
}

QString AlgorithmRunnerItem::name() const {
  return _pluginName;
}

tlp::Graph *AlgorithmRunnerItem::graph() const {
  return _graph;
}

template <typename PROP>
void asLocal(QVariant var, DataSet &data, Graph *g) {
  if (var.userType() == qMetaTypeId<PROP *>()) {
    PROP *prop = var.value<PROP *>();
    if (!prop) {
      data.remove("result");
      return;
    }
    const std::string &propName = prop->getName();
    bool hasProp = g->existLocalProperty(propName);
    PROP *local = g->getLocalProperty<PROP>(propName);

    if (!hasProp) {
      // copy default property values to ensure
      // the inheritance of user defined property settings
      local->setAllNodeValue(prop->getNodeDefaultValue());
      local->setAllEdgeValue(prop->getEdgeDefaultValue());
    }

    data.set("result", local);
  }
}

static void copyToLocal(DataSet &data, Graph *g) {
  if (!data.exists("result"))
    return;

  DataType *d = data.getData("result");
  QVariant var = TulipMetaTypes::dataTypeToQvariant(d, "");
  delete d;
  asLocal<DoubleProperty>(var, data, g);
  asLocal<IntegerProperty>(var, data, g);
  asLocal<LayoutProperty>(var, data, g);
  asLocal<SizeProperty>(var, data, g);
  asLocal<ColorProperty>(var, data, g);
  asLocal<BooleanProperty>(var, data, g);
  asLocal<StringProperty>(var, data, g);
}

// simple structure to hold an output property parameter
// while running an algorithm
struct OutPropertyParam {
  std::string name;        // the name of the parameter
  PropertyInterface *dest; // the destination property
  PropertyInterface *tmp;  // the temporary property

  static bool inUse;

  OutPropertyParam(const std::string &pName) : name(pName), dest(nullptr), tmp(nullptr) {}

  ~OutPropertyParam() {
    if (!inUse)
      delete tmp;
  }
};

bool OutPropertyParam::inUse = false;

class AlgorithmPreviewHandler : public ProgressPreviewHandler {
  Graph *graph;
  const std::vector<OutPropertyParam> &outPropParams;
  bool inited;
  std::map<std::string, tlp::PropertyInterface *> outPropsMap;

public:
  AlgorithmPreviewHandler(Graph *g, std::vector<OutPropertyParam> &opp)
      : graph(g), outPropParams(opp), inited(false) {}

  ~AlgorithmPreviewHandler() override {
    if (!outPropsMap.empty()) {
      // build outPropsMap with initial properties
      for (auto &outPropParam : outPropParams) {
        const std::string &outPropName = outPropParam.dest->getName();

        if (outPropParam.tmp && !outPropName.empty())
          outPropsMap[outPropName] = outPropParam.dest;
      }

      // restore initial properties
      Perspective::typedInstance<GraphPerspective>()->setGlMainViewPropertiesForGraph(graph,
                                                                                      outPropsMap);
    }
  }

  void progressStateChanged(int, int) override {
    if (!inited) {
      // build outPropsMap with temporary properties
      for (auto &outPropParam : outPropParams) {
        const std::string &outPropName = outPropParam.dest->getName();

        if (outPropParam.tmp && !outPropName.empty())
          outPropsMap[outPropName] = outPropParam.tmp;
      }

      inited = true;

      if (!outPropsMap.empty() &&
          // set temporary properties as drawing properties
          (Perspective::typedInstance<GraphPerspective>()->setGlMainViewPropertiesForGraph(
               graph, outPropsMap) == false))
        // clear map if there is nothing to do
        outPropsMap.clear();
    }

    // draw with temporary computed properties
    if (!outPropsMap.empty()) {
      Perspective::typedInstance<GraphPerspective>()->centerPanelsForGraph(graph, true, true);
    }
  }
};

#define TN(T) typeid(T).name()

void AlgorithmRunnerItem::run(Graph *g) {
  initModel();

  if (g == nullptr)
    g = _graph;

  if (g == nullptr) {
    qCritical() << QStringToTlpString(name()) << ": No graph selected";
    return;
  }

  Observable::holdObservers();
  DataSet originalDataSet =
      static_cast<ParameterListModel *>(_ui->parameters->model())->parametersValues();

  DataSet dataSet(originalDataSet);

  // ensure each input property
  // is a local one when it exits
  std::string algorithm = QStringToTlpString(_pluginName);
  ParameterDescriptionList paramList = PluginLister::getPluginParameters(algorithm);
  for (const ParameterDescription &desc : paramList.getParameters()) {
    if (desc.getDirection() == IN_PARAM) {

      std::string typeName(desc.getTypeName());

      if (DataType::isTulipProperty(typeName)) {
        PropertyInterface *prop = nullptr;
        dataSet.get(desc.getName(), prop);

        if (prop != nullptr) {
          PropertyInterface *localProp = g->getProperty(prop->getName());

          if (prop != localProp)
            dataSet.set(desc.getName(), localProp);
        }
      }
    }
  }

  g->push();

  if (_storeResultAsLocal)
    copyToLocal(dataSet, g);

  std::vector<std::string> outNonPropertyParams;
  // use temporary output properties
  // to ease the undo in case of failure
  std::vector<OutPropertyParam> outPropertyParams;
  // ensure OutPropertyParam.tmp is no deleted
  // during outPropertyParams resizing
  OutPropertyParam::inUse = true;
  for (const ParameterDescription &desc : paramList.getParameters()) {
    std::string typeName(desc.getTypeName());

    // forget non property out param
    if (!DataType::isTulipProperty(typeName)) {
      if (desc.getDirection() != IN_PARAM)
        outNonPropertyParams.push_back(desc.getName());

      continue;
    }

    auto displayMandatoryMessage = [&]() {
      QString message("The mandatory property parameter\n'");
      message += tlp::tlpStringToQString(desc.getName());
      message += "' cannot be empty.";
      qCritical() << message;
      QMessageBox::critical(parentWidget(), name(), message);
    };

    if (desc.getDirection() == IN_PARAM) {
      if (desc.isMandatory()) {
        // if it is a mandatory input property
        // check it is not null
        PropertyInterface *prop = nullptr;
        dataSet.get(desc.getName(), prop);

        if (prop == nullptr) {
          g->pop();
          Observable::unholdObservers();
          displayMandatoryMessage();
          return;
        }
      }

      continue;
    }

    OutPropertyParam outPropParam(desc.getName());
    // get destination property
    dataSet.get(desc.getName(), outPropParam.dest);
    // if it is a mandatory property
    // it cannot be null
    if (desc.isMandatory() && !outPropParam.dest) {
      g->pop();
      Observable::unholdObservers();
      displayMandatoryMessage();
      return;
    }

    // clone it in a not registered (because unnamed)
    // temporary property
    outPropParam.tmp = outPropParam.dest
                           ? outPropParam.dest->clonePrototype(outPropParam.dest->getGraph(), "")
                           : nullptr;
    // set the temporary as the destination property
    dataSet.set(desc.getName(), outPropParam.tmp);

    if (outPropParam.tmp) {
      outPropertyParams.push_back(outPropParam);

      if (desc.getDirection() == OUT_PARAM) {
        DataMem *defaultData = outPropParam.dest->getNodeDefaultDataMemValue();
        outPropParam.tmp->setAllNodeDataMemValue(defaultData);
        delete defaultData;
        defaultData = outPropParam.dest->getEdgeDefaultDataMemValue();
        outPropParam.tmp->setAllEdgeDataMemValue(defaultData);
        delete defaultData;
      } else
        // inout property
        outPropParam.tmp->copy(outPropParam.dest);
    }
  }

  // Perspective::typedInstance<GraphPerspective>()->setAutoCenterPanelsOnDraw(true);
  std::string errorMessage;
  PluginProgress *progress = Perspective::instance()->progress();
  progress->setTitle(algorithm);

  // set preview handler if needed
  if (!outPropertyParams.empty())
    progress->setPreviewHandler(new AlgorithmPreviewHandler(g, outPropertyParams));
  else
    progress->showPreview(false);

  // take time before run
  QTime start = QTime::currentTime();
  bool result = g->applyAlgorithm(algorithm, errorMessage, &dataSet, progress);

  // get spent time
  int spentTime = start.msecsTo(QTime::currentTime());

  if (!outPropertyParams.empty())
    progress->setPreviewHandler(nullptr);

  if (!result) {
    if (progress->state() == TLP_CANCEL && errorMessage.empty()) {
      errorMessage = "Cancelled by user";
      tlp::warning() << QStringToTlpString(name()) << ": " << errorMessage;
      QMessageBox::warning(parentWidget(), name(), errorMessage.c_str());
    } else {
      tlp::error() << QStringToTlpString(name()) << ": " << errorMessage;
      QMessageBox::critical(parentWidget(), name(), errorMessage.c_str());
    }
    progress->setComment("Cancelling graph changes...");
    g->pop();
  } else {
    if (progress->state() == TLP_STOP) {
      errorMessage = "Stopped by user";
      tlp::warning() << QStringToTlpString(name()) << ": " << errorMessage;
      QMessageBox::warning(parentWidget(), name(), errorMessage.c_str());
    }
  }

  if (result) {
    progress->setComment("Applying graph changes...");
    // copy or cleanup out properties
    for (auto &opp : outPropertyParams) {
      // copy computed property in the original output property
      opp.dest->copy(opp.tmp);
      // restore it in the dataset
      dataSet.set(opp.name, opp.dest);

      if (opp.name == "result" && TulipSettings::isResultPropertyStored()) {
        // store the result property values in an automatically named property
        std::string storedResultName =
            algorithm + " - " + originalDataSet.toString() + "(" + opp.dest->getName() + ")";
        PropertyInterface *storedResultProp =
            opp.dest->clonePrototype(opp.dest->getGraph(), storedResultName);
        storedResultProp->copy(opp.tmp);
      }
    }

    // display spentTime if needed
    if (TulipSettings::logPluginCall() != TulipSettings::NoLog) {
      std::stringstream log;
      log << algorithm.c_str() << " - " << dataSet.toString().c_str();

      if (TulipSettings::logPluginCall() == TulipSettings::LogCallWithExecutionTime)
        log << ": " << spentTime << "ms";

      qDebug() << log.str().c_str();
    }
  }

  afterRun(g, dataSet);

  if (result && !outNonPropertyParams.empty()) {
    // only show computed value of non property output parameters.
    // output property params are not taken into account
    // because they may have been created on the fly
    // (local properties see copyToLocal function above)
    // and thus they may be deleted further in case of undo
    for (auto &paramName : outNonPropertyParams) {
      tlp::DataType *dataType = dataSet.getData(paramName);
      originalDataSet.setData(paramName, dataType);
      delete dataType;
    }

    ParameterListModel *model = static_cast<ParameterListModel *>(_ui->parameters->model());
    model->setParametersValues(originalDataSet);
  }

  while (Observable::observersHoldCounter() > 0)
    Observable::unholdObservers();

  g->popIfNoUpdates();

  // OutputPropParam.tmp can now be deleted
  OutPropertyParam::inUse = false;

  // keep progress alive until the end
  // to give feedback to user
  delete progress;
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
  const Plugin &p = PluginLister::pluginInformation(QStringToTlpString(_pluginName).c_str());
  QPixmap icon(QPixmap(p.icon().c_str()).scaled(64, 64));
  QFont f;
  f.setBold(true);
  QFontMetrics metrics(f);
  int textHeight =
      metrics
          .boundingRect(0, 0, icon.width(), INT_MAX,
                        Qt::AlignTop | Qt::AlignHCenter | Qt::TextWordWrap, _pluginName)
          .height();
  QPixmap pix(icon.width() + textHeight, icon.height() + textHeight);
  pix.fill(Qt::white);
  QPainter painter(&pix);
  painter.drawPixmap(pix.width() / 2 - icon.width() / 2, 0, icon.width(), icon.height(), icon);
  painter.setFont(f);
  painter.drawText(0, icon.height(), pix.width(), pix.height() - icon.height(),
                   Qt::AlignCenter | Qt::AlignHCenter | Qt::TextWordWrap, _pluginName);
  painter.setBrush(Qt::transparent);
  painter.setPen(QColor(169, 169, 169));
  painter.drawRect(0, 0, pix.width() - 1, pix.height() - 1);
  drag->setPixmap(pix);

  initModel();
  AlgorithmMimeType *mimeData = new AlgorithmMimeType(
      name(), static_cast<ParameterListModel *>(_ui->parameters->model())->parametersValues());
  connect(mimeData, SIGNAL(mimeRun(tlp::Graph *)), this, SLOT(run(tlp::Graph *)));
  drag->setMimeData(mimeData);
  drag->exec(Qt::CopyAction | Qt::MoveAction);
}

bool AlgorithmRunnerItem::eventFilter(QObject *, QEvent *ev) {
  if (ev->type() == QEvent::MouseButtonRelease) {
    QMouseEvent *qMouseEv = static_cast<QMouseEvent *>(ev);
    if (qMouseEv->button() == Qt::RightButton) {
      QMessageBox::information(parentWidget(), this->name().append(" documentation"),
                               _ui->playButton->toolTip());
      return true;
    }
  }
  return false;
}

void AlgorithmRunnerItem::afterRun(Graph *g, const tlp::DataSet &dataSet) {
  std::string stdName = QStringToTlpString(name());

  if (PluginLister::pluginExists<LayoutAlgorithm>(stdName)) {
    if (TulipSettings::isAutomaticRatio()) {
      LayoutProperty *prop = nullptr;
      dataSet.get<LayoutProperty *>("result", prop);

      if (prop)
        prop->perfectAspectRatio(g);
    }

    if (TulipSettings::isAutomaticCentering())
      Perspective::typedInstance<GraphPerspective>()->centerPanelsForGraph(g);
  } else if (TulipSettings::isAutomaticCentering() &&
             PluginLister::pluginExists<Algorithm>(stdName) &&
             !PluginLister::pluginExists<PropertyAlgorithm>(stdName) &&
             !PluginLister::pluginExists<GraphTest>(stdName)) {
    Perspective::typedInstance<GraphPerspective>()->centerPanelsForGraph(g);
  } else if (PluginLister::pluginExists<DoubleAlgorithm>(stdName) &&
             TulipSettings::isAutomaticMapMetric()) {
    DoubleProperty *prop = nullptr;
    dataSet.get<DoubleProperty *>("result", prop);

    if ((prop != nullptr) && (prop->getName().compare("viewMetric") == 0)) {
      bool applyMapping = prop->numberOfNonDefaultValuatedNodes() != 0;

      std::string errMsg;
      ColorProperty *color;

      if (g->existLocalProperty("viewColor")) {
        color = g->getLocalProperty<ColorProperty>("viewColor");
        if (!applyMapping && (color->numberOfNonDefaultValuatedNodes() != 0)) {
          color->setAllNodeDataMemValue(color->getNodeDefaultDataMemValue());
          color->setAllEdgeDataMemValue(color->getEdgeDefaultDataMemValue());
        }
      } else {
        color = g->getLocalProperty<ColorProperty>("viewColor");
        ColorProperty *ancestorColor = g->getSuperGraph()->getProperty<ColorProperty>("viewColor");
        if (!applyMapping && (ancestorColor->numberOfNonDefaultValuatedNodes(g) != 0)) {
          // same default values as ancestor property default values
          color->setAllNodeDataMemValue(ancestorColor->getNodeDefaultDataMemValue());
          color->setAllEdgeDataMemValue(ancestorColor->getEdgeDefaultDataMemValue());
        }
      }

      if (applyMapping) {
        // set value of "color scale" parameter of "Color Mapping" plugin
        // to the user defined value
        tlp::DataSet data;
        ColorScale cs;

        if (colorMappingModel)
          colorMappingModel->parametersValues().get<ColorScale>("color scale", cs);
        else
          cs = ColorScalesManager::getLatestColorScale();

        data.set<ColorScale>("color scale", cs);
        g->applyPropertyAlgorithm("Color Mapping", color, errMsg, &data);
      }
    }
  } else if (PluginLister::pluginExists<GraphTest>(stdName)) {
    bool result = true;
    dataSet.get<bool>("result", result);
    std::string str = "\"" + stdName + "\" test " + (result ? "succeeded" : "failed") + " on:\n" +
                      g->getName() + ".";

    if (result) {
      tlp::debug() << str << std::endl;
      QMessageBox::information(parentWidget(), "Tulip test result", tlp::tlpStringToQString(str));
    } else {
      tlp::warning() << str << std::endl;
      QMessageBox::warning(parentWidget(), "Tulip test result", tlp::tlpStringToQString(str));
    }
  }
}

void AlgorithmRunnerItem::setFavorite(bool f) {
  _ui->favoriteCheck->setChecked(f);
}

void AlgorithmRunnerItem::favoriteChanged(int state) {
  _ui->favoriteCheck->setToolTip((state == Qt::Unchecked) ? QString("Add to Favorites")
                                                          : QString("Remove from Favorites"));
}

tlp::DataSet AlgorithmRunnerItem::data() const {
  if (_ui->parameters->model() == nullptr)
    return DataSet();

  return static_cast<ParameterListModel *>(_ui->parameters->model())->parametersValues();
}

ParameterListModel *AlgorithmRunnerItem::colorMappingModel = nullptr;

void AlgorithmRunnerItem::initModel() {
  if (_ui->parameters->model() != nullptr)
    return;

  ParameterListModel *model =
      new ParameterListModel(PluginLister::getPluginParameters(QStringToTlpString(_pluginName)),
                             _graph, _ui->parameters, true);

  if (_pluginName == "Color Mapping") {
    colorMappingModel = model;
    tlp::DataSet data = model->parametersValues();
    data.set<ColorScale>("color scale", ColorScalesManager::getLatestColorScale());
    model->setParametersValues(data);
  }

  _ui->parameters->setModel(model);
  int h = 10;

  for (int i = 0; i < model->rowCount(); ++i)
    h += _ui->parameters->rowHeight(i);

  _ui->parameters->setMinimumSize(_ui->parameters->minimumSize().width(), h);
  _ui->parameters->setMaximumSize(_ui->parameters->maximumSize().width(), h);

  if (!_initData.empty()) {
    DataSet dataSet = model->parametersValues();
    for (const std::pair<std::string, tlp::DataType *> &it : _initData.getValues()) {
      dataSet.setData(it.first, it.second);
    }
    model->setParametersValues(dataSet);
  }
}
