#include "tulip/GlSelectSceneVisitor.h"
#include "tulip/GlSimpleEntity.h"
#include "tulip/GlNode.h"
#include "tulip/GlEdge.h"
#include "tulip/GlLayer.h"

using namespace std;

namespace tlp {

  void GlSelectSceneVisitor::visit(GlSimpleEntity *entity) {
    if(selectionFlag==SelectSimpleEntities)
      calculator->addSimpleEntityBoundingBox((unsigned long)entity,entity->getBoundingBox());
  }

  void GlSelectSceneVisitor::visit(GlNode *glNode) {
    if(selectionFlag == SelectNodes)
      calculator->addComplexeEntityBoundingBox((unsigned long)glNode,glNode->getBoundingBox(inputData));
  }
  
  void GlSelectSceneVisitor::visit(GlEdge *glEdge) {
    if(selectionFlag == SelectEdges)
      calculator->addComplexeEntityBoundingBox((unsigned long)glEdge,glEdge->getBoundingBox(inputData));
  }

  void GlSelectSceneVisitor::visit(GlLayer *layer) {
    calculator->beginNewCamera(layer->getCamera());
  }

}
