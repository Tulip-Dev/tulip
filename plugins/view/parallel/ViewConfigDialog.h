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

#ifndef VIEW_CONFIG_DIALOG_H
#define VIEW_CONFIG_DIALOG_H	

#include <list>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QRadioButton>

#include "ParallelCoordinatesGraphProxy.h"
#include "ItemsDialogList.h"

namespace tlp {

class ViewConfigDialog : public QDialog {

  Q_OBJECT

public :
  
  ViewConfigDialog(ParallelCoordinatesGraphProxy *data, QWidget *parent = 0);

  std::list<std::string> &getSelectedProperties();

  viewType getViewType();
  void setViewType(viewType vType);

  DataLocation getDataLocation();
  void setDataLocation(DataLocation location);

public slots:
  
  
  void pressButtonAdd();
  
  void pressButtonRem();

  void pressButtonUp();

  void pressButtonDown();

  void pushOK();

  void viewCheckboxClicked();

  void splineCheckboxClicked();


private :

  QHash<QString,itemInfo > hashDataBase;
  std::list<std::string> res;
  
  ItemsDialogList * items;
  ItemsDialogList * choices;

  QRadioButton *nodesButton;
  QRadioButton *edgesButton;
  QCheckBox *view3D;
  QCheckBox *splineView;

};

}


#endif // VIEW_CONFIG_DIALOG_H	    

