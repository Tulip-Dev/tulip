#include "tulip/GlSVGFeedBackBuilder.h"

using namespace std;

namespace tlp {

  struct Feedback3DColor
  {
    float x, y, z, r, g, b, a;
  };

  void GlSVGFeedBackBuilder::begin(const Vector<int, 4> &viewport,GLfloat* clearColor,GLfloat pointSize,GLfloat lineWidth) {
    this->clearColor[0]=clearColor[0];
    this->clearColor[1]=clearColor[1];
    this->clearColor[2]=clearColor[2];
    this->pointSize=pointSize;
    this->lineWidth=lineWidth;
	
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
    fillColor[0]=data[0];fillColor[1]=data[1];fillColor[2]=data[2];
    strokeColor[0]=data[3];strokeColor[1]=data[4];strokeColor[2]=data[5];
    textColor[0]=data[6];textColor[1]=data[7];textColor[2]=data[8];
  }
  void GlSVGFeedBackBuilder::beginGlEntity(GLfloat data) {
    stream_out << "<e id=\"" << data << "\"><!-- Entity " << data << "-->" << endl;
  }

  void GlSVGFeedBackBuilder::endGlEntity() {
  }

  void GlSVGFeedBackBuilder::beginGlGraph(GLfloat data) {
    stream_out << "<g id=\"g" << data << "\"><!-- Graph " << data << "-->" << endl;
  }

  void GlSVGFeedBackBuilder::endGlGraph() {
  }

  void GlSVGFeedBackBuilder::beginNode(GLfloat data) {
    stream_out << "\t<g id=\"n"<< data << "\"><!-- Node " << data << "-->" << endl;
  }
  void GlSVGFeedBackBuilder::endNode() {
    stream_out << "</g>" << endl;
  }
 
  void GlSVGFeedBackBuilder::beginEdge(GLfloat data) {
    stream_out << "\t<g id=\"e"<< data << "\"><!-- Edge " << data << "-->" << endl;
  }
  void GlSVGFeedBackBuilder::endEdge() {
    stream_out << "</g>" << endl;
  }
  void GlSVGFeedBackBuilder::pointToken(GLfloat *data) {
    Feedback3DColor *vertex = (Feedback3DColor *)data;
    stream_out << "<circle cx=\"" << vertex->x 
	       << "\" cy=\"" << vertex->y 
	       << "\" r=\"" << pointSize 
	       << "\" fill=\"rgb(" << (int)strokeColor.getR() 
	       << ", " << (int)strokeColor.getG() 
	       << ", " << (int)strokeColor.getB() 
	       << ")\" stroke=\"rgb(" << (int)strokeColor.getR() 
	       << ", " << (int)strokeColor.getG() 
	       << ", " << (int)strokeColor.getB() 
	       << "\"/>" << endl;
  }
  void GlSVGFeedBackBuilder::lineToken(GLfloat *data) {
    Feedback3DColor *vertex1 = (Feedback3DColor *)data;
    Feedback3DColor *vertex2 = (Feedback3DColor *)(data+7);
    stream_out << "<line x1=\"" << vertex1->x 
	       << "\" y1=\"" << vertex1->y
	       << "\" x2=\"" << vertex2->x
	       << "\" y2=\"" << vertex2->y
	       << "\" fill=\"none\" stroke=\"rgb(" << (int)strokeColor.getR()
	       << ", " << (int)strokeColor.getG()
	       << ", " << (int)strokeColor.getB()
	       << ")\"/>" << endl;
  }
  void GlSVGFeedBackBuilder::lineResetToken(GLfloat *data) {
    lineToken(data);
  }
  void GlSVGFeedBackBuilder::polygonToken(GLfloat *data) {
    stream_out << "<polygon points=\"";
    unsigned int nbvertices = (unsigned int)(*data);
    for(unsigned int i = 0; i < nbvertices; i++) {
      Feedback3DColor *vertex = (Feedback3DColor *)(data+7*i+1);
      stream_out << ((i == 0) ? "" : " ") << vertex->x << "," << vertex->y;
    }
    stream_out << "\" fill=\"rgb(" << (int)fillColor.getR() 
	       << ", " << (int)fillColor.getG() 
	       << ", " << (int)fillColor.getB()
	       << ")\" stroke=\"rgb(" << (int)fillColor.getR() 
	       << ", " << (int)fillColor.getG()
	       << ", " << (int)fillColor.getB()
	       << ")\"/>" << endl;
  }
  void GlSVGFeedBackBuilder::bitmapToken(GLfloat *data) {
  }
  void GlSVGFeedBackBuilder::drawPixelToken(GLfloat *data) {
  }
  void GlSVGFeedBackBuilder::copyPixelToken(GLfloat *data) {
  }
  void GlSVGFeedBackBuilder::end() {
    stream_out << "</svg>" << endl;
  }
  void GlSVGFeedBackBuilder::getResult(string* str) {
    *str=stream_out.str();
  }
}
