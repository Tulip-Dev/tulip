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
  
  bool SoapResponseReader::getReturnedData(char **data){
    if(!(*data))
      return false;
    if(doc.isNull())
      return false;
    QString result=doc.documentElement().firstChild().firstChild().toElement().text();
    *data=new char[result.length()];
    strcpy(*data,result.toStdString().c_str());
    return true;
  }

}
