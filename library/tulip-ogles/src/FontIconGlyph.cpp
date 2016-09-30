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

#include <GL/glew.h>

#include <FTVectoriser.h>

#include <FTLibrary.h>

#include <tulip/FontIconGlyph.h>
#include <tulip/TulipFontAwesome.h>
#include <tulip/BoundingBox.h>
#include <tulip/TlpTools.h>
#include <tulip/GlUtils.h>
#include <tulip/TulipMaterialDesignIcons.h>

#include <map>

#define HRES 64
#define HRESf 64.f
#define DPI 72

using namespace std;
using namespace tlp;

FontIconGlyph::FontIconGlyph(PluginContext *context, const std::string &fontFile, unsigned int iconCodePoint) : Glyph(context) {

  const FT_Library *library = FTLibrary::Instance().GetLibrary();

  FT_Face face;

  FT_Error err = FT_New_Face(*library, fontFile.c_str(), 0, &face);

  if (err) {
    return;
  }

  err = FT_Select_Charmap(face, FT_ENCODING_UNICODE);

  if (err) {
    return;
  }

  float size = 20;

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
  for (unsigned int t = 0; t < mesh->TesselationCount(); ++t) {
    const FTTesselation *subMesh = mesh->Tesselation(t);
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

  for (unsigned int t = 0; t < vectoriser.ContourCount(); ++t) {
    vector<unsigned short> outlineIndices;
    const FTContour *contour = vectoriser.Contour(t);
    for (unsigned int i = 0; i < contour->PointCount(); ++i) {
      FTPoint point = contour->Point(i);
      tlp::Coord p(point.Xf() / HRESf, point.Yf() / HRESf, 0.0f);
      outlineIndices.push_back(ushort_cast(vertexIdx[p]));
    }
    FTPoint point = contour->Point(0);
    tlp::Coord p(point.Xf() / HRESf, point.Yf() / HRESf, 0.0f);
    outlineIndices.push_back(ushort_cast(vertexIdx[p]));
    _outlinesIndices.push_back(outlineIndices);
  }

  tlp::Coord minC = meshBB[0];
  tlp::Coord maxC = meshBB[1];

  for (size_t i = 0; i < _vertices.size(); ++i) {
    if (meshBB.height() > meshBB.width()) {
      _vertices[i][0] = ((_vertices[i][0] - minC[0]) / (maxC[0] - minC[0]) - 0.5) * (meshBB.width() / float(meshBB.height()));
      _vertices[i][1] = ((_vertices[i][1] - minC[1]) / (maxC[1] - minC[1])) - 0.5;
    } else {
      _vertices[i][0] = ((_vertices[i][0] - minC[0]) / (maxC[0] - minC[0])) - 0.5;
      _vertices[i][1] = (((_vertices[i][1] - minC[1]) / (maxC[1] - minC[1])) - 0.5) * (meshBB.height() / float(meshBB.width()));
    }
  }
}

FontAwesomeGlyph::FontAwesomeGlyph(PluginContext *context, unsigned int iconCodePoint)
    : FontIconGlyph(context, TulipFontAwesome::getFontAwesomeTrueTypeFileLocation(), iconCodePoint) {
}

MaterialDesignIconGlyph::MaterialDesignIconGlyph(PluginContext *context, unsigned int iconCodePoint)
    : FontIconGlyph(context, TulipMaterialDesignIcons::getMaterialDesignIconsTrueTypeFileLocation(), iconCodePoint) {
}

PLUGIN(FontAwesomeGlyph)
PLUGIN(MaterialDesignIconGlyph)
