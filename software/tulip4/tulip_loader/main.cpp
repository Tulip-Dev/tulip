#include <tulip/PluginLibraryLoader.h>

#include <iostream>

int main(int argc, char **argv) {
  if (argc != 2)
    return 1;
  std::cout << "Loading library " << argv[1] << std::endl;
//  tlp::PluginLibraryLoader::loadPluginLibrary();
  return 0;
}
