#include <tulip/PluginInformations.h>

#include <QtCore/QFileInfo>
#include <QDir>

using namespace tlp;

const QString LocalPluginInformations::DefaultIconPath = QString::fromStdString(tlp::TulipBitmapDir) + "logo32x32.bmp";
const QString LocalPluginInformations::DefaultLongDescriptionPath = QString::fromStdString(tlp::TulipLibDir) + "../doc/default.html";

LocalPluginInformations::LocalPluginInformations(const tlp::AbstractPluginInfo* info, const std::string& type, const std::list< Dependency >& dependencies, const std::string& library)
  :_infos(info), _type(type.c_str()), _libraryPath(library.c_str()), _dependencies(dependencies) {
    for(std::list<tlp::Dependency>::const_iterator it = _dependencies.begin(); it != _dependencies.end(); ++it) {
      _dependenciesNames.push_back(it->pluginName.c_str());
    }
    QFileInfo libraryInfo(library.c_str());
    _installDate = libraryInfo.lastModified();
    
    QDir libraryDir(libraryInfo.canonicalPath());
    libraryDir.cdUp();
    libraryDir.cd("share/doc");
    libraryDir.cd(_infos->getName().c_str());

    _longDescriptionPath = libraryDir.canonicalPath() + "index.html";
    if(!QFile::exists(_longDescriptionPath)) {
      _longDescriptionPath = LocalPluginInformations::DefaultLongDescriptionPath;
    }

    libraryDir.cdUp();
    libraryDir.cd("icons");
    _iconPath = libraryDir.canonicalPath() + _infos->getName().c_str() + ".png";
    if(!QFile::exists(_iconPath)) {
      _iconPath = LocalPluginInformations::DefaultIconPath;
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
  return _longDescriptionPath;
}

QString LocalPluginInformations::iconPath() const{
  return _iconPath;
}

QDateTime LocalPluginInformations::installDate() const{
  return _installDate;
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
