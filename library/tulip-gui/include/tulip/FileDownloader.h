/*
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

///@cond DOXYGEN_HIDDEN

#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <tulip/tulipconf.h>

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

namespace tlp {

/**
 * @brief Helper class to download the content of a file referenced by an url
 **/
class TLP_QT_SCOPE FileDownloader : public QObject {

  Q_OBJECT

public:
  FileDownloader();

  /**
   * Synchronously download the content of a file referenced by an url
   * and returns it as a raw byte array.
   *
   */
  const QByteArray &download(const QUrl &url);

signals:

  void downloaded();

private slots:

  void fileDownloaded(QNetworkReply *pReply);

private:
  QNetworkAccessManager _webCtrl;
  QByteArray _downloadedData;
};
} // namespace tlp

#endif // FILEDOWNLOADER_H

///@endcond
