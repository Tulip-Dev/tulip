#include "HttpRequest.h"
#include "SoapResponseReader.h"

#include <iostream>

using namespace std;

namespace tlp {

  HttpRequest::HttpRequest(const string &serverName,QObject *parent) : QObject(parent),outFile(NULL),server(serverName),mainWindow(parent){
    connect(&http, SIGNAL(done(bool)), this, SLOT(requestDone(bool)));
  }
  
  void HttpRequest::request(const string &msg){
    
    QUrl url(server.c_str());
    
    QHttpRequestHeader header;
    header.setContentType("text/xml; charset=ISO-8859-1");
    header.setRequest( "POST", url.path() );
    header.setValue( "Host", url.host() ); 
    
    http.setHost( url.host() );
    
    QByteArray bamsg(msg.c_str());
    http.request( header, bamsg );
  }

  void HttpRequest::get(const string &getFileName, const string &outFileName) {

    QUrl url(QString(server.c_str())+getFileName.c_str());

    QHttpRequestHeader header;
    header.setContentType("text/xml; charset=ISO-8859-1");
    header.setRequest( "GET", url.path() );
    header.setValue( "Host", url.host() ); 
    
    http.setHost( url.host() );
    
    outFile=new QFile(outFileName.c_str());
    outFile->open(QIODevice::ReadWrite);
    http.request( header, NULL, outFile);
  }
  
  void HttpRequest::requestDone(bool error){
    if(!error){
      if(outFile) {
	outFile->close();
	delete outFile;
	outFile=NULL;
      }else{
	result = (string)http.readAll();
      }
    }
    emit done();
  }
  
  void HttpRequest::getResponse( std::string &response){
    response=result;
  }
  
  void HttpRequest::getServerName(std::string &serverName){
    serverName=server;
  }

}
