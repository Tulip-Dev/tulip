//-*-c++-*-
/**
 Author: Morgan Mathiaut and Samuel Carruesco, Mickael Melas, Laurent Peyronnet, Michael Roche, Sabrina Wons
 Email : mathiaut@labri.fr
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef _Tulip_HTTPREQUEST_H
#define _Tulip_HTTPREQUEST_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

#include <QtNetwork/QHttp>
#include <QtCore/QFile>

namespace tlp {

  class TLP_PLUGINSMANAGER_SCOPE HttpRequest : public QObject{
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
    
  signals:
    void done();
	       
  private slots:
    void requestDone(bool error);
    
  };

}

#endif
