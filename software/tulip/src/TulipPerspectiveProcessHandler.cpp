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
#include "TulipPerspectiveProcessHandler.h"

#include <QDir>
#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QPainter>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QStandardPaths>
#else
#include <QDesktopServices>
#endif
#include <QTcpSocket>
#include <tulip/TulipProject.h>

#include <ctime>
#include <iostream>
#include <CrashHandling.h>

#include "TulipPerspectiveCrashHandler.h"
#include "TulipMainWindow.h"

#ifdef _WIN32
#include <windows.h>
#endif

SelectionButton::SelectionButton(QWidget *parent): QPushButton(parent) {}
void SelectionButton::paintEvent(QPaintEvent *e) {
  QPushButton::paintEvent(e);
  QPainter p(this);
  QPixmap pixmap(":/tulip/app/ui/list_bullet_arrow.png");
  p.drawPixmap(10,height()/2-pixmap.height()/2,pixmap);
}

TulipPerspectiveProcessHandler *TulipPerspectiveProcessHandler::_instance = 0;

TulipPerspectiveProcessHandler::TulipPerspectiveProcessHandler() {
  listen(QHostAddress::LocalHost);
  connect(this,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
  QFile f(QDir(QStandardPaths::standardLocations(QStandardPaths::TempLocation).at(0)).filePath("tulip.lck"));
#else
  QFile f(QDir(QDesktopServices::storageLocation(QDesktopServices::TempLocation)).filePath("tulip.lck"));
#endif
  f.open(QIODevice::WriteOnly);
  f.write(QString::number(serverPort()).toUtf8());
  f.flush();
  f.close();
}

QProcess *TulipPerspectiveProcessHandler::fromId(unsigned int id) {
  foreach(QProcess* k, _processInfos.keys()) {
    if (_processInfos[k]._perspectiveId == id)
      return k;
  }
  return NULL;
}

TulipPerspectiveProcessHandler *TulipPerspectiveProcessHandler::instance() {
  if (!_instance) {
    _instance = new TulipPerspectiveProcessHandler;
  }

  return _instance;
}

void TulipPerspectiveProcessHandler::createPerspective(const QString &perspective, const QString &file, const QVariantMap &parameters) {
  QStringList args;

  if (!perspective.isEmpty())
    args << "--perspective=" + perspective;

  if (!file.isEmpty())
    args << file;

  QString k;
  foreach(k,parameters.keys())
  args << "--" + k + "=" + parameters[k].toString();

  args << "--port=" + QString::number(serverPort());
  time_t perspectiveId = time(NULL);
  args << "--id=" + QString::number(perspectiveId);

  QDir appDir(QApplication::applicationDirPath());

  QProcess *process = new QProcess;
#ifdef WIN32
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("STDERR_NO_ANSI_ESCAPES", "1");
  process->setProcessEnvironment(env);
#endif
  connect(process,SIGNAL(error(QProcess::ProcessError)),this,SLOT(perspectiveCrashed(QProcess::ProcessError)));
  connect(process,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(perspectiveFinished(int,QProcess::ExitStatus)));
  process->setProcessChannelMode(QProcess::ForwardedChannels);
  process->start(appDir.absoluteFilePath("tulip_perspective"),args);
  _processInfos[process] = PerspectiveProcessInfos(perspective,parameters,file,perspectiveId);
}

void TulipPerspectiveProcessHandler::perspectiveCrashed(QProcess::ProcessError) {
  QProcess *process = static_cast<QProcess *>(sender());
  process->setReadChannel(QProcess::StandardError);
  PerspectiveProcessInfos infos = _processInfos[process];

  TulipPerspectiveCrashHandler crashHandler;

  QRegExp plateform("^" + QString(TLP_PLATEFORM_HEADER) + " (.*)\n"),
          arch("^" + QString(TLP_ARCH_HEADER) + " (.*)\n"),
          compiler("^" + QString(TLP_COMPILER_HEADER) + " (.*)\n"),
          version("^" + QString(TLP_VERSION_HEADER) + " (.*)\n");

  // TODO: replace reading process by reading file
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
  QFile f(QDir(QStandardPaths::standardLocations(QStandardPaths::TempLocation).at(0)).filePath("tulip_perspective-" + QString::number(infos._perspectiveId) + ".log"));
#else
  QFile f(QDir(QDesktopServices::storageLocation(QDesktopServices::TempLocation)).filePath("tulip_perspective-" + QString::number(infos._perspectiveId) + ".log"));
#endif
  f.open(QIODevice::ReadOnly);

  QMap<QRegExp *,QString> envInfos;
  envInfos[&plateform] = "";
  envInfos[&arch] = "";
  envInfos[&compiler] = "";
  envInfos[&version] = "";

  QString stackTrace;
  bool grabStackTrace = false;

  while (!f.atEnd()) {
    QString line(f.readLine());

    if (line.startsWith(TLP_STACK_BEGIN_HEADER)) {
      grabStackTrace = true;
      continue;
    }
    else if (line.startsWith(TLP_STACK_END_HEADER)) {
      grabStackTrace = false;
      continue;
    }

    if (grabStackTrace) {
      stackTrace += line;
    }
    else {
      QRegExp *re;
      foreach(re,envInfos.keys()) {
        if (re->exactMatch(line)) {
          envInfos[re] = re->cap(1);
          break;
        }
      }
    }
  }

  f.close();
  f.remove();

  // it may happens that there is nothing to show
  // (perhaps if our signal handler was not called)
  if (stackTrace.isEmpty())
    return;

  crashHandler.setEnvData(envInfos[&plateform],envInfos[&arch],envInfos[&compiler],envInfos[&version],stackTrace);
  crashHandler.setPerspectiveData(infos);
  crashHandler.exec();

}

void TulipPerspectiveProcessHandler::perspectiveFinished(int, QProcess::ExitStatus) {
  QProcess *process = static_cast<QProcess *>(sender());
  delete process;
  _processInfos.remove(process);
}

void TulipPerspectiveProcessHandler::acceptConnection() {
  QTcpSocket* socket = nextPendingConnection();
  connect(socket,SIGNAL(readyRead()),this,SLOT(perspectiveReadyRead()));
  connect(socket,SIGNAL(disconnected()),socket,SLOT(deleteLater()));
}

void TulipPerspectiveProcessHandler::perspectiveReadyRead() {
  QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
  QString data(QString::fromUtf8(socket->readAll()));
  QStringList tokens = data.split("\t");
  QString args = QString(data).remove(0,tokens[0].length()+1); // arguments except first one
  QString args2 = QString(args).remove(0,tokens[1].length()+1); // arguments except two firsts

  if (tokens[0] == "SHOW_AGENT") {
    if (tokens[1] == "PLUGINS")
      emit showPluginsAgent();
    else if (tokens[1] == "PROJECTS")
      emit showProjectsAgent();
    else if (tokens[1] == "ABOUT")
      emit showAboutAgent();
  }
  else if (tokens[0] == "ERROR_MESSAGE")
    emit showErrorMessage(tokens[1],args2);
  else if (tokens[0] == "TRAY_MESSAGE")
    emit showTrayMessage(args);
  else if (tokens[0] == "OPEN_PROJECT") {
    emit openProject(args);
  }
  else if (tokens[0] == "OPEN_PROJECT_WITH") {
    emit openProjectWith(args2,tokens[1]);
  }
  else if (tokens[0] == "CREATE_PERSPECTIVE")
    emit openPerspective(args);
  else if (tokens[0] == "PROJECT_LOCATION") {
    QProcess* p = fromId(tokens[1].toUInt());

    if (p != NULL) {
      PerspectiveProcessInfos infos = _processInfos[p];
      infos.projectPath = args2;
      _processInfos[p] = infos;
    }
  }
}
