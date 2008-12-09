//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 10/08/2008
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef Tulip_RENDERINGPARAMETERSDIALOG_H
#define Tulip_RENDERINGPARAMETERSDIALOG_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

#include "tulip/RenderingParametersDialogData.h"

#include <QtGui/QTreeWidgetItem>

namespace tlp {

  class GlScene;
  class GlLayer;
  class GlComposite;
  class GlGraphComposite;
  class GlMainView;
  class GlMainWidget;

  class TLP_QT_SCOPE RenderingParametersDialog : public QDialog, public Ui::RenderingParametersDialogData {
    Q_OBJECT
      
    GlMainView *mainView;
    GlMainWidget* observedMainWidget;
    bool holdUpdateView;

  public:
    RenderingParametersDialog(QWidget* parent);
    void windowActivationChange(bool oldActive);

    void setGlMainView(GlMainView *view);
    void attachMainWidget(GlMainWidget *graphWidget);
						    
    void addLayer(tlp::GlScene*, const std::string&, tlp::GlLayer*);
    void addComposite(tlp::GlComposite *composite,QTreeWidgetItem *parent);
    void createGraphCompositeItem(tlp::GlGraphComposite *glGraphComposite,QTreeWidgetItem *item);
    void updateLayer(const std::string& name,tlp::GlLayer* layer);
    void delLayer(tlp::GlScene*, const std::string&, tlp::GlLayer*);
    void buildHierarchie(QTreeWidgetItem *item,QList<std::string>& hierarchie);		

  public slots:

    void updateView();
    void backColor();	
    void setBackgroundColor(QColor tmp);
    
  protected slots:
    
    void checkBoxClicked(QTreeWidgetItem* item, int column);
  };
  
}

#endif // Tulip_RENDERINGPARAMETERSDIALOG_H
