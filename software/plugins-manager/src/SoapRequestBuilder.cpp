#include "SoapRequestBuilder.h"

namespace tlp {

  SoapRequestBuilder::SoapRequestBuilder(){
    isFunctionNameSpecified = false;
    
    //xmlDoc.appendChild(xmlDoc.createElement("?xml version=\"1.0\" encoding=\"ISO-8859-1\"?"));
    
    // Soap enveloppe
    soapEnvelopeNode = xmlDoc.createElement("SOAP-ENV:Envelope");
    soapEnvelopeNode.setAttribute("SOAP-ENV:encodingStyle", "http://schemas.xmlsoap.org/soap/encoding/");
    soapEnvelopeNode.setAttribute("xmlns:SOAP-ENV", "http://schemas.xmlsoap.org/soap/envelope/");
    soapEnvelopeNode.setAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema");
    soapEnvelopeNode.setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
    soapEnvelopeNode.setAttribute("xmlns:SOAP-ENC", "http://schemas.xmlsoap.org/soap/encoding/");
    
    // body
    soapBodyNode = xmlDoc.createElement("SOAP-ENV:Body"); 
    
    // Encoding
    QDomNode encNode = xmlDoc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"ISO-8859-1\"");
    xmlDoc.insertBefore(encNode,xmlDoc.firstChild());
  }
  
  void 
  SoapRequestBuilder::setFunctionName(const std::string &name){
    
    // :TODO: find ou about this ns
    
    soapFunctionNode = xmlDoc.createElement("ns1:"+QString(name.c_str()));
    soapFunctionNode.setAttribute("xmlns:ns1","http://tulip-plugin-ws.sourceforge.net/tulip-plugin-ws.php");
    isFunctionNameSpecified = true;
  }
  
  bool 
  SoapRequestBuilder::addFunctionParameter(const std::string &name,const std::string &type,const std::string &data){
    
    if(!isFunctionNameSpecified) return false;
    QDomElement Node = xmlDoc.createElement(name.c_str());
    Node.setAttribute("xsi:type", "xsd:"+QString(type.c_str()));
    Node.appendChild(xmlDoc.createTextNode(data.c_str()));
    soapFunctionNode.appendChild(Node);  
    return true;
  }
  
  void SoapRequestBuilder::getXML(std::string &result){
    soapBodyNode.appendChild(soapFunctionNode);
    soapEnvelopeNode.appendChild(soapBodyNode);
    xmlDoc.appendChild(soapEnvelopeNode);
    result=xmlDoc.toString().toStdString();
  }

}
