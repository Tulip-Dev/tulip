//-*-c++-*-
/*
 Author: Antoine Lambert

 Email : antoine.lambert@labri.fr

 Last modification : 12/08

 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/

#ifndef PARALLELCOORDSAXISSLIDERS_H_
#define PARALLELCOORDSAXISSLIDERS_H_

#include <tulip/GlRect.h>
#include <tulip/GlQuad.h>
#include <tulip/GlLabel.h>
#include <tulip/GlBoundingBoxSceneVisitor.h>

#include "ParallelCoordsGlEntitiesSelector.h"

namespace tlp {

enum sliderType {TOP_SLIDER = 0, BOTTOM_SLIDER = 1};

class AxisSlider : public GlComposite {

public :

  AxisSlider(const sliderType type, const Coord &sliderCoord, const float halfWidth, const float halfHeight, const Color &sliderColor, const Color &labelColor) :
    type(type), sliderCoord(sliderCoord) {

    std::vector<Coord> sliderPolygonCoords;
    std::vector<Color> sliderPolygonColors;
    std::vector<Coord> arrowPolygonCoords;
    std::vector<Color> arrowPolygonColors;

    int n;

    if (type == TOP_SLIDER) {
      n = 1;
    } else {
      n = -1;
    }

    Coord arrowPolygonVertice1 = sliderCoord;
    Coord arrowPolygonVertice2 = Coord(sliderCoord.getX() + halfWidth / 2, sliderCoord.getY() + n * (halfHeight / 2));
    Coord arrowPolygonVertice3 = Coord(sliderCoord.getX() + halfWidth / 4, sliderCoord.getY() + n * (halfHeight / 2));
    Coord arrowPolygonVertice4 = Coord(sliderCoord.getX() + halfWidth / 4, sliderCoord.getY() + n * halfHeight);
    Coord arrowPolygonVertice5 = Coord(sliderCoord.getX() - halfWidth / 4, sliderCoord.getY() + n * halfHeight);
    Coord arrowPolygonVertice6 = Coord(sliderCoord.getX() - halfWidth / 4, sliderCoord.getY() + n * (halfHeight / 2));
    Coord arrowPolygonVertice7 = Coord(sliderCoord.getX() - halfWidth / 2, sliderCoord.getY() + n * (halfHeight / 2));

    Coord sliderPolygonVertice2 = Coord(sliderCoord.getX() + 1.5 * halfWidth, sliderCoord.getY() + n * halfHeight);
    Coord sliderPolygonVertice3 = Coord(sliderCoord.getX() + 1.5 * halfWidth, sliderCoord.getY() + 2.5 * n * halfHeight);
    Coord sliderPolygonVertice4 = Coord(sliderCoord.getX() - 1.5 * halfWidth, sliderCoord.getY() + 2.5 * n * halfHeight);
    Coord sliderPolygonVertice1 = Coord(sliderCoord.getX() - 1.5 * halfWidth, sliderCoord.getY() + n * halfHeight);

    arrowPolygonCoords.push_back(arrowPolygonVertice1);
    arrowPolygonColors.push_back(sliderColor);
    arrowPolygonCoords.push_back(arrowPolygonVertice2);
    arrowPolygonColors.push_back(sliderColor);
    arrowPolygonCoords.push_back(arrowPolygonVertice3);
    arrowPolygonColors.push_back(sliderColor);
    arrowPolygonCoords.push_back(arrowPolygonVertice4);
    arrowPolygonColors.push_back(sliderColor);
    arrowPolygonCoords.push_back(arrowPolygonVertice5);
    arrowPolygonColors.push_back(sliderColor);
    arrowPolygonCoords.push_back(arrowPolygonVertice6);
    arrowPolygonColors.push_back(sliderColor);
    arrowPolygonCoords.push_back(arrowPolygonVertice7);
    arrowPolygonColors.push_back(sliderColor);

    Coord sliderQuadCoords[4];
    sliderQuadCoords[0] = sliderPolygonVertice1;
    sliderQuadCoords[1] = sliderPolygonVertice2;
    sliderQuadCoords[2] = sliderPolygonVertice3;
    sliderQuadCoords[3] = sliderPolygonVertice4;

    sliderQuad = new GlQuad(sliderQuadCoords, sliderColor);
    sliderQuad->setTextureName(TulipBitmapDir + SLIDER_TEXTURE_NAME);
    addGlEntity(sliderQuad, "slider quad");

    sliderPolygonCoords.push_back(sliderPolygonVertice1);
    sliderPolygonColors.push_back(sliderColor);
    sliderPolygonCoords.push_back(sliderPolygonVertice2);
    sliderPolygonColors.push_back(sliderColor);
    sliderPolygonCoords.push_back(sliderPolygonVertice3);
    sliderPolygonColors.push_back(sliderColor);
    sliderPolygonCoords.push_back(sliderPolygonVertice4);
    sliderPolygonColors.push_back(sliderColor);

    sliderPolygon = new GlPolygon(sliderPolygonCoords, sliderPolygonColors, sliderPolygonColors, false, true);
    addGlEntity(sliderPolygon, "slider polygon");

    arrowPolygon = new GlPolygon(arrowPolygonCoords, arrowPolygonColors, arrowPolygonColors, true, false);
    addGlEntity(arrowPolygon, "arrow polygon");

    sliderLabel = new GlLabel(Coord(sliderCoord.getX(), sliderCoord.getY()+ n * 1.75 * halfHeight), Coord(3 * halfWidth, 1.5 * halfHeight), labelColor);
    addGlEntity(sliderLabel, "slider label");

    computeBoundingBox();
  }

  ~AxisSlider() {
    reset(true);
  }

  void setSliderFillColor(const Color &color) {
    sliderQuad->setColor(color);
    for (unsigned int i = 0 ; i < 7 ; ++i) {
      arrowPolygon->fcolor(i) = color;
    }
  }

  void setSliderOutlineColor(const Color &color) {
    for (unsigned int i = 0 ; i < 4 ; ++i) {
      sliderPolygon->ocolor(i) = color;
    }
  }

  void setSliderLabel(const std::string &label) {
    sliderLabel->setText(label);
  }

  void draw(float lod,Camera *camera) {
    glEnable(GL_LIGHTING);
    arrowPolygon->draw(lod, camera);
    sliderQuad->draw(lod, camera);
    sliderLabel->draw(lod, camera);
    glDisable(GL_LIGHTING);
    sliderPolygon->draw(lod, camera);
  }

  void computeBoundingBox() {
    GlBoundingBoxSceneVisitor glBBSV(NULL);
    acceptVisitor(&glBBSV);
    boundingBox = glBBSV.getBoundingBox();
  }

  Coord getSliderCoord() const {
    return sliderCoord;
  }

  void translate(const Coord &move) {
    sliderCoord += move;
    GlComposite::translate(move);
  }

  void moveToCoord(const Coord &coord) {
    translate(coord - sliderCoord);
  }

  sliderType getSliderType() const {
    return type;
  }

  Color getColor() {
    return arrowPolygon->fcolor(0);
  }

private :

  sliderType type;

  GlQuad *sliderQuad;
  GlPolygon *sliderPolygon;
  GlPolygon *arrowPolygon;
  GlLabel *sliderLabel;
  Coord sliderCoord;
};

class ParallelCoordsAxisSliders : public InteractorComponent {

public :

  ParallelCoordsAxisSliders() : currentGraph(NULL), selectedAxis(NULL), selectedSlider(NULL),
                  axisSliderDragStarted(false), drawSliders(false), slidersRangeDragStarted(false),
                  lastAxisHeight(0), lastNbAxis(0), multiFilteringActivated(false) {}
  ~ParallelCoordsAxisSliders();
  bool eventFilter(QObject *, QEvent *);
  bool draw(GlMainWidget *glMainWidget);
  bool compute(GlMainWidget *glMainWidget);
  InteractorComponent *clone() {return new ParallelCoordsAxisSliders(); }
  void setView(View *view);

private :

  void initOrUpdateSliders();
  AxisSlider *getSliderUnderPointer(ParallelAxis *axis, Coord pointerSceneCoords);
  void updateOtherAxisSliders();
  void buildGlSliders(std::vector<ParallelAxis *> axis);
  void deleteGlSliders();
  void setSlidersColor(const Color &color);
  void updateSlidersYBoundaries();

  ParallelCoordinatesView *parallelView;
  Graph *currentGraph;
  std::map<ParallelAxis *, std::vector<AxisSlider *> > axisSlidersMap;
  ParallelAxis *selectedAxis;
  std::vector<ParallelAxis *> lastSelectedAxis;
  AxisSlider *selectedSlider;
  bool axisSliderDragStarted;
  bool drawSliders;
  bool pointerBetweenSliders;
  bool slidersRangeDragStarted;
  int slidersRangeLength;
  int yClick;
  float lastAxisHeight;
  unsigned int lastNbAxis;
  bool multiFilteringActivated;
  std::map<ParallelAxis *, std::pair<float, float> > slidersYBoundaries;
};


}

#endif /* PARALLELCOORDSAXISSLIDERS_H_ */
