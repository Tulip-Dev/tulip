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

// Current Python versions
static const char *pythonVersion[]  = {
  "3.4", "3.3", "3.2", "3.1", "3.0", "2.7", "2.6", "2.5", 0
};

// Windows specific functions
#ifdef WIN32

#include <windows.h>

#ifndef I64
// function to Process if a 32 bits program run on a 64 bits system
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

// Function to get the path to Python home directory for a specific Python version.
// Returns an empty string if the provided version is not installed on the host system.
// The path to the Python home directory is retrieved from the windows registry.
// On windows, Python can be installed for all users or for the current user only. That function handles both cases
static QString pythonHome(const QString &pythonVersion) {
// 32 bit Python
#ifndef I64
  // on windows 64 bit
  if (isWow64()) {
    QString win64RegKeyAllUsers = QString("HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Python\\PythonCore\\") + pythonVersion + QString("\\InstallPath");
    QSettings win64SettingsAllUsers(win64RegKeyAllUsers, QSettings::NativeFormat);
    QString win64RegKeyCurrentUser = QString("HKEY_CURRENT_USER\\SOFTWARE\\Wow6432Node\\Python\\PythonCore\\") + pythonVersion + QString("\\InstallPath");
    QSettings win64SettingsCurrentUser(win64RegKeyCurrentUser, QSettings::NativeFormat);
    return win64SettingsAllUsers.value("Default").toString().replace("\\", "/") + win64SettingsCurrentUser.value("Default").toString().replace("\\", "/");
  }
  // on windows 32 bit
  else {
    QString win32RegKeyAllUsers = QString("HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\") + pythonVersion + QString("\\InstallPath");
    QSettings win32SettingsAllUsers(win32RegKeyAllUsers, QSettings::NativeFormat);
    QString win32RegKeyCurrentUser = QString("HKEY_CURRENT_USER\\SOFTWARE\\Python\\PythonCore\\") + pythonVersion + QString("\\InstallPath");
    QSettings win32SettingsCurrentUser(win32RegKeyCurrentUser, QSettings::NativeFormat);
    return win32SettingsAllUsers.value("Default").toString().replace("\\", "/")+win32SettingsCurrentUser.value("Default").toString().replace("\\", "/");
  }
// 64 bit Python
#else
  QString win64RegKeyAllUsers = QString("HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\") + pythonVersion + QString("\\InstallPath");
  QSettings win64SettingsAllUsers(win64RegKeyAllUsers, QSettings::NativeFormat);
  QString win64RegKeyCurrentUser = QString("HKEY_CURRENT_USER\\SOFTWARE\\Python\\PythonCore\\") + pythonVersion + QString("\\InstallPath");
  QSettings win64SettingsCurrentUser(win64RegKeyCurrentUser, QSettings::NativeFormat);
  return win64SettingsAllUsers.value("Default").toString().replace("\\", "/") + win64SettingsCurrentUser.value("Default").toString().replace("\\", "/");
#endif
}

// Linux and Mac OS specific
#else

// Function to get the default Python version if any by running the python process.
static QString getDefaultPythonVersionIfAny() {
  QString defaultPythonVersion;
  QProcess pythonProcess;
  // Before Python 3.4, the version number was printed on the standard error output.
  // Starting Python 3.4 the version number is printed on the standard output.
  // So merge the output channels of the process.
  pythonProcess.setReadChannelMode(QProcess::MergedChannels);
  pythonProcess.setReadChannel(QProcess::StandardOutput);
  pythonProcess.start(QString("python3.4"),QStringList() << "--version");
  pythonProcess.waitForFinished(-1);

  if (pythonProcess.exitStatus() == QProcess::NormalExit) {

    QString result = pythonProcess.readAll();

    QRegExp versionRegexp(".*([0-9]*\\.[0-9]*)\\..*");

    if (versionRegexp.exactMatch(result)) {
      defaultPythonVersion = versionRegexp.cap(1);
    }
  }
  return defaultPythonVersion;
}

// Function which tries to run a specific version of the python interpreter.
static bool runPython(const QString &version) {
  QProcess pythonProcess;
  pythonProcess.start(QString("python")+version,QStringList() << "--version");
  return pythonProcess.waitForFinished(-1) && pythonProcess.exitStatus() == QProcess::NormalExit;
}

#endif


QStringList PythonVersionChecker::installedVersions() {
  QStringList ret;

// On Linux and Mac OS, we check the presence of Python by trying to
// run the interpreter on a separate process
#ifndef WIN32

  int i = 0;

  // Try to run pythonX.Y executable
  while (pythonVersion[i]) {
    if (runPython(pythonVersion[i])) {
      ret.append(pythonVersion[i]);
    }
    ++i;
  }

  // Also try to run python executable
  QString defaultPythonVersion = getDefaultPythonVersionIfAny();
  if (!defaultPythonVersion.isEmpty() && !ret.contains(defaultPythonVersion)) {
    ret.append(defaultPythonVersion);
  }

// On windows, we check the presence of Python by looking into the registry
#else

    int i = 0;

    while (pythonVersion[i]) {
      if (!pythonHome(pythonVersion[i]).isEmpty()) {
        ret.append(pythonVersion[i]);
      }
      ++i;
    }

#endif
    return ret;
}

QString PythonVersionChecker::compiledVersion() {
  return TLP_PYTHON;
}

bool PythonVersionChecker::isPythonVersionMatching() {
  return installedVersions().contains(compiledVersion());
}

#ifdef WIN32
QString PythonVersionChecker::getPythonHome() {
  if (isPythonVersionMatching()) {
    return pythonHome(compiledVersion());
  }

  return "";
}
#endif
