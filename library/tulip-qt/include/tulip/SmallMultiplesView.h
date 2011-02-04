#ifndef SMALLMULTIPLESVIEW_H
#define SMALLMULTIPLESVIEW_H

#include "tulip/AbstractView.h"
#include <QtCore/QVector>
class QGraphicsView;
class QGraphicsItem;

namespace tlp {

class GlMainWidgetGraphicsView;
class GlMainWidget;
class GlLayer;

class TLP_QT_SCOPE SmallMultiplesView: public AbstractView {
  Q_OBJECT
public:
  enum SmallMultiplesDataRole {
    Label     = 1, // 0b001 QString
    Texture   = 2, // 0b010 QImage
    Position  = 4, // 0b100 tlp::Coord
    AllRoles  = 7
  };
  Q_DECLARE_FLAGS(Roles, SmallMultiplesDataRole)

  SmallMultiplesView();
  virtual ~SmallMultiplesView();
  virtual QWidget *construct(QWidget *parent);

  virtual QVariant data(int id, SmallMultiplesDataRole role);
  virtual int countItems() = 0;

  GlMainWidget *overview() const;
  void centerOverview();

  int nodeItemId(node);

  bool isZoomAnimationActivated() const { return _zoomAnimationActivated; }
  bool isAutoDisableInteractors() const { return _autoDisableInteractors; }
  int maxLabelSize() const { return _maxLabelSize; }
  double spacing() const { return _spacing; }

  virtual void setActiveInteractor(Interactor *interactor);

  void setOverviewVisible(bool);
  GlLayer *overviewLayer() const;
  virtual bool isOverviewVisible() const;

  virtual void itemSelected(int) {}
  virtual void overviewSelected() {}

signals:
  void changeData(int fromId, int toId, SmallMultiplesView::Roles dataRoles = SmallMultiplesView::AllRoles);
  void reverseItems(int a,int b);

public slots:
  void setZoomAnimationActivated(bool f) { _zoomAnimationActivated = f; }
  void setAutoDisableInteractors(bool f) { _autoDisableInteractors = f; }
  void setMaxLabelSize(int s) { _maxLabelSize = s; }
  void setSpacing(double s) { _spacing = s; }

  void toggleInteractors(bool);
  void selectItem(int);

protected slots:
  void dataChanged(int from, int to, SmallMultiplesView::Roles dataRoles = SmallMultiplesView::AllRoles);
  void dataChanged(int id, SmallMultiplesView::Roles dataRoles = SmallMultiplesView::AllRoles);
  void refreshItems();
  void addItem();
  void delItem(int);
  void itemsReversed(int,int);

private:
  GlMainWidget *_glMainWidget;
  QVector<node> _items;
  bool _zoomAnimationActivated;
  bool _autoDisableInteractors;
  int _maxLabelSize;
  double _spacing;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(SmallMultiplesView::Roles)
}

#endif // SMALLMULTIPLESVIEW_H
