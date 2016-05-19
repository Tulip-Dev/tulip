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

#include <cmath>

using namespace tlp;
using namespace std;

//=============================================
void Glyph::getIncludeBoundingBox(BoundingBox &boundingBox) {
  boundingBox[0] = Coord(-0.5,-0.5,-0.5);
  boundingBox[1] = Coord(0.5,0.5,0.5);
}
//=============================================
Coord Glyph::getAnchor(const Coord &nodeCenter, const Coord &from, const Size &scale, const double zRotation) const {
  Coord anchor = from - nodeCenter;

  if( anchor.getX() == 0.0f && anchor.getY() == 0.0f)
    return nodeCenter;

  if( scale.getW() == 0.0f || scale.getH() == 0.0f)
    return nodeCenter;

  if(zRotation!=0) {
    //unrotate
    Coord saveAnchor(anchor);
    double zRot =  - 2.0*M_PI * zRotation / 360.0;
    anchor[0] = saveAnchor[0]*cos(zRot) - saveAnchor[1]*sin(zRot);
    anchor[1] = saveAnchor[0]*sin(zRot) + saveAnchor[1]*cos(zRot);
  }

  // unscale
  anchor.setX( anchor.getX() / scale.getW() );
  anchor.setY( anchor.getY() / scale.getH() );

  if(scale.getD() != 0.0f)
    anchor.setZ( anchor.getZ() / scale.getD() );
  else
    anchor.setZ(0.0f);

  anchor = getAnchor( anchor );

  // rescale
  anchor.setX( anchor.getX() * scale.getW() );
  anchor.setY( anchor.getY() * scale.getH() );
  anchor.setZ( anchor.getZ() * scale.getD() );

  if(zRotation!=0) {
    //rerotate
    Coord saveAnchor(anchor);
    double zRot = 2.0*M_PI * zRotation / 360.0;
    anchor[0] = saveAnchor[0]*cos(zRot) - saveAnchor[1]*sin(zRot);
    anchor[1] = saveAnchor[0]*sin(zRot) + saveAnchor[1]*cos(zRot);
  }

  return nodeCenter + anchor;
}
//=============================================
Coord Glyph::getAnchor(const Coord &vector) const {
  Coord anchor = vector;
  /* anchor must be on the surface of a sphere centered on nodecenter, radius is 0.5 */
  anchor *= 0.5f / anchor.norm();
  return anchor;
}
//=============================================
const vector<Vec2f> &Glyph::getGlyphTexCoords() {
  if (_texCoords.empty()) {
    for (size_t i = 0 ; i < _vertices.size() ; ++i) {
      tlp::Vec2f st;
      st[0] = _vertices[i][0] + 0.5f;
      st[1] = _vertices[i][1] + 0.5f;
      _texCoords.push_back(st);
    }
  }
  return _texCoords;
}

const std::vector<tlp::Coord> &Glyph::getGlyphNormals() {
  if (_normals.empty() && !glyph2D()) {
    _normals.resize(_vertices.size(), Coord(0,0,0));
    for (size_t i = 0 ; i < _indices.size() ; i += 3) {
      Coord v1 = _vertices[_indices[i]], v2 = _vertices[_indices[i+1]], v3 = _vertices[_indices[i+2]];
      Coord normal = (v2 - v1) ^ (v3 - v1);
      //            if (v1[2] <= 0 && v2[2] <= 0 && v3[2] <= 0)
      //              normal = (v3 - v1) ^ (v2 - v1);
      if (normal.norm() != 0) {
        normal /= normal.norm();
      }
      _normals[_indices[i]] += normal;
      _normals[_indices[i+1]] += normal;
      _normals[_indices[i+2]] += normal;
    }
    for (size_t i = 0 ; i < _normals.size() ; ++i) {
      if (_normals[i].norm() != 0) {
        _normals[i] /= _normals[i].norm();
      }
    }
  }
  return _normals;
}
