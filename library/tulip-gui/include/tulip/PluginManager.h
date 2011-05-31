#include <tulip/PluginInformations.h>
#include <tulip/AbstractPluginInfo.h>

#include <QtCore/QList>
#include <QtCore/QMap>

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

namespace tlp {

class TLP_QT_SCOPE DistantPluginInfo : public tlp::AbstractPluginInfo {
  public:
    DistantPluginInfo(const std::string& author, const std::string& date, const std::string& group, const std::string& name, const std::string& info, const std::string& release, const std::string& tulipRelease)
    : _author(author), _date(date), _group(group), _name(name), _info(info), _release(release), _tulipRelease(tulipRelease) {
    }
    virtual std::string getAuthor() const {
      return _author;
    }
    virtual std::string getDate() const {
      return _date;
    }
    virtual std::string getGroup() const {
      return _group;
    }
    virtual std::string getName() const {
      return _name;
    }
    virtual std::string getInfo() const {
      return _info;
    }
    virtual std::string getRelease() const {
      return _release;
    }
    virtual std::string getTulipRelease() const {
      return _tulipRelease;
    }
  private:
    const std::string& _author;
    const std::string& _date;
    const std::string& _group;
    const std::string& _name;
    const std::string& _info;
    const std::string& _release;
    const std::string& _tulipRelease;
};
  
class TLP_QT_SCOPE PluginManager :QObject {
  Q_OBJECT
public:
  
  enum PluginLocation {
    All = 3,
    Local = 1,
    Remote = 2
  };
  Q_DECLARE_FLAGS(Location, PluginLocation);
  
  static QList<const tlp::PluginInformationsInterface*> pluginsList(Location list = All);

  static void addRemoteLocation(const QString& location);
  static QString getPluginServerDescription(const QString& location);
  static QList<tlp::PluginInformationsInterface*> parseDescription(const QString& description);

private:
  static QMap<QString, QList<tlp::PluginInformationsInterface*> > _remoteLocations;

};

Q_DECLARE_OPERATORS_FOR_FLAGS(PluginManager::Location)

}
#endif //PLUGINMANAGER_H
