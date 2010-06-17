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
#include "SoapResponseReader.h"

#include <cassert>
#include <iostream>

using namespace std;

namespace tlp {

  SoapResponseReader::SoapResponseReader(const string &xml){
    string env;
    extractSoapEnv(xml,env);
    if(env.length()!=0)
      doc.setContent(QString(env.c_str()));
      
    //xml=result.toStdString();
  }
  
  
  void SoapResponseReader::extractSoapEnv(const string &xml,string &result){
    string qXml(xml);
    string startTag ("<SOAP-ENV");
    string endTag ("</SOAP-ENV:Envelope>");

    int startSoapEnv = qXml.find(startTag);
    if(startSoapEnv==-1){
      result="";
      return;
    }
    int endSoapEnv = qXml.find(endTag) + endTag.length();

    result=qXml.substr(startSoapEnv, endSoapEnv - startSoapEnv);
    
  }
  
  bool SoapResponseReader::getFunctionName(string &functionName){
    if(doc.isNull())
      return false;
    string s(doc.documentElement().firstChild().firstChild().toElement().tagName().toAscii());
    functionName=s.substr(0,s.find(':'));
    return true;
  }
  
  bool SoapResponseReader::getReturnedData(string &data){
    if(doc.isNull())
      return false;
    data=doc.documentElement().firstChild().firstChild().toElement().text().toStdString();
    return true;
  }

}
