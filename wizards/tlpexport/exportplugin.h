#ifndef %ProjectName:u%_H
#define %ProjectName:u%_H

#include <tulip/ExportModule.h>

class %ProjectName:c%: public tlp::ExportModule {
public:
  PLUGININFORMATION("%PluginName%", "%Author%", "%Date%", "%Informations%", "%Version%", "%Group%")

  %ProjectName:c%(tlp::PluginContext* context);
  virtual ~%ProjectName:c%();

  bool exportGraph(std::ostream & fileOut);
  std::string fileExtension() const;
};

#endif // %ProjectName:u%_H


