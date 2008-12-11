//-*-c++-*-
/*
 Author: Antoine Lambert

 Email : antoine.lambert@labri.fr

 Last modification : 12/08

 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/

#ifndef ITEMSDIALOGLIST_H
#define ITEMSDIALOGLIST_H

#ifndef DOXYGEN_NOTFOR_DEVEL

#include <tulip/tulipconf.h>

#include <QtGui/QListWidget>
#include <QtGui/QMouseEvent>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDropEvent>
#include <QtGui/QMouseEvent>
#include <QtCore/QObject>
#include <QtCore/QHash>


struct itemInfo {
  QString attribut;
  bool choice;
};

// A Custom List Widget which supports drag and drop
class ItemsDialogList : public QListWidget {

  Q_OBJECT

 public:

  ItemsDialogList(QWidget *parent = NULL);

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
  QHash<QString,itemInfo> hashDataBase;
};

#endif // DOXYGEN_NOTFOR_DEVEL

#endif


