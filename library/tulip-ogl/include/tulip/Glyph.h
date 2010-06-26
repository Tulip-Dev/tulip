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
#ifndef GLYPH_H
#define GLYPH_H

#ifndef DOXYGEN_NOTFOR_DEVEL

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/Plugin.h>
#include <tulip/TulipRelease.h>
#include <tulip/WithParameter.h>
#include <tulip/WithDependency.h>
#include <tulip/Size.h>
#include <tulip/Coord.h>
#include <tulip/Color.h>
#include <tulip/BoundingBox.h>
#include <tulip/TlpTools.h>
#include <tulip/TemplateFactory.h>

#include "tulip/GlGraphInputData.h"


namespace tlp {

  class Graph;
  struct node;

  class GlyphContext {
  public:
    GlGraphInputData *glGraphInputData;
    ///
    GlyphContext(Graph ** =NULL, GlGraphInputData *glGraphInputData=NULL, int =5, int =5): 
      glGraphInputData(glGraphInputData) {
    }
    ///
    ~GlyphContext() {
    }
  };
  //==========================================================
  class TLP_GL_SCOPE Glyph : public WithParameter, public WithDependency {
  public:
    Glyph(GlyphContext *);
    virtual ~Glyph();
    virtual void getIncludeBoundingBox(BoundingBox &boundingBox) {
      boundingBox[0] = Coord(0,0,0);
      boundingBox[1] = Coord(1,1,1);
    }
    virtual void draw(node,float)=0;
    /*
     * return a point where an edge coming from "from" can be attached
     * by default, the point will be on the surface of the largest sphere contained
     * inside the unit cube (before scaling).
     */
    virtual Coord getAnchor(const Coord &nodeCenter, const Coord &from,
			    const Size &scale, const double zRotation) const;

 
  protected:
    GlGraphInputData *glGraphInputData;
  
  protected:
    /*
     * called by public method getAnchor to actually compute the anchor point
     * vector is coordinate of the point to anchor to, relative to nodecenter
     * glyph size is (1,1,1)
     * this is the method to redefine for each glyph where the default 'getAnchor' method
     * is inapropriated
     * Returned value is a vector to be applied to 'nodeCenter' in the public method
     */
    virtual Coord getAnchor(const Coord &vector) const;
  };

  class TLP_GL_SCOPE GlyphFactory: public Plugin {
  public:
    virtual ~GlyphFactory() {}
    ///
    virtual Glyph *createPluginObject(GlyphContext *gc)=0;
    ///
    virtual int getId() const=0;
    
    virtual  std::string getMajor() const {
      return tlp::getMajor(getRelease());
    }
    virtual  std::string getMinor() const  {
      return tlp::getMinor(getRelease());
    }
    virtual  std::string getTulipMajor() const {
      return tlp::getMajor(getTulipRelease());
    }
    virtual  std::string getTulipMinor() const  {
      return tlp::getMinor(getTulipRelease());
    }

    static TemplateFactory<GlyphFactory,Glyph,GlyphContext *> *factory;
    static void initFactory() {
      if (!factory) {
	factory = new TemplateFactory<GlyphFactory,Glyph,GlyphContext *>;
      }
    }
  };

}

#define GPLUGINFACTORY(T,C,N,A,D,I,R,ID,G)     \
class C##T##Factory:public tlp::T##Factory	 \
{                                                \
public:                                          \
  C##T##Factory(){				 \
    initFactory(); 			         \
    factory->registerPlugin(this);	         \
  }       					 \
  string getName() const { return string(N);}	 \
  string getGroup() const { return string(G);}	 \
  string getAuthor() const {return string(A);}	 \
  string getDate() const {return string(D);}	 \
  string getInfo() const {return string(I);}	 \
  string getRelease() const {return string(R);}\
  string getTulipRelease() const {return string(TULIP_RELEASE);}\
  int    getId() const {return ID;}		 \
  tlp::T * createPluginObject(tlp::GlyphContext *gc)	 \
  {						 \
    C *tmp = new C(gc);				 \
    return ((tlp::T *) tmp);			 \
  }						 \
};                                               \
extern "C" {                                            \
  C##T##Factory C##T##FactoryInitializer;               \
}

#define GLYPHPLUGINOFGROUP(C,N,A,D,I,R,ID,G) GPLUGINFACTORY(Glyph,C,N,A,D,I,R,ID,G)
#define GLYPHPLUGIN(C,N,A,D,I,R,ID) GLYPHPLUGINOFGROUP(C,N,A,D,I,R,ID,"") 

#endif // DOXYGEN_NOTFOR_DEVEL

#endif //GLYPH_H
