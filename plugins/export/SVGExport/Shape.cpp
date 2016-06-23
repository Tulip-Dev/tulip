#include "Shape.h"

#include <tulip/TulipFontAwesome.h>

using namespace std;
using namespace tlp;

void ExtremityShape::Arrow(QXmlStreamWriter& res, const QString& color, bool tgt) {
  res.writeAttribute("viewBox",(tgt?"0 0 10 10":"-10 0 10 10"));
  res.writeAttribute("refX",(tgt?"6.7":"-6.7"));
  res.writeAttribute("refY","5");
  res.writeStartElement("polyline");
  res.writeAttribute("points",(tgt?"0,0 10,5 0,10 1,5":"0,0 -10,5 0,10 -1,5"));
  res.writeAttribute("fill",color);
}

void ExtremityShape::Circle(QXmlStreamWriter& res, const QString &color, bool tgt) {
  res.writeAttribute("viewBox","1 1 10 10");
  res.writeAttribute("refX",(tgt?"10.5":"1.5"));
  res.writeAttribute("refY","6");
  res.writeStartElement("circle");
  res.writeAttribute("cx","6");
  res.writeAttribute("cy","6");
  res.writeAttribute("r","5");
  res.writeAttribute("fill",color);
}

void ExtremityShape::Cross(QXmlStreamWriter& res, const QString &color, bool tgt) {
  res.writeAttribute("viewBox","0 0 10 10");
  res.writeAttribute("refX",(tgt?"6":"4"));
  res.writeAttribute("refY","5");
  res.writeStartElement("path");
  res.writeAttribute("d","M 0 0 L 10 10 M 0 10 L 10 0");
  res.writeAttribute("stroke",color);
  res.writeAttribute("stroke-width","2");
}

void ExtremityShape::Diamond(QXmlStreamWriter& res, const QString &color, bool tgt) {
  res.writeAttribute("viewBox","0 0 10 10");
  res.writeAttribute("refX",(tgt?"8.5":"1.7"));
  res.writeAttribute("refY","5");
  res.writeStartElement("polygon");
  res.writeAttribute("points","5,0 0,5 5,10 10,5");
  res.writeAttribute("fill", color);
}

void ExtremityShape::Hexagon(QXmlStreamWriter& res, const QString &color, bool tgt) {
  res.writeAttribute("viewBox","0 0 10 10");
  res.writeAttribute("refX",(tgt?"10":"0"));
  res.writeAttribute("refY","5");
  res.writeStartElement("polygon");
  res.writeAttribute("points","5,0 0,3 0,7 5,10 10,7 10,3");
  res.writeAttribute("fill",color);
}

void ExtremityShape::Pentagon(QXmlStreamWriter& res, const QString &color, bool tgt) {
  res.writeAttribute("viewBox","0 0 10 10");
  res.writeAttribute("refX",(tgt?"8.5":"0"));
  res.writeAttribute("refY","5");
  res.writeStartElement("polygon");
  res.writeAttribute("points","6,0 0,2 0,8 6,10 10,5");
  res.writeAttribute("fill",color);
}
void ExtremityShape::Ring(QXmlStreamWriter& res, const QString &color, bool tgt) {
  res.writeAttribute("viewBox","0 0 10 10");
  res.writeAttribute("refX",(tgt?"9":"1"));
  res.writeAttribute("refY","5");
  res.writeAttribute("stroke",color);
  res.writeAttribute("stroke-width","0.55");
  res.writeStartElement("circle");
  res.writeAttribute("cx","5");
  res.writeAttribute("cy","5");
  res.writeAttribute("r","4.5");
  res.writeAttribute("fill","white");
  res.writeEndElement();
  res.writeStartElement("circle");
  res.writeAttribute("cx","5");
  res.writeAttribute("cy","5");
  res.writeAttribute("r","3.2");
  res.writeAttribute("fill","white");
  res.writeAttribute("stroke-width","2.5");
}

void ExtremityShape::Square(QXmlStreamWriter& res, const QString &color, bool tgt) {
  res.writeAttribute("viewBox","0 0 10 10");
  res.writeAttribute("refX",(tgt?"10":"0"));
  res.writeAttribute("refY","5");
  res.writeStartElement("rect");
  res.writeAttribute("x","0");
  res.writeAttribute("y","0");
  res.writeAttribute("width","10");
  res.writeAttribute("height","10");
  res.writeAttribute("fill",color);
}

void ExtremityShape::Star(QXmlStreamWriter& res, const QString &color, bool tgt) {
  res.writeAttribute("viewBox","0 0 10 10");
  res.writeAttribute("refX",(tgt?"5":"2"));
  res.writeAttribute("refY","5");
  res.writeStartElement("polygon");
  res.writeAttribute("points","6,0 3.75,3 0,2 2.25,5 0,8 3.75,6.9 6,10 6,6.25 10,5 6,3.75");
  res.writeAttribute("fill",color);
}

void ExtremityShape::Cube(QXmlStreamWriter& res, const QString &color, bool tgt) {
  res.writeAttribute("viewBox","0 0 10 10");
  res.writeAttribute("refX",(tgt?"10":"0"));
  res.writeAttribute("refY","5");
  res.writeStartElement("rect");
  res.writeAttribute("x","0");
  res.writeAttribute("y","0");
  res.writeAttribute("width","10");
  res.writeAttribute("height","10");
  res.writeAttribute("fill",color);
}

void ExtremityShape::CubeOutlinedTransparent(QXmlStreamWriter& res, const QString &color, bool tgt) {
  res.writeAttribute("viewBox","0 0 10 10");
  res.writeAttribute("refX",(tgt?"10":"0"));
  res.writeAttribute("refY","5");
  res.writeAttribute("stroke",color);
  res.writeStartElement("rect");
  res.writeAttribute("x","0");
  res.writeAttribute("y","0");
  res.writeAttribute("width","10");
  res.writeAttribute("height","10");
  res.writeAttribute("fill","white");
}
void ExtremityShape::Cone(QXmlStreamWriter& res, const QString& color, bool tgt) {
  res.writeAttribute("viewBox","0 0 11 10");
  res.writeAttribute("refX",(tgt?"10":"1"));
  res.writeAttribute("refY","5");
  res.writeStartElement("polygon");
  res.writeAttribute("points","0,5 0.94,8 3.44,10 6.94,10 9.88,8.15 10.94,5 9.88,2 6.94,0 3.44,0 0.94,2");
  res.writeAttribute("fill",color);
}

void ExtremityShape::Cylinder(QXmlStreamWriter& res, const QString& color, bool tgt) {
  res.writeAttribute("viewBox","0 0 11 10");
  res.writeAttribute("refX",(tgt?"10":"1"));
  res.writeAttribute("refY","5");
  res.writeStartElement("polygon");
  res.writeAttribute("points","0,5 0.94,8 3.44,10 6.94,10 9.88,8.15 10.94,5 9.88,2 6.94,0 3.44,0 0.94,2");
  res.writeAttribute("fill",color);
}

void ExtremityShape::Sphere(QXmlStreamWriter& res, const tlp::Color& color, bool tgt, const unsigned id_src_gradient) {
  res.writeAttribute("viewBox","1 1 10 10");
  res.writeAttribute("refX",(tgt?"10.5":"1.5"));
  res.writeAttribute("refY","6");

  QString milieu_gradient_sphere("RadGradientSrc" + QString::number(id_src_gradient));
  QString gradient_sphere("url(#" + milieu_gradient_sphere + ")");
  res.writeStartElement("circle");
  res.writeAttribute("cx","6");
  res.writeAttribute("cy","6");
  res.writeAttribute("r","5");
  res.writeAttribute("fill",gradient_sphere);
  res.writeEndElement();

  res.writeStartElement("radialGradient");
  res.writeAttribute("id",milieu_gradient_sphere);
  res.writeAttribute("gradientUnits","userSpaceOnUse");
  res.writeAttribute("cx","6");
  res.writeAttribute("cy","6");
  res.writeAttribute("r","5");

  res.writeStartElement("stop");
  res.writeAttribute("offset","0%");
  res.writeAttribute("stop-color",QString("rgb(")+QString::number(static_cast<int>(color.getR()))+","+QString::number(static_cast<int>(color.getG()))+","+QString::number(static_cast<int>(color.getB()))+")");
  res.writeEndElement();

  unsigned colorR = static_cast<unsigned>(color.getR());
  unsigned colorG = static_cast<unsigned>(color.getG());
  unsigned colorB = static_cast<unsigned>(color.getB());
  QString Rcolor(QString::number(colorR - colorR/8));
  QString Gcolor(QString::number(colorG - colorG/8));
  QString Bcolor(QString::number(colorB - colorB/8));
  QString couleur("rgb(" + Rcolor + "," + Gcolor + ","+ Bcolor + ")");

  res.writeStartElement("stop");
  res.writeAttribute("offset","33.3%");
  res.writeAttribute("stop-color",couleur);
  res.writeEndElement();

  Rcolor = QString::number(colorR - colorR/5);
  Gcolor = QString::number(colorG - colorG/5);
  Bcolor = QString::number(colorB - colorB/5);
  couleur = "rgb(" + Rcolor + "," + Gcolor + "," + Bcolor + ")";

  res.writeStartElement("stop");
  res.writeAttribute("offset","66.6%");
  res.writeAttribute("stop-color",couleur);
  res.writeEndElement();

  Rcolor = QString::number(colorR - colorR/3);
  Gcolor = QString::number(colorG - colorG/3);
  Bcolor = QString::number(colorB - colorB/3);
  couleur = "rgb(" + Rcolor + "," + Gcolor + "," + Bcolor + ")";

  res.writeStartElement("stop");
  res.writeAttribute("offset","100%");
  res.writeAttribute("stop-color",couleur);
  res.writeEndElement();
}

void ExtremityShape::FontAwesomeIcon(QXmlStreamWriter& res, const QString& color, const string &iconName, bool tgt ) {
    res.writeStartElement("text");
    res.writeAttribute("font-family","fontawesome");
    res.writeAttribute("transform", "scale(1,-1)");
    res.writeAttribute("font-size", "2");
    res.writeAttribute("text-anchor","middle");
    res.writeAttribute("refX",(tgt?"10.5":"1.5"));
    res.writeAttribute("refY","6");
    res.writeAttribute("fill", color);
    //res.writeAttribute("fill-opacity", tlpAlphaColor2Opacity(color));
    res.writeCharacters("");
    res.device()->write("&"); //do not escape the character
    res.writeCharacters("#x"+QString::number(TulipFontAwesome::getFontAwesomeIconCodePoint(iconName), 16)+";");
}

void ExtremityShape::GlowSphere(QXmlStreamWriter& res, const tlp::Color& color, bool tgt, const unsigned id_gradient, const tlp::Coord &node_coord, const tlp::Size &node_size) {
  QString milieu_gradient_sphere("RadGradientTgt" + QString::number(id_gradient));
  QString gradient_sphere("url(#" + milieu_gradient_sphere +")");

  res.writeAttribute("viewBox","1 1 10 10");
  res.writeAttribute("refX",(tgt?"10.5":"1.5"));
  res.writeAttribute("refY","6");

  res.writeStartElement("circle");
  res.writeAttribute("cx","6");
  res.writeAttribute("cy","6");
  res.writeAttribute("r","5");
  res.writeAttribute("fill",gradient_sphere);
  res.writeEndElement();

  res.writeStartElement("radialGradient");
  res.writeAttribute("id",milieu_gradient_sphere);
  res.writeAttribute("gradientUnits","userSpaceOnUse");
  res.writeAttribute("cx","6");
  res.writeAttribute("cy","6");
  res.writeAttribute("r","5");

  res.writeStartElement("stop");
  res.writeAttribute("offset","0%");
  res.writeAttribute("stop-color",QString("rgb(")+QString::number(static_cast<int>(color.getR()))+","+QString::number(static_cast<int>(color.getG()))+","+QString::number(static_cast<int>(color.getB()))+")");
  res.writeEndElement();

  unsigned colorR = static_cast<unsigned>(color.getR());
  unsigned colorG = static_cast<unsigned>(color.getG());
  unsigned colorB = static_cast<unsigned>(color.getB());
  QString Rcolor = QString::number(colorR - colorR/8);
  QString Gcolor = QString::number(colorG - colorG/8);
  QString Bcolor = QString::number(colorB - colorB/8);
  QString couleur("rgb(" + Rcolor + "," + Gcolor + "," + Bcolor + ")");

  res.writeStartElement("stop");
  res.writeAttribute("offset","33.3%");
  res.writeAttribute("stop-color",couleur);
  res.writeEndElement();

  Rcolor = QString::number(colorR - colorR/5);
  Gcolor = QString::number(colorG - colorG/5);
  Bcolor = QString::number(colorB - colorB/5);
  couleur = "rgb(" + Rcolor + "," + Gcolor + "," + Bcolor + ")";

  res.writeStartElement("stop");
  res.writeAttribute("offset","66.6%");
  res.writeAttribute("stop-color",couleur);
  res.writeEndElement();

  Rcolor = QString::number(colorR - colorR/3);
  Gcolor = QString::number(colorG - colorG/3);
  Bcolor = QString::number(colorB - colorB/3);
  couleur = "rgb(" + Rcolor + "," + Gcolor + "," + Bcolor + ")";

  res.writeStartElement("stop");
  res.writeAttribute("offset","100%");
  res.writeAttribute("stop-color",couleur);
  res.writeEndElement();

  res.writeEndElement();// End element "radialGRadient"
  res.writeEndElement();// End context "marker"
  res.writeEndElement();// End context "def"

  // Starting the glow effect
  res.writeStartElement("def");

  QString milieu_glow_sphere = "RadGradientTgt" + QString::number(id_gradient+1);
  QString glow_sphere("url(#" + milieu_glow_sphere +")");

  res.writeStartElement("radialGradient");
  res.writeAttribute("id",milieu_glow_sphere);
  res.writeAttribute("cx","50%");
  res.writeAttribute("cy","50%");
  res.writeAttribute("r","50%");
  res.writeAttribute("fx","50%");
  res.writeAttribute("fy","50%");

  res.writeStartElement("stop");
  res.writeAttribute("offset","80%");
  res.writeAttribute("stop-color","white");
  res.writeEndElement();

  Rcolor = QString::number(colorR - colorR/9);
  Gcolor = QString::number(colorG - colorG/9);
  Bcolor = QString::number(colorB - colorB/9);
  QString couleur90 = "rgb(" + Rcolor + "," + Gcolor + "," + Bcolor + ")";

  res.writeStartElement("stop");
  res.writeAttribute("offset","90%");
  res.writeAttribute("stop-color",couleur90);
  res.writeEndElement();

  res.writeStartElement("stop");
  res.writeAttribute("offset","100%");
  res.writeAttribute("stop-color","white");
  res.writeEndElement();

  res.writeEndElement();// End element "radialGradient"
  res.writeEndElement();// End context "def"

  res.writeStartElement("ellipse");
  res.writeAttribute("cx",QString::number(node_coord.getX()));
  res.writeAttribute("cy",QString::number(node_coord.getY()));
  res.writeAttribute("rx",QString::number(node_size.getW()/2));
  res.writeAttribute("ry",QString::number(node_size.getH()/2));
  res.writeAttribute("fill",glow_sphere);
  res.writeAttribute("fill-opacity","0.5");
  res.writeEndElement();

  res.writeStartElement("ellipse");
  res.writeAttribute("cx",QString::number(node_coord.getX()));
  res.writeAttribute("cy",QString::number(node_coord.getY()));
  res.writeAttribute("rx",QString::number(node_size.getW()/2 - node_size.getW()/8));
  res.writeAttribute("ry",QString::number(node_size.getH()/2 - node_size.getH()/8));
  res.writeAttribute("fill","white");
  res.writeAttribute("fill-opacity","0");
}
