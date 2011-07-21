#include "AlgorithmRunner.h"

#include <tulip/ColorProperty.h>
#include <tulip/PropertyAlgorithm.h>
#include <tulip/Algorithm.h>
#include <tulip/PluginLister.h>

#include "ui_AlgorithmRunner.h"
#include "ExpandableGroupBox.h"

#include <QtCore/QDebug>

#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>

// **********************************************
// Helper classes
// **********************************************
QWidget *PluginListWidgetManagerInterface::buildListWidget() {
  QMap<QString,QStringList> algorithmInfos = algorithms();
  QWidget *result = new QWidget;
  result->setObjectName("algorithmListContents");
  QVBoxLayout *layout = new QVBoxLayout;
  layout->setObjectName("algorithmListContentsLayout");

  if (algorithmInfos.empty()) {
    QLabel *errorLabel = new QLabel("<span style=\"color:#626262;\">" + AlgorithmRunner::trUtf8("There is no plugin to display.") + "</span>");
    errorLabel->setWordWrap(true);
    layout->addWidget(errorLabel);
  }
  else {
    QString group;
    foreach(group,algorithmInfos.keys()) {
      ExpandableGroupBox *groupBox = new ExpandableGroupBox(group);
      layout->addWidget(groupBox);
      QVBoxLayout *testLayout = new QVBoxLayout;
      for (int i=0;i<10;++i)
        testLayout->addWidget(new QLabel("prouuuuuuuuuuuuuuut"));
      groupBox->setLayout(testLayout);
    }
    layout->addItem(new QSpacerItem(2,2,QSizePolicy::Maximum,QSizePolicy::Expanding));
  }

  result->setLayout(layout);
  return result;
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
  _ui->algorithmList->setWidget(_pluginsListMgr->buildListWidget());
}
