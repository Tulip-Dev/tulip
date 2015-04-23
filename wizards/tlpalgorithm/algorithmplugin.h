#ifndef %ProjectName:u%_H
#define %ProjectName:u%_H

#include <tulip/Algorithm.h>

class %ProjectName:c%:public tlp::Algorithm {
public:

  PLUGININFORMATION("%PluginName%", "%Author%", "%Date%", "%Informations%", "%Version%", "%Group%")

  %ProjectName:c%(tlp::PluginContext* context);
  virtual ~%ProjectName:c%();

  bool run();
  bool check(std::string &);
};

#endif // %ProjectName:u%_H


