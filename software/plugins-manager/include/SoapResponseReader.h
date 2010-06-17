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
