#include <tulip/TlpTools.h>
#include "PluginInfo.h"

using namespace std;
using namespace tlp;

int main(int argc,char **argv) {
  if(argc != 2)  {
    cerr << "library plugin file missing." << endl;
  }

  initTulipLib(NULL);
  // only check that the dynamic loading
  // of the library is OK
  loadPlugin(argv[1], NULL);

  cout << "pluginLoaded" << endl;

}
