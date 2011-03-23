#ifndef PUSHBUTTONITEM_H
#define PUSHBUTTONITEM_H

#include <QtGui/QGraphicsObject>
#include <QtGui/QIcon>
#include <QtCore/QSize>
#include <QtCore/QString>
#include <QtGui/QGraphicsLayoutItem>

namespace tlp {
  class PushButtonItem: public QGraphicsObject, public QGraphicsLayoutItem {
Q_INTERFACES(QGraphicsLayoutItem);
Q_OBJECT;
Q_PROPERTY(QString text READ text WRITE setText);
Q_PROPERTY(QIcon icon READ icon WRITE setIcon);
Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize);

public:
  PushButtonItem(const QString &text = "", const QIcon &icon = QIcon(), const QSize &iconSize = QSize(32,32), QGraphicsItem *parent=0);
  virtual ~PushButtonItem();

  QString text() const;
  QIcon icon() const;
  QSize iconSize() const;

  void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);
  QRectF boundingRect() const;

  void setGeometry(const QRectF &rect);

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
  QString _text;
  QIcon _icon;
  QSize _iconSize;

  bool _pressed;
  bool _hovered;
  bool _clicking;
};
}
#endif // PUSHBUTTONITEM_H
