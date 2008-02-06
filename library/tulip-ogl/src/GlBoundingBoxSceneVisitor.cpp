#include "tulip/GlBoundingBoxSceneVisitor.h"

#include <iostream>

#include <GL/glu.h>
#include <GL/gl.h>

#include "tulip/GlSimpleEntity.h"
#include "tulip/GlNode.h"
#include "tulip/GlEdge.h"
#include "tulip/GlLayer.h"
#include "tulip/Camera.h"
#include "tulip/GlTools.h"

using namespace std;

namespace tlp {

  void GlBoundingBoxSceneVisitor::visit(GlSimpleEntity *entity) {
    if(entity->isVisible()) {
      BoundingBox bb=entity->getBoundingBox();
      
      if(bb.isValid && entity->isCheckByBoundingBoxVisitor()) {
	boundingBox.check(bb.first);
	boundingBox.check(bb.second);
      }
    }
  }
  
  void GlBoundingBoxSceneVisitor::visit(GlNode *glNode) {
    BoundingBox bb=glNode->getBoundingBox(inputData);

    boundingBox.check(bb.first);
    boundingBox.check(bb.second);
  }

  void GlBoundingBoxSceneVisitor::visit(GlEdge *glEdge) {
    BoundingBox bb=glEdge->getBoundingBox(inputData);

    boundingBox.check(bb.first);
    boundingBox.check(bb.second);
  }
}
