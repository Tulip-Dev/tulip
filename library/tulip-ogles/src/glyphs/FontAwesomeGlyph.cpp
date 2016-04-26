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

#include <GL/glew.h>

#include <tulip/glyphs/FontAwesomeGlyph.h>

#include <FTVectoriser.h>

#include <FTLibrary.h>

#include <tulip/TulipFontAwesome.h>
#include <tulip/BoundingBox.h>
#include <tulip/TlpTools.h>

#include <map>

#define HRES  64
#define HRESf 64.f
#define DPI   72

using namespace tlp;

FontAwesomeGlyph::FontAwesomeGlyph(unsigned int iconCodePoint) {

  const FT_Library* library = FTLibrary::Instance().GetLibrary();

  FT_Error err;

  FT_Face face;

  /* Load face */

  err = FT_New_Face(*library, TulipFontAwesome::getFontAwesomeTrueTypeFileLocation().c_str(), 0, &face);

  if (err) {
    return;
  }

  /* Select charmap */
  err = FT_Select_Charmap(face, FT_ENCODING_UNICODE);

  if (err) {
    return;
  }

  float size = 20;

  /* Set char size */
  err = FT_Set_Char_Size(face, static_cast<int>(size * HRES), 0, DPI * HRES, DPI * HRES);

  if (err) {
    return;
  }

  FT_UInt glyph_index = FT_Get_Char_Index(face, iconCodePoint);

  if (err) {
    return;
  }

  err = FT_Load_Glyph(face, glyph_index, FT_LOAD_NO_HINTING);

  if (err) {
    return;
  }

  FTVectoriser vectoriser(face->glyph);

  vectoriser.MakeMesh(1.0, 1, 0.0);

  const FTMesh *mesh = vectoriser.GetMesh();

  tlp::BoundingBox meshBB;

  std::map<tlp::Coord, unsigned int> vertexIdx;

  unsigned int idx = 0;
  for(unsigned int t = 0; t < mesh->TesselationCount(); ++t) {
    const FTTesselation* subMesh = mesh->Tesselation(t);
    for (unsigned int i = 0; i < subMesh->PointCount(); ++i) {
      FTPoint point = subMesh->Point(i);
      tlp::Coord p(point.Xf() / HRESf, point.Yf() / HRESf, 0.0f);
      if (vertexIdx.find(p) == vertexIdx.end()) {
        meshBB.expand(p);
        _vertices.push_back(p);
        _indices.push_back(idx++);
        vertexIdx[_vertices.back()] = _indices.back();
      } else {
        _indices.push_back(vertexIdx[p]);
      }
    }
  }

  for (unsigned int t = 0 ; t < vectoriser.ContourCount() ; ++t)  {
    const FTContour* contour = vectoriser.Contour(t);
    for (unsigned int i = 0 ; i < contour->PointCount() ; ++i) {
      FTPoint point = contour->Point(i);
      tlp::Coord p(point.Xf() / HRESf, point.Yf() / HRESf, 0.0f);
      FTPoint point2;
      if (i < contour->PointCount() - 1) {
        point2 = contour->Point(i+1);
      } else {
        point2 = contour->Point(0);
      }
      tlp::Coord p2(point2.Xf() / HRESf, point2.Yf() / HRESf, 0.0f);
      _outlineIndices.push_back(vertexIdx[p]);
      _outlineIndices.push_back(vertexIdx[p2]);
    }
  }


  tlp::Coord minC = meshBB[0];
  tlp::Coord maxC = meshBB[1];

  for (size_t i = 0 ; i < _vertices.size() ; ++i) {
    if (meshBB.height() > meshBB.width()) {
      _vertices[i][0] = ((_vertices[i][0] - minC[0]) / (maxC[0]-minC[0]) - 0.5) * (meshBB.width() / float(meshBB.height()));
      _vertices[i][1] = ((_vertices[i][1] - minC[1]) / (maxC[1]-minC[1])) - 0.5;
    } else {
      _vertices[i][0] = ((_vertices[i][0] - minC[0]) / (maxC[0]-minC[0])) - 0.5;
      _vertices[i][1] = (((_vertices[i][1] - minC[1]) / (maxC[1]-minC[1])) - 0.5) * (meshBB.height() / float(meshBB.width()));
    }

  }

}
