#ifndef %ProjectName:u%_H
#define %ProjectName:u%_H

#include <string>
#include <tulip/TulipPluginHeaders.h>

class %ProjectName:c%: public tlp::ImportModule {
public:
  PLUGININFORMATIONS("%PluginName%", "%Author%", "%Date%", "%Informations%", "%Version%", "%Group%")
%ProjectName:
  c%(tlp::PluginContext* context);
~%ProjectName:
  c%();
  bool importGraph();
};

#endif // %ProjectName:u%_H


