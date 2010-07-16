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

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

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

#include "tulip/GlSimpleEntity.h"

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
   * class to create a complex polygon (concave polygon or polygon with hole)
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
     * Draw the complex polygon
     */
    virtual void draw(float lod,Camera *camera);

    /**
     * Add a new point in polygon
     */
    virtual void addPoint(const Coord& point);
    /**
     * Begin a new hole in the polygon
     */
    virtual void beginNewHole();

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
    Color getFillColor() {return fillColor;}

    /**
     * Set fill color of GlComplexPolygon
     */
    void setFillColor(const Color &color){fillColor=color;}

    /**
     * Get outline color of GlComplexPolygon
     */
    Color getOutlineColor() {return outlineColor;}

    /**
     * Set outline color of GlComplexPolygon
     */
    void setOutlineColor(const Color &color){outlineColor=color;}

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
	
    void runTesselation();
    void createPolygon(const std::vector<Coord> &coords,int polygonEdgesType);

	void startPrimitive(GLenum primitive);
	void endPrimitive();
	void addVertex(const Coord &vertexCoord, const Color &vertexColor, const Vec2f &vertexTexCoord);
	VERTEX *allocateNewVertex();
	
    std::vector<std::vector<Coord> > points;
    std::set<GLenum> primitivesSet;
    std::map<GLenum, std::vector<Coord> > verticesMap;
    std::map<GLenum, std::vector<Color> > colorsMap;
    std::map<GLenum, std::vector<Vec2f> > texCoordsMap;
    std::map<GLenum, std::vector<int> >startIndicesMap;
    std::map<GLenum, std::vector<int> >verticesCountMap;
	std::vector<VERTEX *> allocatedVertices;
	GLenum currentPrimitive;
	int nbPrimitiveVertices;
    int currentVector;
    bool outlined;
    Color fillColor;
    Color outlineColor;
    double outlineSize;
    std::string textureName;
  };
}
#endif
