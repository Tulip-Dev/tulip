#include "tulip/GlLODSceneVisitor.h"

#include <iostream>

#include <GL/glu.h>
#include <GL/gl.h>

#include "tulip/GlSimpleEntity.h"
#include "tulip/GlNode.h"
#include "tulip/GlEdge.h"
#include "tulip/GlLayer.h"
#include "tulip/Camera.h"
#include "tulip/GlTools.h"
#include "tulip/GlLODCalculator.h"

using namespace std;

namespace tlp {

  void GlLODSceneVisitor::visit(GlSimpleEntity *entity) {
    if(entity->isVisible())
      calculator->addSimpleEntityBoundingBox((unsigned long)entity,entity->getBoundingBox());
  }
  
  void GlLODSceneVisitor::visit(GlNode *glNode) {
    calculator->addComplexeEntityBoundingBox((unsigned long)glNode,glNode->getBoundingBox(inputData));
  }

  void GlLODSceneVisitor::visit(GlEdge *glEdge) {
    calculator->addComplexeEntityBoundingBox((unsigned long)glEdge,glEdge->getBoundingBox(inputData));
  }

  void GlLODSceneVisitor::visit(GlLayer *layer) {
    calculator->beginNewCamera(layer->getCamera());
  }
}
