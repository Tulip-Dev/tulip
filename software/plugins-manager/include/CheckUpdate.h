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


#ifndef _Tulip_CHECKUPDATE_H_
#define _Tulip_CHECKUPDATE_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

#include <QtCore/QObject>

namespace tlp {

  class MultiServerManager;
 
  class CheckUpdate : public QObject {

    Q_OBJECT

    MultiServerManager *msm;
    int serverNumber;
 
  public : 
    
    CheckUpdate(std::vector<LocalPluginInfo> &pluginsList);

  private slots :

    void getResponse();

  };
  
}
#endif //_Tulip_MULTISERVERMANAGER_H_

