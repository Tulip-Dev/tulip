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

#ifndef ITEMSDAILOGLIST_H
#define ITEMSDIALOGLIST_H

#include <QtGui/QListWidget>
#include <QtGui/QMouseEvent>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDropEvent>
#include <QtGui/QMouseEvent>
#include <QtCore/QObject>
#include <QtCore/QHash>

namespace tlp {

struct itemInfo{
  QString attribut;
  bool choice;
};

class ItemsDialogList : public QListWidget{
  
  Q_OBJECT
    
 public:
  ItemsDialogList(QHash<QString,itemInfo> *hashDataBaseDialog,
		  QWidget *parent = NULL);
  
   void addItemList(QString item);
  void deleteItemList(QListWidgetItem *item);
  
 protected:
   void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);

  void dragEnterEvent(QDragEnterEvent *event);
  void dragMoveEvent(QDragMoveEvent *event);
  void dropEvent(QDropEvent *event);
  

 private:
  void startDrag(QListWidgetItem *item);
   void changeStatus(QListWidgetItem *item);
  void dragMoveOrEnterEvent(QDragMoveEvent *event);
  QPoint startPos;
  QHash<QString,itemInfo> *hashDataBase;

};

}

#endif

 
