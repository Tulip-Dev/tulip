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

#ifndef TULIPFONTAWESOME_H
#define TULIPFONTAWESOME_H

#include <tulip/tulipconf.h>

#include <vector>

namespace tlp {

/**
 * @brief Helper class for the configuration of a Font Awesome glyph.
 *
 * Font Awesome is a free iconic font by Dave Gandy (see https://fontawesome.com)
 * offering more than 500 customizable scalable vector icons.
 *
 * A glyph has been added to Tulip enabling to use these great icons
 * as nodes and edges extremities shapes.
 *
 * That class offers utility functions and supported icons names constants.
 *
 * To set a node glyph as a Font Awesome icon, the Icon glyph must be associated
 * to the node through the modification of the "viewShape" integer property attached to the graph.
 * The name of the icon to use must then be set in the "viewIcon" string property.
 * As an example, the following code snippet activates the Font Awesome glyph for all nodes
 * and sets the "user" icon.
 *
 * @code
 *    // graph is a pointer to a tlp::Graph object
 *    tlp::IntegerProperty *viewShape = graph->getProperty<tlp::IntegerProperty>("viewShape");
 *    tlp::StringProperty *viewIcon = graph->getProperty<tlp::StringProperty>("viewIcon");
 *
 *    // sets the Icon glyph on all nodes
 *    viewShape->setAllNodeValue(tlp::NodeShape::Icon);
 *    // sets the "user" glyph for all nodes
 *    viewIcon->setAllNodeValue("fa-user");
 * @endcode
 **/

class TLP_SCOPE TulipFontAwesome {

public:
  /**
   * Returns the version of the Font Awesome icons bundled with Tulip
   */
  static std::string getVersion();

  /**
   * Returns the location of the Font Awesome TrueType font file bundled with Tulip
   */
  static std::string getTrueTypeFileLocation();

  _DEPRECATED static std::string getFontAwesomeTrueTypeFileLocation() {
    return getTrueTypeFileLocation();
  }

  /**
   * Returns the list of supported Font Awesome icons names
   */
  static const std::vector<std::string> &getSupportedIcons();

  _DEPRECATED static const std::vector<std::string> &getSupportedFontAwesomeIcons() {
    return getSupportedIcons();
  }

  /**
   * Checks if the provided Font Awesome icon name is supported
   * @param iconName the name of the icon to check support
   */
  static bool isIconSupported(const std::string &iconName);

  _DEPRECATED static bool isFontAwesomeIconSupported(const std::string &iconName) {
    return isIconSupported(iconName);
  }

  /**
   * Returns the Unicode code point associated to an icon name
   * @param iconName the name of the icon to get the codepoint
   */
  static unsigned int getIconCodePoint(const std::string &iconName);

  _DEPRECATED static unsigned int getFontAwesomeIconCodePoint(const std::string &iconName) {
    return getIconCodePoint(iconName);
  }

  /**
   * Returns an UTF-8 encoded string of a Font Awesome icon
   * @param iconName a Font Awesome icon name
   * @return
   */
  static std::string getIconUtf8String(const std::string &iconName);

  _DEPRECATED static std::string getFontAwesomeIconUtf8String(const std::string &iconName) {
    return getIconUtf8String(iconName);
  }
};
}

#endif // TULIPFONTAWESOME_H
