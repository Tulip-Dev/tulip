/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include <tulip/GlMetaNode.h>

#include <tulip/OpenGlConfigManager.h>

#include <tulip/Graph.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/ColorProperty.h>

#include <tulip/Glyph.h>
#include <tulip/GlCPULODCalculator.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/DrawingTools.h>
#include <tulip/GlEdge.h>
#include <tulip/GlComplexeEntity.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/Camera.h>
#include <tulip/GlTools.h>
#include <tulip/GlTextureManager.h>

#include <iostream>

using namespace std;

namespace tlp {

static const Color colorSelect2 = Color(255, 102, 255, 255);

void GlMetaNode::draw(float lod,GlGraphInputData* data,Camera* camera) {
  node n=node(id);

  if (data->getElementSelected()->getNodeValue(n)) {
    glStencilFunc(GL_LEQUAL,data->parameters->getSelectedMetaNodesStencil(),0xFFFF);
  }
  else {
    glStencilFunc(GL_LEQUAL,data->parameters->getMetaNodesStencil(),0xFFFF);
  }

  if(!(((data->getElementColor()->getNodeValue(n))[3]==255) && (data->parameters->getNodesStencil()==0xFFFF)))
    data->getMetaNodeRenderer()->render(n,lod,camera);

  if(lod>=20)
    GlNode::draw(lod,data,camera);
  else
    GlNode::draw(20,data,camera);
}

void GlMetaNode::drawLabel(OcclusionTest* test, GlGraphInputData* data) {
  drawLabel(test,data,0.);
}

void GlMetaNode::drawLabel(OcclusionTest* test, GlGraphInputData* data, float lod, Camera *camera) {

  node n=node(id);

  if(data->parameters->isViewNodeLabel())
    GlNode::drawLabel(test,data,lod,camera);
}
}
