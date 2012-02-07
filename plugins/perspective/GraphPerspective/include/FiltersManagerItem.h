#ifndef FILTERSMANAGERITEM_H
#define FILTERSMANAGERITEM_H


#include <QtGui/QFrame>

namespace Ui {
class FiltersManagerItem;
class FiltersManagerAlgorithmItem;
class FiltersManagerInvertItem;
class FiltersManagerCompareItem;
}


/*
  Helper classes
  */
class AbstractFiltersManagerItem: public QWidget {
  Q_OBJECT
public:
  explicit AbstractFiltersManagerItem(QWidget* parent = NULL): QWidget(parent) {}
  virtual void applyFilter()=0;
  virtual QString title() const=0;
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
  Q_PROPERTY(Mode mode READ mode)

public:
  enum Mode {
    Invert,
    Compare,
    Algorithm,
    Invalid
  };

  explicit FiltersManagerItem(QWidget *parent=0);
  virtual ~FiltersManagerItem();

  Mode mode() const;

public slots:

protected slots:
  void setCompareMode();
  void setAlgorithmMode();
  void setInvertMode();
  void setMode(Mode);

  void addButtonClicked();
};

#endif // FILTERSMANAGERITEM_H
