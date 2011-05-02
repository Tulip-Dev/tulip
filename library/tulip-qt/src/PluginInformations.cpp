#include <tulip/PluginInformations.h>

QString PluginInformations::identifier() const{
  return "plugin";
}
QString PluginInformations::name() const{
  return "Plugin name";
}

QString PluginInformations::shortDescription() const{
  return "This is a plugin.";
}
QString PluginInformations::longDescriptionPath() const{
  return "longDescription.html";
}

QIcon PluginInformations::iconPath() const{
  return ":/logo32x32.png";
}

QDate PluginInformations::installDate() const{
  return QDate::currentTime();
}

QString PluginInformations::type() const{
  return "Algorithm";
}
QStringList PluginInformations::dependencies() const{
  QStringList l;
  return l;
}
QString PluginInformations::version() const{
  return "4.0.0";
}
