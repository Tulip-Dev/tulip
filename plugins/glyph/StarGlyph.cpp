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

/**
 *
 * tulip-ogles library : rewrite of old tulip-ogl library using OpenGL ES API
 * Copyright (c) 2016 Antoine Lambert, Thales Services
 * antoine-e.lambert@thalesgroup.com / antoine.lambert33@gmail.com
 *
 */

#include <tulip/Glyph.h>
#include <tulip/GlUtils.h>
#include <tulip/TulipViewSettings.h>

using namespace std;
using namespace tlp;

const float startAngle = static_cast<float>(M_PI)/2.0f;
const unsigned short numberOfStarPoints = 5;

static vector<Coord> computeStar(const Coord &position, const Size &size, unsigned int numberOfStarPoints) {
  BoundingBox box;
  vector<Coord> points;
  float delta = (2.0f * M_PI) / static_cast<float>(numberOfStarPoints);

  for (unsigned int i = 0 ; i < numberOfStarPoints ; ++i) {
    float deltaX = cos(i * delta + startAngle);
    float deltaY = sin(i * delta + startAngle);
    points.push_back(Coord(deltaX,deltaY,0));
    box.expand(points.back());
    deltaX = 0.5f * cos(i * delta + delta/2.0f + startAngle);
    deltaY = 0.5f * sin(i * delta + delta/2.0f + startAngle);
    points.push_back(Coord(deltaX,deltaY,0));
    box.expand(points.back());
  }

  for(vector<Coord>::iterator it=points.begin(); it!=points.end(); ++it) {
    (*it)[0]=position[0]+(((*it)[0]-((box[1][0]+box[0][0])/2.))/((box[1][0]-box[0][0])/2.))*size[0];
    (*it)[1]=position[1]+(((*it)[1]-((box[1][1]+box[0][1])/2.))/((box[1][1]-box[0][1])/2.))*size[1];
  }

  return points;
}

class StarGlyph : public Glyph {

public:

  GLYPHINFORMATION("2D - Star", "2D - Star extremity", "Antoine Lambert", "20/05/2016", "Star", "1.0", tlp::NodeShape::Star)

  StarGlyph(PluginContext *context) : Glyph(context) {
    _vertices = computeStar(Coord(0,0,0),Size(.5,.5,0),numberOfStarPoints);
    _vertices.insert(_vertices.begin(), Coord(0,0,0));
    for (unsigned short i = 0 ; i < ushort_cast(2*numberOfStarPoints-1) ; ++i) {
      _indices.insert(_indices.end(), {0, ushort_cast(i+1), ushort_cast(i+2)});
    }
    _indices.insert(_indices.end(), {0, 10, 1});

    vector<unsigned short> outlineIndices;
    for (unsigned short i = 1 ; i < ushort_cast(_vertices.size()) ; ++i) {
      outlineIndices.push_back(i);
    }
    outlineIndices.push_back(1);
    _outlinesIndices.push_back(outlineIndices);
  }

  void getIncludeBoundingBox(BoundingBox &boundingBox) {
    boundingBox[0] = Coord(-0.3f, -0.35f, 0);
    boundingBox[1] = Coord(0.3f, 0.35f, 0);
  }

};

PLUGIN(StarGlyph)
