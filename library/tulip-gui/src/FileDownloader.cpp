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
