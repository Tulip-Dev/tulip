#include "tulip/GlRegularPolygon.h"



using namespace std;

namespace tlp {

  GlRegularPolygon::GlRegularPolygon(const Coord &position,
                                     const Size &size,
                                     unsigned int numberOfSides,
                                     const Color &fillColor,
                                     const Color &outlineColor,
                                     bool filled,
                                     bool outlined,
                                     const string &textureName,
                                     float outlineSize):
  numberOfSides(numberOfSides)
  {
    BoundingBox box;
    vector<Coord> points;
    float delta = (2.0f * M_PI) / (float)numberOfSides;
    float startAngle=M_PI/2.;
    for (unsigned int i=0; i < numberOfSides; ++i) {
      float deltaX = cos(i * delta + startAngle);
      float deltaY = sin(i * delta + startAngle);
      points.push_back(Coord(deltaX,deltaY,0));
      box.expand(points.back());
    }
    for(vector<Coord>::iterator it=points.begin();it!=points.end();++it){
      (*it)[0]=position[0]+((*it)[0]/((box[1][0]-box[0][0])/2.))*size[0];
      (*it)[1]=position[1]+((*it)[1]/((box[1][1]-box[0][1])/2.))*size[1];
    }
    boundingBox.expand(position+size/2.);
    boundingBox.expand(position-size/2.);

    setPoints(points);
    setFillColor(fillColor);
    setOutlineColor(outlineColor);
    setFillMode(filled);
    setOutlineMode(outlined);
    setTextureName(textureName);
    setOutlineSize(outlineSize);
  }
  //=====================================================
  GlRegularPolygon::~GlRegularPolygon() {
  }
}
