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

#ifndef COLORSCALESMANAGER_H
#define COLORSCALESMANAGER_H

#include <tulip/ColorScale.h>

#include <list>

namespace tlp {

/**
 * @brief Helper class for handling Tulip color scales
 *
 * @since Tulip 4.10
 *
 * That static class aims to facilitate working with Tulip color scales.
 * It allows to easily retrieve Tulip predefined ones but also to register new ones
 * in a persistent database for further reuse.
 *
 */
class TLP_QT_SCOPE ColorScalesManager {

public:
  /**
   * @brief Returns a list of the registered color scales names.
   *
   */
  static std::list<std::string> getColorScalesList();

  /**
   * @brief Returns the color scale registered with the provided name.
   *
   * @warning If there is no such colorscale, the returned one will be empty.
   *
   */
  static tlp::ColorScale getColorScale(const std::string &colorScaleName);

  /**
   * @brief Registers a color scale in the persistent database for further reuse.
   *
   * @warning If there is already a color scale registered with such a name, it will not be
   * overwritten.
   *
   */
  static void registerColorScale(const std::string &colorScaleName,
                                 const tlp::ColorScale &colorScale);

  /**
   * @brief Removes the color scale registered with the provided name from the persistent database.
   *
   */
  static void removeColorScale(const std::string &colorScaleName);

  static ColorScale getLatestColorScale();
  static void setLatestColorScale(ColorScale &cs);

private:
  static void getColorScalesFromDir(const std::string &colorScalesDir,
                                    std::list<std::string> &colorScalesList);
  static std::string findColorScaleFile(const std::string &rootDir,
                                        const std::string &colorScaleName);
};
} // namespace tlp

#endif

///@endcond
