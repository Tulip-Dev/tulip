/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
#include <QDialog>
#include <QMap>
#include <QByteArray>

namespace Ui {
class GraphPerspectiveLogger;
}

class QShowEvent;
class QHideEvent;

class GraphPerspectiveLogger : public QDialog {
  Q_OBJECT

  QtMsgType _logType;
  Ui::GraphPerspectiveLogger *_ui;
  bool _pythonOutput;
  QMap<int, int> _logCounts;
  QByteArray _windowGeometry;
  bool _anchored;

public:
  enum LogType { Info, Warning, Error, Python };

  GraphPerspectiveLogger(QWidget *parent = nullptr);
  ~GraphPerspectiveLogger() override;
  QPixmap icon(LogType logType) const;
  int count() const;
  int countByType(LogType logType) const;
  GraphPerspectiveLogger::LogType getLastLogType() const;
  void log(QtMsgType, const QMessageLogContext &, const QString &);

  bool eventFilter(QObject *, QEvent *) override;

  void showEvent(QShowEvent *) override;
  void hideEvent(QHideEvent *) override;

  bool anchored() const {
    return _anchored;
  }

  void setGeometry(int x, int y, int w, int h);

public slots:
  void setAnchored(bool anchored);

  void clear();

private slots:
  void copy();
  void showContextMenu(const QPoint &pos);
  void decreaseFontSize();
  void increaseFontSize();

private:
  void logImpl(QtMsgType, const QString &);

signals:
  void cleared();
  void resetLoggerPosition();
};

#endif // GRAPHPERSPECTIVELOGGER_H
