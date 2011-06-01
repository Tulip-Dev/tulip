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

#ifndef _Tulip_SOAPREQUESTBUILDER_H_
#define _Tulip_SOAPREQUESTBUILDER_H_

#include <tulip/tulipconf.h>

#include <QtXml/QDomDocument>

namespace tlp {

  class SoapRequestBuilder {

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
