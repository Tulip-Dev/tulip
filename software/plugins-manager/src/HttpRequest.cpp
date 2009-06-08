#include "HttpRequest.h"
#include "SoapResponseReader.h"

#include <iostream>
#include <QtCore/QUrl>
#include <QtNetwork/QNetworkProxy>
#include <QtCore/QSettings>

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
    QNetworkProxy proxy=getProxy();
    http.setProxy(proxy.hostName(), proxy.port(), proxy.user(), proxy.password());

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

  QNetworkProxy HttpRequest::getProxy(){
    QSettings settings("TulipSoftware","Tulip");
    settings.beginGroup("PluginsManager");
    bool proxyEnable=settings.value("proxyEnable",false).toBool();
    QString proxyAddress=settings.value("proxyAddress","").toString();
    quint16 proxyPort=settings.value("proxyPort",0).toUInt();
    QString proxyUsername=settings.value("proxyUsername","").toString();
    QString proxyPassword=settings.value("proxyPassword","").toString();
    settings.endGroup();

    if(proxyEnable){
      return QNetworkProxy(QNetworkProxy::Socks5Proxy,proxyAddress,proxyPort,proxyUsername,proxyPassword);
    }else{
      return QNetworkProxy();
    }
  }

}
