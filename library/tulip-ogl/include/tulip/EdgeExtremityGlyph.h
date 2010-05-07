/*
 * EdgeExtremityGlyph.h
 *
 *  Created on: 6 avr. 2009
 *      Author: Jonathan Dubois
 *       Email: jonathan.dubois@labri.fr
 *
 *	This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 */

#ifndef EDGEEXTREMITYGLYPH_H_
#define EDGEEXTREMITYGLYPH_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/TulipRelease.h>
#include <tulip/WithDependency.h>
#include <tulip/WithParameter.h>
#include <tulip/Node.h>
#include <tulip/Edge.h>
#include <tulip/Color.h>
#include <tulip/Size.h>
#include <tulip/Coord.h>
#include <tulip/Matrix.h>
#include <tulip/Plugin.h>
#include <tulip/TemplateFactory.h>

namespace tlp {
typedef Matrix<float, 4> MatrixGL;
class GlGraphInputData;

class EdgeExtremityGlyphContext {
public:
	GlGraphInputData *glGraphInputData;
	///
	EdgeExtremityGlyphContext(Graph **s = NULL, GlGraphInputData *glGraphInputData = NULL, int LOF =
			5, int LOD = 5) :
		glGraphInputData(glGraphInputData) {
	}
	///
	~EdgeExtremityGlyphContext() {
	}
};

 class TLP_GL_SCOPE EdgeExtremityGlyph: public WithDependency, public WithParameter {
public:
	EdgeExtremityGlyph(EdgeExtremityGlyphContext *gc);
	virtual ~EdgeExtremityGlyph();

	virtual void draw(edge e, node n, const Color& glyphColor,const Color &borderColor, float lod) = 0;

	virtual void getTransformationMatrix(const Coord &src, const Coord &dest,
			const Size &glyphSize, MatrixGL& transformationMatrix, MatrixGL& scalingMatrix) = 0;
	//	virtual MatrixGL getTransformationMatrix(const Coord &src, const Coord &dest,
	//			const Size &glyphSize) = 0;
	//
	//	virtual MatrixGL getScalingMatrix(const Size &glyphSize) = 0;


protected:
	GlGraphInputData *edgeExtGlGraphInputData;
};

class TLP_GL_SCOPE EdgeExtremityGlyphFactory: public Plugin {
public:
	virtual ~EdgeExtremityGlyphFactory() {
	}
	///
	virtual EdgeExtremityGlyph *createPluginObject(EdgeExtremityGlyphContext *gc)=0;
	///
	virtual int getId() const=0;

	virtual std::string getMajor() const {
		return tlp::getMajor(getRelease());
	}
	virtual std::string getMinor() const {
		return tlp::getMinor(getRelease());
	}
	virtual std::string getTulipMajor() const {
		return tlp::getMajor(getTulipRelease());
	}
	virtual std::string getTulipMinor() const {
		return tlp::getMinor(getTulipRelease());
	}

	static TemplateFactory<EdgeExtremityGlyphFactory, EdgeExtremityGlyph,
			EdgeExtremityGlyphContext *> *factory;
	static void initFactory() {
		if (!factory) {
			factory = new TemplateFactory<EdgeExtremityGlyphFactory, EdgeExtremityGlyph,
					EdgeExtremityGlyphContext *> ;
		}
	}
};

class TLP_GL_SCOPE EdgeExtremityGlyphFrom3DGlyph: public EdgeExtremityGlyph {
public:
	EdgeExtremityGlyphFrom3DGlyph(EdgeExtremityGlyphContext *gc);
	virtual ~EdgeExtremityGlyphFrom3DGlyph();

	virtual void draw(edge e, node n, const Color& glyphColor, float lod,
			GlGraphInputData* inputData) {
	}

	virtual void getTransformationMatrix(const Coord &src, const Coord &dest,
			const Size &glyphSize, MatrixGL& transformationMatrix, MatrixGL& scalingMatrix);

	//	virtual MatrixGL getTransformationMatrix(const Coord &src, const Coord &dest,
	//			const Size &glyphSize);
	//
	//	virtual MatrixGL getScalingMatrix(const Size &glyphSize);
};

class TLP_GL_SCOPE EdgeExtremityGlyphFrom2DGlyph: public EdgeExtremityGlyph {
public:
	EdgeExtremityGlyphFrom2DGlyph(EdgeExtremityGlyphContext *gc);
	virtual ~EdgeExtremityGlyphFrom2DGlyph();

	virtual void draw(edge e, node n, const Color& glyphColor, float lod,
			GlGraphInputData* inputData) {
	}

	virtual void getTransformationMatrix(const Coord &src, const Coord &dest,
			const Size &glyphSize, MatrixGL& transformationMatrix, MatrixGL& scalingMatrix);
	//	virtual MatrixGL getTransformationMatrix(const Coord &src, const Coord &dest,
	//			const Size &glyphSize);
	//
	//	virtual MatrixGL getScalingMatrix(const Size &glyphSize);
};

}

#define EEGPLUGINFACTORY(T,C,N,A,D,I,R,ID,G)     \
class C##T##Factory:public tlp::T##Factory	 \
{                                                \
public:                                          \
  C##T##Factory(){				 \
    initFactory(); 			         \
    factory->registerPlugin(this);	         \
  }       					 \
  std::string getName() const { return std::string(N);}	 \
  std::string getGroup() const { return std::string(G);}	 \
  std::string getAuthor() const {return std::string(A);}	 \
  std::string getDate() const {return std::string(D);}	 \
  std::string getInfo() const {return std::string(I);}	 \
  std::string getRelease() const {return std::string(R);}\
  std::string getTulipRelease() const {return std::string(TULIP_RELEASE);}\
  int    getId() const {return ID;}		 \
  tlp::T * createPluginObject(tlp::EdgeExtremityGlyphContext *gc)	 \
  {						 \
    C *tmp = new C(gc);				 \
    return ((tlp::T *) tmp);			 \
  }						 \
};                                               \
extern "C" {                                            \
  C##T##Factory C##T##FactoryInitializer;               \
}

#define EEGLYPHPLUGINOFGROUP(C,N,A,D,I,R,ID,G) EEGPLUGINFACTORY(EdgeExtremityGlyph,C,N,A,D,I,R,ID,G)
#define EEGLYPHPLUGIN(C,N,A,D,I,R,ID) EEGLYPHPLUGINOFGROUP(C,N,A,D,I,R,ID,"")

#endif /* EDGEEXTREMITYGLYPH_H_ */
