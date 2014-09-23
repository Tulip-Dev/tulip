/*
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
///@cond DOXYGEN_HIDDEN

#include <QNetworkAccessManager>
#include <QUrl>
#include <QMap>

#include <tulip/tulipconf.h>

class QNetworkReply;

class TLP_QT_SCOPE DownloadManager: public QNetworkAccessManager {
  Q_OBJECT
  QList<QNetworkReply *> currentDownloads;
  QMap<QUrl, QString> downloadDestinations;

  static DownloadManager* _instance;
  DownloadManager();
public:
  static DownloadManager* getInstance();
  QNetworkReply* downloadPlugin(const QUrl &url, const QString& destination);
  bool saveToDisk(const QString &filename, QIODevice *data);

public slots:
  void downloadFinished(QNetworkReply *reply);
};
///@endcond
