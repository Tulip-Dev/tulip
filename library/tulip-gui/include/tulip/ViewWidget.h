#ifndef VIEWWIDGET_H
#define VIEWWIDGET_H

#include <tulip/tulipconf.h>
#include <tulip/View.h>

class QGraphicsItem;

namespace tlp {

class ViewWidget : public tlp::View {
  Q_OBJECT

  QSet<QGraphicsItem *> _items;
  QGraphicsView* _graphicsView;
  QWidget* _centralWidget;
  QGraphicsItem* _centralWidgetItem;

public:
  ViewWidget();
  virtual ~ViewWidget();
  virtual QGraphicsView* graphicsView() const;

public slots:
  virtual void setupUi();

protected slots:
  virtual void currentInteractorChanged(tlp::Interactor*);
  virtual void graphChanged(tlp::Graph*)=0;

protected:
  virtual void setupWidget()=0;

  void addToScene(QGraphicsItem*);
  void removeFromScene(QGraphicsItem*);
  void refreshItemsParenthood();
  void setCentralWidget(QWidget*);

  virtual bool eventFilter(QObject *, QEvent *);
};

}

#endif // VIEWWIDGET_H
