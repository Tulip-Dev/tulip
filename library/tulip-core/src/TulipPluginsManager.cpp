#include "tulip/TulipPluginsManager.h"

#include <QtCore/QDir>
#include <QtCore/QLibrary>

#include <tulip/PluginProgress.h>
#include <tulip/TulipPlugin.h>
#include <tulip/TulipRelease.h>

using namespace tlp;

const char* TulipPluginsManager::PLUGIN_SYMBOL_NAME = "pluginObject";

QMap<QString,QLibrary*> TulipPluginsManager::_pluginLibraries;

TulipPluginsManager::TulipPluginsManager() {
}

void TulipPluginsManager::loadDirectory(const QString& directory, tlp::PluginProgress* progress) {
  QFileInfoList infoList(QDir(directory).entryInfoList(QDir::NoDotAndDotDot | QDir::Files));

  int step=0,max_step=infoList.size();

  foreach(QFileInfo info, infoList) {
    progress->setComment((QObject::trUtf8("Loading ") + info.fileName()).toStdString());
    progress->progress(step++,max_step);

    QString fullPath = info.absoluteFilePath();
    PluginLoadStatus result = loadLibrary(fullPath);

    switch(result) {
    case NoError:
      progress->setComment((info.fileName() + QObject::trUtf8(" loaded sucessfuly")).toStdString());
      break;
    case NotPlugin:
      progress->setError((info.fileName() + QObject::trUtf8(" is not a Tulip plugin")).toStdString());
      break;
    case InstantiationFailed:
      progress->setError((info.fileName() + QObject::trUtf8(" failed to instantiate plugin")).toStdString());
      break;
    case VersionMismatch:
      progress->setError((info.fileName() + QObject::trUtf8(" is not compatible with this version of Tulip.")).toStdString());
      break;
    }
  }
}

TulipPluginsManager::PluginLoadStatus TulipPluginsManager::loadLibrary(const QString& fullPath) {
  if (!QLibrary::isLibrary(fullPath))
    return NotPlugin;

  QLibrary* lib = new QLibrary(fullPath);
  PluginCreator creator = (PluginCreator)(lib->resolve(PLUGIN_SYMBOL_NAME));

  if (!lib->load() || creator == NULL) {
    delete lib;
    return NotPlugin;
  }

  TulipPlugin* plugin = creator();
  if (plugin == NULL) {
    delete lib;
    return InstantiationFailed;
  }

  if (plugin->tulipRelease() != TULIP_MM_RELEASE) {
    delete plugin;
    delete lib;
    return VersionMismatch;
  }


  _pluginLibraries[plugin->name()] = lib;
  delete plugin;
  return NoError;
}
