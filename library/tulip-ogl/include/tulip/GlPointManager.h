//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 17/2/2009
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/
#ifndef Tulip_GLPOINTMANAGER_H
#define Tulip_GLPOINTMANAGER_H

#include <cassert>
#include <iostream>
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>
#include <tulip/Coord.h>
#include <tulip/Color.h>

namespace tlp {

  /** \brief Singleton used to manage Glew
   *
   * Singleton used to manage Glew.
   */
  class TLP_GL_SCOPE GlPointManager {

  public:

    /**
     * Return the current instance. If instance doesn't exist, create it.
     */
    static GlPointManager &getInst() {
      if(!inst)
        inst=new GlPointManager();
      return *inst;
    }

    inline bool renderingIsBegin(){
      return isBegin;
    }

    inline void beginRendering() {
      isBegin=true;
      currentId1=0;
      currentId2=0;
    }

    inline void copyToNewTabs(GLuint **indices,Color **colors,Coord **coords,unsigned int oldSize, unsigned int newSize){
      GLuint *newIndices=new GLuint[newSize];
      Color *newColors=new Color[newSize];
      Coord *newCoords=new Coord[newSize];
      memcpy(newIndices,*indices,oldSize*sizeof(GLuint));
      memcpy(newColors,*colors,oldSize*sizeof(Color));
      memcpy(newCoords,*coords,oldSize*sizeof(Coord));
      delete[] *indices;
      delete[] *colors;
      delete[] *coords;
      *indices=newIndices;
      *colors=newColors;
      *coords=newCoords;
    }

    inline void addPoint(const Coord &coord,const Color &color,bool size) {
      if(!size){
        if(currentId1>=maxId1){
          copyToNewTabs(&indices1,&colors1,&coords1,maxId1,maxId1*2);
          maxId1=maxId1*2;
        }
        indices1[currentId1]=currentId1;
        coords1[currentId1]=coord;
        colors1[currentId1]=color;
        currentId1++;
      }else{
        if(currentId2>=maxId2){
          copyToNewTabs(&indices2,&colors2,&coords2,maxId2,maxId2*2);
          maxId2=maxId2*2;
        }
        indices2[currentId2]=currentId2;
        coords2[currentId2]=coord;
        colors2[currentId2]=color;
        currentId2++;
      }
    }

    void endRendering();

  private:

    /**
     * Private constructor for singleton
     */
    GlPointManager():isBegin(false){
      maxId1=1024;
      maxId2=1024;
      indices1 = new GLuint[1024];
      colors1 = new Color[1024];
      coords1 = new Coord[1024];
      indices2 = new GLuint[1024];
      colors2 = new Color[1024];
      coords2 = new Coord[1024];
    }

    static GlPointManager* inst;

    bool isBegin;

    unsigned int currentId1;
    unsigned int currentId2;
    unsigned int maxId1;
    unsigned int maxId2;

    GLuint *indices1;
    Color *colors1;
    Coord *coords1;
    GLuint *indices2;
    Color *colors2;
    Coord *coords2;

  };

}

#endif // Tulip_GLEWMANAGER_H
