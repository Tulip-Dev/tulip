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
class BooleanProperty;
class PropertyInterface;
}
class QTableView;
class QComboBox;

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

  explicit FiltersManagerItem(QWidget *parent=NULL);
  virtual ~FiltersManagerItem();

signals:
  void removed();
  void modeChanged(FiltersManagerItem::Mode);

public slots:
  void applyFilter(tlp::BooleanProperty*);

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
