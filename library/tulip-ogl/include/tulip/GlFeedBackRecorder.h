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
#ifndef Tulip_GLFEEDBACKRECORDER_H
#define Tulip_GLFEEDBACKRECORDER_H

#include <tulip/tulipconf.h>

#include <tulip/Vector.h>

#include "tulip/GlFeedBackBuilder.h"

namespace tlp {
  
  class TLP_GL_SCOPE GlFeedBackRecorder {
  
  public:
    
    GlFeedBackRecorder(GlFeedBackBuilder *builder):
      feedBackBuilder(builder) {}
    void record(bool doSort, GLint, GLfloat *,const Vector<int,4>& viewport);
    
  private:

    void sortAndRecord(GLint size, GLfloat *feedBackBuffer);
    void record(GLint size, GLfloat *feedBackBuffer);
    GLfloat* recordPrimitive(GLfloat *loc);
    
    GlFeedBackBuilder *feedBackBuilder;
    
  };
 
}

#endif // Tulip_GLFEEDBACKRECORDER_H
