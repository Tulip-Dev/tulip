#include <tulip/PluginInformations.h>

#include <tulip/WithDependency.h>
#include <tulip/AbstractPluginInfo.h>

#include <QtCore/QFileInfo>

using namespace tlp;

PluginInformations::PluginInformations(const tlp::AbstractPluginInfo* info, const std::string& type, const std::list< Dependency >& dependencies, const std::string& library)
    :_type(type.c_str()), _isLocal(true), _installedVersion(info->getRelease().c_str()), _updateAvailable(false) {
  _versions << info->getRelease().c_str();
  PluginInfoWithDependencies pluginInfo(info, dependencies);
  _infos[info->getName().c_str()] = pluginInfo;
}

PluginInformations::PluginInformations(const tlp::AbstractPluginInfo* info, const std::string& type, const std::list<tlp::Dependency>& dependencies, const QString& longDescriptionPath, const QString& iconPath)
    :_type(type.c_str()), _isLocal(false), _installedVersion(QString::null), _updateAvailable(false) {
  _versions << info->getRelease().c_str();
  PluginInfoWithDependencies pluginInfo(info, dependencies);
  _infos[info->getName().c_str()] = pluginInfo;
}

void PluginInformations::AddPluginInformations(const tlp::AbstractPluginInfo* info, const std::string& type, const std::list<tlp::Dependency>& dependencies) {
    QString newVersion = info->getRelease().c_str();
    if(_installedVersion > newVersion) {
      _updateAvailable = true;
    }
    
    _versions << newVersion;
    PluginInfoWithDependencies pluginInfo(info, dependencies);
    _infos[info->getName().c_str()] = pluginInfo;
}

void PluginInformations::AddPluginInformations(const tlp::PluginInformations* info) {
  foreach(const QString& version, info->versions()) {
    if(version > _installedVersion) {
      _updateAvailable = true;
    }
    _versions << version;
  }
}

QString PluginInformations::identifier() const{
  return _infos.begin().value().infos->getName().c_str();
}

QString PluginInformations::name() const{
  return _infos.begin().value().infos->getName().c_str();
}

QString PluginInformations::shortDescription() const{
  return _infos.begin().value().infos->getInfo().c_str();
}

QString PluginInformations::longDescriptionPath() const{
  return "index.html";
}

QString PluginInformations::iconPath() const{
  return ":/tulip/gui/icons/logo32x32.png";
}

QDateTime PluginInformations::installDate() const{
  return QDateTime::currentDateTime();
}

QString PluginInformations::type() const{
  return _type;
}

const QStringList& PluginInformations::dependencies(QString version) const{
  return _infos[version].dependenciesNames;
}

const QStringList& PluginInformations::versions() const {
  return _versions;
}

QString PluginInformations::installedVersion() const {
  return _installedVersion;
}

bool PluginInformations::isInstalled(QString version) const {
  return version == _installedVersion;
}

bool PluginInformations::isInstalled() const {
  return !(installedVersion().isEmpty());
}

bool PluginInformations::updateAvailable() const {
  return _updateAvailable;
}

bool PluginInformations::fetch(QString version) const {
  //FIXME implement me
}

bool PluginInformations::remove() const {
  //FIXME implement me
}
