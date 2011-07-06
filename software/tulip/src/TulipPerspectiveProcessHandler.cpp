#include "TulipPerspectiveProcessHandler.h"

#include <QtCore/QDir>
#include <QtGui/QApplication>
#include <QtCore/QDebug>
#include <QtGui/QFileDialog>
#include <QtGui/QPainter>
#include <tulip/TulipProject.h>
#include <tulip/CrashHandling.h>
#include "TulipPerspectiveCrashHandler.h"

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
  process->setReadChannel(QProcess::StandardOutput);
  process->setReadChannelMode(QProcess::ForwardedChannels);
  process->setReadChannel(QProcess::StandardError);
  process->setProcessChannelMode(QProcess::SeparateChannels);
  process->start(appDir.absoluteFilePath("tulip_perspective"),args);
  _processInfos[process] = PerspectiveProcessInfos(perspective,parameters,file);
}

void TulipPerspectiveProcessHandler::perspectiveCrashed(QProcess::ProcessError e) {
  QProcess *process = static_cast<QProcess *>(sender());
  process->setReadChannel(QProcess::StandardError);
  PerspectiveProcessInfos infos = _processInfos[process];

  TulipPerspectiveCrashHandler crashHandler;

#ifdef USE_GOOGLE_BREAKPAD
  QRegExp plateform("^" + QString(BREAKPAD_PLATEFORM_HEADER) + " (.*)\n"),
          arch("^" + QString(BREAKPAD_ARCH_HEADER) + " (.*)\n"),
          compiler("^" + QString(BREAKPAD_COMPILER_HEADER) + " (.*)\n"),
          version("^" + QString(BREAKPAD_VERSION_HEADER) + " (.*)\n"),
          dump("^" + QString(BREAKPAD_DUMP_HEADER) + " (.*)\n");

  QMap<QRegExp *,QString> envInfos;
  envInfos[&plateform] = "";
  envInfos[&arch] = "";
  envInfos[&compiler] = "";
  envInfos[&version] = "";
  envInfos[&dump] = "";

  while (!process->atEnd()) {
    QString line(process->readLine());
    QRegExp *re;
    foreach(re,envInfos.keys()) {
      if (re->exactMatch(line)) {
        envInfos[re] = re->cap(1);
        break;
      }
    }
  }

  crashHandler.setEnvData(envInfos[&plateform],envInfos[&arch],envInfos[&compiler],envInfos[&version],envInfos[&dump]);
#endif
  crashHandler.setPerspectiveData(infos);
  crashHandler.exec();

}

void TulipPerspectiveProcessHandler::perspectiveFinished(int exitCode, QProcess::ExitStatus exitStatus) {
  QProcess *process = static_cast<QProcess *>(sender());
  delete process;
  _processInfos.remove(process);
}

void TulipPerspectiveProcessHandler::enableCrashHandling(qlonglong perspectivePid, const QString &perspectiveProjectPath) {
  QProcess *p;
  foreach(p,_processInfos.keys()) {
#ifdef _WIN32

    if (p->pid()->dwProcessId == perspectivePid) {
#else

    if (p->pid() == perspectivePid) {
#endif
      PerspectiveProcessInfos infos = _processInfos[p];
      infos.projectPath = perspectiveProjectPath;
      _processInfos[p] = infos;
    }
  }
}
