#ifndef GLMAINWIDGETGRAPHICSITEM_H
#define GLMAINWIDGETGRAPHICSITEM_H

#include <QtGui/QGraphicsObject>

namespace tlp {

class GlMainWidget;

class GlMainWidgetGraphicsItem : public QGraphicsObject {
  Q_OBJECT
public:
  GlMainWidgetGraphicsItem(tlp::GlMainWidget *glMainWidget, int width, int height);
  virtual ~GlMainWidgetGraphicsItem();

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

  QRectF boundingRect() const;

  void resize(int width, int height);

  void setRedrawNeeded(bool redrawNeeded){
    this->redrawNeeded=redrawNeeded;
  }

  tlp::GlMainWidget *getGlMainWidget() { return glMainWidget;}

  bool eventFilter(QObject *, QEvent *evt);

protected :
  void wheelEvent(QGraphicsSceneWheelEvent *event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
  void hoverMoveEvent(QGraphicsSceneHoverEvent * event);
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
  void contextMenuEvent(QGraphicsSceneContextMenuEvent * event);

protected slots:
  void glMainWidgetDraw(GlMainWidget *,bool);
  void glMainWidgetRedraw(GlMainWidget *);

private :
  tlp::GlMainWidget *glMainWidget;
  bool redrawNeeded;
  int width, height;
  unsigned char *renderingStore;
};

}

#endif // GLMAINWIDGETGRAPHICSITEM_H
