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

#include <tulip/PythonVersionChecker.h>

#include <QProcess>
#include <QSettings>

#include <cstdlib>
#include <iostream>

using namespace tlp;
using namespace std;

// Current Python versions
static const char *pythonVersion[] = {"3.6", "3.5", "3.4", "3.3", "3.2", "3.1",
                                      "3.0", "2.7", "2.6", "2.5", 0};

// Windows specific functions
#ifdef WIN32

#include <QFileInfo>

#include <windows.h>

#ifndef X86_64
// function to check if a 32 bits program run on a 64 bits system
static bool isWow64() {
  BOOL bIsWow64 = FALSE;

  typedef BOOL(APIENTRY * LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);

  LPFN_ISWOW64PROCESS fnIsWow64Process;

  HMODULE module = GetModuleHandle(TEXT("kernel32"));
  const char funcName[] = "IsWow64Process";
  fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(module, funcName);

  if (fnIsWow64Process != NULL) {
    fnIsWow64Process(GetCurrentProcess(), &bIsWow64);
  }

  return bIsWow64 != FALSE;
}
#endif

#ifndef MSYS2_PYTHON
// Check if a path is a valid Python Home, meaning it is not empty and contains the python
// executable
static bool validPythonHome(const QString &pythonHome) {
  return !pythonHome.isEmpty() && QFileInfo(pythonHome + "/python.exe").exists();
}
#endif

// Function to get the path to Python home directory for a specific Python version.
// Returns an empty string if the provided version is not installed on the host system.
// The path to the Python home directory is retrieved from the windows registry.
// On windows, Python can be installed for all users or for the current user only. That function
// handles both cases.
// The current user installation will be prefered over the all users one.
static QString pythonHome(const QString &pythonVersion) {

// special case when using Python provided by MSYS2
#ifdef MSYS2_PYTHON
  (void)pythonVersion;
  return PYTHON_HOME_PATH;

// standard Python installation on Windows
#else

  QString winRegKeyAllUsers, winRegKeyCurrentUser;

// 32 bit Python
#ifndef X86_64

  // on windows 64 bit
  if (isWow64()) {
    winRegKeyAllUsers = QString("HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Python\\PythonCore\\") +
                        pythonVersion + QString("\\InstallPath");
    winRegKeyCurrentUser =
        QString("HKEY_CURRENT_USER\\SOFTWARE\\Wow6432Node\\Python\\PythonCore\\") + pythonVersion +
        QString("\\InstallPath");
  }
  // on windows 32 bit
  else {
    winRegKeyAllUsers = QString("HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\") +
                        pythonVersion + QString("\\InstallPath");
    winRegKeyCurrentUser = QString("HKEY_CURRENT_USER\\SOFTWARE\\Python\\PythonCore\\") +
                           pythonVersion + QString("\\InstallPath");
  }

// 64 bit Python
#else

  winRegKeyAllUsers = QString("HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\") +
                      pythonVersion + QString("\\InstallPath");
  winRegKeyCurrentUser = QString("HKEY_CURRENT_USER\\SOFTWARE\\Python\\PythonCore\\") +
                         pythonVersion + QString("\\InstallPath");

#endif

  QSettings winSettingsAllUsers(winRegKeyAllUsers, QSettings::NativeFormat);
  QSettings winSettingsCurrentUser(winRegKeyCurrentUser, QSettings::NativeFormat);
  QString pythonHomeAllUsers = winSettingsAllUsers.value("Default").toString().replace("\\", "/");
  QString pythonHomeCurrentUser =
      winSettingsCurrentUser.value("Default").toString().replace("\\", "/");

  if (validPythonHome(pythonHomeCurrentUser)) {
    return pythonHomeCurrentUser;
  } else if (validPythonHome(pythonHomeAllUsers)) {
    return pythonHomeAllUsers;
  } else {
    return "";
  }

#endif
}

// Linux and Mac OS specific
#else

// Function which tries to run a specific version of the python interpreter.
static bool runPython(const QString &version) {
  QProcess pythonProcess;
  pythonProcess.start(QString("python") + version, QStringList() << "--version");
  return pythonProcess.waitForFinished(-1) && pythonProcess.exitStatus() == QProcess::NormalExit;
}

#endif

// Function to get the default Python version if any by running the python process.
static QString getDefaultPythonVersionIfAny() {
  QString defaultPythonVersion;
  QProcess pythonProcess;

  QString pythonCommand = "python";

// This is a hack for MinGW to allow the debugging of Tulip through GDB when compiled with Python
// 3.X installed in a non standard way.
#ifdef __MINGW32__
  char *pythonDirEv = getenv("PYTHONDIR");

  if (pythonDirEv) {
    pythonCommand = QString(pythonDirEv) + "/" + pythonCommand;
  }

#endif

  // Before Python 3.4, the version number was printed on the standard error output.
  // Starting Python 3.4 the version number is printed on the standard output.
  // So merge the output channels of the process.
  pythonProcess.setReadChannelMode(QProcess::MergedChannels);
  pythonProcess.setReadChannel(QProcess::StandardOutput);
  pythonProcess.start(pythonCommand, QStringList() << "--version");
  pythonProcess.waitForFinished(-1);

  if (pythonProcess.exitStatus() == QProcess::NormalExit) {

    QString result = pythonProcess.readAll();

    QRegExp versionRegexp(".*([0-9]*\\.[0-9]*)\\..*");

    if (versionRegexp.exactMatch(result)) {
      defaultPythonVersion = versionRegexp.cap(1);

// This is a hack for MinGW to allow the debugging of Tulip through GDB when compiled with Python
// 3.X installed in a non standard way.
#ifdef __MINGW32__

      if (pythonDirEv) {
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        env.insert("PYTHONHOME", QString(pythonDirEv));
        pythonProcess.setProcessEnvironment(env);
      }

#endif

      // Check the binary type of the python executable (32 or 64 bits)
      pythonProcess.start(
          pythonCommand,
          QStringList()
              << "-c"
              << "import struct;import sys;sys.stdout.write(str(struct.calcsize('P')*8))");
      pythonProcess.waitForFinished(-1);
      QString arch = pythonProcess.readAll();

#ifdef X86_64

      if (arch != "64") {
        defaultPythonVersion = "";
      }

#else

      if (arch != "32") {
        defaultPythonVersion = "";
      }

#endif
    }
  }

  return defaultPythonVersion;
}

QStringList PythonVersionChecker::_installedVersions;
bool PythonVersionChecker::_installedVersionsChecked(false);

QStringList PythonVersionChecker::installedVersions() {

  if (!_installedVersionsChecked) {

// On Linux and Mac OS, we check the presence of Python by trying to
// run the interpreter on a separate process
#ifndef WIN32

    int i = 0;

    // Try to run pythonX.Y executable
    while (pythonVersion[i]) {
      if (runPython(pythonVersion[i])) {
        _installedVersions.append(pythonVersion[i]);
      }

      ++i;
    }

// On windows, we check the presence of Python by looking into the registry
#else

    int i = 0;

    while (pythonVersion[i]) {
      if (!pythonHome(pythonVersion[i]).isEmpty()) {
        _installedVersions.append(pythonVersion[i]);
      }

      ++i;
    }

#endif

    // Also try to run python executable
    QString defaultPythonVersion = getDefaultPythonVersionIfAny();

    if (!defaultPythonVersion.isEmpty() && !_installedVersions.contains(defaultPythonVersion)) {
      _installedVersions.append(defaultPythonVersion);
    }

    _installedVersionsChecked = true;
  }

  return _installedVersions;
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
    QString pythonHomeDir = pythonHome(compiledVersion());
// This is a hack for MinGW to allow the debugging of Tulip through GDB when compiled with Python
// 3.X installed in a non standard way.
#ifdef __MINGW32__

    if (pythonHomeDir.isEmpty()) {
      char *pythonDirEv = getenv("PYTHONDIR");

      if (pythonDirEv) {
        return QString(pythonDirEv);
      }
    }

#endif
    return pythonHomeDir;
  }

  return "";
}
#endif
