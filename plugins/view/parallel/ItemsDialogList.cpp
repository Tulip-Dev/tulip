//-*-c++-*-
/*
 Author: Didier Bathily, Nicolas Bellino, Jonathan Dubois, Christelle Jolly, Antoine Lambert, Nicolas Sorraing

 Email : didier.bathily@etu.u-bordeaux1.fr, nicolas.bellino@etu.u-bordeaux1.fr, jonathan.dubois@etu.u-bordeaux1.fr, christelle.jolly@etu.u-bordeaux1.fr, antoine.lambert@etu.u-bordeaux1.fr, nicolas.sorraing@etu.u-bordeaux1.fr

 Last modification : 03/08

 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/

#include "ItemsDialogList.h"
#include <Qt/qapplication.h>

ItemsDialogList::ItemsDialogList(QWidget *parent) :
	QListWidget(parent) {
  setAcceptDrops(true);
}

void ItemsDialogList::mousePressEvent(QMouseEvent *event){
  if(event->button() == Qt::LeftButton)
    startPos = event->pos();
  QListWidget::mousePressEvent(event);
}

void ItemsDialogList::mouseMoveEvent(QMouseEvent *event){
  if(event->buttons() & Qt::LeftButton){
    int distance = (event->pos() - startPos).manhattanLength();
    if(distance >= QApplication::startDragDistance()){
      QListWidgetItem *item = currentItem();
      startDrag(item);
    }
  }
  QListWidget::mouseMoveEvent(event);
}

void ItemsDialogList::startDrag(QListWidgetItem *item){
  if(item){
    QMimeData *mimeData = new QMimeData;
    mimeData->setText(item->text());
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    if(drag->start(Qt::MoveAction) == Qt::MoveAction){
      changeStatus(item);
      delete item;
    }
  }
}

void ItemsDialogList:: dragEnterEvent(QDragEnterEvent *event){
  dragMoveOrEnterEvent(event);
}

void ItemsDialogList:: dragMoveEvent(QDragMoveEvent *event){
  dragMoveOrEnterEvent(event);
}

void ItemsDialogList:: dragMoveOrEnterEvent(QDragMoveEvent *event){
   ItemsDialogList *source =
    qobject_cast<ItemsDialogList *>(event->source());
  if(source && source !=this){
    event->setDropAction(Qt::MoveAction);
    event->accept();
  }
}

void ItemsDialogList::dropEvent(QDropEvent *event){
  ItemsDialogList *source =
    qobject_cast<ItemsDialogList *>(event->source());
  if(source && source !=this){
    addItem(event->mimeData()->text());
    event->setDropAction(Qt::MoveAction);
    event->accept();
  }
}

void ItemsDialogList::addItemList(QString item){
  addItem(item);
}

void ItemsDialogList::deleteItemList(QListWidgetItem *item){
  changeStatus(item);
  delete item;
}

void ItemsDialogList::changeStatus(QListWidgetItem *item){
  bool x = (hashDataBase.value(item->text())).choice;
  struct itemInfo temp;
  if(x==false){
    temp.choice =true;
  }else{
    temp.choice =false;
  }
  temp.attribut = (hashDataBase.value(item->text())).attribut;
  hashDataBase.remove(item->text());
  hashDataBase.insert(item->text(),temp);
}

