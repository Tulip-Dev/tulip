#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QMap>

class QNetworkReply;
class QNetworkReply;

class DownloadManager: public QNetworkAccessManager {
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
