#ifndef TABLEVIEWCONFIGURATION_H
#define TABLEVIEWCONFIGURATION_H

#include <QtGui/QWidget>

namespace Ui {
class TableViewConfiguration;
}

namespace tlp {
class Graph;
class BooleanProperty;
}

class TableViewConfiguration : public QWidget {
  Q_OBJECT

  Ui::TableViewConfiguration* _ui;
public:
  explicit TableViewConfiguration(QWidget *parent = NULL);
  virtual ~TableViewConfiguration();

  void setGraph(tlp::Graph* g);

  bool isShowNodes() const;
  bool isShowEdges() const;
  tlp::BooleanProperty* filteringProperty() const;

public slots:
  void setShowNodes(bool);
  void setShowEdges(bool);
  void setFilteringProperty(QString);

signals:
  void settingsChanged();
};

#endif // TABLEVIEWCONFIGURATION_H
