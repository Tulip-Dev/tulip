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

#ifndef GRAPHPERSPECTIVELOGGER_H
#define GRAPHPERSPECTIVELOGGER_H

#include <QPixmap>
#include <QDebug>
#include <QFrame>
#include <QMap>

namespace Ui {
class GraphPerspectiveLogger;
}

class GraphPerspectiveLogger: public QFrame {
  Q_OBJECT

  QtMsgType _logType;
  Ui::GraphPerspectiveLogger* _ui;
  bool _pythonOutput;
  QMap<int, int> _logCounts;

public:

  enum LogType {
    Info,
    Warning,
    Error,
    Python
  };

  GraphPerspectiveLogger(QWidget* parent = NULL);
  ~GraphPerspectiveLogger();
  QPixmap icon(LogType logType) const;
  int count() const;
  int countByType(LogType logType) const;
  GraphPerspectiveLogger::LogType getLastLogType() const;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
  void log(QtMsgType, const QMessageLogContext &, const QString &);
#else
  void log(QtMsgType, const char*);
#endif

  bool eventFilter(QObject *, QEvent *);

public slots:
  void clear();

private slots:
  void copy();
  void showContextMenu(const QPoint&pos);

private:
  void logImpl(QtMsgType, const QString &);

signals:
  void cleared();
};

#endif // GRAPHPERSPECTIVELOGGER_H
