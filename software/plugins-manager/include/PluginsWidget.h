/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

  class TLP_PLUGINSMANAGER_SCOPE PluginsWidget : public QWidget{

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
    void modifyTreeView(int viewNumber, bool lastVersion, bool compatibleVersion, bool notinstalledVersion);
    
    
  public slots :
    virtual void updatePluginsTree();
    virtual void serverPopup();
    void applyChange();
    void restore();
    void serverView();
    void groupView();
    void pluginView();
    //void configPluginPathIsModified();
    void clickOnPluginSlot(const PluginInfo *pi);

  signals:
    void pluginsViewIsUpdated();
};
  

}

#endif
