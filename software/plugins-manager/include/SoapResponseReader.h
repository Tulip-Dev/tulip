//-*-c++-*-
/**
 Author: Morgan Mathiaut and Samuel Carruesco, Mickael Melas, Laurent Peyronnet, Michael Roche, Sabrina Wons
 Email : mathiaut@labri.fr
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef _Tulip_SOAPRESPONSEREADER_H_
#define _Tulip_SOAPRESPONSEREADER_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

#include <QtXml/QDomDocument>

namespace tlp {

  class TLP_PLUGINSMANAGER_SCOPE SoapResponseReader{

    QDomDocument doc;
 
  public : 
    SoapResponseReader(const std::string &XML);
    
    void attachXML();
    bool getFunctionName(std::string &functionName);
    bool getReturnedData(std::string &data);
    
  private :
    void extractSoapEnv(const std::string &XML,std::string &result);
    
  };
  
}

#endif //_SOAPRESPONSEREADER_H_
