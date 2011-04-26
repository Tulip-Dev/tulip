#ifndef ABSTRACTGRAPHICSVIEW_H
#define ABSTRACTGRAPHICSVIEW_H

#include <tulip/View.h>

#include <QtCore/QSet>
#include <QtCore/QPointF>

class QGraphicsProxyWidget;
class QGraphicsView;
class QGraphicsItem;
class QGLWidget;
class ToolbarItem;

namespace tlp {
class AbstractGraphicsView: public tlp::View {
  Q_OBJECT
public:
  AbstractGraphicsView();
  virtual ~AbstractGraphicsView();

  virtual QWidget *construct(QWidget *parent);

  virtual QWidget *getWidget();

  virtual bool createPicture(const std::string &pictureName, int width, int height, bool center, int zoom, int xOffset, int yOffset);

  virtual void setInteractors(const std::list<tlp::Interactor *> &interactors);

  virtual std::list<tlp::Interactor *> getInteractors();

  virtual void setActiveInteractor(tlp::Interactor *interactor);

  virtual tlp::Interactor *getActiveInteractor();

  QWidget *getCentralWidget();

  void setCentralWidget(QWidget *);

protected:
  std::list<tlp::Interactor *> _interactors;

  tlp::Interactor *_activeInteractor;

  QGraphicsView *_centralView;

  QVBoxLayout *_mainLayout;


  virtual void buildGraphicsUi();

  void addToScene(QGraphicsItem *item);

  void removeFromScene(QGraphicsItem *item);

  bool eventFilter(QObject *obj, QEvent *e);

protected slots:
  void toggleInteractorConfigurationWidget();
  void activeInteractorChanged();

private:
  QWidget *_centralWidget;
  QGraphicsItem *_centralWidgetItem;
  QWidget *_mainWidget;

  QSet<QGraphicsItem *> _items;

  ToolbarItem *_toolbarItem;
  QGraphicsProxyWidget *_activeConfigurationWidget;
  void refreshItemsParenthood();
};
}

#endif // ABSTRACTGRAPHICSVIEW_H
