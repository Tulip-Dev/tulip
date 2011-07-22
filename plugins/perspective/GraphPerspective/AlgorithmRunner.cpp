#include "AlgorithmRunner.h"

#include <tulip/ColorProperty.h>
#include <tulip/PropertyAlgorithm.h>
#include <tulip/Algorithm.h>
#include <tulip/PluginLister.h>

#include "ui_AlgorithmRunner.h"
#include "ui_AlgorithmRunnerItem.h"
#include "ExpandableGroupBox.h"

#include <QtCore/QDebug>

#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>

// **********************************************
// Helper classes
// **********************************************

AlgorithmRunnerItem::AlgorithmRunnerItem(const QString &group, const QString &name, QWidget *parent): QWidget(parent), _ui(new Ui::AlgorithmRunnerItemData), _group(group) {
  _ui->setupUi(this);
  _ui->algNameLabel->setText(name);
  _ui->algNameLabel->setToolTip(_ui->algNameLabel->text());
  _ui->settingsButton->setToolTip(trUtf8("Set up ") + name);
  _ui->playButton->setToolTip(trUtf8("Run ") + name);
  setObjectName(group + "_" + name + "_runnerItem");
}

AlgorithmRunnerItem::~AlgorithmRunnerItem() {
}

template<typename ALG,typename PROPTYPE>
class TemplatePluginListWidgetManager: public PluginListWidgetManagerInterface {
public:
  QMap<QString,QStringList> algorithms() {
    QMap<QString,QStringList> result;
    typedef tlp::StaticPluginLister<ALG,tlp::PropertyContext> Lister;
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

  bool computeProperty(tlp::Graph *, const QString &alg, const QString &outPropertyName, QString &msg, tlp::PluginProgress *progress, tlp::DataSet *data) {
    return false;
  }
};
// **********************************************

QMap<QString,PluginListWidgetManagerInterface *> AlgorithmRunner::PLUGIN_LIST_MANAGERS_DISPLAY_NAMES;

void AlgorithmRunner::staticInit() {
  if (PLUGIN_LIST_MANAGERS_DISPLAY_NAMES.empty()) {
    PLUGIN_LIST_MANAGERS_DISPLAY_NAMES[trUtf8("Coloring algorithms")] = new TemplatePluginListWidgetManager<tlp::ColorAlgorithm,tlp::ColorProperty>();
    PLUGIN_LIST_MANAGERS_DISPLAY_NAMES[trUtf8("Filtering algorithms")] = new TemplatePluginListWidgetManager<tlp::BooleanAlgorithm,tlp::BooleanProperty>();
    PLUGIN_LIST_MANAGERS_DISPLAY_NAMES[trUtf8("Metric algorithms (double)")] = new TemplatePluginListWidgetManager<tlp::DoubleAlgorithm,tlp::DoubleProperty>();
    PLUGIN_LIST_MANAGERS_DISPLAY_NAMES[trUtf8("Metric algorithms (integer)")] = new TemplatePluginListWidgetManager<tlp::IntegerAlgorithm,tlp::IntegerProperty>();
    PLUGIN_LIST_MANAGERS_DISPLAY_NAMES[trUtf8("Labeling algorithms")] = new TemplatePluginListWidgetManager<tlp::StringAlgorithm,tlp::StringProperty>();
    PLUGIN_LIST_MANAGERS_DISPLAY_NAMES[trUtf8("Layout algorithms")] = new TemplatePluginListWidgetManager<tlp::LayoutAlgorithm,tlp::LayoutProperty>();
    PLUGIN_LIST_MANAGERS_DISPLAY_NAMES[trUtf8("Resizing algorithms")] = new TemplatePluginListWidgetManager<tlp::SizeAlgorithm,tlp::SizeProperty>();
    PLUGIN_LIST_MANAGERS_DISPLAY_NAMES[trUtf8("General purpose algorithms")] = new TemplatePluginListWidgetManager<tlp::Algorithm,tlp::DoubleProperty>();
  }
}

AlgorithmRunner::AlgorithmRunner(QWidget *parent): QWidget(parent), _ui(new Ui::AlgorithmRunnerData), _pluginsListMgr(0) {
  staticInit();
  _ui->setupUi(this);
  connect(_ui->header,SIGNAL(menuChanged(QString)),this,SLOT(algorithmTypeChanged(QString)));
  _ui->header->setMenus(PLUGIN_LIST_MANAGERS_DISPLAY_NAMES.keys());
}

void AlgorithmRunner::algorithmTypeChanged(const QString &type) {
  _pluginsListMgr = PLUGIN_LIST_MANAGERS_DISPLAY_NAMES[type];
  assert(_pluginsListMgr);
  _ui->algorithmList->setWidget(buildListWidget());
}

QWidget *AlgorithmRunner::buildListWidget() {
  _currentAlgorithmsList = _pluginsListMgr->algorithms();

  QWidget *result = new QWidget;
  result->setObjectName("algorithmListContents");
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
      QWidget *groupWidget;

      QVBoxLayout *groupLayout = new QVBoxLayout;
      groupLayout->setObjectName(group + "_boxLayout");
      groupLayout->setSpacing(0);
      if (group == "") {
        groupWidget = new QWidget;
        groupLayout->setContentsMargins(0,0,0,0);
      }
      else {
        groupWidget = new ExpandableGroupBox(group);
        groupLayout->setContentsMargins(0,15,0,15);
      }

      groupWidget->setObjectName(group + "_groupBox");
      int i=0;
      QString algName;
      foreach(algName,_currentAlgorithmsList[group]) {
        i=(i+1)%2;
        AlgorithmRunnerItem *item = new AlgorithmRunnerItem(group,algName);
        if (i == 0)
          item->setStyleSheet("#algFrame { background-color: #EFEFEF }");
        groupLayout->addWidget(item);
      }
      groupWidget->setLayout(groupLayout);
      layout->addWidget(groupWidget);
    }
    layout->addItem(new QSpacerItem(2,2,QSizePolicy::Maximum,QSizePolicy::Expanding));
  }

  result->setLayout(layout);
  return result;
}
