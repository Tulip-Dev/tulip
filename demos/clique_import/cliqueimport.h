#ifndef CLIQUEIMPORT_H
#define CLIQUEIMPORT_H

#include <string>
#include <tulip/TulipPluginHeaders.h>

class CliqueImport: public tlp::ImportModule {
public:
  PLUGININFORMATIONS("Clique", "Tulip Team", "05/10/2012", "Clique Import Plugin", "1.0", "Graph")
  CliqueImport(tlp::PluginContext* context);
  ~CliqueImport();
  bool importGraph();
};

#endif // CLIQUEIMPORT_H
