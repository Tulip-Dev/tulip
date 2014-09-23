/*
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
///@cond DOXYGEN_HIDDEN

#ifndef MOUSEEDGEBENDEDITION_H
#define MOUSEEDGEBENDEDITION_H

#include <tulip/GlCircle.h>
#include <tulip/GlTriangle.h>
#include <tulip/GLInteractor.h>
#include <tulip/GlScene.h>
#include <tulip/GlLines.h>

namespace tlp {

class LayoutProperty;
class BooleanProperty;
class SizeProperty;
class DoubleProperty;
class CoordVectorProperty;
class IntegerProperty;

class EdgeEntity : public GlSimpleEntity {

  Coord start;
  Coord end;
  std::vector<Coord> bends;

public :

  void setCoordinates(const Coord &startPos, const Coord &endPos,const std::vector<Coord> &bendsPos) {
    boundingBox = BoundingBox();
    start=startPos;
    end=endPos;
    bends=bendsPos;
    boundingBox.expand(start);
    boundingBox.expand(end);

    for(std::vector<Coord>::iterator it=bends.begin(); it!=bends.end(); ++it) {
      boundingBox.expand(*it);
    }
  }

  void draw(float,Camera *) {
    GlLines::glDrawCurve(start,bends,end,10,0,Color(127,127,127,255),Color(127,127,127,255));
  }

  void getXML(std::string &) {}
  void setWithXML(const std::string &, unsigned int &) {}
};

/// This interactor allows to move/add/delete EdgeBends
class TLP_QT_SCOPE MouseEdgeBendEditor:public GLInteractorComponent {

public:

  MouseEdgeBendEditor();
  ~MouseEdgeBendEditor();
  bool compute(GlMainWidget *glMainWidget);
  bool draw(GlMainWidget *);
  void clear();
  bool eventFilter(QObject *, QEvent *);

protected:
  enum EditOperation { NONE_OP=0, TRANSLATE_OP, NEW_OP, DELETE_OP};

  EditOperation operation()const {
    return _operation;
  }

  tlp::edge getEdge()const {
    return mEdge;
  }

  void stopEdition();

private:

  enum OperationTarget { COORD = 0, SIZE, COORD_AND_SIZE};

  Graph *_graph;
  GlMainWidget* glMainWidget;
  LayoutProperty *_layout;
  BooleanProperty *_selection;
  DoubleProperty *_rotation;
  SizeProperty *_sizes;
  IntegerProperty *_shape;
  CoordVectorProperty *_coordsVectorProperty;

  void initProxies(GlMainWidget *glMainWidget);


  EditOperation _operation;
  OperationTarget mode;

  Coord editPosition;

  GlLayer *layer;
  std::vector<tlp::GlCircle> circles;
  EdgeEntity *edgeEntity;
  std::vector<Coord> coordinates;
  GlCircle basicCircle;
  GlTriangle targetTriangle;
  GlCircle sourceCircle;
  GlComposite *circleString;
  std::vector <SelectedEntity> select;
  bool edgeSelected;
  tlp::edge mEdge;
  node mNode;
  Coord start, end;
  std::string selectedEntity;
  bool mouseButtonPressOnEdge;
  bool belong(Coord, Coord, Coord, GlMainWidget*);
  bool haveSelection(GlMainWidget *);
  void computeSrcTgtEntities(GlMainWidget *);
  bool computeBendsCircles(GlMainWidget*);
  void mMouseTranslate(double, double, GlMainWidget*);
  void mMouseDelete();
  void mMouseCreate(double, double, GlMainWidget*);
};

}


#endif
///@endcond
