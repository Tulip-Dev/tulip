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

#ifndef _Tulip_PLUGINVIEWWIDGET_H_
#define _Tulip_PLUGINVIEWWIDGET_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

#include <QtGui/QTreeWidget>
#include <QtXml/QDomDocument>

#include "PluginInfo.h"
#include "MultiServerManager.h"
#include "TPWFonctor.h"
#include "UpdatePlugin.h"
#include "InstallPluginDialog.h"

namespace tlp {

  enum PluginAttributePosition {
    PLUGIN_NAME_POSITION=0,
    PLUGIN_TYPE_POSITION,
    PLUGIN_VERSION_POSITION,
    PLUGIN_FILENAME_POSITION,
    PLUGIN_LOCAL_VERSION_POSITION,
    PLUGIN_GLOBAL_VERSION_POSITION
  };


  class TLP_PLUGINSMANAGER_SCOPE PluginsViewWidget : public QTreeWidget {

    Q_OBJECT

    QTreeWidgetItem *getNthParent(QTreeWidgetItem *twi,int n);

  protected :
    QDomDocument xmlDoc;
    MultiServerManager* _msm;
    UpdatePlugin updatePlugin;
    std::set<DistPluginInfo,PluginCmp> pluginsToInstall;
    std::set<LocalPluginInfo,PluginCmp> pluginsToRemove;
    bool lastVersion ,compatibleVersion, notInstalledVersion;
    bool listIsChanging;
    bool pluginsDirIsWritable;

    bool activate;

    void init(QWidget * parent = 0);

    // Check if the plugin is installed
    //bool isInstalled(const std::string &typeName,const std::string &pluginName);

    //check if we have write permission on the plugin
    //bool isWritable(const std::string & plugFileName);
    void applyFilter(QTreeWidgetItem * tree);
    bool isMoreRecent(const std::string  & version1, const std::string  &version2);
    bool isCompatible(const std::string  & version);
    std::vector<int> getArrangementGlobalListPositions();

    void updateCheckBox(QTreeWidgetItem*,std::string,std::string,std::string,Qt::CheckState, int, int);
    void updateToInstallList(QTreeWidgetItem*);

    void hideChild(QTreeWidgetItem *parent);
    void removeHiddenChild(QTreeWidgetItem *parent);
    QTreeWidgetItem *findChildrenWithText(QTreeWidgetItem *parent,const std::string &text);
    void setItemCheckability(const PluginInfo *pluginInfo,bool created,QTreeWidgetItem *twi);

  public :
    PluginsViewWidget(MultiServerManager* msm, QWidget * parent=0 );
    virtual ~PluginsViewWidget();

    void changeList();
    void setLastVersion(bool version);
    void setCompatiblesVersion(bool version);
    void setNotinstalledVersion(bool version);
    bool setPluginDisplayInTree(const PluginInfo *pluginInfo,QTreeWidgetItem *twi);
    void applyChange();
    void restore();

    //void installAllDependencies(std::vector<DistPluginInfo> depNoInstall);
    bool isAVersionItem(QTreeWidgetItem *item);

  protected slots :
    void pluginInstalledSlot();
    void pluginUninstalledSlot();
    //ajout a micka
    void getPluginInfoSlot();
    void changed(QTreeWidgetItem*);
    //TMP -> SAB
    void serverNameReceived(MultiServerManager*,std::string,std::string);

  signals :
    void pluginInstalled();
    void pluginClicked(PluginInfo pi);

    void pluginInfoSignal(const PluginInfo *pi);
  };

}
#endif //_Tulip_PLUGINVIEWWIDGET_H_
