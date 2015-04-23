#ifndef %ProjectName:u%_H
#define %ProjectName:u%_H

#include <tulip/ImportModule.h>

class %ProjectName:c%: public tlp::ImportModule {
public:
  PLUGININFORMATION("%PluginName%", "%Author%", "%Date%", "%Informations%", "%Version%", "%Group%")

%ProjectName:c%(tlp::PluginContext* context);
  virtual ~%ProjectName:c%();

  bool importGraph();
  virtual std::list<std::string> fileExtensions() const;
};

#endif // %ProjectName:u%_H


