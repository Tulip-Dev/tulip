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
#ifndef TULIPPERSPECTIVEPROCESSHANDLER_H
#define TULIPPERSPECTIVEPROCESSHANDLER_H

#include <QObject>
#include <QVariantMap>
#include <QProcess>

#include <QPushButton>

#include <QTcpServer>

// Some utility class used in crash handler dialog designer form

class SelectionButton: public QPushButton {
public:
  explicit SelectionButton(QWidget *parent=NULL);
  void paintEvent(QPaintEvent *e);
};

struct PerspectiveProcessInfos {
  PerspectiveProcessInfos() {}

  PerspectiveProcessInfos(const QString &name, const QVariantMap &args, const QString &file, time_t id):
    name(name), args(args), file(file), _perspectiveId(id) {
  }

  QString name;
  QVariantMap args;
  QString file;
  QString projectPath;
  time_t _perspectiveId;
};

class TulipPerspectiveProcessHandler: public QTcpServer {
  Q_OBJECT

  QMap<QProcess *, PerspectiveProcessInfos> _processInfos;

  static TulipPerspectiveProcessHandler *_instance;
  TulipPerspectiveProcessHandler();

  QThread* _serverThread;

  QProcess *fromId(unsigned int);

public:
  static TulipPerspectiveProcessHandler *instance();

public slots:
  void createPerspective(const QString &perspective, const QString &file, const QVariantMap &parameters);

protected slots:
  void perspectiveCrashed(QProcess::ProcessError);
  void perspectiveFinished(int exitCode, QProcess::ExitStatus exitStatus);

  void acceptConnection();
  void perspectiveReadyRead();

signals:
  void showPluginsAgent();
  void showProjectsAgent();
  void showAboutAgent();
  void showTrayMessage(QString);
  void showErrorMessage(QString,QString);
  void openProject(QString);
  void openProjectWith(QString,QString);
  void openPerspective(QString);

};

#endif // TULIPPERSPECTIVEPROCESSHANDLER_H
