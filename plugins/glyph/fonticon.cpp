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
#include <GL/glew.h>

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#endif
#include <FTVectoriser.h>
#include <FTLibrary.h>
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#include <tulip/Glyph.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/TlpTools.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/TulipFontAwesome.h>
#include <tulip/TulipMaterialDesignIcons.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/GlTools.h>
#include <tulip/OpenGlConfigManager.h>
#include <tulip/GlTextureManager.h>
#include <tulip/TulipViewSettings.h>

#define ushort_cast(x) static_cast<unsigned short>((x))

#define HRES 64
#define HRESf 64.f
#define DPI 72

using namespace std;
using namespace tlp;

static const string defaultIcon = "fa-question-circle";

class FontIcon {

  string fontFile;
  unsigned int iconCodePoint;
  GLuint renderingDataBuffer;
  GLuint indicesBuffer;
  unsigned int nbVertices;
  unsigned int nbIndices;
  unsigned int nbOutlineIndices;
  BoundingBox boundingBox;

public:
  FontIcon()
      : iconCodePoint(0), renderingDataBuffer(0), indicesBuffer(0), nbVertices(0), nbIndices(0),
        nbOutlineIndices(0) {}

  FontIcon(const std::string &fontFile, unsigned int iconCodePoint)
      : fontFile(fontFile), iconCodePoint(iconCodePoint), renderingDataBuffer(0), indicesBuffer(0),
        nbVertices(0), nbIndices(0), nbOutlineIndices(0) {}

  ~FontIcon() {
    if (renderingDataBuffer != 0) {
      glDeleteBuffers(1, &renderingDataBuffer);
    }

    if (indicesBuffer != 0) {
      glDeleteBuffers(1, &indicesBuffer);
    }
  }

  void render(const tlp::Color &fillColor, const tlp::Color &outlineColor,
              const float outlineSize) {

    if (renderingDataBuffer == 0) {
      tesselateIcon();
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, renderingDataBuffer);
    glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
    glTexCoordPointer(2, GL_FLOAT, 0, BUFFER_OFFSET(nbVertices * 3 * sizeof(float)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
    setMaterial(fillColor);
    glDrawElements(GL_TRIANGLES, nbIndices, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    if (outlineSize > 0) {
      setMaterial(outlineColor);
      glLineWidth(outlineSize);
      glDrawElements(GL_LINES, nbOutlineIndices, GL_UNSIGNED_SHORT,
                     BUFFER_OFFSET(nbIndices * sizeof(unsigned short)));
    }

    glDisableClientState(GL_VERTEX_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  const BoundingBox &getBoundingBox() const {
    return boundingBox;
  }

private:
  void tesselateIcon() {

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

    err = FT_Set_Char_Size(face, int(size * HRES), 0, DPI *HRES, DPI *HRES);

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

    vector<Coord> vertices;
    vector<Vec2f> texCoords;
    vector<unsigned short> indices;
    vector<unsigned short> outlineIndices;

    map<Coord, unsigned int> vertexIdx;

    unsigned int idx = 0;

    for (unsigned int t = 0; t < mesh->TesselationCount(); ++t) {
      const FTTesselation *subMesh = mesh->Tesselation(t);

      for (unsigned int i = 0; i < subMesh->PointCount(); ++i) {
        FTPoint point = subMesh->Point(i);
        tlp::Coord p(point.Xf() / HRESf, point.Yf() / HRESf, 0.0f);

        if (vertexIdx.find(p) == vertexIdx.end()) {
          meshBB.expand(p);
          vertices.push_back(p);
          indices.push_back(idx++);
          vertexIdx[vertices.back()] = indices.back();
        } else {
          indices.push_back(vertexIdx[p]);
        }
      }
    }

    for (unsigned int t = 0; t < vectoriser.ContourCount(); ++t) {
      const FTContour *contour = vectoriser.Contour(t);

      for (unsigned int i = 0; i < contour->PointCount() - 1; ++i) {
        FTPoint point = contour->Point(i);
        tlp::Coord p(point.Xf() / HRESf, point.Yf() / HRESf, 0.0f);
        outlineIndices.push_back(ushort_cast(vertexIdx[p]));
        point = contour->Point(i + 1);
        p = Coord(point.Xf() / HRESf, point.Yf() / HRESf, 0.0f);
        outlineIndices.push_back(ushort_cast(vertexIdx[p]));
      }

      FTPoint point = contour->Point(contour->PointCount() - 1);
      tlp::Coord p(point.Xf() / HRESf, point.Yf() / HRESf, 0.0f);
      outlineIndices.push_back(ushort_cast(vertexIdx[p]));
      point = contour->Point(0);
      p = Coord(point.Xf() / HRESf, point.Yf() / HRESf, 0.0f);
      outlineIndices.push_back(ushort_cast(vertexIdx[p]));
    }

    tlp::Coord minC = meshBB[0];
    tlp::Coord maxC = meshBB[1];

    for (size_t i = 0; i < vertices.size(); ++i) {
      if (meshBB.height() > meshBB.width()) {
        vertices[i][0] = ((vertices[i][0] - minC[0]) / (maxC[0] - minC[0]) - 0.5) *
                         (meshBB.width() / float(meshBB.height()));
        vertices[i][1] = ((vertices[i][1] - minC[1]) / (maxC[1] - minC[1])) - 0.5;
      } else {
        vertices[i][0] = ((vertices[i][0] - minC[0]) / (maxC[0] - minC[0])) - 0.5;
        vertices[i][1] = (((vertices[i][1] - minC[1]) / (maxC[1] - minC[1])) - 0.5) *
                         (meshBB.height() / float(meshBB.width()));
      }

      const tlp::Coord &v = vertices[i];
      texCoords.push_back(Vec2f(v[0] + 0.5, v[1] + 0.5));
    }

    glGenBuffers(1, &renderingDataBuffer);
    glGenBuffers(1, &indicesBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, renderingDataBuffer);
    glBufferData(GL_ARRAY_BUFFER, (vertices.size() * 3 + texCoords.size() * 2) * sizeof(float),
                 NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * 3 * sizeof(float), &vertices[0]);
    glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(float),
                    texCoords.size() * 2 * sizeof(float), &texCoords[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 (indices.size() + outlineIndices.size()) * sizeof(unsigned short), NULL,
                 GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(unsigned short),
                    &indices[0]);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short),
                    outlineIndices.size() * sizeof(unsigned short), &outlineIndices[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    nbVertices = vertices.size();
    nbIndices = indices.size();
    nbOutlineIndices = outlineIndices.size();

    for (size_t i = 0; i < vertices.size(); ++i) {
      boundingBox.expand(vertices[i]);
    }
  }
};

static map<string, FontIcon> fontIcons;

static void drawIcon(const string &iconName, const string &fontFile,
                     const unsigned int iconCodePoint, const Color &color,
                     const Color &outlineColor, const float outlineSize, const string &texture) {
  if (fontIcons.find(iconName) == fontIcons.end()) {
    fontIcons[iconName] = FontIcon(fontFile, iconCodePoint);
  }

  if (texture != "") {
    GlTextureManager::getInst().activateTexture(texture);
  }

  fontIcons[iconName].render(color, outlineColor, outlineSize);

  GlTextureManager::getInst().desactivateTexture();
}

class FontIconGlyph : public Glyph {

public:
  GLYPHINFORMATION("2D - Icon", "Antoine Lambert", "26/02/2015", "Icon", "2.0", NodeShape::Icon)

  FontIconGlyph(const tlp::PluginContext *context = NULL) : Glyph(context) {}

  ~FontIconGlyph() {}

  void draw(node n, float) {

    string iconName = getNodeIcon(n);

    const tlp::Color &nodeColor = glGraphInputData->getElementColor()->getNodeValue(n);
    const tlp::Color &nodeBorderColor = glGraphInputData->getElementBorderColor()->getNodeValue(n);
    float nodeBorderWidth = glGraphInputData->getElementBorderWidth()->getNodeValue(n);
    const string &nodeTexture = glGraphInputData->parameters->getTexturePath() +
                                glGraphInputData->getElementTexture()->getNodeValue(n);

    if (iconName.substr(0, 3) == "fa-") {
      drawIcon(iconName, TulipFontAwesome::getFontAwesomeTrueTypeFileLocation(),
               TulipFontAwesome::getFontAwesomeIconCodePoint(iconName), nodeColor, nodeBorderColor,
               nodeBorderWidth, nodeTexture);
    } else {
      drawIcon(iconName, TulipMaterialDesignIcons::getMaterialDesignIconsTrueTypeFileLocation(),
               TulipMaterialDesignIcons::getMaterialDesignIconCodePoint(iconName), nodeColor,
               nodeBorderColor, nodeBorderWidth, nodeTexture);
    }
  }

  void getIncludeBoundingBox(BoundingBox &boundingBox, node n) {
    string iconName = getNodeIcon(n);

    if (fontIcons.find(iconName) != fontIcons.end()) {
      boundingBox = fontIcons[iconName].getBoundingBox();
    }
  }

private:
  string getNodeIcon(node n) {
    StringProperty *viewIcon = glGraphInputData->getElementIcon();
    string iconName = viewIcon->getNodeValue(n);

    if (iconName.empty() || (!TulipFontAwesome::isFontAwesomeIconSupported(iconName) &&
                             !TulipMaterialDesignIcons::isMaterialDesignIconSupported(iconName))) {
      iconName = defaultIcon;
    }

    return iconName;
  }
};

PLUGIN(FontIconGlyph)

class EEFontIconGlyph : public EdgeExtremityGlyph {

public:
  GLYPHINFORMATION("2D - Icon extremity", "Antoine Lambert", "02/03/2015",
                   "Icon for edge extremities", "2.0", EdgeExtremityShape::Icon)

  EEFontIconGlyph(const tlp::PluginContext *context) : EdgeExtremityGlyph(context) {}

  void draw(edge e, node, const Color &glyphColor, const Color &borderColor, float) {
    StringProperty *viewIcon = edgeExtGlGraphInputData->getElementIcon();
    string iconName = viewIcon->getEdgeValue(e);

    if (iconName.empty() || (!TulipFontAwesome::isFontAwesomeIconSupported(iconName) &&
                             !TulipMaterialDesignIcons::isMaterialDesignIconSupported(iconName))) {
      iconName = defaultIcon;
    }

    string edgeTexture = edgeExtGlGraphInputData->parameters->getTexturePath() +
                         edgeExtGlGraphInputData->getElementTexture()->getEdgeValue(e);
    float borderWidth = edgeExtGlGraphInputData->getElementBorderWidth()->getEdgeValue(e);

    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);

    if (iconName.substr(0, 2) == "fa") {
      drawIcon(iconName, TulipFontAwesome::getFontAwesomeTrueTypeFileLocation(),
               TulipFontAwesome::getFontAwesomeIconCodePoint(iconName), glyphColor, borderColor,
               borderWidth, edgeTexture);
    } else {
      drawIcon(iconName, TulipMaterialDesignIcons::getMaterialDesignIconsTrueTypeFileLocation(),
               TulipMaterialDesignIcons::getMaterialDesignIconCodePoint(iconName), glyphColor,
               borderColor, borderWidth, edgeTexture);
    }
  }
};

PLUGIN(EEFontIconGlyph)
