#ifndef TOOLBARITEM_H
#define TOOLBARITEM_H

#include <QtGui/QGraphicsItemGroup>
#include <QtCore/QEasingCurve>

class QAnimationGroup;
class QAction;

namespace tlp {
class PushButtonItem;

class ToolbarItem: public QObject, public QGraphicsItemGroup {
    Q_OBJECT
public:
  ToolbarItem(QGraphicsItem *parent=0, QGraphicsScene *scene=0);
  virtual ~ToolbarItem();

  void addAction(QAction *);
  void removeAction(QAction *);
  void setActiveAction(QAction *);

  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget=0);
  virtual QRectF boundingRect() const;

public slots:
  void collapse() { setExpanded(false); }
  void expand() { setExpanded(true); }
  void setExpanded(bool f);

protected slots:
  void buttonClicked();
  void buttonHovered(bool);
  void expandAnimationFinished();

private:
  QVector<QAction *> _actions;
  QMap<QAction *,PushButtonItem *> _actionButton;
  QAction *_activeAction;
  PushButtonItem *_activeButton;
  bool _expanded;
  QAnimationGroup *_currentExpandAnimation;

  // Display parameters
  QSize _iconSize;
  int _margin;
  Qt::Orientation _orientation;
  int _animationMsec;
  QEasingCurve _animationEasing;

  QColor _outerOutline;
  QColor _innerOutline;
  QColor _backgroundGradientStep1;
  QColor _backgroundGradientStep2;

  QPointF translationVector() const;
  QSize hoveredIconSize() const;
  QRectF computeBoundingRect() const;
  PushButtonItem *buildButton(QAction *);
  void modifyButton(PushButtonItem *btn, const QSize &newSize, const QPointF &newPos) const;
  void layout();
};
}

#endif // TOOLBARITEM_H
