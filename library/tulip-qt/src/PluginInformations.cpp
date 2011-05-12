#include <tulip/PluginInformations.h>

using namespace tlp;

LocalPluginInformations::LocalPluginInformations(const AbstractPluginInfo* info) :_infos(info) {
}

QString LocalPluginInformations::identifier() const{
  return _infos->getName().c_str();
}
QString LocalPluginInformations::name() const{
  return "Plugin name";
}

QString LocalPluginInformations::shortDescription() const{
  return "This is a plugin.";
}
QString LocalPluginInformations::longDescriptionPath() const{
  return "longDescription.html";
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
  return "4.0.0";
}
