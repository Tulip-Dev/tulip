#include "tulip/GlBoundingBoxSceneVisitor.h"

#include <iostream>

#include "tulip/GlGraphInputData.h"
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
      
      if(bb.isValid() && entity->isCheckByBoundingBoxVisitor()) {
        boundingBox.expand(bb[0]);
        boundingBox.expand(bb[1]);
      }
    }
  }
  
  void GlBoundingBoxSceneVisitor::visit(GlNode *glNode) {
    BoundingBox bb=glNode->getBoundingBox(inputData);

    boundingBox.expand(bb[0]);
    boundingBox.expand(bb[1]);
  }

  void GlBoundingBoxSceneVisitor::visit(GlEdge *glEdge) {
    BoundingBox bb=glEdge->getBoundingBox(inputData);

    boundingBox.expand(bb[0]);
    boundingBox.expand(bb[1]);
  }
}
