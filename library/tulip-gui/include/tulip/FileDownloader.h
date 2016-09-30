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
}

#endif // FILEDOWNLOADER_H
