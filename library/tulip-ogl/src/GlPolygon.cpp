
#include "tulip/GlPolygon.h"
#include "tulip/GlTools.h"
#include "tulip/GlLayer.h"



using namespace std;

namespace tlp {
  GlPolygon::GlPolygon(const bool filled,const bool outlined,const string &textureName,const float outlineSize)
  {
    setFillMode(filled);
    setOutlineMode(outlined);
    setTextureName(textureName);
    setOutlineSize(outlineSize);
  }
  //=====================================================
  GlPolygon::GlPolygon(const vector<Coord> &points,
		       const vector<Color> &fcolors,
		       const vector<Color> &ocolors,
		       const bool filled,
		       const bool outlined,
           const string &textureName,
           const float outlineSize)
  {
    setPoints(points);
    setFillColors(fcolors);
    setOutlineColors(ocolors);
    setFillMode(filled);
    setOutlineMode(outlined);
    setTextureName(textureName);
    setOutlineSize(outlineSize);
  }
  //=====================================================
  GlPolygon::GlPolygon(const unsigned int nbPoints,
		       const unsigned int nbFillColors,
		       const unsigned int nbOutlineColors,
		       const bool filled,
		       const bool outlined,
           const string &textureName,
           const float outlineSize) {
    vector<Coord> points;
    points.resize(nbPoints);
    setPoints(points);
    vector<Color> fillColors;
    fillColors.resize(nbFillColors,Color(0,0,0,255));
    setFillColors(fillColors);
    vector<Color> outlineColors;
    outlineColors.resize(nbOutlineColors);
    setOutlineColors(outlineColors);
    setFillMode(filled);
    setOutlineMode(outlined);
    setTextureName(textureName);
    setOutlineSize(outlineSize);
  }
  //=====================================================
  GlPolygon::~GlPolygon() {
  }
  //=====================================================
  void GlPolygon::resizePoints(const unsigned int nbPoints) {
    assert(nbPoints >= 3);
    points.resize(nbPoints);
  }
  //=====================================================
  void GlPolygon::resizeColors(const unsigned int nbColors) {
    assert(nbColors >= 1);
    points.resize(nbColors);
  }
  //=====================================================
  const tlp::Coord& GlPolygon::point(const unsigned int i) const {
    return points[i];
  }
  //=====================================================
  tlp::Coord& GlPolygon::point(const unsigned int i) {
    return points[i];
  }
}
