#include "tulip/GlLayer.h"

#include <iostream>

#include <GL/gl.h>
#include <GL/glu.h>

#include "tulip/Camera.h"
#include "tulip/GlEntity.h"
#include "tulip/GlSceneVisitor.h"

using namespace std;

namespace tlp {

  GlLayer::GlLayer() 
    :visible(true),scene(0),camera(0){
  }

  void GlLayer::acceptVisitor(GlSceneVisitor *visitor) {
    if(visible) {
      visitor->visit(this);
      composite.acceptVisitor(visitor);
    }
  }
  
}
