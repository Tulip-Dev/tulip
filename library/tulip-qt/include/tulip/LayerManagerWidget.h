//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 28/11/2007
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef Tulip_LAYERMANAGERWIDGET_H
#define Tulip_LAYERMANAGERWIDGET_H

#include <tulip/tulipconf.h>

#include <QtGui/QButtonGroup>

#include <tulip/GlMainWidget.h>
#include <tulip/GlScene.h>
#include <tulip/GlGraphComposite.h>

#include "tulip/LayerManagerWidgetData.h"


/** \addtogroup Tulip_Widgets */ 
/*@{*/

namespace tlp {

class TLP_QT_SIMPLE_SCOPE LayerManagerWidget : public LayerManagerWidgetData { //, public SceneObserver { 
  Q_OBJECT
  
public:
  LayerManagerWidget(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
  ~LayerManagerWidget();
  
  void attachMainWidget(GlMainWidget *graphWidget);
						    
  void addLayer(tlp::GlScene*, const std::string&, tlp::GlLayer*);
  void addComposite(tlp::GlComposite *composite,QTreeWidgetItem *parent);
  void createGraphCompositeItem(tlp::GlGraphComposite *glGraphComposite,QTreeWidgetItem *item);
  void updateLayer(const std::string& name,tlp::GlLayer* layer);
  void delLayer(tlp::GlScene*, const std::string&, tlp::GlLayer*);
  void buildHierarchie(QTreeWidgetItem *item,QList<std::string>& hierarchie);								 
									 

protected slots:

  void checkBoxClicked(QTreeWidgetItem* item, int column);

private:

  GlMainWidget* observedMainWidget;
 
};

}

/*@}*/
#endif // Tulip_LAYERMANAGERWIDGET_H
