#ifndef FILTERSMANAGERITEM_H
#define FILTERSMANAGERITEM_H


#include <QtGui/QFrame>
#include <tulip/WithParameter.h>

namespace Ui {
class FiltersManagerItem;
class FiltersManagerAlgorithmItem;
class FiltersManagerInvertItem;
class FiltersManagerCompareItem;
}
namespace tlp {
class Graph;
}
class QTableView;

/*
  Helper classes
  */
class AbstractFiltersManagerItem: public QWidget {
  Q_OBJECT
protected:
  tlp::Graph* _graph;
public:
  explicit AbstractFiltersManagerItem(QWidget* parent = NULL): QWidget(parent), _graph(NULL) {}
  virtual void applyFilter()=0;
  virtual QString title() const=0;
  void setGraph(tlp::Graph*);
protected:
  virtual void graphChanged();

signals:
  void titleChanged();
};

class FiltersManagerAlgorithmItem: public AbstractFiltersManagerItem {
  Q_OBJECT
  Ui::FiltersManagerAlgorithmItem* _ui;
public:
  explicit FiltersManagerAlgorithmItem(QWidget* parent = NULL);
  void applyFilter();
  QString title() const;
protected slots:
  void algorithmSelected(int);
protected:
  void graphChanged();
};

class FiltersManagerCompareItem: public AbstractFiltersManagerItem {
  Q_OBJECT
  Ui::FiltersManagerCompareItem* _ui;
public:
  explicit FiltersManagerCompareItem(QWidget* parent = NULL);
  void applyFilter();
  QString title() const;
};

class FiltersManagerInvertItem: public AbstractFiltersManagerItem {
  Q_OBJECT
  Ui::FiltersManagerInvertItem* _ui;
public:
  explicit FiltersManagerInvertItem(QWidget* parent = NULL);
  void applyFilter();
  QString title() const;
};
// ==========================

class FiltersManagerItem: public QFrame {
  Q_OBJECT
  Ui::FiltersManagerItem *_ui;

  Q_ENUMS(Mode)
public:
  enum Mode {
    Invert,
    Compare,
    Algorithm,
    Invalid
  };

  explicit FiltersManagerItem(QWidget *parent=0);
  virtual ~FiltersManagerItem();

signals:
  void removed();
  void modeChanged(FiltersManagerItem::Mode);

protected slots:
  void setCompareMode();
  void setAlgorithmMode();
  void setInvertMode();
  void setMode(Mode);

  void addButtonClicked();
  void dataBoxTitleChanged();

  void graphChanged(tlp::Graph*);
};

#endif // FILTERSMANAGERITEM_H
