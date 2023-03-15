/*
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#include <cassert>

#include "tulip/TulipIconicFont.h"
#include "tulip/TulipFontAwesome.h"
#include "tulip/TulipMaterialDesignIcons.h"

using namespace tlp;

std::string TulipIconicFont::getTTFLocation(const std::string &iconName) {
  return (iconName[0] == 'f') ? TulipFontAwesome::getTTFLocation(iconName)
                              : TulipMaterialDesignIcons::getTTFLocation();
}

std::string TulipIconicFont::getWOFF2Location(const std::string &iconName) {
  assert(isIconSupported(iconName));
  return (iconName[0] == 'f') ? TulipFontAwesome::getWOFF2Location(iconName)
                              : TulipMaterialDesignIcons::getWOFF2Location();
}

bool TulipIconicFont::isIconSupported(const std::string &iconName) {
  return (iconName[0] == 'f') ? TulipFontAwesome::isIconSupported(iconName)
                              : TulipMaterialDesignIcons::isIconSupported(iconName);
}

unsigned int TulipIconicFont::getIconCodePoint(const std::string &iconName) {
  assert(isIconSupported(iconName));
  return (iconName[0] == 'f') ? TulipFontAwesome::getIconCodePoint(iconName)
                              : TulipMaterialDesignIcons::getIconCodePoint(iconName);
}

std::string TulipIconicFont::getIconFamily(const std::string &iconName) {
  assert(isIconSupported(iconName));
  return (iconName[0] == 'f') ? TulipFontAwesome::getIconFamily(iconName)
                              : TulipMaterialDesignIcons::getIconFamily(iconName);
}

std::string TulipIconicFont::getIconStyle(const std::string &iconName) {
  assert(isIconSupported(iconName));
  return (iconName[0] == 'f') ? TulipFontAwesome::getIconStyle(iconName)
                              : TulipMaterialDesignIcons::getIconStyle(iconName);
}

std::string TulipIconicFont::getIconUtf8String(const std::string &iconName) {
  return (iconName[0] == 'f') ? TulipFontAwesome::getIconUtf8String(iconName)
                              : TulipMaterialDesignIcons::getIconUtf8String(iconName);
}
