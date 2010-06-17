/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#ifndef Tulip_UPDATEPLUGINSDIALOG_H
#define Tulip_UPDATEPLUGINSDIALOG_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

#include <QtNetwork/QNetworkProxy>

#include "UpdatePluginsDialogData.h"
#include "PluginInfo.h"

namespace tlp {

  class TLP_PLUGINSMANAGER_SCOPE UpdatePluginsDialog : public QDialog, public Ui::UpdatePluginsDialogData {

    Q_OBJECT

  public:

    UpdatePluginsDialog(const std::vector<DistPluginInfo*> &pluginsOutOfDate,QWidget* parent);

    void getPluginsToUpdate(std::set<DistPluginInfo,PluginCmp> &pluginsToUpdate);

  protected slots :
    void itemChangedSlot(QListWidgetItem *);
    void okSlot();

  protected :

    const std::vector<DistPluginInfo *> *pluginsOutOfDate;
    std::set<std::string> pluginsToNoUpdate;

  };

}

#endif // Tulip_UPDATEPLUGINSDIALOG_H
