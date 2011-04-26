#ifndef TOOLBARITEM_H
#define TOOLBARITEM_H

#include <QtGui/QGraphicsItemGroup>
#include <QtCore/QEasingCurve>
#include "tulip/PushButtonItem.h"

class QAnimationGroup;
class QAction;
class QTimer;
class QGraphicsSvgItem;

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

  int spacing() const { return _spacing; }
  void setSpacing(int m) { _spacing = m; update(); }

  Qt::ToolBarArea area() const { return _snapArea; }
  void setAllowedAreas(Qt::ToolBarAreas a) { _allowedSnapAreas = a; }
  Qt::ToolBarAreas allowedAreas() const { return _allowedSnapAreas; }

signals:
  void expanded(bool);
  void buttonClicked(PushButtonItem *);
  void activeButtonClicked();

public slots:
  void collapse();
  void expand() { setExpanded(true); }
  void setExpanded(bool f);

protected slots:
  void buttonClicked();
  void buttonHovered(bool f);
  void expandAnimationFinished();
  void sceneResized();

protected:
  virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
  virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
  QVariant itemChange(GraphicsItemChange change, const QVariant &value);

  Qt::Orientation orientation() const { return _orientation; }
  void setOrientation(Qt::Orientation o);

private:
  // Inner state
    // Actions & buttons
  QVector<QAction *> _actions;
  QMap<QAction *,PushButtonItem *> _actionButton;
  QAction *_activeAction;
  PushButtonItem *_activeButton;
  QGraphicsSvgItem *_settingsIcon;
    // Expand & collapse
  bool _expanded;
  QAnimationGroup *_currentExpandAnimation;
  QTimer *_collapseTimeout;
    // Toolbar snapping
  Qt::ToolBarArea _snapArea;
  Qt::ToolBarAreas _allowedSnapAreas;

  // Toolbar style
  QSize _iconSize;
  int _spacing;
  Qt::Orientation _orientation;
  int _backgroundRectangleRound;
  bool _showSettingsButton;

    // Animations look & feel
  int _animationMsec;
  QEasingCurve _animationEasing;

    //Colors
  QColor _toolbarBorder;
  QColor _toolbarBackground;
  QColor _buttonBackground;
  QColor _buttonBorder;
  QColor _buttonHighlight;
  QColor _buttonHovered;

  QPointF translationVector() const;
  QSizeF buttonSize() const;
  PushButtonItem *buildButton(QAction *);
  void modifyButton(PushButtonItem *btn, const QPointF &newPos) const;
  QPointF setArea(Qt::ToolBarArea, const QPointF &);
};

#endif // TOOLBARITEM_H
