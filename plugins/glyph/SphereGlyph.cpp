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
 * Copyright (c) 2016 Antoine Lambert, Thales Services SAS
 * antoine-e.lambert@thalesgroup.com / antoine.lambert33@gmail.com
 *
 */

#include <tulip/Glyph.h>
#include <tulip/TulipViewSettings.h>

using namespace tlp;
using namespace std;

static void triangleStripIndicesToTrianglesIndices(const vector<unsigned short> &triangleStripIndices, vector<unsigned short> &trianglesIndices) {
  unsigned int nbTriangles = triangleStripIndices.size() - 3 + 1;
  trianglesIndices.resize(nbTriangles*3);

  for (unsigned int i = 0 ; i < nbTriangles ; ++i) {
    if ((i%2)==0) {
      trianglesIndices[i*3+0] = triangleStripIndices[i+0];
      trianglesIndices[i*3+1] = triangleStripIndices[i+1];
      trianglesIndices[i*3+2] = triangleStripIndices[i+2];
    } else {
      trianglesIndices[i*3+0] = triangleStripIndices[i+2];
      trianglesIndices[i*3+1] = triangleStripIndices[i+1];
      trianglesIndices[i*3+2] = triangleStripIndices[i+0];
    }
  }
}

class SphereGlyph : public Glyph {

public:

  GLYPHINFORMATION("3D - Sphere", "3D - Sphere extremity", "Antoine Lambert", "20/05/2016", "Sphere", "1.0", tlp::NodeShape::Sphere)

  SphereGlyph(PluginContext *context) : Glyph(context) {
    generateSphereData(9);
  }

  void getIncludeBoundingBox(BoundingBox &boundingBox) {
    boundingBox[0] = Coord(-0.35f, -0.35f, -0.35f);
    boundingBox[1] = Coord(0.35f, 0.35f, 0.35f);
  }

  void getTextBoundingBox(tlp::BoundingBox &boundingBox) {
    Glyph::getIncludeBoundingBox(boundingBox);
  }

  bool glyph2D() const {
    return false;
  }

  const vector<Coord> &getGlyphNormals() {
    return _vertices;
  }

private:

  void generateSphereData(int space) {
    unsigned int vertexCount = (90 / space) * (360 / space) * 4;

    _vertices.resize(vertexCount*2);

    double PI = M_PI;

    vector<unsigned short> indicesTriangleStrip;
    indicesTriangleStrip.resize(vertexCount*2+1);

    int n = 0;

    for(float j = 0; j <= 90 - space; j+=space) {
      for(float i = 0; i <= 360 - space; i+=space) {
        indicesTriangleStrip[n]=n;
        indicesTriangleStrip[n+1]=n+1;
        indicesTriangleStrip[n+2]=n+2;
        indicesTriangleStrip[n+3]=n+3;

        indicesTriangleStrip[vertexCount*2-n]=n+vertexCount;
        indicesTriangleStrip[vertexCount*2-n-1]=n+vertexCount+1;
        indicesTriangleStrip[vertexCount*2-n-2]=n+vertexCount+2;
        indicesTriangleStrip[vertexCount*2-n-3]=n+vertexCount+3;

        float x = sin((i) / 180 * PI) * sin((j) / 180 * PI) /2.;
        float y = cos((i) / 180 * PI) * sin((j) / 180 * PI) /2.;
        float z = -cos((j) / 180 * PI) /2.;

        _vertices[n] = Coord(x, y, z);
        _vertices[vertexCount+n] = Coord(x, y, -z);

        n++;

        x = sin((i) / 180 * PI) * sin((j + space) / 180 * PI) /2.;
        y = cos((i) / 180 * PI) * sin((j + space) / 180 * PI) /2.;
        z = -cos((j + space) / 180 * PI) /2.;

        _vertices[n] = Coord(x, y, z);
        _vertices[vertexCount+n] = Coord(x, y, -z);

        n++;

        x = sin((i + space) / 180 * PI) * sin((j) / 180 * PI) /2.;
        y = cos((i + space) / 180 * PI) * sin((j) / 180 * PI) /2.;
        z = -cos((j) / 180 * PI) /2.;

        _vertices[n] = Coord(x, y, z);
        _vertices[vertexCount+n] = Coord(x, y, -z);

        n++;

        x = sin((i + space) / 180 * PI) * sin((j + space) / 180 * PI) /2.;
        y = cos((i + space) / 180 * PI) * sin((j + space) / 180 * PI) /2.;
        z = -cos((j + space) / 180 * PI) /2.;

        _vertices[n] = Coord(x, y, z);
        _vertices[vertexCount+n] = Coord(x, y, -z);

        n++;
      }
    }

    indicesTriangleStrip[vertexCount]=vertexCount*2-1;
    triangleStripIndicesToTrianglesIndices(indicesTriangleStrip, _indices);
  }

};

PLUGIN(SphereGlyph)
