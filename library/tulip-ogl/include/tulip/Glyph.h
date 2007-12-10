//-*-c++-*-
#ifndef GLYPH_H
#define GLYPH_H

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
    GlyphContext(Graph **s=NULL, GlGraphInputData *glGraphInputData=NULL, int LOF=5, int LOD=5): 
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
      boundingBox.first=Coord(0,0,0);
      boundingBox.second=Coord(1,1,1);
    }
    virtual void draw(node)=0;
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

  class GlyphFactory: public Plugin {
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

    static TLP_GL_SCOPE TemplateFactory<GlyphFactory,Glyph,GlyphContext *> *factory;
    static TLP_GL_SCOPE void initFactory() {
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

#endif //GLYPH_H
