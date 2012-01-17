#include "tulip/WorkspaceExposeWidget.h"

#include <QtCore/QEvent>
#include <QtCore/QDebug>
#include <QtCore/QPropertyAnimation>
#include <QtCore/QParallelAnimationGroup>
#include <QtGui/QGraphicsObject>

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

WorkspaceExposeWidget::WorkspaceExposeWidget(QWidget *parent): QGraphicsView(parent), _positionAnimation(NULL) {
  setScene(new QGraphicsScene);
  setAlignment(Qt::AlignCenter | Qt::AlignTop);
}

int WorkspaceExposeWidget::currentPanelIndex() const {
}

QVector<WorkspacePanel*> WorkspaceExposeWidget::panels() const {
}

void WorkspaceExposeWidget::setData(const QVector<WorkspacePanel *> &panels, int currentPanelIndex) {
  scene()->clear();
  foreach(WorkspacePanel* p, panels) {
    for (int i=0;i<50;++i) {
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
  const int spacing = 30;
  const int animDuration = 150;
  QParallelAnimationGroup* group = new QParallelAnimationGroup(this);
  int x=spacing,y=spacing;
  foreach(PreviewItem* i, _items) {
    QPropertyAnimation* moveAnim = new QPropertyAnimation(i,"pos",group);
    moveAnim->setDuration(animDuration);
    moveAnim->setStartValue(i->pos());
    moveAnim->setEndValue(QPointF(x,y));
    x+=i->boundingRect().width() + spacing;
    if (x>=width()-i->boundingRect().width()-spacing) {
      x=spacing;
      y+=i->boundingRect().height()+spacing;
    }
    group->addAnimation(moveAnim);
  }

  _positionAnimation = group;
  connect(group,SIGNAL(finished()),this,SLOT(updatePositionsAnimationFinished()));
  group->start(QAbstractAnimation::DeleteWhenStopped);
}

bool WorkspaceExposeWidget::eventFilter(QObject* obj, QEvent* ev) {
  if (ev->type() == QEvent::GraphicsSceneMove) {
    qWarning() << "prout";
  }
  return false;
}
