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
#include <QtCore/QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkProxy>
#include <QtNetwork/QNetworkReply>

#include <tulip/TlpNetworkAccess.h>
#include "HttpRequest.h"
#include "SoapResponseReader.h"


using namespace std;

namespace tlp {

HttpRequest::HttpRequest(const string &serverName,QObject *parent) : QObject(parent),outFile(NULL),server(serverName),mainWindow(parent) {
}

void HttpRequest::request(const string &msg) {
  QNetworkRequest request(QUrl(server.c_str()));
  request.setHeader(QNetworkRequest::ContentTypeHeader,
                    "text/xml; charset=ISO-8859-1");
  request.setRawHeader("User-Agent", "Tulip Agent");
  QByteArray bamsg(msg.c_str());
  request.setHeader(QNetworkRequest::ContentLengthHeader, bamsg.length());
  QNetworkReply* reply = getNetworkAccessManager()->post(request, bamsg);
  connect(reply, SIGNAL(finished()), this, SLOT(finished()));
}

void HttpRequest::get(const string &getFileName, const string &outFileName) {
  QNetworkRequest request(QUrl(QString(server.c_str())+getFileName.c_str()));
  request.setHeader(QNetworkRequest::ContentTypeHeader,
                    "text/xml; charset=ISO-8859-1");
  request.setRawHeader("User-Agent", "Tulip Agent");
  outFile=new QFile(outFileName.c_str());
  outFile->open(QIODevice::ReadWrite);
  QNetworkReply* reply = getNetworkAccessManager()->get(request);
  connect(reply, SIGNAL(finished()), this, SLOT(finished()));
}

void HttpRequest::finished() {
  QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

  if (reply) {
    if (reply->error() == QNetworkReply::NoError) {
      if (outFile) {
        outFile->write(reply->readAll());
        outFile->close();
        delete outFile;
        outFile=NULL;
      }
      else {
        result = (string)reply->readAll();
      }
    }

    reply->close();
    reply->deleteLater();
  }

  emit done();
}

void HttpRequest::getResponse( std::string &response) {
  response=result;
}

void HttpRequest::getServerName(std::string &serverName) {
  serverName=server;
}

}
