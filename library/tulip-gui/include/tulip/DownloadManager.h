#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QMap>

class QNetworkReply;

class DownloadManager: public QObject {
  Q_OBJECT
  QNetworkAccessManager manager;
  QList<QNetworkReply *> currentDownloads;
  QMap<QUrl, QString> downloadDestinations;

  static DownloadManager* _instance;
  DownloadManager();
public:
  static DownloadManager* getInstance();
  void downloadPlugin(const QUrl &url, const QString& destination);
  bool saveToDisk(const QString &filename, QIODevice *data);

public slots:
  void downloadFinished(QNetworkReply *reply);
};
