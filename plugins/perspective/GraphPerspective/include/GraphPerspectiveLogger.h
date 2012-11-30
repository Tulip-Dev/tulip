/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

#ifndef GRAPHPERSPECTIVELOGGER_H
#define GRAPHPERSPECTIVELOGGER_H

#include <QtGui/QPixmap>
#include <QtCore/QDebug>
#include <QtGui/QFrame>

namespace Ui {
class GraphPerspectiveLogger;
}

class GraphPerspectiveLogger: public QFrame {
  Q_OBJECT

  QtMsgType _logSeverity;
  unsigned int _logCount;

  Ui::GraphPerspectiveLogger* _ui;

  QString iconForType(QtMsgType);
public:
  GraphPerspectiveLogger(QWidget* parent = NULL);
  ~GraphPerspectiveLogger();
  QPixmap icon();
  unsigned int count() const;
  void log(QtMsgType,const char*);

public slots:
  void clear();

signals:
  void cleared();
};

#endif // GRAPHPERSPECTIVELOGGER_H
