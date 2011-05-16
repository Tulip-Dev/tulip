#ifndef TULIPSPLASHSCREEN_H
#define TULIPSPLASHSCREEN_H

#include <QtGui/QSplashScreen>
#include <tulip/PluginLoader.h>
#include <QtCore/QMap>

/**
  @brief a splash screen used when loading plugins.
  Loading plugins in the tulip_app process allows to check for plugins error before loading any perspective. It provides the user with some basic preemptive informations
  and forbid the use of invalid perspectives.
  */
class TulipSplashScreen: public tlp::PluginLoader, public QSplashScreen {
public:
  TulipSplashScreen();
  virtual ~TulipSplashScreen() {}

  virtual void start(const std::string &path,const std::string &type);
  virtual void loading(const std::string &filename);
  virtual void loaded(const tlp::AbstractPluginInfo* infos, const std::list <tlp::Dependency>& deps);
  virtual void numberOfFiles(int n) { _numberOfFiles = n; _fileCounter = 0; }
  virtual void aborted(const std::string &filename,const  std::string &erreurmsg);
  virtual void finished(bool state,const std::string &msg);

  QMap<QString,QString> abortedPlugins() const { return _abortedPlugins; }

protected:
  virtual void drawContents(QPainter *painter);

private:
  QString _title;
  QString _message;

  int _fileCounter;
  int _numberOfFiles;

  QMap<QString,QString> _abortedPlugins;
};

#endif // TULIPSPLASHSCREEN_H
