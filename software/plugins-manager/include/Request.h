//-*-c++-*-
/**
 Author: Morgan Mathiaut and Samuel Carruesco, Mickael Melas, Laurent Peyronnet, Michael Roche, Sabrina Wons
 Email : mathiaut@labri.fr
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef _Tulip_REQUEST_H_
#define _Tulip_REQUEST_H_

#include <cassert>
#include <iostream>

#include <QtCore/QObject>

#include "ResponseTreatment.h"
#include "Server.h"

namespace tlp {

  class TLP_PLUGINSMANAGER_SCOPE  Request {

  public : 
    virtual ~Request() {}

    bool post;
    
  protected :  
    Request():post(false){};

  };

  class TLP_PLUGINSMANAGER_SCOPE  GetRequest : public Request {

  protected :

    RequestFinish* requestFinish;

  public :  
    GetRequest(RequestFinish* requestFinish,const std::string &getFileName,const std::string &outFileName):requestFinish(requestFinish),getFileName(getFileName),outFileName(outFileName){};
    virtual ~GetRequest();
    virtual void applyRequestFinish() const;

    std::string getFileName;
    std::string outFileName;

  };
  
  class TLP_PLUGINSMANAGER_SCOPE  SOAPRequest : public Request{
    
  protected :
    ResponseTreatment* respTreatment;

  public : 
    virtual ~SOAPRequest();

    virtual void getXml(std::string &xml) const = 0;
    virtual void applyResponseTreatment(char **data) const;
    
  protected :  
    SOAPRequest():respTreatment(0){post=true;};
    
  };

  class TLP_PLUGINSMANAGER_SCOPE  DownloadPluginRequest : public SOAPRequest{
    
    std::string name;

  public:   
    DownloadPluginRequest(const std::string &name);
    
    void getXml(std::string &xml) const;
    
  };

  class TLP_PLUGINSMANAGER_SCOPE  ConnectServerRequest : public SOAPRequest{

  public:   
    
    void getXml(std::string &xml) const;
    
  };

  class TLP_PLUGINSMANAGER_SCOPE  GetServerNameRequest: public SOAPRequest{

  public: 
    
    GetServerNameRequest(ResponseTreatment* resp);
    
    void getXml(std::string &xml) const;
    
  };
  
  
  class TLP_PLUGINSMANAGER_SCOPE  ServerNameTreatment : public QObject, public ResponseTreatment{
    Q_OBJECT
    
  private:
    std::string addr;
    
  public :
    ServerNameTreatment(std::string addr);
    
  void operator()(char **response);
    
  signals:
    void nameReceived(ServerNameTreatment*,std::string,std::string);
    
  };

  class TLP_PLUGINSMANAGER_SCOPE  GetPluginsListRequest: public SOAPRequest{
    
  public:   
    GetPluginsListRequest(ResponseTreatment* respT);
    
    void getXml(std::string &xml) const;
    
  };

  class TLP_PLUGINSMANAGER_SCOPE  GetPluginRequest: public GetRequest {
  
  public: 
    
    GetPluginRequest(RequestFinish* requestFinish,const std::string& pluginName,const std::string &outFileName)
      :GetRequest(requestFinish,std::string("/plugins/")+pluginName,outFileName){};
    
  };

  class TLP_PLUGINSMANAGER_SCOPE GetPluginInfoRequest: public SOAPRequest {
  
  public: 
    
    GetPluginInfoRequest(const std::string& pluginFileName, const std::string& pluginVersion, ResponseTreatment* treatment)
      :pluginFileName(pluginFileName), pluginVersion(pluginVersion){
      respTreatment=treatment;
    };
    
    void getXml(std::string &xml) const {
      SoapRequestBuilder request;
      
      request.setFunctionName("getPluginXMLInfo");
      
      request.addFunctionParameter("pluginFileName","string",pluginFileName);
      request.addFunctionParameter("pluginVersion","string",pluginVersion);
      
      request.getXML(xml);
  };

  private :
    
    std::string pluginFileName;
    std::string pluginVersion;
    Platform platform;
    
  };

  class TLP_PLUGINSMANAGER_SCOPE GetPluginDocRequest: public SOAPRequest {
  
  public: 
    
    GetPluginDocRequest(const std::string& pluginFileName, const std::string& pluginVersion, ResponseTreatment* treatment)
      :pluginFileName(pluginFileName), pluginVersion(pluginVersion){
      respTreatment=treatment;
    };
    
    void getXml(std::string &xml) const {
      SoapRequestBuilder request;
      
      request.setFunctionName("getPluginXMLDoc");
      
      request.addFunctionParameter("pluginFileName","string",pluginFileName);
      request.addFunctionParameter("pluginVersion","string",pluginVersion);
      
      request.getXML(xml);
  };

  private :
    
    std::string pluginFileName;
    std::string pluginVersion;
    Platform platform;
    
  };

}


#endif //_REQUEST_H_
