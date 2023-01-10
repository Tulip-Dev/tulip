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

#ifndef TULIPMATERIALDESIGNICONS_H
#define TULIPMATERIALDESIGNICONS_H

#include <tulip/tulipconf.h>

#include <vector>

#undef linux

namespace tlp {

/**
 * @brief Helper class for the configuration of a Material Design Icon glyph.
 *
 * @since Tulip 5.0
 *
 * Material Design Icons (https://materialdesignicons.com/) :
 * a growing icon collection allows designers and developers
 * targeting various platforms to download icons in the format,
 * color and size they need for any project.
 *
 * A glyph has been added to Tulip enabling to use these great icons
 * as nodes and edges extremities shapes.
 *
 * That class offers utility functions and supported icons names constants.
 *
 * To set a node glyph as a Material Design icon, the Icon glyph must be associated
 * to the node through the modification of the "viewShape" integer property attached to the graph.
 * The name of the icon to use must then be set in the "viewIcon" string property.
 * As an example, the following code snippet activates the Material Design glyph for all nodes
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
 *    viewIcon->setAllNodeValue("mdi-user");
 * @endcode
 **/

class TLP_GL_SCOPE TulipMaterialDesignIcons {

public:
  /**
   * Returns the current Material Design Icons version
   */
  static std::string getVersion();

  /**
   * Returns the location of the Material Design Icons .ttf file bundled with Tulip
   */
  static std::string getTTFLocation();

  /**
   * Returns the location of the Material Design Icons .woff2 file bundled with Tulip
   */
  static std::string getWOFF2Location();

  /**
   * Returns the list of supported Material Design icons names
   */
  static const std::vector<std::string> &getSupportedIcons();

  /**
   * Checks if the provided Material Design icon name is supported
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
   * @param iconName the name of the icon
   */
  static std::string getIconFamily(const std::string &iconName);

  /**
   * Returns the font style name associated to an icon name
   * @param iconName the name of the icon
   */
  static std::string getIconStyle(const std::string &iconName);

  /**
   * Returns an UTF-8 encoded string of a Material Design icon
   * @param iconName a Material Design icon name
   * @return
   */
  static std::string getIconUtf8String(const std::string &iconName);
};
} // namespace tlp

#endif // TULIPMATERIALDESIGNICONS_H

///@endcond
