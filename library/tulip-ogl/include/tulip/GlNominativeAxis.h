/*
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
///@cond DOXYGEN_HIDDEN


#ifndef GLNOMINATIVEAXIS_H_
#define GLNOMINATIVEAXIS_H_

#include <map>

#include <tulip/GlAxis.h>

namespace tlp {

/**
 * \brief A class to render an axis graduated with string values
 *
 * This class allow to draw a nominative axis (i.e. an axis graduated with string values)
 */
class TLP_GL_SCOPE GlNominativeAxis : public GlAxis {

public :

  /**
   * GlNominativeAxis constructor. Build a nominative axis with no graduations (need to call setAxisGraduationsLabels to build them)
   *
   * \param axisName the name of the axis
   * \axisBaseCoord the base coord of the axis (if the axis is horizontal, it is the the left end, if vertical it is the down end)
   * \axisLength the length of the axis
   * \axisOrientation the orientation of the axis, 2 possible values (HORIZONTAL_AXIS or VERTICAL_AXIS)
   * \axisColor the color of the axis
   */
  GlNominativeAxis(const std::string &axisName, const Coord &axisBaseCoord, const float axisLength,
                   const AxisOrientation &axisOrientation, const Color &axisColor);


  /**
   * Method to set the axis graduations labels. A call to updateAxis has to be done after calling this method to build or update the axis graduations. The labels will be placed on the axis in the same order as the vector passed as parameter (from bottom to top if the axis is vertical, from left to right if it is horizontal).
   *
   * \param axisGradsLabels a vector of string containing the graduations labels
   * \param labelsPosition the relative position of the axis graduations label. Two possible values : LEFT_OR_BELOW (if the axis is vertical, labels will be on the left of the axis, otherwise below) or RIGHT_OR_ABOVE
   */
  void setAxisGraduationsLabels(const std::vector<std::string> &axisGradsLabels, const LabelPosition &labelsPosition);

  /**
   * Method to update the axis drawing. It has to be called when one (or more) of the setters method above has been used.
   * This method redraw the whole axis and the graduations.
   */
  void updateAxis();

  /**
   * Method to get the axis point coordinates associated to string value
   *
   * \param value the string value we want to retrieve axis point coordinates
   */
  Coord getAxisPointCoordForValue(const std::string& value);

  /**
   * Method to get the string value associated to an axis point. Return "" if there is not.
   *
   * \param axisPointCoord the axis point coordinates we want to retrieve the associated string value
   */
  std::string getValueAtAxisPoint(const Coord &axisPointCoord);

  void translate(const Coord &c);


private :

  void buildAxisGraduations();

  std::vector<std::string> labelsOrder;
  std::map<std::string, Coord> labelsCoord;
  LabelPosition axisLabelsPosition;

};

}

#endif /* GLNOMINATIVEAXIS_H_ */
///@endcond
