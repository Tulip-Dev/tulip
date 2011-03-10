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
  
  class Server : public QObject {  
  
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
