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
#ifndef FORMPOST_H
#define FORMPOST_H

#include <QObject>
#include <QStringList>

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
