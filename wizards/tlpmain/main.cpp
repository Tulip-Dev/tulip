#include <tulip/TlpQtTools.h>
#include <tulip/PluginLoaderTxt.h>
#include <QApplication>

using namespace tlp;

int main(int argc,char ** argv ) {
  QApplication app(argc,argv);

  PluginLoaderTxt txtLoader;
  tlp::initTulipSoftware(&txtLoader);

  return app.exec();
}
