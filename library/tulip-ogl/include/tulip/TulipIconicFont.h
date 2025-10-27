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

///@cond DOXYGEN_HIDDEN

#ifndef TULIPICONICFONT_H
#define TULIPICONICFONT_H

#include <tulip/tulipconf.h>

namespace tlp {

/**
 * @brief Helper class to ease the use of iconic fonts bundled with Tulip
 */
class TLP_GL_SCOPE TulipIconicFont {

public:
  /**
   * Returns the location of the TrueType font file bundled with Tulip
   * defining the icon with the given name
   * @param iconName the name of the icon
   */
  static std::string getTTFLocation(const std::string &iconName);

  /**
   * Returns the location of the .woff2 font file bundled with Tulip
   * defining the icon with the given name
   * @param iconName the name of the icon
   */
  static std::string getWOFF2Location(const std::string &iconName);

  /**
   * Checks if the provided icon name is supported
   * @param iconName the name of the icon to check support
   */
  static bool isIconSupported(const std::string &iconName);

  /**
   * Returns the Unicode code point associated to an icon name
   * @param iconName the name of the icon to get the codepoint
   */
  static unsigned int getIconCodePoint(const std::string &iconName);

  /**
   * Returns the font family name associated to an icon name
   * @param iconName the icon name
   */
  static std::string getIconFamily(const std::string &iconName);

  /**
   * Returns the font style name associated to an icon name
   * @param iconName the icon name
   */
  static std::string getIconStyle(const std::string &iconName);

  /**
   * Returns an UTF-8 encoded string of a Font Awesome icon
   * @param iconName the icon name
   * @return
   */
  static std::string getIconUtf8String(const std::string &iconName);
};
} // namespace tlp

#endif // TULIPFONTICONIC_H

///@endcond
