//-*-c++-*-
/**
 Author: Morgan Mathiaut and Samuel Carruesco, Mickael Melas, Laurent Peyronnet, Michael Roche, Sabrina Wons
 Email : mathiaut@labri.fr
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef _Tulip_SOAPREQUESTBUILDER_H_
#define _Tulip_SOAPREQUESTBUILDER_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

#include <Qt/QtXml>

namespace tlp {

  class TLP_PLUGINSMANAGER_SCOPE  SoapRequestBuilder {

  private :
    // Document Structure
    QDomDocument xmlDoc;
    QDomElement soapEnvelopeNode;
    QDomElement soapBodyNode;
    
    // Function remote call
    QDomElement soapFunctionNode;
    bool isFunctionNameSpecified;
    
  public : 
    SoapRequestBuilder();  
    void setFunctionName(const std::string &name);
    bool addFunctionParameter(const std::string &name,const std::string &type,const std::string &data);
    void getXML(std::string &result);
    
  };

}

#endif //_SOAPREQUESTBUILDER_H_
