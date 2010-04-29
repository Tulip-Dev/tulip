#include "tulip/OpenGlConfigManager.h"

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

    bool empty=true;
    for(int i=0;i<3;i++){
      if(currentId[i]!=0)
        empty=false;
    }
    if(empty)
      return;

    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glDisable(GL_LIGHTING);

    for(int i=0;i<3;++i){
      if(currentId[i]!=0){

        if(i<2)
          glPointSize(i+1);
        else
          glPointSize(5);

        glColorPointer(4, GL_UNSIGNED_BYTE, 0, &colors[i][0]);
        glVertexPointer(3, GL_FLOAT, 0, &coords[i][0]);

        glDrawElements(GL_POINTS, currentId[i], GL_UNSIGNED_INT, &indices[i][0]);
      }
    }

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    for(int i=0;i<3;++i){
      if(maxId[i]>1024){
        if(currentId[i]<maxId[i]/2){
          unsigned int newSize=1024;
          while(newSize<currentId[i]){
            newSize*=2;
          }
          delete[] indices[i];
          delete[] colors[i];
          delete[] coords[i];
          indices[i] = new GLuint[newSize];
          colors[i] = new Color[newSize];
          coords[i] = new Coord[newSize];
          maxId[i]=newSize;
        }
      }
    }
  }
}
