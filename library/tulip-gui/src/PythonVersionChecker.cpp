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

#include <tulip/PythonVersionChecker.h>
#include <tulip/TlpQtTools.h>

#include <QtCore/QProcess>
#include <QtCore/QSettings>

#include <iostream>

using namespace tlp;
using namespace std;

#ifdef WIN32
static const char *pythonVersion[]  = {
  "3.3", "3.2", "3.1", "3.0", "2.7", "2.6", "2.5", 0
};

static bool isPythonVersionInstalled(const QString &pythonVersion) {
  QString win32RegKey = QString("HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\") + pythonVersion + QString("\\InstallPath");
  QString win64RegKey = QString("HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Python\\PythonCore\\") + pythonVersion + QString("\\InstallPath");
  QSettings win32Settings(win32RegKey, QSettings::NativeFormat);
  QSettings win64Settings(win64RegKey, QSettings::NativeFormat);
  return win32Settings.value("Default").toString() != "" || win64Settings.value("Default").toString() != "";
}

static QString getInstalledPythonVersionIfAny() {
  int i = 0;

  while (pythonVersion[i]) {
    if (isPythonVersionInstalled(pythonVersion[i])) {
      return pythonVersion[i];
    }

    ++i;
  }

  return "";
}
#endif


QString PythonVersionChecker::installedVersion() {
#ifndef WIN32
  QProcess pythonCheck;
  pythonCheck.start(QString("python")+compiledVersion(),QStringList() << "--version");
  pythonCheck.waitForFinished(-1);

  if (pythonCheck.exitStatus() != QProcess::NormalExit) {
    pythonCheck.start(QString("python"),QStringList() << "--version");
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
  }
  else {
    return compiledVersion();
  }

#else

  if (isPythonVersionInstalled(compiledVersion())) {
    return compiledVersion();
  }

  QString installedPythonVersion = getInstalledPythonVersionIfAny();

  if (!installedPythonVersion.isEmpty()) {
    return installedPythonVersion;
  }

#endif

  return QString::null;
}

QString PythonVersionChecker::compiledVersion() {
  return TLP_PYTHON;
}

bool PythonVersionChecker::isPythonVersionMatching() {
  return compiledVersion() == installedVersion();
}
