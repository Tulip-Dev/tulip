#ifndef PLUGINLOADERREPORTER_H
#define PLUGINLOADERREPORTER_H

#include <tulip/PluginLoader.h>
#include <QtCore/QMap>
#include <QtCore/QString>

class PluginLoaderReporter: public tlp::PluginLoader {
public:
  PluginLoaderReporter();

  virtual void start(const std::string &) {}
  virtual void numberOfFiles(int) {}
  virtual void loading(const std::string &) {}
  virtual void loaded(const tlp::AbstractPluginInfo *, const std::list <tlp::Dependency>&) {}
  virtual void aborted(const std::string &,const  std::string &);
  virtual void finished(bool ,const std::string &) {}

  QMap<QString,QString> errors() { return _errors; }

private:
  QMap<QString,QString> _errors;
};

#endif // PLUGINLOADERREPORTER_H
