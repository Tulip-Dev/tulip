/**
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
#include <tulip/GlTLPFeedBackBuilder.h>

namespace tlp {

void GlTLPFeedBackBuilder::passThroughToken(GLfloat *data) {
  if(!needData) {

    switch ((int)(*data)) {

    case TLP_FB_COLOR_INFO :
      inColorInfo=true;
      needData=true;
      break;

    case TLP_FB_BEGIN_ENTITY :
      inGlEntity=true;
      needData=true;
      break;

    case TLP_FB_END_ENTITY :
      assert(inGlEntity);
      inGlEntity=false;
      endGlEntity();
      break;

    case TLP_FB_BEGIN_GRAPH :
      inGlGraph=true;
      needData=true;
      break;

    case TLP_FB_END_GRAPH :
      assert(inGlGraph);
      inGlGraph=false;
      endGlGraph();
      break;

    case TLP_FB_BEGIN_NODE :
      inNode=true;
      needData=true;
      break;

    case TLP_FB_END_NODE :
      assert(inNode);
      inNode=false;
      endNode();
      break;

    case TLP_FB_BEGIN_EDGE :
      inEdge=true;
      needData=true;
      break;

    case TLP_FB_END_EDGE :
      assert(inEdge);
      inEdge=false;
      endEdge();
      break;

    default :
      assert(false);
      break;

    }

  }
  else {
    if(!inColorInfo) {
      needData=false;

      if(inGlEntity) {
        beginGlEntity(*data);
      }
      else if(inEdge) {
        beginEdge(*data);
      }
      else if(inNode) {
        beginNode(*data);
      }
      else if(inGlGraph) {
        beginGlGraph(*data);
      }
    }
    else {
      dataBuffer.push_back(*data);

      if(dataBuffer.size()>=12) {
        GLfloat* tmp = new GLfloat[dataBuffer.size()];

        for(unsigned int i=0; i<dataBuffer.size(); i++)
          tmp[i]=dataBuffer[i];

        colorInfo(tmp);
        dataBuffer.clear();
        needData=false;
        inColorInfo=false;
      }
    }
  }
}
}
