/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#include <string>
#include <QtNetwork/qhttp.h>
#include <QtCore/qeventloop.h>
#include <QtCore/qtimer.h>

class HttpContext :public QHttp {
  Q_OBJECT
public:
  bool status;
  int rqid;
  bool processed;
  bool redirected;
  bool isHtml;
  std::string newLocation;

  HttpContext();
  void setTimer(QTimer *timer);

public slots:
  void finished(int id, bool error);
  void headerReceived(const QHttpResponseHeader &);
  void timeout();
};
