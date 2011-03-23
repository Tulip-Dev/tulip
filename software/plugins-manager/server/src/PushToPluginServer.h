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
#include <iostream>

#include <QtCore/QObject>
#include <QtCore/QCoreApplication>
#include <QtNetwork/QHttp>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <assert.h>

class WebDavManager : public QObject {
  Q_OBJECT
  public: 
    WebDavManager(const QString& host, const QString& url, const QString& base64credentials) : _host(host), _url(url), _credentials("Basic " + base64credentials), _ongoingRequests(0) {
      _credentials = _credentials.replace("\012", "");
      connect(&_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished(QNetworkReply*)));
    }

    bool folderExists(const QString& folder) {
      _requestSucessFull = true;
      ++_ongoingRequests;
      std::cout << "propfind on " << folder.toStdString() << std::endl;

      _manager.sendCustomRequest(initRequest(folder), QString("PROPFIND").toUtf8());
//       connect(&_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedPropFind(QNetworkReply*)));
      
      while(_ongoingRequests > 0) {
        QCoreApplication::processEvents();
      }
      return _requestSucessFull;
    }
    
    bool mkdir(const QString& folder) {

      if(folderExists(folder)) {
        std::cout << "folder " << folder.toStdString() << " already exists." << std::endl;
        return true;
      }
      
      _requestSucessFull = true;
      ++_ongoingRequests;

      std::cout << "mkcol on " << folder.toStdString() << std::endl;
      _manager.sendCustomRequest(initRequest(folder), QString("MKCOL").toUtf8());
      

      while(_ongoingRequests > 0) {
        QCoreApplication::processEvents();
      }
      return _requestSucessFull;
    }
    
    bool putFile(const QString& folder, QIODevice* data) {
      _requestSucessFull = true;
      ++_ongoingRequests;
//       std::cout << "putting file: " << _url.toStdString() + "/" + folder.toStdString() << std::endl; 
      std::cout << "put on " << folder.toStdString() << std::endl;
      _manager.sendCustomRequest(initRequest(folder), QString("PUT").toUtf8(), data);
//       connect(&_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished(QNetworkReply*)));
      while(_ongoingRequests > 0) {
        QCoreApplication::processEvents();
      }

      return _requestSucessFull;
    }

    QUrl initUrl(const QString& dest) {
      QUrl url;
      url.setScheme("https");
      url.setHost(_host);
      url.setPort(443);
      url.setPath(_url + "/" + dest);

      if(!url.isValid()) {
        std::cout << "URL not valid: " << url.toString().toStdString() << std::endl;
        assert(false);
      }
      
      return url;
    }

    QNetworkRequest initRequest(const QString & destination, QIODevice* data = NULL, QVariant mimetype = QVariant("binary/octet-stream")) {
      QNetworkRequest request(initUrl(destination));
      request.setRawHeader(QByteArray("Authorization"), _credentials.toAscii());
      request.setRawHeader(QByteArray("Host"), _host.toUtf8());
      if(data != NULL) {
        request.setHeader(QNetworkRequest::ContentLengthHeader, QVariant(data->size()));
      }
      request.setHeader(QNetworkRequest::ContentTypeHeader, mimetype);
      return request;
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
        _requestSucessFull = false;
      }
      if(!QString(reply->readAll()).contains("200")) {
        _requestSucessFull = false;
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
    bool _requestSucessFull;
};