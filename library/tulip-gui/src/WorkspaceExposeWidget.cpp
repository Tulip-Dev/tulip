#include "tulip/WorkspaceExposeWidget.h"

#include <math.h>

#include <QtCore/QEvent>
#include <QtCore/QDebug>
#include <QtCore/QPropertyAnimation>
#include <QtCore/QParallelAnimationGroup>
#include <QtGui/QGraphicsObject>
#include <QtGui/QGraphicsSceneMouseEvent>

#include <tulip/View.h>
#include <tulip/WorkspacePanel.h>

using namespace tlp;

// Helper classes

class PreviewItem: public QGraphicsObject {
  QPixmap _pixmap;
  WorkspacePanel* _panel;

public:
  explicit PreviewItem(const QPixmap& pixmap, WorkspacePanel* panel, QGraphicsItem* parent = 0): QGraphicsObject(parent), _pixmap(pixmap), _panel(panel) {
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
  }

  QRectF boundingRect() const {
    QRectF result = QRectF(0,0,WorkspaceExposeWidget::previewSize().width(),WorkspaceExposeWidget::previewSize().height()+30);
    return result;
  }

  void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    painter->drawPixmap(0,0,WorkspaceExposeWidget::previewSize().width(),WorkspaceExposeWidget::previewSize().height(),_pixmap);
    QFont f;
    f.setBold(true);
    painter->setFont(f);
    painter->drawText(0,WorkspaceExposeWidget::previewSize().height()+5,WorkspaceExposeWidget::previewSize().width(),30,Qt::AlignHCenter | Qt::TextSingleLine | Qt::TextWordWrap,_panel->windowTitle());
  }

};
// *************************

QSize WorkspaceExposeWidget::previewSize() {
  return QSize(150,100);
}

const int WorkspaceExposeWidget::MARGIN = 50;

WorkspaceExposeWidget::WorkspaceExposeWidget(QWidget *parent): QGraphicsView(parent), _positionAnimation(NULL), _selectedItem(NULL), _placeholderItem(NULL) {
  setScene(new QGraphicsScene);
  setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

int WorkspaceExposeWidget::currentPanelIndex() const {
}

QVector<WorkspacePanel*> WorkspaceExposeWidget::panels() const {
}

void WorkspaceExposeWidget::setData(const QVector<WorkspacePanel *> &panels, int currentPanelIndex) {
  scene()->clear();
  foreach(WorkspacePanel* p, panels) {
    for (int i=0; i<5; ++i) {
      QPixmap pixmap = p->view()->snapshot(previewSize());
      PreviewItem* item = new PreviewItem(pixmap,p);
      scene()->addItem(item);
      _items << item;
      item->installEventFilter(this);
    }
  }
  updatePositions();
}

void WorkspaceExposeWidget::resizeEvent(QResizeEvent *event) {
  updatePositions();
}

void WorkspaceExposeWidget::updatePositionsAnimationFinished() {
  setSceneRect(scene()->itemsBoundingRect());
  _positionAnimation = NULL;
}

void WorkspaceExposeWidget::updatePositions() {
  delete _positionAnimation;
  const int animDuration = 150;

  QParallelAnimationGroup* group = new QParallelAnimationGroup(this);
  int x=MARGIN,y=MARGIN;
  foreach(PreviewItem* i, _items) {
    if (i != _selectedItem) {
      QPropertyAnimation* moveAnim = new QPropertyAnimation(i,"pos",group);
      moveAnim->setDuration(animDuration);
      moveAnim->setStartValue(i->pos());
      moveAnim->setEndValue(QPointF(x,y));
      group->addAnimation(moveAnim);
    }
    else if (_selectedItem != NULL) {
      if (_placeholderItem == NULL) {
        _placeholderItem = new QGraphicsRectItem(0,0,previewSize().width(),previewSize().height());
        _placeholderItem->setBrush(QColor(230, 230, 230));
        _placeholderItem->setPen(QColor(200, 200, 200));
        scene()->addItem(_placeholderItem);
      }
      _placeholderItem->setPos(x,y);
    }
    x+=WorkspaceExposeWidget::previewSize().width() + MARGIN;
    if (x>=width()-WorkspaceExposeWidget::previewSize().width()-MARGIN) {
      x=MARGIN;
      y+=WorkspaceExposeWidget::previewSize().height()+MARGIN;
    }
  }

  _positionAnimation = group;
  connect(group,SIGNAL(finished()),this,SLOT(updatePositionsAnimationFinished()));
  group->start(QAbstractAnimation::DeleteWhenStopped);
}

bool WorkspaceExposeWidget::eventFilter(QObject* obj, QEvent* ev) {
  PreviewItem* item = static_cast<PreviewItem*>(obj);
  if (ev->type() == QEvent::GraphicsSceneMousePress) {
    _selectedItem = item;
    _selectedItem->setZValue(1);
    return false;
  }

  if (item == _selectedItem) {
    if (ev->type() == QEvent::GraphicsSceneMouseMove) {
      QGraphicsSceneMouseEvent* mouseEv = static_cast<QGraphicsSceneMouseEvent*>(ev);
      int itemPerLine = floor(width()/(WorkspaceExposeWidget::previewSize().width() + MARGIN));
      QPointF itemPos = mouseEv->scenePos();
      int line = itemPos.y() / (previewSize().height()+MARGIN);
      int col = itemPos.x() / (previewSize().width()+MARGIN);
      int index = line*itemPerLine + col;
      if (index != _items.indexOf(item)) {
        _items.removeOne(item);
        _items.insert(std::min<int>(index,_items.size()),item);
        updatePositions();
      }
    }
    else if (ev->type() == QEvent::GraphicsSceneMouseRelease) {
      _selectedItem->setZValue(0);
      _selectedItem = NULL;
      updatePositions();
      delete _placeholderItem;
      _placeholderItem = NULL;
    }
  }

  return false;
}
