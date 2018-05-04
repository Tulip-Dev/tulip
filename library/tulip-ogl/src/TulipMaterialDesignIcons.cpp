/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#include <cstring>
#include <tulip/TulipMaterialDesignIcons.h>
#include <tulip/TlpTools.h>

#include <utf8.h>

#include <algorithm>
#include <map>

using namespace std;

namespace tlp {

struct cmpCharPtr {
  bool operator()(const char *s1, const char *s2) const {
    return strcmp(s1, s2) < 0;
  }
};
static map<const char *, vector<unsigned int>, cmpCharPtr> iconCodePoint;
static vector<std::string> iconsNames;

static inline void addIconCodePoint(const char *iconName, unsigned int codePoint) {
  iconCodePoint[iconName].push_back(codePoint);
}

// code generated by generate_md_init_cpp_code.sh
#include "TulipInitMaterialDesignIcons.cpp"

std::string TulipMaterialDesignIcons::getTTFLocation() {
  return string(tlp::TulipBitmapDir).append("materialdesignicons-webfont.ttf");
}

std::string TulipMaterialDesignIcons::getWOFFLocation() {
  return string(tlp::TulipBitmapDir).append("materialdesignicons-webfont.woff");
}

std::string TulipMaterialDesignIcons::getWOFF2Location() {
  return string(tlp::TulipBitmapDir).append("materialdesignicons-webfont.woff2");
}

bool TulipMaterialDesignIcons::isIconSupported(const std::string &iconName) {
  if (iconCodePoint.empty()) {
    initIconCodePoints();
  }

  return iconCodePoint.find(iconName.c_str()) != iconCodePoint.end();
}

const vector<std::string> &TulipMaterialDesignIcons::getSupportedIcons() {
  if (iconCodePoint.empty()) {
    initIconCodePoints();
  }

  return iconsNames;
}

unsigned int TulipMaterialDesignIcons::getIconCodePoint(const std::string &iconName) {
  if (iconCodePoint.empty()) {
    initIconCodePoints();
  }

  auto it = iconCodePoint.find(iconName.c_str());

  if (it != iconCodePoint.end())
    return (it->second[0]);

  return 0;
}

std::string TulipMaterialDesignIcons::getIconFamily(const std::string &) {
  return "materialdesignicons";
}

std::string TulipMaterialDesignIcons::getIconUtf8String(const std::string &iconName) {
  if (iconCodePoint.empty()) {
    initIconCodePoints();
  }

  std::string iconString;
  utf8::utf32to8(iconCodePoint[iconName.c_str()].begin(), iconCodePoint[iconName.c_str()].end(),
                 back_inserter(iconString));
  return iconString;
}
}
