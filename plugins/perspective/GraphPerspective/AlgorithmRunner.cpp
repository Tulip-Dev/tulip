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
  setObjectName(name);
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
  connect(_ui->searchBox,SIGNAL(textEdited(QString)),this,SLOT(setFilter(QString)));
  _ui->header->setMenus(PLUGIN_LIST_MANAGERS_DISPLAY_NAMES.keys());
}

void AlgorithmRunner::algorithmTypeChanged(const QString &type) {
  _pluginsListMgr = PLUGIN_LIST_MANAGERS_DISPLAY_NAMES[type];
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
        AlgorithmRunnerItem *item = new AlgorithmRunnerItem(group,algName);
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
}

void AlgorithmRunner::setFilter(const QString &filter) {
  ExpandableGroupBox *gi;
  foreach(gi, findChildren<ExpandableGroupBox *>())
  gi->setVisible(gi->objectName().contains(filter,Qt::CaseInsensitive));
  AlgorithmRunnerItem *it;
  foreach(it, findChildren<AlgorithmRunnerItem *>()) {
    if (!it->parentWidget()->isVisible())
      it->hide();
  }
  foreach(it, findChildren<AlgorithmRunnerItem *>()) {
    if (it->objectName().contains(filter,Qt::CaseInsensitive)) {
      it->show();
      it->parentWidget()->parentWidget()->show();
    }
  }
}
