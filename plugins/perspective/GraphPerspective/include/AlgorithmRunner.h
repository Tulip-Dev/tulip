#ifndef ALGORITHMRUNNER_H
#define ALGORITHMRUNNER_H

#include <QtGui/QWidget>
#include <QtCore/QMap>

#include <tulip/WithParameter.h>
#include "GraphHierarchiesModel.h"

namespace tlp {
class Graph;
class PluginProgress;
class DataSet;
}
namespace Ui {
class AlgorithmRunnerData;
class AlgorithmRunnerItemData;
}

class PluginListWidgetManagerInterface {
public:
  virtual QMap<QString,QStringList> algorithms()=0;
  virtual bool computeProperty(tlp::Graph *,const QString &alg, const QString &outPropertyName, QString &msg, tlp::PluginProgress *progress=0, tlp::DataSet *data=0)=0;
  virtual tlp::ParameterList parameters(const QString& alg)=0;
};
// **********************************************
class AlgorithmRunner : public QWidget {
  Q_OBJECT

  static QMap<QString, PluginListWidgetManagerInterface *> PLUGIN_LIST_MANAGERS_DISPLAY_NAMES;
  static void staticInit();

  Ui::AlgorithmRunnerData *_ui;
  PluginListWidgetManagerInterface *_pluginsListMgr;
  QMap<QString,QStringList> _currentAlgorithmsList;

  GraphHierarchiesModel* _model;
public:
  explicit AlgorithmRunner(QWidget *parent = 0);
  void setModel(GraphHierarchiesModel* model);

public slots:
  void buildListWidget();

protected slots:
  void algorithmTypeChanged(const QString &);
  void setFilter(const QString &);
  void currentGraphChanged(tlp::Graph* g);

  void itemSettingsToggled(bool);
};
// **********************************************
class AlgorithmRunnerItem: public QWidget {
  Q_OBJECT
  Ui::AlgorithmRunnerItemData *_ui;

  Q_PROPERTY(QString group READ group)
  QString _group;
  Q_PROPERTY(QString name READ name)

  tlp::ParameterList _params;
public:
  explicit AlgorithmRunnerItem(const QString &group,const QString &name, const tlp::ParameterList& params, QWidget *parent=0);
  virtual ~AlgorithmRunnerItem();

  QString group() const {
    return _group;
  }
  QString name() const {
    return "";
  }

  void setGraph(tlp::Graph*);

public slots:
  void toggleParameters(bool);

protected slots:
  void settingsButtonToggled(bool);

signals:
  void settingsToggled(bool);

};

#endif // ALGORITHMRUNNER_H
