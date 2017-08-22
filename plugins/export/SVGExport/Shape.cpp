#include "Shape.h"

#include <tulip/TulipFontAwesome.h>
#include <tulip/TulipMaterialDesignIcons.h>

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
  res.writeAttribute("stop-color",QString("rgb(")+QString::number(int(color.getR()))+","+QString::number(int(color.getG()))+","+QString::number(int(color.getB()))+")");
  res.writeEndElement();

  unsigned char colorR = color.getR();
  unsigned char colorG = color.getG();
  unsigned char colorB = color.getB();
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

void ExtremityShape::Icon(QXmlStreamWriter& res, const QString& color, const string &iconName, bool tgt ) {
  bool faIcon = iconName.substr(0, 3) == "fa-";
  res.writeStartElement("text");

  if (faIcon) {
    res.writeAttribute("font-family", "fontawesome");
  }
  else {
    res.writeAttribute("font-family", "materialdesignicons");
  }

  res.writeAttribute("transform", "scale(1,-1)");
  res.writeAttribute("font-size", "2");
  res.writeAttribute("text-anchor", "middle");
  res.writeAttribute("refX", (tgt?"10.5":"1.5"));
  res.writeAttribute("refY", "6");
  res.writeAttribute("fill", color);
  //res.writeAttribute("fill-opacity", tlpAlphaColor2Opacity(color));
  res.writeCharacters("");
  res.device()->write("&"); //do not escape the character

  if (faIcon) {
    res.writeCharacters("#x"+QString::number(TulipFontAwesome::getFontAwesomeIconCodePoint(iconName), 16)+";");
  }
  else {
    res.writeCharacters("#x"+QString::number(TulipMaterialDesignIcons::getMaterialDesignIconCodePoint(iconName), 16)+";");
  }
}

void ExtremityShape::GlowSphere(QXmlStreamWriter& res, const tlp::Color& color, bool tgt, const unsigned id_gradient) {
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
  res.writeAttribute("filter", "url(#fglow1)");
  res.writeEndElement();

  res.writeStartElement("radialGradient");
  res.writeAttribute("id",milieu_gradient_sphere);
  res.writeAttribute("gradientUnits","userSpaceOnUse");
  res.writeAttribute("cx","6");
  res.writeAttribute("cy","6");
  res.writeAttribute("r","5");

  res.writeStartElement("stop");
  res.writeAttribute("offset","0%");
  res.writeAttribute("stop-color",QString("rgb(")+QString::number(int(color.getR()))+","+QString::number(int(color.getG()))+","+QString::number(int(color.getB()))+")");
  res.writeEndElement();

  unsigned char colorR = color.getR();
  unsigned char colorG = color.getG();
  unsigned char colorB = color.getB();
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
}
