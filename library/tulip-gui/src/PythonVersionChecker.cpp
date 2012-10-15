#include <tulip/PythonVersionChecker.h>

#include <QtCore/QProcess>

using namespace tlp;

QString PythonVersionChecker::installedVersion() {
  QProcess pythonCheck;
  pythonCheck.start("python",QStringList() << "--version");
  pythonCheck.waitForFinished(-1);

  if (pythonCheck.exitStatus() == QProcess::NormalExit) {
    pythonCheck.setReadChannel(QProcess::StandardError);
    QString result = pythonCheck.readAll();
    QRegExp versionRegexp(".*([0-9]*\\.[0-9]*)\\..*");
    if (versionRegexp.exactMatch(result)) {
      QString installedVersion = versionRegexp.cap(1);
      return installedVersion;
    }
    else {
      qWarning() << QObject::trUtf8("Error parsing python version");
    }
  }
  else {
    qWarning() << QObject::trUtf8("Python installation not found");
  }

  return QString::null;
}

QString PythonVersionChecker::compiledVersion() {
  return TLP_PYTHON;
}
