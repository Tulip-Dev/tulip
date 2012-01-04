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
#ifndef TULIPEXCEPTION_H
#define TULIPEXCEPTION_H

#include <tulip/tulipconf.h>
#include <string>

namespace tlp {
/*@{*/
//=======================================
/**
 * @class TulipException
 * @brief TulipException is a basic class to build exceptions from string
 **/
class  TLP_SCOPE TulipException : public std::exception {
public:
  TulipException(const std::string &desc):desc(desc) {
  }
  virtual ~TulipException() throw () {
  }
  virtual const char* what() const throw() {
    return desc.c_str();
  }
private:
  std::string desc;
};
}
#endif // TULIPEXCEPTION
