#include "tulip/WorkspaceExposeWidget.h"

#include <tulip/WorkspacePanel.h>
#include <QtGui/QGraphicsItemGroup>
#include <QtGui/QGraphicsPixmapItem>
#include <QtGui/QGraphicsTextItem>

using namespace tlp;

WorkspaceExposeWidget::WorkspaceExposeWidget(QWidget *parent): QGraphicsView(parent) {
  setScene(new QGraphicsScene);
}

int WorkspaceExposeWidget::currentPanelIndex() const {
  return _currentPanelIndex;
}

QVector<WorkspacePanel*> WorkspaceExposeWidget::panels() const {
  return _panels;
}

void WorkspaceExposeWidget::setData(const QVector<WorkspacePanel *> &panels, const QMap<WorkspacePanel*, QPixmap>& previews, int currentPanelIndex) {
  foreach(QGraphicsItem* it, _previewItems.values())
    delete it;

  _panels = panels;
  _previewPixmaps = previews;
  _currentPanelIndex = currentPanelIndex;

  _previewItems.clear();
  foreach(WorkspacePanel* panel, _panels) {

    QGraphicsItemGroup* group = new QGraphicsItemGroup();
    QGraphicsPixmapItem* pixmap = new QGraphicsPixmapItem(_previewPixmaps[panel]);
    QGraphicsTextItem* text = new QGraphicsTextItem(panel->windowTitle());
    group->addToGroup(pixmap);
    group->addToGroup(text);
    text->setPos(pixmap->boundingRect().width()/2-text->boundingRect().width()/2,pixmap->boundingRect().height()+5);
    scene()->addItem(group);
    group->setFlag(QGraphicsItem::ItemIsMovable);
    group->setPos(20,20);
  }
}

void WorkspaceExposeWidget::resizeEvent(QResizeEvent *event) {
  QGraphicsView::resizeEvent(event);
  scene()->setSceneRect(rect());
}
