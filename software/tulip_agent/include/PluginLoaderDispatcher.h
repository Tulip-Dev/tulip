#ifndef PLUGINLOADERDISPATCHER_H
#define PLUGINLOADERDISPATCHER_H

#include <tulip/PluginLoader.h>
#include <QtCore/QSet>

class PluginLoaderDispatcher: public tlp::PluginLoader {
public:
  PluginLoaderDispatcher();

  virtual void start(const std::string &path);
  virtual void finished(bool state,const std::string &msg);

  virtual void numberOfFiles(int n);

  virtual void loading(const std::string &filename);
  virtual void loaded(const tlp::AbstractPluginInfo* infos, const std::list <tlp::Dependency>& dependencies);
  virtual void aborted(const std::string &filename,const  std::string &errormsg);

  void registerLoader(tlp::PluginLoader *loader);
  void removeLoader(tlp::PluginLoader *loader);

private:
  QSet<tlp::PluginLoader *> _loaders;
};

#endif // PLUGINLOADERDISPATCHER_H
