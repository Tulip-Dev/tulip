#ifndef TOOLBARITEM_H
#define TOOLBARITEM_H

#include <QtGui/QGraphicsItemGroup>
#include <QtCore/QEasingCurve>
#include "tulip/PushButtonItem.h"

class QAnimationGroup;
class QAction;
class QTimer;
class QGraphicsSvgItem;

namespace tlp {
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

  QSize hoveredIconSize() const { return _hoveredIconSize; }
  void setHoveredIconSize(const QSize &s) { _hoveredIconSize = s; update(); }

  int spacing() const { return _spacing; }
  void setSpacing(int m) { _spacing = m; update(); }

  QColor buttonForegroundColor() const { return _buttonForegroundColor; }
  void setButtonForegroundColor(const QColor &c) { _buttonForegroundColor = c; update(); }

  QColor buttonBackgroundColor() const { return _buttonBackgroundColor; }
  void setButtonBackgroundColor(const QColor &c) { _buttonBackgroundColor = c; update(); }

  PushButtonItem::BackgroundShape buttonBackgroundShape() const { return _buttonBackgroundShape; }
  void setButtonBackgroundShape(PushButtonItem::BackgroundShape s) { _buttonBackgroundShape = s; }

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
  void buttonHovered(bool);
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
  PushButtonItem *_focusedButton;
  QGraphicsSvgItem *_settingsIcon;
    // Expand & collapse
  bool _expanded;
  QAnimationGroup *_currentExpandAnimation;
  QTimer *_collapseTimeout;
    // Toolbar snapping
  Qt::ToolBarArea _snapArea;
  Qt::ToolBarAreas _allowedSnapAreas;

  // Display parameters
    // Toolbar style
  QSize _iconSize;
  QSize _hoveredIconSize;
  int _spacing;
  Qt::Orientation _orientation;
  int _backgroundRectangleRound;
  QColor _backgroundColor;
  QColor _borderColor;
  bool _showSettingsButton;
    // Buttons style
  QColor _buttonForegroundColor;
  QColor _buttonBackgroundColor;
  QColor _highlightColor;
  PushButtonItem::BackgroundShape _buttonBackgroundShape;
    // Animations look & feel
  int _animationMsec;
  QEasingCurve _animationEasing;

  QPointF translationVector() const;
  QSizeF hoveredButtonSize() const;
  QSizeF buttonSize() const;
  PushButtonItem *buildButton(QAction *);
  void modifyButton(PushButtonItem *btn, const QSize &newSize, const QPointF &newPos) const;
  QPointF setArea(Qt::ToolBarArea, const QPointF &);
};
}

#endif // TOOLBARITEM_H
