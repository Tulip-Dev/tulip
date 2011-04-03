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

  QSize iconSize() const { return _iconSize; }
  void setIconSize(const QSize &s) { _iconSize = s; update(); }

  int margin() const { return _margin; }
  void setMargin(int m) { _margin = m; update(); }

  Qt::Orientation orientation() const { return _orientation; }
  void setOrientation(Qt::Orientation o) { _orientation = o; update(); }

  int animationMsec() const { return _animationMsec; }
  void setAnimationMsec(int ms) { _animationMsec = ms; update(); }

  QEasingCurve animationEasing() const { return _animationEasing; }
  void setAnimationEasing(const QEasingCurve &c) { _animationEasing = c; update(); }

  QColor outerOutline() const { return _outerOutline; }
  void setOuterOutline(const QColor &c) { _outerOutline = c; update(); }

  QColor innerOutline() const { return _innerOutline; }
  void setInnerOutline(const QColor &c) { _innerOutline = c; update(); }

  QColor backgroundGradientStep1() const { return _backgroundGradientStep1; }
  void setBackgroundGradientStep1(const QColor &c) { _backgroundGradientStep1 = c; update(); }

  QColor backgroundGradientStep2() const { return _backgroundGradientStep2; }
  void setBackgroundGradientStep2(const QColor &c) { _backgroundGradientStep2 = c; update(); }

  bool isExpanded() const { return _expanded; }

signals:
  void expanded(bool);
  void buttonClicked(PushButtonItem *);
  void activeButtonClicked();

public slots:
  void collapse() { setExpanded(false); }
  void expand() { setExpanded(true); }
  void setExpanded(bool f);

protected slots:
  void buttonClicked();
  void buttonHovered(bool);
  void expandAnimationFinished();

protected:
  virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
  virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

private:
  // Inner state
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
