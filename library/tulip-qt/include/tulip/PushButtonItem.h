#ifndef PUSHBUTTONITEM_H
#define PUSHBUTTONITEM_H

#include <QtGui/QIcon>
#include <QtCore/QSize>
#include <QtCore/QString>
#include <QtGui/QGraphicsLayoutItem>
#include <QtGui/QPen>
#include <QtGui/QBrush>
#include "tulip/AnimatedGraphicsObject.h"

namespace tlp {
  class PushButtonItem: public AnimatedGraphicsObject, public QGraphicsLayoutItem {
Q_INTERFACES(QGraphicsLayoutItem);
Q_OBJECT;
Q_PROPERTY(QString text READ text WRITE setText);
Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize);
Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor);
Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor);

public:
  enum BackgroundShape {
    NoShape,
    CircleShape,
    SquareShape
  };

  PushButtonItem(QAction *action, const QSize &iconSize = QSize(32,32), QGraphicsItem *parent=0, bool ownAction=false);
  virtual ~PushButtonItem();

  QString text() const;
  QSize iconSize() const;

  void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);
  QRectF boundingRect() const;

  void setGeometry(const QRectF &rect);

  void setAction(QAction *);
  QAction *action() const { return _action; }

  void setItemSize(const QSizeF &s);
  QSizeF itemSize() const;

  bool hovered() const { return _hovered; }

  BackgroundShape buttonShape() const { return _backgroundShape; }
  void setButtonShape(BackgroundShape s) { _backgroundShape = s; update(); }

  QColor borderColor() const { return _borderColor; }
  void setBorderColor(const QColor &c) { _borderColor = c; update(); }

  QColor backgroundColor() const { return _backgroundColor; }
  void setBackgroundColor(const QColor &c) { _backgroundColor = c; update(); }

  bool fadeout() const { return _fadeout; }
  void setFadeout(bool f) { _fadeout = f; update(); }

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
  void hoverEnterEvent(QGraphicsSceneHoverEvent *);
  void hoverLeaveEvent(QGraphicsSceneHoverEvent *);

  QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;

public slots:
  void setText(const QString &);
  void setIconSize(const QSize &);

signals:
  void clicked();
  void pressed();
  void released();
  void hovered(bool);

private:
  QSize _iconSize;
  QAction *_action;

  bool _pressed;
  bool _hovered;
  bool _clicking;
  bool _fadeout;
  bool _ownAction;

  BackgroundShape _backgroundShape;
  QColor _borderColor;
  QColor _backgroundColor;
};
}
#endif // PUSHBUTTONITEM_H
