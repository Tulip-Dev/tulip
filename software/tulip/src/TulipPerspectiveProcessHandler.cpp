#include "TulipPerspectiveProcessHandler.h"

#include <QtCore/QDir>
#include <QtGui/QApplication>
#include <QtCore/QDebug>
#include <QtGui/QFileDialog>
#include <QtGui/QPainter>
#include <tulip/TulipProject.h>

#include "ui_TulipPerspectiveCrashHandler.h"

SelectionButton::SelectionButton(QWidget *parent): QPushButton(parent) {}
void SelectionButton::paintEvent(QPaintEvent *e) {
  QPushButton::paintEvent(e);
  QPainter p(this);
  QPixmap pixmap(":/tulip/app/ui/list_bullet_arrow.png");
  p.drawPixmap(10,height()/2-pixmap.height()/2,pixmap);
}

TulipPerspectiveProcessHandler *TulipPerspectiveProcessHandler::_instance = 0;

TulipPerspectiveProcessHandler::TulipPerspectiveProcessHandler() {
}

TulipPerspectiveProcessHandler &TulipPerspectiveProcessHandler::instance() {
  if (!_instance)
   _instance = new TulipPerspectiveProcessHandler;
  return *_instance;
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

    QDir appDir(QApplication::applicationDirPath());

    QProcess *process = new QProcess;
    connect(process,SIGNAL(error(QProcess::ProcessError)),this,SLOT(perspectiveCrashed(QProcess::ProcessError)));
    connect(process,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(perspectiveFinished(int,QProcess::ExitStatus)));
    process->start(appDir.absoluteFilePath("tulip_perspective"),args);
    _processInfos[process] = PerspectiveProcessInfos(perspective,parameters,file);
  }

void TulipPerspectiveProcessHandler::perspectiveCrashed(QProcess::ProcessError e) {
  QProcess *process = static_cast<QProcess *>(sender());
  PerspectiveProcessInfos infos = _processInfos[process];
  QString error;
  switch(e) {
  case QProcess::FailedToStart:
    error = trUtf8("The process failed to start. Either the invoked program is missing, or you may have insufficient permissions to invoke the program.");
    break;
  case QProcess::Crashed:
    error = trUtf8("The process crashed some time after starting successfully.");
    break;
  default:
    error = trUtf8("An unknown error occurred.");
    break;
  }
}

void TulipPerspectiveProcessHandler::perspectiveFinished(int exitCode, QProcess::ExitStatus exitStatus) {
  QProcess *process = static_cast<QProcess *>(sender());
  delete process;
  _processInfos.remove(process);
}

void TulipPerspectiveProcessHandler::enableCrashHandling(qlonglong perspectivePid, const QString &perspectiveProjectPath) {
  QProcess *p;
  foreach(p,_processInfos.keys()) {
    if (p->pid() == perspectivePid) {
      PerspectiveProcessInfos infos = _processInfos[p];
      infos.projectPath = perspectiveProjectPath;
      _processInfos[p] = infos;
    }
  }
}
