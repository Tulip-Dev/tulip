#ifndef STRINGCOLLECTION_H
#define STRINGCOLLECTION_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>

#include "tulipconf.h"

namespace tlp {

struct TLP_SCOPE StringCollection : public std::vector<std::string> {

private:
  int current;

public:
  StringCollection();
  explicit StringCollection(const std::vector<std::string> &vectorParam);
  StringCollection(const std::string param);   
  StringCollection(const std::vector<std::string>&  vectorParam, int currentParam);
  StringCollection(const std::vector<std::string>& vectorParam, std::string currentString);
  std::string getCurrentString();
  bool        setCurrent(unsigned int param);
  bool        setCurrent(std::string param);
  int         getCurrent();
};

}
#endif
