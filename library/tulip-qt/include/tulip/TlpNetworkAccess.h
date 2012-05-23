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
#ifndef _TLPNETWORKACCESS_H
#define _TLPNETWORKACCESS_H

#include <QtNetwork/QNetworkAccessManager>
#include <tulip/tulipconf.h>

namespace tlp {

struct NetworkProxyConfiguration {
  bool proxyEnabled;
  QString address;
  quint16 port;
  bool authenticationEnabled;
  QString login;
  QString passwd;
};

/**
 * @brief return the current network proxy configuration
 *
 **/
TLP_QT_SCOPE NetworkProxyConfiguration getNetworkProxyConfiguration();

/**
 * @brief allow to set the current network proxy configuration
 *
 **/
TLP_QT_SCOPE void setNetworkProxyConfiguration(const NetworkProxyConfiguration&);

/**
 * @brief return a proxy configured QNetworkAccessManager
 * Only one QNetworkAccessManager is needed by application
 *
 **/
TLP_QT_SCOPE QNetworkAccessManager* getNetworkAccessManager();

}

#endif


