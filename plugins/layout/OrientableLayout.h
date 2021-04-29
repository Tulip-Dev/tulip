/**
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
#ifndef ORIENTABLELAYOUT_H
#define ORIENTABLELAYOUT_H

#include "OrientableLayoutInterface.h"

class OrientableLayout : public OrientableLayoutInterface {
  friend class OrientableCoord;

public:
  OrientableLayout(tlp::LayoutProperty *layout, orientationType mask = ORI_DEFAULT);

  OrientableCoord createCoord(const float x = 0, const float y = 0, const float z = 0) override;
  OrientableCoord createCoord(const tlp::Coord &v) override;

  void setOrientation(orientationType mask) override;

  void setAllNodeValue(const PointType &v) override;
  void setAllEdgeValue(const LineType &v) override;

  void setEdgeValue(const tlp::edge e, const LineType &v) override;
  void setNodeValue(tlp::node n, const PointType &v) override;

  PointType getNodeValue(const tlp::node n) override;
  LineType getEdgeValue(const tlp::edge e) override;

  PointType getNodeDefaultValue() override;
  LineType getEdgeDefaultValue() override;

  void setOrthogonalEdge(const tlp::Graph *tree, float interNodeDistance);

private:
  typedef std::vector<tlp::Coord> CoordLineType;
  std::vector<OrientableCoord> convertEdgeLinetype(const std::vector<tlp::Coord> &v);

  tlp::LayoutProperty *layout;
  orientationType orientation;

  float (OrientableCoord::*readX)() const;
  float (OrientableCoord::*readY)() const;
  float (OrientableCoord::*readZ)() const;
  void (OrientableCoord::*writeX)(float);
  void (OrientableCoord::*writeY)(float);
  void (OrientableCoord::*writeZ)(float);
  void addControlPoints(const tlp::Graph *tree, OrientableCoord fatherCoord, tlp::edge e,
                        float interNodeDistance);
};

#endif
