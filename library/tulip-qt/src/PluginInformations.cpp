#include <tulip/PluginInformations.h>

using namespace tlp;

LocalPluginInformations::LocalPluginInformations(const AbstractPluginInfo* info) :_infos(info) {
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
  return "Algorithm";
}

QStringList LocalPluginInformations::dependencies() const{
  QStringList l;
  return l;
}

QString LocalPluginInformations::version() const{
  return _infos->getRelease().c_str();
}
