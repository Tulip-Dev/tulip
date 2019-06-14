/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
#include "tulip/FileDownloader.h"

#include <QEventLoop>

using namespace tlp;

FileDownloader::FileDownloader() : QObject() {
  connect(&_webCtrl, SIGNAL(finished(QNetworkReply *)), this,
          SLOT(fileDownloaded(QNetworkReply *)));
}

void FileDownloader::fileDownloaded(QNetworkReply *pReply) {
  _downloadedData = pReply->readAll();
  pReply->deleteLater();
  emit downloaded();
}

const QByteArray &FileDownloader::download(const QUrl &url) {
  QNetworkRequest request(url);
  _webCtrl.get(request);
  QEventLoop loop;
  connect(this, SIGNAL(downloaded()), &loop, SLOT(quit()));
  loop.exec();
  return _downloadedData;
}
