#include "WebDavManager.h"
#include <QDomDocument>

WebDavManager::WebDavManager(const QString& host, const QString& url, const QString& base64credentials) : _host(host), _url(url), _credentials("Basic " + base64credentials), _ongoingRequests(0), _displayErrors(true) {
  _credentials = _credentials.replace("\012", "");
  connect(&_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished(QNetworkReply*)));
}

bool WebDavManager::folderExists(const QString& folder) {
  _displayErrors = false;
  _requestSucessFull = true;
  ++_ongoingRequests;

  _manager.sendCustomRequest(initRequest(folder), QString("PROPFIND").toUtf8());
  
  while(_ongoingRequests > 0) {
    QCoreApplication::processEvents();
  }
  
  std::cout << "folder " << folder.toStdString() << (_requestSucessFull ? "exists" : "does not exists") << std::endl;
  _displayErrors = true;
  return _requestSucessFull;
}

bool WebDavManager::mkdir(const QString& folder) {
  
  if(folderExists(folder)) {
    return true;
  }
  
  _requestSucessFull = true;
  ++_ongoingRequests;
  
  std::cout << "creating fodler " << folder.toStdString() << std::endl;
  _manager.sendCustomRequest(initRequest(folder), QString("MKCOL").toUtf8());
  
  
  while(_ongoingRequests > 0) {
    QCoreApplication::processEvents();
  }
  return _requestSucessFull;
}

bool WebDavManager::putFile(const QString& filename, QIODevice* data) {
  _requestSucessFull = true;
  ++_ongoingRequests;
  std::cout << "uploading " << filename.toStdString() << std::endl;
  _manager.sendCustomRequest(initRequest(filename), QString("PUT").toUtf8(), data);
  while(_ongoingRequests > 0) {
    QCoreApplication::processEvents();
  }
  
  return _requestSucessFull;
}

void WebDavManager::finish() {
  while(_ongoingRequests > 0) {
    QCoreApplication::processEvents();
  }
}

void WebDavManager::requestFinished(QNetworkReply* reply) {
  --_ongoingRequests;
  if(reply->error() != QNetworkReply::NoError) {
    if(_displayErrors) {
      std::cout << "error: " << reply->errorString().toStdString() << std::endl;
      std::cout << "reply: " << QString(reply->readAll()).toStdString() << std::endl;
      std::cout << "request: " << reply->request().url().toString().toStdString() << std::endl;
    }
    _requestSucessFull = false;
  }
  
  if(!QString(reply->readAll()).contains("200")) {
    _requestSucessFull = false;
  }
  reply->deleteLater();
}

QUrl WebDavManager::initUrl(const QString& dest) {
  QUrl url("https://" + _host + ":" + "443" + "/" + _url + "/" + dest);
  if(!url.isValid()) {
    std::cout << "URL not valid: " << url.toString().toStdString() << std::endl;
    assert(false);
  }
  
  return url;
}

QNetworkRequest WebDavManager::initRequest(const QString& destination, QIODevice* data, QVariant mimetype) {
  QNetworkRequest request(initUrl(destination));
  request.setRawHeader(QByteArray("Authorization"), _credentials.toAscii());
  request.setRawHeader(QByteArray("Host"), _host.toUtf8());
  if(data != NULL) {
    request.setHeader(QNetworkRequest::ContentLengthHeader, QVariant(data->size()));
  }
  request.setHeader(QNetworkRequest::ContentTypeHeader, mimetype);
  return request;
}

QDomDocument* WebDavManager::getRemoteDescription() {
  QNetworkRequest request = initRequest("archive/serverDescription.xml");

  QNetworkAccessManager manager;
  QNetworkReply* reply = manager.get(request);
  
  QDomDocument* currentServerDescription = new QDomDocument();
  QString errorMsg;
  bool result = currentServerDescription->setContent(reply->readAll(), &errorMsg);
  std::cout << result << ":"<< errorMsg.toStdString() << std::endl;

  reply->deleteLater();
  
  return currentServerDescription;
}
