//-*-c++-*-
/*
 Author: Didier Bathily, Nicolas Bellino, Jonathan Dubois, Christelle Jolly, Antoine Lambert, Nicolas Sorraing

 Email : didier.bathily@etu.u-bordeaux1.fr, nicolas.bellino@etu.u-bordeaux1.fr, jonathan.dubois@etu.u-bordeaux1.fr, christelle.jolly@etu.u-bordeaux1.fr, antoine.lambert@etu.u-bordeaux1.fr, nicolas.sorraing@etu.u-bordeaux1.fr

 Last modification : 03/08

 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/

#ifndef GL_CYLINDER_H
#define GL_CYLINDER_H

#include <tulip/tulipconf.h>

#include <tulip/Coord.h>
#include <tulip/Color.h>

#include <tulip/GlSimpleEntity.h>

using namespace std;

namespace tlp {

  /** \brief General class used to render cylinders as GlEntity.
   *
   * This class is a generic class to render cylinders as GlEntity.
   */
  class TLP_GL_SCOPE GlCylinder : public GlSimpleEntity {

  public :

	/**
	 * Constructor for a non textured cylinder
	 *
	 * \param start The first end of the cylinder
	 * \param end The other end of the cylinder
	 * \param startRadius The cylinder's radius at the first end
	 * \param endRadius The cylinder's radius at the other end (set it to 0 to draw a cone)
	 * \param color The cylinder's color
	 * \param withCaps If true draw cylinder caps
	 */
    GlCylinder(const Coord &start, const Coord &end, const float startRadius, const float endRadius, const Color &color, const bool withCaps=true);


    /**
  	 * Constructor for a textured cylinder
   	 *
   	 * \param start The first end of the cylinder
   	 * \param end The other end of the cylinder
   	 * \param startRadius The cylinder's radius at the first end
   	 * \param endRadius The cylinder's radius at the other end (set it to 0 to draw a cone)
   	 * \param textureFile The absolute path of the texture image file to use
   	 * \param color The cylinder's color
   	 * \param withCaps If true draw cylinder caps
   	 */
    GlCylinder(const Coord &start, const Coord &end, const float startRadius, const float endRadius, const string& textureFile, const Color &color, const bool withCaps=true);

    /**
     * Virtual function used to draw the box.
     */
    void draw(float lod,Camera *camera);

    /**
     * Function to export data in XML
     */
    void getXML(xmlNodePtr rootNode) ;

    /**
     * Function to set data with XML
     */
    void setWithXML(xmlNodePtr rootNode) ;

  private:

    void computeVectors();

    Coord startPoint; // coordinates of the first end of the cylinder
    Coord endPoint;  // coordinates of the other end of the cylinder

    float startRadius;
    float endRadius;

    float cylinderLength;

    // orientation vector
    float vx;
    float vy;
    float vz;

    // rotation vector
    float rx;
    float ry;
    float rz;

    bool displayCaps;

    Color color;
    string textureFile;


  };

}

#endif // GL_CYLINDER_H
