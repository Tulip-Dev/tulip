#include <iostream>

#include <stdlib.h>

#include <QtGui/QApplication>
#include <QtCore/QDir>
#include <QtCore/QProcess>
#include <QtCore/QSettings>

using namespace std;

int main( int argc, char **argv ) {
  QApplication app(argc,argv);

  QDir appDir(QApplication::applicationDirPath());
  QProcess proc;
#if defined(_WIN32)
  QString program = appDir.absoluteFilePath("tulip_app.exe");
#else
  QString program = appDir.absoluteFilePath("tulip_app");
#endif
  QStringList arguments;

  proc.setReadChannelMode(QProcess::ForwardedChannels);
  bool needStart=true;
  while(needStart){
    proc.start(program,arguments);
    proc.waitForFinished(-1);
    QSettings settings("TulipSoftware","Tulip");
    settings.beginGroup("PluginsManager");
    needStart=settings.value("needRestart",false).toBool();
    settings.setValue("needRestart",false);
    settings.endGroup();
  }

  return 0;
}
