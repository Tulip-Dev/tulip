/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
#include <tulip/Plugin.h>

using namespace tlp;
using namespace std;

std::string tlp::getMinor(const std::string &release) {
#ifdef _MSC_VER
  char sep = '_';

  // handle the case where the version number separator is not an underscore (e.g. for Tulip plugins)
  if (release.find(sep) == std::string::npos) {
    sep = '.';
  }

#else
  char sep = '.';
#endif
  size_t pos = release.find(sep);

  //if there is no minor version number, return a default '0'
  if (pos == std::string::npos)
    return std::string("0");

  size_t rpos = release.rfind(sep);

  //if there is only one dot, return everything after it
  if (pos == rpos)
    return release.substr(pos+1);

  //if there is more than one dot, return everything between the first and last dots
  return release.substr(pos + 1, rpos - pos - 1);
}

std::string tlp::getMajor(const std::string &release) {
#ifdef _MSC_VER
  char sep = '_';

  // handle the case where the version number separator is not an underscore (e.g. for Tulip plugins)
  if (release.find(sep) == std::string::npos) {
    sep = '.';
  }

#else
  char sep = '.';
#endif
  size_t pos = release.find(sep);
  return release.substr(0, pos);
}

string Plugin::major() const {
  return tlp::getMajor(release());
}

string Plugin::minor() const {
  return tlp::getMinor(release());
}

string Plugin::tulipMajor() const {
  return tlp::getMajor(tulipRelease());
}

string Plugin::tulipMinor() const  {
  return tlp::getMinor(tulipRelease());
}

int Plugin::id() const {
  return 0;
}

std::string Plugin::icon() const {
  return ":/tulip/gui/icons/logo32x32.png";
}

std::string Plugin::programmingLanguage() const {
  return "C++";
}
