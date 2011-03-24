#ifndef PUSHBUTTONITEM_H
#define PUSHBUTTONITEM_H

#include <QtGui/QIcon>
#include <QtCore/QSize>
#include <QtCore/QString>
#include <QtGui/QGraphicsLayoutItem>
#include "tulip/AnimatedGraphicsObject.h"

namespace tlp {
  class PushButtonItem: public AnimatedGraphicsObject, public QGraphicsLayoutItem {
Q_INTERFACES(QGraphicsLayoutItem);
Q_OBJECT;
Q_PROPERTY(QString text READ text WRITE setText);
Q_PROPERTY(QIcon icon READ icon WRITE setIcon);
Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize);

public:
  PushButtonItem(const QString &text = "", const QIcon &icon = QIcon(), const QSize &iconSize = QSize(32,32), QGraphicsItem *parent=0);
  PushButtonItem(QAction *action, const QSize &iconSize = QSize(32,32), QGraphicsItem *parent=0);
  virtual ~PushButtonItem();

  QString text() const;
  QIcon icon() const;
  QSize iconSize() const;

  void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);
  QRectF boundingRect() const;

  void setGeometry(const QRectF &rect);

  void setAction(QAction *);
  QAction *action() const { return _action; }

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
  void hoverEnterEvent(QGraphicsSceneHoverEvent *);
  void hoverLeaveEvent(QGraphicsSceneHoverEvent *);

  QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;

public slots:
  void setText(const QString &);
  void setIcon(const QIcon &);
  void setIconSize(const QSize &);

signals:
  void clicked();
  void pressed();
  void released();
  void hovered(bool);

private:
  QIcon _icon;
  QSize _iconSize;
  QAction *_action;

  bool _pressed;
  bool _hovered;
  bool _clicking;

};
}
#endif // PUSHBUTTONITEM_H
