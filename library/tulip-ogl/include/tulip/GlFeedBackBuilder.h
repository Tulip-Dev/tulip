/*
 *
 * This file is part of Tulip (www.tulip-software.org)
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
///@cond DOXYGEN_HIDDEN

#ifndef Tulip_GLFEEDBACKBUILDER_H
#define Tulip_GLFEEDBACKBUILDER_H

#if defined(_MSC_VER)
#include <Windows.h>
#endif

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <tulip/tulipconf.h>
#include <tulip/Vector.h>

namespace tlp {

//====================================================
typedef struct _FeedBack3Dcolor {
  GLfloat x;
  GLfloat y;
  GLfloat z;
  GLfloat red;
  GLfloat green;
  GLfloat blue;
  GLfloat alpha;
} Feedback3Dcolor;

//====================================================
typedef struct _DepthIndex {
  GLfloat *ptr;
  GLfloat depth;
} DepthIndex;

//====================================================
/**
 * Abstract class used to build a object with a OpenGl feedback buffer
 */
class TLP_GL_SCOPE GlFeedBackBuilder {

public:

  virtual ~GlFeedBackBuilder() {}

  virtual void begin(const Vector<int, 4> &) {}
  virtual void passThroughToken(GLfloat*) {}
  virtual void pointToken(GLfloat*) {}
  virtual void lineToken(GLfloat*) {}
  virtual void lineResetToken(GLfloat*) {}
  virtual void polygonToken(GLfloat*) {}
  virtual void bitmapToken(GLfloat*) {}
  virtual void drawPixelToken(GLfloat*) {}
  virtual void copyPixelToken(GLfloat*) {}
  virtual void end() {}

  virtual void getResult(std::string* str) = 0;

};

}

#endif // Tulip_GLFEEDBACKBUILDER_H
///@endcond
