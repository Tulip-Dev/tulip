#include <tulip/TlpTools.h>
#include "PluginInfo.h"

using namespace std;
using namespace tlp;

int main(int argc,char **argv) {
  if(argc != 2)  {
    cerr << "library plugin file missing." << endl;
    exit(EXIT_FAILURE);
  }

  initTulipLib(NULL);
  // only check that the dynamic loading
  // of the library is OK  
#ifndef NDEBUG
  cout << "loading " << argv[1] << endl;
#endif
  loadPlugin(argv[1], NULL);
#ifndef NDEBUG
  cout << "loaded " << endl;
#endif

  exit(TLP_CHECK_PL_RETURN_SUCCESS);
}
