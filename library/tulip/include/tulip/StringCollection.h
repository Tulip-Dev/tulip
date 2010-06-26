/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
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
