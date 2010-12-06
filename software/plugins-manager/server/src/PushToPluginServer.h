#include <iostream>

#include <QtCore/QObject>
#include <QtCore/QCoreApplication>
#include <QtNetwork/QHttp>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

class WebDavManager : public QObject {
  Q_OBJECT
  public: 
    WebDavManager(const QString& host, const QString& url, const QString& base64credentials) : _host(host), _url(url), _credentials("Basic " + base64credentials), _ongoingRequests(0) {
      _credentials = _credentials.replace("\012", "");
    }
    
    void mkdir(const QString& folder) {
      ++_ongoingRequests;
      QUrl url;
      url.setScheme("https");
      url.setHost(_host);
      url.setPort(443);
      url.setPath(_url + folder);
      QNetworkRequest mkcol(url);
      mkcol.setRawHeader(QByteArray("Authorization"), _credentials.toAscii());
//       mkcol.setRawHeader(QByteArray("Host"), _host.toAscii());
      _manager.sendCustomRequest(mkcol, QString("MKCOL").toUtf8());
      connect(&_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished(QNetworkReply*)));
      
    }
    
    void putFile(const QString& folder, QIODevice* data, QVariant mimetype = QVariant("binary/octet-stream")) {
      ++_ongoingRequests;
//       std::cout << "putting file: " << _url.toStdString() + "/" + folder.toStdString() << std::endl; 
      QUrl url;
      url.setScheme("https");
      url.setHost(_host);
      url.setPort(443);
      url.setPath(_url + "/" + folder);
      QNetworkRequest putFile(url);
      putFile.setRawHeader(QByteArray("Authorization"), _credentials.toAscii());
      putFile.setRawHeader(QByteArray("Host"), _host.toUtf8());
      putFile.setHeader(QNetworkRequest::ContentLengthHeader, QVariant(data->size()));
      putFile.setHeader(QNetworkRequest::ContentTypeHeader, mimetype);

      _manager.sendCustomRequest(putFile, QString("PUT").toUtf8(), data);
      connect(&_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished(QNetworkReply*)));
    }
    
    void finish() {
      while(_ongoingRequests > 0) {
        QCoreApplication::processEvents();
      }
    }
    
  public slots:
    void requestFinished(QNetworkReply* reply) {
      --_ongoingRequests;
      if(reply->error() != QNetworkReply::NoError) {
        std::cout << "error: " << reply->errorString().toStdString() << std::endl; 
        std::cout << "reply: " << QString(reply->readAll()).toStdString() << std::endl; 
        std::cout << "request: " << reply->request().url().toString().toStdString() << std::endl;
      }
//       else {
//         std::cout << "success: " << QString(reply->readAll()).toStdString() << std::endl; 
//       }
      reply->deleteLater();
    }
  private:
    QString _host;
    QString _url;
    QString _credentials;
    QNetworkAccessManager _manager;
    uint _ongoingRequests;
};