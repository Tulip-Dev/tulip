//-*-c++-*-
/*
 Author: Antoine Lambert

 Email : antoine.lambert@labri.fr

 Last modification : 02/09

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


namespace tlp {

struct itemInfo {
  QString attribut;
  bool choice;
};

// A Custom List Widget which supports drag and drop
class TLP_QT_SCOPE ItemsListWidget : public QListWidget {

  Q_OBJECT

 public:

  ItemsListWidget(QWidget *parent = NULL, const unsigned int maxListSize = 0);

  // Method which adds an item in the list
  // return true if the item has been added, false if the maximum size of the list is already reached
  bool addItemList(QString item);

  void deleteItemList(QListWidgetItem *item);

  // Method which sets the maximum size of the list
  // if 0, there is no size restriction
  void setMaxListSize(const unsigned int maxListSize) {this->maxListSize = maxListSize;}
  unsigned int getMaxListSize() const {return maxListSize;}

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
  unsigned int maxListSize;
};

}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif


