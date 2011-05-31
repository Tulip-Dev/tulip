#include <tulip/PluginInformations.h>

#include <tulip/WithDependency.h>
#include <tulip/AbstractPluginInfo.h>

#include <QtCore/QFileInfo>

using namespace tlp;

LocalPluginInformations::LocalPluginInformations(const tlp::AbstractPluginInfo* info, const std::string& type, const std::list< Dependency >& dependencies, const std::string& library)
    :_infos(info), _type(type.c_str()), _dependencies(dependencies) {
  for(std::list<tlp::Dependency>::const_iterator it = _dependencies.begin(); it != _dependencies.end(); ++it) {
    _dependenciesNames.push_back(it->pluginName.c_str());
  }
}

LocalPluginInformations::LocalPluginInformations(const tlp::AbstractPluginInfo* info, const std::string& type, const std::list<tlp::Dependency>& dependencies, const QString& longDescriptionPath, const QString& iconPath)
    :_infos(info), _type(type.c_str()), _dependencies(dependencies) {
  for(std::list<tlp::Dependency>::const_iterator it = _dependencies.begin(); it != _dependencies.end(); ++it) {
    _dependenciesNames.push_back(it->pluginName.c_str());
  }
}

QString LocalPluginInformations::identifier() const{
  return _infos->getName().c_str();
}

QString LocalPluginInformations::name() const{
  return _infos->getName().c_str();
}

QString LocalPluginInformations::shortDescription() const{
  return _infos->getInfo().c_str();
}

QString LocalPluginInformations::longDescriptionPath() const{
  return "index.html";
}

QString LocalPluginInformations::iconPath() const{
  return ":/logo32x32.png";
}

QDateTime LocalPluginInformations::installDate() const{
  return QDateTime::currentDateTime();
}

QString LocalPluginInformations::type() const{
  return _type;
}

const QStringList& LocalPluginInformations::dependencies() const{
  return _dependenciesNames;
}

QString LocalPluginInformations::version() const{
  return _infos->getRelease().c_str();
}
