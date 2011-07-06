#include "FormPost.h"

#include <QtCore/QFile>
#include <QtCore/QDateTime>
#include <QtCore/QVariant>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

FormPost::FormPost():
  QObject() {
  _userAgent="";
  _userAgent="utf-8";
  _referer="";
}

QString FormPost::userAgent() {
  return _userAgent;
}

void FormPost::setUserAgent(QString agent) {
  _userAgent=agent;
}

QString FormPost::referer() {
  return _referer;
}

void FormPost::setReferer(QString ref) {
  _referer=ref;
}

QString FormPost::encoding() {
  return _encoding;
}

void FormPost::setEncoding(QString enc) {
  if (enc=="utf-8" || enc=="ascii") {
    _encoding=enc;
  }
}

QByteArray FormPost::strToEnc(QString s) {
  if (_encoding=="utf-8") {
    return s.toUtf8();
  }
  else {
    return s.toAscii();
  }
}

void FormPost::addField(QString name, QString value) {
  _fieldNames.append(name);
  _fieldValues.append(value);
}

void FormPost::addFile(QString fieldName, QByteArray file, QString name, QString mime) {
  files.append(file);
  _fileFieldNames.append(fieldName);
  _fileNames.append(name);
  _fileMimes.append(mime);
}

void FormPost::addFile(QString fieldName, QString fileName, QString mime) {
  QFile f(fileName);
  f.open(QIODevice::ReadOnly);
  QByteArray file=f.readAll();
  f.close();
  QString name;

  if (fileName.contains("/")) {
    int pos=fileName.lastIndexOf("/");
    name=fileName.right(fileName.length()-pos-1);
  }
  else if (fileName.contains("\\")) {
    int pos=fileName.lastIndexOf("\\");
    name=fileName.right(fileName.length()-pos-1);
  }
  else {
    name=fileName;
  }

  addFile(fieldName,file,name,mime);
}

QNetworkReply * FormPost::postData(QString url) {
  QString host;
  host=url.right(url.length()-url.indexOf("://")-3);
  host=host.left(host.indexOf("/"));
  QString crlf="\r\n";
  qsrand(QDateTime::currentDateTime().toTime_t());
  QString b=QVariant(qrand()).toString()+QVariant(qrand()).toString()+QVariant(qrand()).toString();
  QString boundary="---------------------------"+b;
  QString endBoundary=crlf+"--"+boundary+"--"+crlf;
  QString contentType="multipart/form-data; boundary="+boundary;
  boundary="--"+boundary+crlf;
  QByteArray bond=boundary.toAscii();
  QByteArray send;
  bool first=true;

  for (int i=0; i<_fieldNames.size(); i++) {
    send.append(bond);

    if (first) {
      boundary=crlf+boundary;
      bond=boundary.toAscii();
      first=false;
    }

    send.append(QString("Content-Disposition: form-data; name=\""+_fieldNames.at(i)+"\""+crlf).toAscii());

    if (_encoding=="utf-8") send.append(QString("Content-Transfer-Encoding: 8bit"+crlf).toAscii());

    send.append(crlf.toAscii());
    send.append(strToEnc(_fieldValues.at(i)));
  }

  for (int i=0; i<files.size(); i++) {
    send.append(bond);
    send.append(QString("Content-Disposition: form-data; name=\""+_fileFieldNames.at(i)+"\"; filename=\""+_fileNames.at(i)+"\""+crlf).toAscii());
    send.append(QString("Content-Type: "+_fileMimes.at(i)+crlf+crlf).toAscii());
    send.append(files.at(i));
  }

  send.append(endBoundary.toAscii());

  _fieldNames.clear();
  _fieldValues.clear();
  _fileFieldNames.clear();
  _fileNames.clear();
  _fileMimes.clear();
  files.clear();

  QNetworkAccessManager * http=new QNetworkAccessManager(this);
  connect(http,SIGNAL(finished(QNetworkReply *)),this,SLOT(readData(QNetworkReply *)));
  QNetworkRequest request;
  request.setRawHeader("Host", host.toAscii());

  if (_userAgent!="") request.setRawHeader("User-Agent", _userAgent.toAscii());

  if (_referer!="") request.setRawHeader("Referer", _referer.toAscii());

  request.setHeader(QNetworkRequest::ContentTypeHeader, contentType.toAscii());
  request.setHeader(QNetworkRequest::ContentLengthHeader, QVariant(send.size()).toString());
  request.setUrl(QUrl(url));
  QNetworkReply * reply=http->post(request,send);
  return reply;
}

void FormPost::readData(QNetworkReply * r) {
  _data=r->readAll();
}

QByteArray FormPost::response() {
  return _data;
}
