//-*-c++-*-
/**
 Author: Morgan Mathiaut and Samuel Carruesco, Mickael Melas, Laurent Peyronnet, Michael Roche, Sabrina Wons
 Email : mathiaut@labri.fr
 Last modification : 04/02/2008
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef Tulip_PLUGINSWIDGET_H
#define Tulip_PLUGINSWIDGET_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

#include <QtGui/QDialog>

#include "MultiServerManager.h"

class QBoxLayout;
class QPushButton;

namespace tlp {

  class PluginsViewWidget;
  class PluginsInfoWidget;
  class ServersOptionDialog;

  class TLP_PLUGINSMANAGER_SCOPE PluginsWidget : public QDialog{

    Q_OBJECT
      
  protected:
    
    PluginsViewWidget* pluginsList;
    PluginsInfoWidget* pluginsInfo;
    MultiServerManager* serverManager;
    QBoxLayout* boxLayout;
    ServersOptionDialog* serverDialog;
    QPushButton* applyChangeButton;
    QPushButton* restoreButton;
    QPushButton* serverViewButton;
    QPushButton* groupViewButton;
    QPushButton* pluginViewButton;
    
    //virtual void closeEvent( QCloseEvent * event );
    //void initPW(const string &serverAddr1="",const QString &serverAddr2="");
    
  public :
 
    void createWidget(QWidget *parent);
    PluginsWidget(std::vector<LocalPluginInfo> &plugins,QWidget *parent=0);
    PluginsWidget(MultiServerManager *msm,QWidget *parent=0);
    virtual ~PluginsWidget();
    void addServer(const std::string &serverAddr);
    void modifyTreeView(int viewNumber, bool lastVersion, bool compatibleVersion);
    
    
  public slots :
    virtual void updatePluginsTree();
    virtual void serverPopup();
    void applyChangeSlot();
    void restoreSlot();
    void serverViewSlot();
    void groupViewSlot();
    void pluginViewSlot();
    //void configPluginPathIsModified();
    void clickOnPluginSlot(const PluginInfo *pi);
  

  signals:
    void pluginsViewIsUpdated();

};
  

}

#endif
