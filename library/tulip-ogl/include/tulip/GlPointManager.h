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
      for(int i=0;i<3;i++)
        currentId[i]=0;
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

    inline void addPoint(const Coord &coord,const Color &color,int size) {
      assert(size==1 || size==2 || size==5);
      int indice=size-1;
      if(indice==4)
        indice=2;

      if(currentId[indice]>=maxId[indice]){
        copyToNewTabs(&indices[indice],&colors[indice],&coords[indice],maxId[indice],maxId[indice]*2);
        maxId[indice]=maxId[indice]*2;
      }
      indices[indice][currentId[indice]]=currentId[indice];
      coords[indice][currentId[indice]]=coord;
      colors[indice][currentId[indice]]=color;
      currentId[indice]++;
    }

    void endRendering();

  private:

    /**
     * Private constructor for singleton
     */
    GlPointManager():isBegin(false){
      for(int i=0;i<3;i++){
        maxId[i]=1024;
        indices[i]= new GLuint[1024];
        colors[i] = new Color[1024];
        coords[i] = new Coord[1024];
      }
    }

    static GlPointManager* inst;

    bool isBegin;

    unsigned int currentId[3];
    unsigned int maxId[3];

    GLuint *indices[3];
    Color *colors[3];
    Coord *coords[3];

  };

}

#endif // Tulip_GLEWMANAGER_H
