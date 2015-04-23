/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#include <tulip/ItemsListWidget.h>

#include <QMouseEvent>
#include <QApplication>
#include <QMimeData>
#include <QDrag>

namespace tlp {

ItemsListWidget::ItemsListWidget(QWidget *parent, const unsigned int maxListSize) :
  QListWidget(parent), maxListSize(maxListSize) {
  setAcceptDrops(true);
}

void ItemsListWidget::mousePressEvent(QMouseEvent *event) {
  if(event->button() == Qt::LeftButton)
    startPos = event->pos();

  QListWidget::mousePressEvent(event);
}

void ItemsListWidget::mouseMoveEvent(QMouseEvent *event) {
  if(event->buttons() & Qt::LeftButton) {
    int distance = (event->pos() - startPos).manhattanLength();

    if(distance >= QApplication::startDragDistance()) {
      QListWidgetItem *item = currentItem();
      beginDrag(item);
    }
  }

  QListWidget::mouseMoveEvent(event);
}

void ItemsListWidget::beginDrag(QListWidgetItem *item) {
  if(item) {
    QMimeData *mimeData = new QMimeData;
    mimeData->setText(item->text());
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);

    if(drag->start(Qt::MoveAction) == Qt::MoveAction) {
      changeStatus(item);
      delete item;
    }
  }
}

void ItemsListWidget:: dragEnterEvent(QDragEnterEvent *event) {
  dragMoveOrEnterEvent(event);
}

void ItemsListWidget:: dragMoveEvent(QDragMoveEvent *event) {
  dragMoveOrEnterEvent(event);
}

void ItemsListWidget:: dragMoveOrEnterEvent(QDragMoveEvent *event) {
  ItemsListWidget *source =
    qobject_cast<ItemsListWidget *>(event->source());

  if(source && source !=this) {
    event->setDropAction(Qt::MoveAction);
    event->accept();
  }
}

void ItemsListWidget::dropEvent(QDropEvent *event) {
  ItemsListWidget *source =
    qobject_cast<ItemsListWidget *>(event->source());

  if(source && source !=this) {
    if (addItemList(event->mimeData()->text())) {
      event->setDropAction(Qt::MoveAction);
      event->accept();
    }
  }
}

bool ItemsListWidget::addItemList(QString item) {
  if (maxListSize == 0 || (maxListSize != 0 && ((unsigned int) count()) < maxListSize)) {
    addItem(item);
    return true;
  }
  else {
    return false;
  }
}

void ItemsListWidget::deleteItemList(QListWidgetItem *item) {
  changeStatus(item);
  delete item;
}

void ItemsListWidget::changeStatus(QListWidgetItem *item) {
  bool x = (hashDataBase.value(item->text())).choice;
  struct itemInfo temp;

  if(x==false) {
    temp.choice =true;
  }
  else {
    temp.choice =false;
  }

  temp.attribut = (hashDataBase.value(item->text())).attribut;
  hashDataBase.remove(item->text());
  hashDataBase.insert(item->text(),temp);
}

}
