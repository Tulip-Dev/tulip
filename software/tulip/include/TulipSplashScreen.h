#ifndef TULIPSPLASHSCREEN_H
#define TULIPSPLASHSCREEN_H

#include <QtGui/QSplashScreen>
#include <tulip/PluginLoader.h>

namespace tlp {
class TulipSplashScreen: public PluginLoader, public QSplashScreen {
public:
  TulipSplashScreen();
  virtual ~TulipSplashScreen() {}

  virtual void start(const std::string &path,const std::string &type);
  virtual void loading(const std::string &filename);
  virtual void loaded(const std::string &name,const std::string &author,const std::string &date,const std::string &info,const std::string &release,const std::string &version,const std::list <Dependency>&);
  virtual void numberOfFiles(int n) { _numberOfFiles = n; _fileCounter = 0; }
  virtual void aborted(const std::string &filename,const  std::string &erreurmsg);
  virtual void finished(bool state,const std::string &msg);

protected:
  virtual void drawContents(QPainter *painter);

private:
  QString _title;
  QString _message;

  int _fileCounter;
  int _numberOfFiles;
};
}

#endif // TULIPSPLASHSCREEN_H
