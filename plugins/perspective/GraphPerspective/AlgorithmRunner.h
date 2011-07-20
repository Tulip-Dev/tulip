#ifndef ALGORITHMRUNNER_H
#define ALGORITHMRUNNER_H

#include <QtGui/QWidget>

namespace tlp {
class Graph;
class PluginProgress;
class DataSet;
}
namespace Ui {
class AlgorithmRunnerData;
}

class PluginListWidgetManagerInterface {
public:
  virtual QWidget *buildListWidget();
  virtual QMap<QString,QStringList> algorithms()=0;
  virtual bool computeProperty(tlp::Graph *,const QString &alg, const QString &outPropertyName, QString &msg, tlp::PluginProgress *progress=0, tlp::DataSet *data=0)=0;
};

class AlgorithmRunner : public QWidget {
  Q_OBJECT

  static QMap<QString, PluginListWidgetManagerInterface *> PLUGIN_LIST_MANAGERS_DISPLAY_NAMES;
  static void staticInit();

  Ui::AlgorithmRunnerData *_ui;
  PluginListWidgetManagerInterface *_pluginsListMgr;
public:
  explicit AlgorithmRunner(QWidget *parent = 0);

signals:

public slots:

protected slots:
  void algorithmTypeChanged(const QString &);

};

#endif // ALGORITHMRUNNER_H
