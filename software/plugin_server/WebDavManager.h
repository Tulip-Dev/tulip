/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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

#ifndef WEBDAVMANAGER_H
#define WEBDAVMANAGER_H

#include <iostream>

#include <QObject>
#include <QCoreApplication>
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QHttp>
#endif
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class QDomDocument;
/**
 * @brief This class uploads files to a server using the WEBDAV protocol.
 * It is tailored for a very specific use and only supports authentified (https) mode.
 **/
class WebDavManager : public QObject {
  Q_OBJECT
public:
  /**
   * @brief Creates a new manager to create folders and upload files on a remote webdav server.
   * All operations take the url as base for where the operations take place, e.g. calling mkdir("foo") will create
   * https://[host]/[url]/foo
   *
   * An easy way to encode your credentials in base64 is to fire up a python interpreter and type:
   * import base64
   * base64.b64encode("username:password")
   *
   * @param host The host of the webdav server (e.g. webdav.labri.fr)
   * @param url The base URL where to upload the files (e.g. perso/huet)
   * @param base64credentials The base64-encoded credentials : base64(username:password)
   **/
  WebDavManager(const QString& host, const QString& url, const QString& base64credentials);

  /**
   * @brief Checks whether a folder already exists.
   *
   * @param folder The folder to check for presence on the remote server.
   * @return bool Whether the folder exists.
   **/
  bool folderExists(const QString& folder);

  /**
   * @brief Creates a folder on the remote server.
   *
   * @param folder The name of the folder to create.
   * @return bool Whether the operation was sucessfull.
   **/
  bool mkdir(const QString& folder);

  /**
   * @brief Uploads a file to the server.
   *
   * @param filename The full name of the file to create.
   * @param data The contents of the file to upload.
   * @return bool Whether the operation was successfull.
   **/
  bool putFile(const QString& filename, QIODevice* data);

  QDomDocument* getRemoteDescription();

  /**
   * @brief Ensure that all operations are finished.
   *
   * @return void
   **/
  void finish();

public slots:
  void requestFinished(QNetworkReply* reply);

private:

  QUrl initUrl(const QString& dest);

  QNetworkRequest initRequest(const QString & destination, QIODevice* data = NULL, QVariant mimetype = QVariant("binary/octet-stream"));

  QString _host;
  QString _url;
  QString _credentials;
  QNetworkAccessManager _manager;
  uint _ongoingRequests;
  bool _requestSucessFull;
  bool _displayErrors;
};

#endif //WEBDAVMANAGER_H
