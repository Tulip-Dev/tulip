#ifndef ABSTRACTFILTERSMANAGERITEM_H
#define ABSTRACTFILTERSMANAGERITEM_H

#include <QtGui/QWidget>
#include <QtGui/QTableView>
#include <tulip/Graph.h>
#include <tulip/WithParameter.h>
#include <tulip/ParameterListModel.h>

namespace tlp {
class BooleanProperty;
}

class QStandardItem;
class QComboBox;

// Combo box helper functions
QStandardItem* itemAt(QComboBox* combo, int i);
QStandardItem* lastItem(QComboBox* combo);
void fillTitle(QComboBox* combo, const QString& s);

class AbstractFiltersManagerItem: public QWidget {
  Q_OBJECT
protected:
  tlp::Graph* _graph;
public:
  explicit AbstractFiltersManagerItem(QWidget* parent = NULL);
  virtual void applyFilter(tlp::BooleanProperty*)=0;
  virtual QString title() const=0;
  void setGraph(tlp::Graph*);
protected:
  virtual void graphChanged();

  template<typename LISTER>
  void updateGraphModel(QTableView* table,const QString& algName, tlp::Graph* g) {
    int h=0;

    if (!algName.isNull()) {
      tlp::ParameterList params = LISTER::getPluginParameters(algName.toStdString());
      table->setModel(new tlp::ParameterListModel(params,g));

      for (int i=0; i<table->model()->rowCount(); ++i)
        h += table->rowHeight(i);
    }
    else {
      table->setModel(NULL);
    }

    table->setMaximumHeight(h);
    table->setMinimumHeight(h);
  }

signals:
  void titleChanged();
};

#endif // ABSTRACTFILTERSMANAGERITEM_H
