#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QtGui/QWidget>

namespace Ui {
class SearchWidget;
}

class QComboBox;
class SearchOperator;
namespace tlp {
class GraphHierarchiesModel;
class Graph;
class PropertyInterface;
}

class SearchWidget : public QWidget {
  Q_OBJECT

  static QVector<SearchOperator*> NUMERIC_OPERATORS;
  static QVector<SearchOperator*> STRING_OPERATORS;

  Ui::SearchWidget* _ui;
  tlp::PropertyInterface* term(QComboBox* combo);

  SearchOperator* searchOperator();
  bool isNumericComparison();


public:
  explicit SearchWidget(QWidget *parent = 0);
  virtual ~SearchWidget();

signals:
  
public slots:
  void setModel(tlp::GraphHierarchiesModel* model);
  void currentGraphChanged(tlp::Graph* g);
  void setGraph(tlp::Graph* g);
  void search();

protected slots:
  void graphIndexChanged(int);
  void termAChanged();
  void termBChanged();
  void updateOperators(tlp::PropertyInterface* a, tlp::PropertyInterface* b);
  void updateOperators(tlp::PropertyInterface* a, const QString& b);
  void setNumericOperatorsEnabled(bool e);
};

#endif // SEARCHWIDGET_H
