#ifndef ALGORITHMRUNNER_H
#define ALGORITHMRUNNER_H

#include <QtGui/QWidget>
#include <QtCore/QMap>

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
};

class AlgorithmRunner : public QWidget {
  Q_OBJECT

  static QMap<QString, PluginListWidgetManagerInterface *> PLUGIN_LIST_MANAGERS_DISPLAY_NAMES;
  static void staticInit();

  Ui::AlgorithmRunnerData *_ui;
  PluginListWidgetManagerInterface *_pluginsListMgr;
  QMap<QString,QStringList> _currentAlgorithmsList;
public:
  explicit AlgorithmRunner(QWidget *parent = 0);

signals:

public slots:
  void buildListWidget();

protected slots:
  void algorithmTypeChanged(const QString &);
  void setFilter(const QString &);
};

class AlgorithmRunnerItem: public QWidget {
  Q_OBJECT

  Ui::AlgorithmRunnerItemData *_ui;

  Q_PROPERTY(QString group READ group)
  QString _group;

  Q_PROPERTY(QString name READ name)
public:
  explicit AlgorithmRunnerItem(const QString &group,const QString &name, QWidget *parent=0);
  virtual ~AlgorithmRunnerItem();

  QString group() const {
    return _group;
  }
  QString name() const {
    return "";
  }
};

#endif // ALGORITHMRUNNER_H
