#ifndef GLMAINWIDGETGRAPHICSITEM_H
#define GLMAINWIDGETGRAPHICSITEM_H

#include <QtGui/QGraphicsObject>
#include <QtGui/QGraphicsLayoutItem>

namespace tlp {

class GlMainWidget;

class GlMainWidgetGraphicsItem : public QGraphicsObject, public QGraphicsLayoutItem {
  Q_OBJECT

  Q_PROPERTY(QSize size READ size WRITE resize)
  QSize _size;

  tlp::GlMainWidget *glMainWidget;
  bool redrawNeeded;

  unsigned char *renderingStore;

public:
  GlMainWidgetGraphicsItem(tlp::GlMainWidget *glMainWidget, const QSize& size);
  virtual ~GlMainWidgetGraphicsItem();

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

  QRectF boundingRect() const;
  QSize size() const {
    return _size;
  }

  void setRedrawNeeded(bool redrawNeeded) {
    this->redrawNeeded=redrawNeeded;
  }

  tlp::GlMainWidget *getGlMainWidget() {
    return glMainWidget;
  }

  void setGeometry(const QRectF &rect);

  bool eventFilter(QObject *, QEvent *evt);

public slots:
  void resize(const QSize& size);

protected :
  void wheelEvent(QGraphicsSceneWheelEvent *event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
  void hoverMoveEvent(QGraphicsSceneHoverEvent * event);
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
  void contextMenuEvent(QGraphicsSceneContextMenuEvent * event);

  QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const;

protected slots:
  void glMainWidgetDraw(GlMainWidget *,bool);
  void glMainWidgetRedraw(GlMainWidget *);

};

}

#endif // GLMAINWIDGETGRAPHICSITEM_H
