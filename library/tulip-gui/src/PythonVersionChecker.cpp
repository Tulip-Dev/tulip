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

#include <QProcess>
#include <QSettings>

#include <iostream>

using namespace tlp;
using namespace std;

#ifdef WIN32
#include <windows.h>

static const char *pythonVersion[]  = {
  "3.4", "3.3", "3.2", "3.1", "3.0", "2.7", "2.6", "2.5", 0
};

#ifndef I64
// function to check if a 32 bits program run on a 64 bits system
static bool isWow64() {
  BOOL bIsWow64 = FALSE;

  typedef BOOL (APIENTRY *LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);

  LPFN_ISWOW64PROCESS fnIsWow64Process;

  HMODULE module = GetModuleHandle(TEXT("kernel32"));
  const char funcName[] = "IsWow64Process";
  fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(module, funcName);

  if (fnIsWow64Process != NULL) {
    fnIsWow64Process(GetCurrentProcess(),&bIsWow64);
  }
  return bIsWow64 != FALSE;
}
#endif

static QString pythonHome(const QString &pythonVersion) {
#ifndef I64
  if (isWow64()) {
      QString win64RegKey = QString("HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Python\\PythonCore\\") + pythonVersion + QString("\\InstallPath");
      QSettings win64Settings(win64RegKey, QSettings::NativeFormat);
      return win64Settings.value("Default").toString().replace("\\", "/");
  } else {
      QString win32RegKey = QString("HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\") + pythonVersion + QString("\\InstallPath");
      QSettings win32Settings(win32RegKey, QSettings::NativeFormat);
      return win32Settings.value("Default").toString().replace("\\", "/");
  }
#else
    QString win64RegKey = QString("HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\") + pythonVersion + QString("\\InstallPath");
    QSettings win64Settings(win64RegKey, QSettings::NativeFormat);
    return win64Settings.value("Default").toString().replace("\\", "/");
#endif
}

static QString getInstalledPythonVersionIfAny() {
  int i = 0;

  while (pythonVersion[i]) {
    if (!pythonHome(pythonVersion[i]).isEmpty()) {
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

  if (!pythonHome(compiledVersion()).isEmpty()) {
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

#ifdef WIN32
QString PythonVersionChecker::getPythonHome() {
    if (isPythonVersionMatching()) {
        return pythonHome(compiledVersion());
    }
    return "";
}
#endif
