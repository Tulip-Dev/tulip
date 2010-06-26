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
#include "Request.h"

#include <tulip/TulipRelease.h>

#include "SoapRequestBuilder.h"

#include <iostream>

using namespace std;

namespace tlp {

  GetRequest::~GetRequest(){
    if(requestFinish != 0){
      delete requestFinish;
    }
  }

  void GetRequest::applyRequestFinish() const { 
    bool noRequestFinish = (requestFinish == 0);
    assert(!noRequestFinish);
    (*requestFinish)();
  }

  SOAPRequest::~SOAPRequest(){
    if(respTreatment != 0){
      delete respTreatment;
    }
  }

  void SOAPRequest::applyResponseTreatment(const string &data) const { // Response as returned data, not complete XML. 
    bool noResponseTreatment = (respTreatment == 0);
    if(noResponseTreatment)
      return;
    (*respTreatment)(data);
  }

  DownloadPluginRequest::DownloadPluginRequest(const string &name):name(name) {}
    
  void DownloadPluginRequest::getXml(std::string &xml) const {
    SoapRequestBuilder request;
    request.setFunctionName("downloadPlugin_v2");
    request.addFunctionParameter("pluginName","string",name);
    request.addFunctionParameter("tulipVersion","string",TULIP_RELEASE);
    request.getXML(xml);
  }
    
  void ConnectServerRequest::getXml(std::string &xml) const {
    SoapRequestBuilder request;
    request.setFunctionName("connect_v2");
    request.addFunctionParameter("tulipVersion","string",TULIP_RELEASE);
    request.getXML(xml);
  }
    
  GetServerNameRequest::GetServerNameRequest(ResponseTreatment* resp){
    respTreatment = resp;
  }
    
  void GetServerNameRequest::getXml(std::string &xml) const {
    SoapRequestBuilder request;
    request.setFunctionName("getServerName");   
    request.getXML(xml);
  }
  
  ServerNameTreatment::ServerNameTreatment(std::string addr):addr(addr){
  }
    
  void ServerNameTreatment::operator()(const std::string &data) {
    string str=data;
    int returnPos=str.find("\n");
    if(returnPos!=-1)
      str.erase(returnPos,returnPos+1);
    emit nameReceived(this,addr,str);
  }
       
  GetPluginsListRequest::GetPluginsListRequest(ResponseTreatment* respT){
    respTreatment = respT;
  }
    
  void GetPluginsListRequest::getXml(std::string &xml) const {
    SoapRequestBuilder request;
    request.setFunctionName("getPluginXMLList_v2");
    
    request.getXML(xml);
  }

}
