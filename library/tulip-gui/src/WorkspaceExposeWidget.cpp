#include "tulip/WorkspaceExposeWidget.h"

#include <QtCore/QPropertyAnimation>
#include <QtCore/QParallelAnimationGroup>
#include <tulip/View.h>
#include <tulip/WorkspacePanel.h>
#include <QtGui/QGraphicsObject>

using namespace tlp;

// Helper classes

class PreviewItem: public QGraphicsObject {
  QPixmap _pixmap;
  WorkspacePanel* _panel;

public:
  explicit PreviewItem(const QPixmap& pixmap, WorkspacePanel* panel, QGraphicsItem* parent = 0): QGraphicsObject(parent), _pixmap(pixmap), _panel(panel) {
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

void WorkspaceExposeWidget::updatePositions() {
  const int spacing = 30;
  QParallelAnimationGroup* group = new QParallelAnimationGroup(this);
  int x=0,y=0;
  foreach(PreviewItem* i, _items) {
    QPropertyAnimation* moveAnim = new QPropertyAnimation(i,"pos",group);
    moveAnim->setDuration(300);
    moveAnim->setStartValue(i->pos());
    moveAnim->setEndValue(QPointF(x,y));
    x+=i->boundingRect().width() + spacing;
    if (x>=width()-i->boundingRect().width()-spacing) {
      x=0;
      y+=i->boundingRect().height()+spacing;
    }
    group->addAnimation(moveAnim);
  }
  group->start(QAbstractAnimation::DeleteWhenStopped);
}

QSize WorkspaceExposeWidget::previewSize() {
  return QSize(150,100);
}

WorkspaceExposeWidget::WorkspaceExposeWidget(QWidget *parent): QGraphicsView(parent) {
  setScene(new QGraphicsScene);
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
    }
  }
}
