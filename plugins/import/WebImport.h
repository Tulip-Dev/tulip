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

#include <QNetworkReply>
#include <QEventLoop>
#include <QTimer>

#include <string>

class HttpContext :public QObject {
  Q_OBJECT
public:
  bool status;
  int code;
  QNetworkReply* reply;
  bool processed;
  bool redirected;
  bool isHtml;
  std::string data;

  HttpContext();
  ~HttpContext();
  void setTimer(QTimer *timer);
  void request(const std::string& url, bool header = false);

public slots:
  void finished();
  void headerReceived();
  void timeout();
};
