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
#include <tulip/GlSVGFeedBackBuilder.h>

using namespace std;

namespace tlp {

struct Feedback3DColor {
  float x, y, z, r, g, b, a;
};

void GlSVGFeedBackBuilder::begin(const Vector<int, 4> &viewport,GLfloat* clearColor,GLfloat pointSize,GLfloat lineWidth) {
  this->clearColor[0]=clearColor[0];
  this->clearColor[1]=clearColor[1];
  this->clearColor[2]=clearColor[2];
  this->pointSize=pointSize;
  this->lineWidth=lineWidth;

  width=viewport[2] - viewport[0];
  height=viewport[3] - viewport[1];

  stream_out << "<?xml version=\"1.0\" standalone=\"no\" ?>" << endl ;
  stream_out << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 20010904//EN\" \"http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd\">" << endl;
  stream_out << "<svg width=\"" << viewport[2] - viewport[0] << "px\" height=\"" << viewport[3] - viewport[1] << "px\" xmlns=\"http://www.w3.org/2000/svg\">" << endl;
  stream_out << "\t<!-- Exported from Tulip - plugin made by " << "OF-JD-NL-SH" << " (using OpenGL feedback) -->" << endl;

  /* Clear the background like OpenGL had it. */
  stream_out << "\t<rect x=\"" << viewport[0] << "\" y=\"" << viewport[1] << "\" widht=\"" << viewport[2] << "\" height=\"" << viewport[3] << "\" " <<
             "fill=\"rgb(" << 0 << "," << 0 << "," << 0 << ")\"/>" <<endl;

  /*fprintf(file, "%s<g id=\"g%d\"><!-- Graph %d -->\n", indentation.c_str(), graphId,
    graphId);*/
}
void GlSVGFeedBackBuilder::colorInfo(GLfloat* data) {
  fillColor[0]=(unsigned char)data[0];
  fillColor[1]=(unsigned char)data[1];
  fillColor[2]=(unsigned char)data[2];
  fillColor[3]=(unsigned char)data[3];
  strokeColor[0]=(unsigned char)data[4];
  strokeColor[1]=(unsigned char)data[5];
  strokeColor[2]=(unsigned char)data[6];
  strokeColor[3]=(unsigned char)data[7];
  textColor[0]=(unsigned char)data[8];
  textColor[1]=(unsigned char)data[9];
  textColor[2]=(unsigned char)data[10];
  textColor[3]=(unsigned char)data[11];
}
void GlSVGFeedBackBuilder::beginGlEntity(GLfloat data) {
  if(inGlEntity)
    endGlEntity();

  stream_out << "<e id=\"" << data << "\"><!-- Entity " << data << "-->" << endl;
  inGlEntity=true;
}

void GlSVGFeedBackBuilder::endGlEntity() {
  inGlEntity=false;
}

void GlSVGFeedBackBuilder::beginGlGraph(GLfloat data) {
  if(inGlGraph)
    endGlGraph();

  stream_out << "<g id=\"g" << data << "\"><!-- Graph " << data << "-->" << endl;
  inGlGraph=true;
}

void GlSVGFeedBackBuilder::endGlGraph() {
  inGlGraph=false;
}

void GlSVGFeedBackBuilder::beginNode(GLfloat data) {
  if(inNode)
    endNode();

  stream_out << "\t<g id=\"n"<< data << "\"><!-- Node " << data << "-->" << endl;
  inNode=true;
}
void GlSVGFeedBackBuilder::endNode() {
  inNode=false;
  stream_out << "</g>" << endl;
}

void GlSVGFeedBackBuilder::beginEdge(GLfloat data) {
  if(inNode)
    endNode();

  if(inEdge)
    endEdge();

  stream_out << "\t<g id=\"e"<< data << "\"><!-- Edge " << data << "-->" << endl;
  inEdge=true;
}
void GlSVGFeedBackBuilder::endEdge() {
  inEdge=false;
  stream_out << "</g>" << endl;
}
void GlSVGFeedBackBuilder::pointToken(GLfloat *data) {
  Feedback3DColor *vertex = (Feedback3DColor *)data;
  stream_out << "<circle cx=\"" << vertex->x
             << "\" cy=\"" << height-vertex->y
             << "\" r=\"" << pointSize
             << "\" fill=\"rgb(" << (int)strokeColor.getR()
             << ", " << (int)strokeColor.getG()
             << ", " << (int)strokeColor.getB()
             << ")\" fill-opacity=\"" << strokeColor.getA()/255.
             << "\" stroke=\"rgb(" << (int)strokeColor.getR()
             << ", " << (int)strokeColor.getG()
             << ", " << (int)strokeColor.getB()
             << ")\" stroke-opacity=\"" << strokeColor.getA()/255.
             << "\"/>" << endl;
}
void GlSVGFeedBackBuilder::lineToken(GLfloat *data) {
  Feedback3DColor *vertex1 = (Feedback3DColor *)data;
  Feedback3DColor *vertex2 = (Feedback3DColor *)(data+7);
  stream_out << "<line x1=\"" << vertex1->x
             << "\" y1=\"" << height-vertex1->y
             << "\" x2=\"" << vertex2->x
             << "\" y2=\"" << height-vertex2->y
             << "\" fill=\"rgb(" << (int)fillColor.getR()
             << ", " << (int)fillColor.getG()
             << ", " << (int)fillColor.getB()
             << ")\" fill-opacity=\"" << fillColor.getA()/255.
             << "\" stroke=\"rgb(" << (int)strokeColor.getR()
             << ", " << (int)strokeColor.getG()
             << ", " << (int)strokeColor.getB()
             << ")\" stroke-opacity=\"" << strokeColor.getA()/255.
             << "\"/>" << endl;
}
void GlSVGFeedBackBuilder::lineResetToken(GLfloat *data) {
  lineToken(data);
}
void GlSVGFeedBackBuilder::polygonToken(GLfloat *data) {
  stream_out << "<polygon points=\"";
  unsigned int nbvertices = (unsigned int)(*data);

  for(unsigned int i = 0; i < nbvertices; i++) {
    Feedback3DColor *vertex = (Feedback3DColor *)(data+7*i+1);
    stream_out << ((i == 0) ? "" : " ") << vertex->x << "," << height-vertex->y;
  }

  stream_out << "\" fill=\"rgb(" << (int)fillColor.getR()
             << ", " << (int)fillColor.getG()
             << ", " << (int)fillColor.getB()
             << ")\" fill-opacity=\"" << fillColor.getA()/255.
             << "\" stroke-opacity=\"0.0\""
             << " stroke=\"rgb(" << (int)fillColor.getR()
             << ", " << (int)fillColor.getG()
             << ", " << (int)fillColor.getB()
             << ")\"/>" << endl;
}
void GlSVGFeedBackBuilder::bitmapToken(GLfloat*) {
}
void GlSVGFeedBackBuilder::drawPixelToken(GLfloat*) {
}
void GlSVGFeedBackBuilder::copyPixelToken(GLfloat*) {
}
void GlSVGFeedBackBuilder::end() {
  stream_out << "</svg>" << endl;
}
void GlSVGFeedBackBuilder::getResult(string* str) {
  *str=stream_out.str();
}
}
