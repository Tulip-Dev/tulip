#include <tulip/Graph.h>
#include <tulip/Interactor.h>
#include <tulip/TlpTools.h>
#include <tulip/TlpQtTools.h>
#include <tulip/PluginLoaderTxt.h>

using namespace std;
using namespace tlp;

int main(int argc,char ** argv ){
  
  tlp::initTulipLib();

  PluginLoaderTxt txtLoader;
  tlp::PluginLibraryLoader::loadPlugins(&txtLoader);
  tlp::PluginLister::checkLoadedPluginsDependencies(&txtLoader);

  return EXIT_SUCCESS;
}
