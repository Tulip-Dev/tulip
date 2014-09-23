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

#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QStringList>
#include <QTimer>
#include <QUrl>

#include <tulip/DownloadManager.h>

#include <cstdio>

DownloadManager* DownloadManager::_instance = NULL;

DownloadManager* DownloadManager::getInstance() {
  if(!_instance) {
    _instance = new DownloadManager();
  }

  return _instance;
}

DownloadManager::DownloadManager()  {
  connect(this, SIGNAL(finished(QNetworkReply*)), SLOT(downloadFinished(QNetworkReply*)));
}

QNetworkReply* DownloadManager::downloadPlugin(const QUrl &url, const QString& destination) {
  downloadDestinations[url] = destination;

  QNetworkRequest request(url);
  QNetworkReply* reply = get(request);
  currentDownloads.append(reply);
  return reply;
}

bool DownloadManager::saveToDisk(const QString &filename, QIODevice *data) {
  QFile file(filename);

  if (!file.open(QIODevice::WriteOnly)) {
    fprintf(stderr, "Could not open %s for writing: %s\n", qPrintable(filename), qPrintable(file.errorString()));
    return false;
  }

  file.write(data->readAll());
  file.close();

  return true;
}

void DownloadManager::downloadFinished(QNetworkReply *reply) {
  QUrl url = reply->url();

  if(currentDownloads.contains(reply)) {

    if (reply->error() == QNetworkReply::NoError) {
      QString filename = downloadDestinations[url];

      if (saveToDisk(filename, reply))
        printf("Download of %s succeeded (saved to %s)\n", url.toEncoded().constData(), qPrintable(filename));
    }
    else {
      fprintf(stderr, "Download of %s failed: %s\n", url.toEncoded().constData(), qPrintable(reply->errorString()));
    }

    currentDownloads.removeAll(reply);
  }

  reply->deleteLater();
}
