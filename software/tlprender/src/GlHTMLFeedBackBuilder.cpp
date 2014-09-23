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
#include "GlHTMLFeedBackBuilder.h"

using namespace std;
using namespace tlp;

namespace tlprender {

struct Feedback3DColor {
  float x, y, z, r, g, b, a;
};

GlHTMLFeedBackBuilder::GlHTMLFeedBackBuilder(bool outputBody,const string &filename,StringProperty *hrefp,StringProperty *altp):
  outputBody(outputBody),
  filename(filename),
  hrefp(hrefp),
  altp(altp),
  poly(NULL),
  nodeId(-1) {
}

void GlHTMLFeedBackBuilder::begin(const Vector<int, 4> &viewport,GLfloat* clearColor,GLfloat pointSize,GLfloat lineWidth) {
  height=viewport[3];

  if (outputBody) {
    stream_out << "<html><body>" << endl;
  }

  stream_out << "<img src=\"" << filename << "\" usemap=\"#usemap\" border=\"0\" ";
  stream_out << "width=\"" << viewport[2] << "\" height=\"" << viewport[3] << "\">" << endl;
  stream_out << "<map name=\"usemap\">" << endl;
}


void GlHTMLFeedBackBuilder::beginNode(GLfloat data) {
  nodeId=data;
  poly=new Shape();
}

void GlHTMLFeedBackBuilder::endNode() {
  if (poly->isValid())
    shapeQueue.push(poly);
  else
    delete poly;

  nodeId=-1;
}


void GlHTMLFeedBackBuilder::polygonToken(GLfloat *data) {
  if(nodeId!=-1) {
    unsigned int nbvertices = (unsigned int)(*data);
    Shape *tmp = new Shape();
    tmp->begin();

    for(unsigned int i = 0; i < nbvertices; i++) {
      Feedback3DColor *vertex = (Feedback3DColor *)(data+3*i+1);
      vertex->y=height-vertex->y;
      tmp->add(vertex->x,vertex->y,vertex->z);
    }

    if(tmp->end()) {
      if(!poly->clip(*tmp)) {
        poly->nodeNum=nodeId;

        if (poly->isValid())
          shapeQueue.push(poly);
        else
          delete poly;

        poly=tmp;
      }
      else
        delete tmp;
    }
  }
}

void GlHTMLFeedBackBuilder::end() {
  while (!shapeQueue.empty()) {
    const Shape *s = shapeQueue.top();

    string href = hrefp->getNodeValue(node(s->nodeNum));

    if (href=="") {
      stringstream ss;
      ss << "javascript:alert(\'node " << s->nodeNum << "\')";
      href = ss.str();
    }

    string alt = altp->getNodeValue(node(s->nodeNum));

    if (alt=="") {
      stringstream ss;
      ss << "node " << s->nodeNum;
      alt = ss.str();
    }

    stream_out << "    <area href=\"" << href << "\" alt=\"" << alt << "\"" << endl;
    stream_out << "          shape=\"poly\" coords=\"" << *s << "\">" << endl;
    delete s;
    shapeQueue.pop();
  }

  stream_out << "</map>" << endl;

  if (outputBody) {
    stream_out << "</body></html>" << endl;
  }
}

void GlHTMLFeedBackBuilder::getResult(string* str) {
  *str=stream_out.str();
}
}
