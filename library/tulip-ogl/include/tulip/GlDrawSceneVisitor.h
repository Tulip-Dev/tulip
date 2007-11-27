//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 23/10/2007
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef Tulip_GLDRAWSCENEVISITOR_H
#define Tulip_GLDRAWSCENEVISITOR_H

#include "tulip/GlSceneVisitor.h"

#if (__GNUC__ < 3)
#include <hash_map>
#else
#include <ext/hash_map>
#endif

namespace tlp {
  
  class GlGraphInputData;

  class TLP_GL_SCOPE GlDrawSceneVisitor : public GlSceneVisitor{

  public:

    GlDrawSceneVisitor(stdext::hash_map<unsigned int,float>* lod,GlGraphInputData* inputData):inputData(inputData) {lodMap=lod;}

    virtual void visit(GlSimpleEntity *entity);
    virtual void visit(GlComplexeEntity *entity);
    virtual void visit(GlLayout *layout);

  private:
    
    stdext::hash_map<unsigned int,float>* lodMap;
    GlGraphInputData *inputData;

  };
  
}

#endif // Tulip_GLDRAWSCENEVISITOR_H
