#ifndef ALGORITHMRUNNERITEM_H
#define ALGORITHMRUNNERITEM_H

#include <QtGui/QWidget>

#include <tulip/DataSet.h>

namespace Ui {
class AlgorithmRunnerItem;
}
namespace tlp {
class Graph;
}

class AlgorithmRunnerItem: public QWidget {
  Q_OBJECT
  Ui::AlgorithmRunnerItem* _ui;
  QString _pluginName;
  tlp::Graph* _graph;
  bool _localMode;
  QPointF _dragStartPosition;


public:
  explicit AlgorithmRunnerItem(QString pluginName, QWidget* parent = NULL);
  virtual ~AlgorithmRunnerItem();

  QString name() const;
  QString python() const;
  tlp::DataSet data() const;

  void setFavorite(bool);

protected:
  virtual void mousePressEvent(QMouseEvent* ev);
  virtual void mouseMoveEvent(QMouseEvent* ev);

public slots:
  void setGraph(tlp::Graph*);
  void setData(const tlp::DataSet& data);
  void setLocalMode(bool);

  void run(tlp::Graph* g = NULL);

signals:
  void favorized(bool);

protected slots:
  void afterRun(tlp::Graph*,tlp::DataSet);
  void initModel();
};
#endif // ALGORITHMRUNNERITEM_H
