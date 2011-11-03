/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

#ifndef GLCOMPLEXPOLYGON_H
#define GLCOMPLEXPOLYGON_H

#include <tulip/OpenGlConfigManager.h>

#ifdef WIN32
#include <windows.h>
#endif

#ifndef CALLBACK
#define CALLBACK
#endif

#include <vector>
#include <map>
#include <set>

#include <tulip/Color.h>
#include <tulip/Coord.h>
#include <tulip/tulipconf.h>

#include <tulip/GlSimpleEntity.h>

namespace tlp {

typedef struct {
  GLdouble x, y, z, r, g, b, a;
} VERTEX;

void CALLBACK beginCallback(GLenum which, GLvoid *polygonData);
void CALLBACK errorCallback(GLenum errorCode);
void CALLBACK endCallback(GLvoid *polygonData);
void CALLBACK vertexCallback(GLvoid *vertex, GLvoid *polygonData);
void CALLBACK combineCallback(GLdouble coords[3], VERTEX *d[4], GLfloat w[4], VERTEX** dataOut, GLvoid *polygonData);

/**
 * \addtogroup GlEntities
 */
/*@{*/
/**
 * Class to create a complex polygon (concave polygon or polygon with hole)
 * If you want to create a complex polygon you have 4 constructors :
 * Constructors with vector of coords : to create a complex polygon without hole
 *   - In this case you have two constructor : with and without outline color
 *   - You can create a polygon like this :
 * \code
 *     vector <Coord> coords;
 *     coords.push_back(Coord(0,0,0));
 *     coords.push_back(Coord(10,0,0));
 *     coords.push_back(Coord(10,10,0));
 *     coords.push_back(Coord(0,10,0));
 *     GlComplexPolygon *complexPolygon=new GlComplexPolygon(coords,Color(255,0,0,255));
 *     layer->addGlEntity(complexPolygon,"complexPolygon");
 * \endcode
 *
 * Constructors with vector of vector of Coords : to create a complex polygon with hole
 *   - In this case you have two constructor : with and without outline color
 *   - The first vector of coords is the polygon and others vector are holes
 *   - You can create a polygon with hole like this :
 * \code
 *     vector <vector <Coord> > coords;
 *     vector <Coord> polygon;
 *     vector <Coord> hole;
 *     polygon.push_back(Coord(0,0,0));
 *     polygon.push_back(Coord(10,0,0));
 *     polygon.push_back(Coord(10,10,0));
 *     polygon.push_back(Coord(0,10,0));
 *     hole.push_back(Coord(4,4,0));
 *     hole.push_back(Coord(6,4,0));
 *     hole.push_back(Coord(6,6,0));
 *     hole.push_back(Coord(4,6,0));
 *     coords.push_back(polygon);
 *     coords.push_back(hole);
 *     GlComplexPolygon *complexPolygon=new GlComplexPolygon(coords,Color(255,0,0,255));
 *     layer->addGlEntity(complexPolygon,"complexPolygon");
 * \endcode
 *
 * In constructors you can specify the polygon border style : polygonEdgesType parameter (0 -> straight lines, 1 -> catmull rom curves, 2 -> bezier curves)
 * You can also specify the texture name if you want to create a textured complex polygon
 *
 * In complex polygon you can add a smooth border : see activateQuadBorder(..) function
 * And you can specify the texture zoom : see setTextureZoom(...) function
 */
class TLP_GL_SCOPE GlComplexPolygon : public GlSimpleEntity {

  friend void CALLBACK beginCallback(GLenum which, GLvoid *polygonData);
  friend void CALLBACK errorCallback(GLenum errorCode);
  friend void CALLBACK endCallback(GLvoid *polygonData);
  friend void CALLBACK vertexCallback(GLvoid *vertex, GLvoid *polygonData);
  friend void CALLBACK combineCallback(GLdouble coords[3], VERTEX *d[4], GLfloat w[4], VERTEX** dataOut, GLvoid *polygonData);

public:
  /**
   * Default constructor
   */
  GlComplexPolygon() {}

  /**
   * Constructor with a vector of coords, a fill color, a polygon edges type(0 -> straight lines, 1 -> catmull rom curves, 2 -> bezier curves)
   * and a textureName if you want
   */
  GlComplexPolygon(const std::vector<Coord> &coords,Color fcolor,int polygonEdgesType=0,const std::string &textureName = "");

  /**
   *  Constructor with a vector of coords, a fill color, an outline color, a polygon edges type(0 -> straight lines, 1 -> catmull rom curves, 2 -> bezier curves)
   * and a textureName if you want
   */
  GlComplexPolygon(const std::vector<Coord> &coords,Color fcolor,Color ocolor,int polygonEdgesType=0,const std::string &textureName = "");

  /**
   * Constructor with a vector of vector of coords (the first vector of coord is the polygon and others vectors are holes in polygon), a fill color,
   * a polygon edges type(0 -> straight lines, 1 -> catmull rom curves, 2 -> bezier curves) and a textureName if you want
   */
  GlComplexPolygon(const std::vector<std::vector<Coord> >&coords,Color fcolor,int polygonEdgesType=0,const std::string &textureName = "");

  /**
   * Constructor with a vector of vector of coords (the first vector of coord is the polygon and others vectors are holes in polygon), a fill color, an outline color
   * a polygon edges type(0 -> straight lines, 1 -> catmull rom curves, 2 -> bezier curves) and a textureName if you want
   */
  GlComplexPolygon(const std::vector<std::vector<Coord> >&coords,Color fcolor,Color ocolor,int polygonEdgesType=0,const std::string &textureName = "");

  virtual ~GlComplexPolygon() {}

  /**
   * Define polygon according to one contour and compute tesselation.
   *
   * Use this method if your polygon is defined by one contour.
   * This method resets any previously defined contours and recompute tesselation.
   *
   * \param contour a vector of tlp::Coord defining the contour.
   * \param polygonEdgesType an integer determining the shape of the polygon edges (0 -> straight lines, 1 -> catmull rom curves, 2 -> bezier curves)
   */
  void setPolygonContour(const std::vector<Coord> &contour, int polygonEdgesType=0);

  /**
   * Define polygon according to several contours and compute tesselation.
   *
   * Use this method if your polygon is defined by several contours (e.g. polygon with holes).
   * This method resets any previously defined contours and recompute tesselation.
   *
   * \param contours a vector of vector of tlp::Coord defining the different contours.
   * \param polygonEdgesType an integer determining the shape of the polygon edges (0 -> straight lines, 1 -> catmull rom curves, 2 -> bezier curves)
   */
  void setPolygonContours(const std::vector<std::vector<Coord> > &contours, int polygonEdgesType=0);

  /**
   * Returns the contours defining the polygon.
   */
  const std::vector<std::vector<Coord> > &getPolygonContours() const;

  /**
   * Draw the complex polygon
   */
  virtual void draw(float lod,Camera *camera);

  /**
   * Set if the polygon is outlined or not
   */
  void setOutlineMode(const bool);

  /**
   * Set size of outline
   */
  void setOutlineSize(double size);

  /**
   * Get fill color of GlComplexPolygon
   */
  Color getFillColor() {
    return fillColor;
  }

  /**
   * Set fill color of GlComplexPolygon
   */
  void setFillColor(const Color &color) {
    fillColor=color;
  }

  /**
   * Get outline color of GlComplexPolygon
   */
  Color getOutlineColor() {
    return outlineColor;
  }

  /**
   * Set outline color of GlComplexPolygon
   */
  void setOutlineColor(const Color &color) {
    outlineColor=color;
  }

  /**
   * Get the texture zoom factor
   */
  float getTextureZoom() {
    return textureZoom;
  }

  /**
   * Get the textureName
   */
  std::string getTextureName();

  /**
   * Set the textureName
   */
  void setTextureName(const std::string &name);

  /**
   * Set the texture zoom factor
   * By default if you have a polygon with a size bigger than (1,1,0) the texture will be repeated
   * If you want to don't have this texture repeat you have to modify texture zoom
   * For example if you have a polygon with coords ((0,0,0),(5,0,0),(5,5,0),(0,5,0)) you can set texture zoom to 5. to don't have texture repeat
   */
  void setTextureZoom(float zoom) {
    textureZoom=zoom;
    runTesselation();
  }

  /**
   * Translate entity
   */
  virtual void translate(const Coord& mouvement);

  /**
   * Function to export data and type in XML
   */
  virtual void getXML(xmlNodePtr rootNode);

  /**
   * Function to export data in XML
   */
  virtual void getXMLOnlyData(xmlNodePtr rootNode);

  /**
   * Function to set data with XML
   */
  virtual void setWithXML(xmlNodePtr rootNode);


protected:

  /**
   * Add a new point in polygon
   */
  virtual void addPoint(const Coord& point);
  /**
   * Begin a new hole in the polygon
   */
  virtual void beginNewHole();

  void runTesselation();
  void createPolygon(const std::vector<Coord> &coords,int polygonEdgesType);
  void startPrimitive(GLenum primitive);
  void endPrimitive();
  void addVertex(const Coord &vertexCoord, const Vec2f &vertexTexCoord);
  VERTEX *allocateNewVertex();

  std::vector<std::vector<Coord> > polygonContours;
  std::vector<std::vector<Coord> > points;
  std::set<GLenum> primitivesSet;
  std::map<GLenum, std::vector<Coord> > verticesMap;
  std::map<GLenum, std::vector<Vec2f> > texCoordsMap;
  std::map<GLenum, std::vector<int> > startIndicesMap;
  std::map<GLenum, std::vector<int> > verticesCountMap;
  std::vector<VERTEX *> allocatedVertices;
  GLenum currentPrimitive;
  int nbPrimitiveVertices;
  int currentVector;
  bool outlined;
  Color fillColor;
  Color outlineColor;
  double outlineSize;
  std::string textureName;
  float textureZoom;
};
/*@}*/
}
#endif
