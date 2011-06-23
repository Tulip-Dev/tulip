#ifndef FORMPOST_H
#define FORMPOST_H

#include <QtCore/QObject>
#include <QtCore/QStringList>

class QNetworkReply;

class FormPost: public QObject {
Q_OBJECT
public:
  FormPost();

  QString userAgent();
  void setUserAgent(QString agent);

  QString referer();
  void setReferer(QString ref);

  QString encoding();
  void setEncoding(QString enc);

  void addField(QString name, QString value);

  void addFile(QString fieldName, QByteArray file, QString name, QString mime);
  void addFile(QString fieldName, QString fileName, QString mime);

  QNetworkReply *postData(QString url);
  QByteArray response();

private:
  QByteArray _data;
  QString _encoding;
  QString _userAgent;
  QString _referer;
  QStringList _fieldNames;
  QStringList _fieldValues;
  QStringList _fileFieldNames;
  QStringList _fileNames;
  QStringList _fileMimes;
  QList<QByteArray> files;
  QByteArray strToEnc(QString s);

private slots:
  void readData(QNetworkReply * r);
};

#endif // FORMPOST_H
