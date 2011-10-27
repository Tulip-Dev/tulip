#ifndef PROCESSINGANIMATIONITEM_H
#define PROCESSINGANIMATIONITEM_H

#include <QtGui/QGraphicsPixmapItem>
#include <QtCore/QTimer>
#include <QtCore/QObject>

class ProcessingAnimationItem : public QObject, public QGraphicsPixmapItem {
  Q_OBJECT

  QVector<QPixmap> _pixmaps;
  QTimer _animationTimer;
  unsigned int _currentFrame;

public:
  explicit ProcessingAnimationItem(const QPixmap& pixmap, const QSize& size, QGraphicsItem* parent=0, QGraphicsScene* scene=0);
  virtual ~ProcessingAnimationItem();

protected slots:
  void animationTimeout();
};

#endif // PROCESSINGANIMATIONITEM_H
