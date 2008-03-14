//-*-c++-*-
/**
 Author: Morgan Mathiaut and Samuel Carruesco, Mickael Melas, Laurent Peyronnet, Michael Roche, Sabrina Wons
 Email : mathiaut@labri.fr
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef _Tulip_SERVER_H_
#define _Tulip_SERVER_H_

#include <list>

#include <QtCore/QTimer>
#include "SoapResponseReader.h"
#include "SoapRequestBuilder.h"
#include "HttpRequest.h"

namespace tlp {
  enum ResultType { 
    csvGetNbRows,
    csvGetNbColumns,
    csvGetNbElements,
    receiveBinaryFile,
    getPluginXMLList,
    receivePlugin,
    unknowMessage
  };

  enum Platform {
    WINDOWS,
    LINUX,
    MACOSX
  };

  class Request;
  
  class TLP_PLUGINSMANAGER_SCOPE  Server : public QObject {  
  
    Q_OBJECT

  public:
    
    Server(const std::string& serverAddress, QObject* parent = 0);
    virtual ~Server();
    
    virtual void send(Request* request);
    virtual void getAddress(std::string &address);
				    
  public slots :
    void requestDone();
    void timeout();
    
  private:
    HttpRequest *http; 
    std::string serverAddress;
    std::list<Request *> requests;
    QTimer timer;

  };

}


#endif // _SERVER_H_
