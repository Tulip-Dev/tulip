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

#ifndef _Tulip_HTTPREQUEST_H
#define _Tulip_HTTPREQUEST_H

#include <tulip/tulipconf.h>

#include <QtNetwork/QHttp>
#include <QtNetwork/QNetworkProxy>
#include <QtCore/QFile>

namespace tlp {

  class HttpRequest : public QObject{
    Q_OBJECT
    std::string result;
    QHttp http;
    QFile *outFile;
    std::string server;
    QObject *mainWindow;

  public:

    HttpRequest(const std::string &serverName,QObject *parent=0);

    void request(const std::string &msg);
    void get(const std::string &getFileName, const std::string &outFileName);

    void getResponse(std::string &response);

    void getServerName(std::string &serverName);

    static QNetworkProxy getProxy();

  signals:
    void done();

  private slots:
    void requestDone(bool error);

  };

}

#endif
