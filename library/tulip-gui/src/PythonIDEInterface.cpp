/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

#include <tulip/PythonIDEInterface.h>

using namespace tlp;

static PythonIDEInterface::Builder *_builder = nullptr;

PythonIDEInterface::Builder::Builder() {
  _builder = this;
}

bool PythonIDEInterface::exists() {
  return _builder != nullptr;
}

PythonIDEInterface *PythonIDEInterface::newIDE(GraphHierarchiesModel *model) {
  if (_builder)
    return _builder->newIDE(model);
  return nullptr;
}

void PythonIDEInterface::loadPlugins() {
  if (_builder)
    _builder->loadPlugins();
}

// Windows specific functions
#ifdef WIN32
#include <QSettings>

// Current Python versions
static const char *pythonVersion[] = {"3.13", "3.12", "3.11", "3.10", "3.9", nullptr};

#include <QFileInfo>

#include <windows.h>

// Function to get the path to Python home directory for a specific Python version.
// Returns an empty string if the provided version is not installed on the host system.
// The path to the Python home directory is retrieved from the windows registry.
// On windows, Python can be installed for all users or for the current user only. That function
// handles both cases.
// The current user installation will be preferred over the all users one.
static QString pythonHome(const QString &pythonVersion) {

// special case when using Python provided by MSYS2
#ifdef MSYS2_PYTHON
  std::ignore = pythonVersion;
  return PYTHON_HOME_PATH;

// standard Python installation on Windows
#else

  QString winRegKeyAllUsers, winRegKeyCurrentUser;

  winRegKeyAllUsers = QString("HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\") +
                      pythonVersion + QString("\\InstallPath");
  winRegKeyCurrentUser = QString("HKEY_CURRENT_USER\\SOFTWARE\\Python\\PythonCore\\") +
                         pythonVersion + QString("\\InstallPath");

  QSettings winSettingsAllUsers(winRegKeyAllUsers, QSettings::NativeFormat);
  QSettings winSettingsCurrentUser(winRegKeyCurrentUser, QSettings::NativeFormat);
  QString pythonHomeAllUsers = winSettingsAllUsers.value("Default").toString().replace("\\", "/");
  QString pythonHomeCurrentUser =
      winSettingsCurrentUser.value("Default").toString().replace("\\", "/");

  // Check if a path is a valid Python Home, meaning it is not empty
  // and contains the python executable
  auto validPythonHome = [](const QString &pythonHome) {
    return !pythonHome.isEmpty() && QFileInfo(pythonHome + "/python.exe").exists();
  };
  if (validPythonHome(pythonHomeCurrentUser)) {
    return pythonHomeCurrentUser;
  } else if (validPythonHome(pythonHomeAllUsers)) {
    return pythonHomeAllUsers;
  } else {
    return "";
  }

#endif
}
#endif

QStringList _installedVersions;
bool _installedVersionsChecked(false);

QString PythonIDEInterface::compiledVersion(bool full) {
  return full ? PYTHON_FULL_VERSION : PYTHON_VERSION;
}

#ifdef WIN32
QString PythonIDEInterface::getPythonHome() {
  QString pythonHomeDir;

  if (!_installedVersionsChecked) {

    int i = 0;

    while (pythonVersion[i]) {
      if (!pythonHome(pythonVersion[i]).isEmpty()) {
        _installedVersions.append(pythonVersion[i]);
      }

      ++i;
    }
    _installedVersionsChecked = true;
  }

  if (_installedVersions.contains(compiledVersion())) {
    pythonHomeDir = pythonHome(compiledVersion());
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
  }
  return pythonHomeDir;
}
#endif
