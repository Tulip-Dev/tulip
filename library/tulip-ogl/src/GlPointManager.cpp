#include "tulip/GlewManager.h"
#include "tulip/GlPointManager.h"

#include <iostream>

//====================================================
#ifdef _WIN32
#ifdef DLL_EXPORT
tlp::GlPointManager* tlp::GlPointManager::inst=0;
#endif
#else
tlp::GlPointManager* tlp::GlPointManager::inst=0;
#endif

using namespace std;

namespace tlp
{
  void GlPointManager::endRendering() {
    isBegin=false;

    if(currentId1==0 && currentId2==0)
      return;
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glDisable(GL_LIGHTING);

    if(currentId1!=0){
      glPointSize(1);

      glColorPointer(4, GL_UNSIGNED_BYTE, 0, &colors1[0]);
      glVertexPointer(3, GL_FLOAT, 0, &coords1[0]);

      glDrawElements(GL_POINTS, currentId1, GL_UNSIGNED_INT, &indices1[0]);
    }
    if(currentId2!=0){
      glPointSize(2);

      glColorPointer(4, GL_UNSIGNED_BYTE, 0, &colors2[0]);
      glVertexPointer(3, GL_FLOAT, 0, &coords2[0]);

      glDrawElements(GL_POINTS, currentId2, GL_UNSIGNED_INT, &indices2[0]);
    }

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    if(maxId1>1024){
      if(currentId1<maxId1/2){
        unsigned int newSize=1024;
        while(newSize<currentId1){
          newSize*=2;
        }
        delete[] indices1;
        delete[] colors1;
        delete[] coords1;
        indices1 = new GLuint[newSize];
        colors1 = new Color[newSize];
        coords1 = new Coord[newSize];
        maxId1=newSize;
      }
    }
    if(maxId2>1024){
      if(currentId2<maxId2/2){
        unsigned int newSize=1024;
        while(newSize<currentId2){
          newSize*=2;
        }
        delete[] indices2;
        delete[] colors2;
        delete[] coords2;
        indices2 = new GLuint[newSize];
        colors2 = new Color[newSize];
        coords2 = new Coord[newSize];
        maxId2=newSize;
      }
    }
  }
}
