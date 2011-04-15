/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#ifndef MOUSEEDGEBENDEDITION_H
#define MOUSEEDGEBENDEDITION_H
#include <vector>
#include <tulip/GlCircle.h>
#include <tulip/GlTriangle.h>
#include <tulip/GlComplexPolygon.h>
#include <tulip/Observable.h>
#include <tulip/InteractorComponent.h>
#include <tulip/LayoutProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/IntegerProperty.h>

/** \addtogroup Mouse_interactor */
/*@{*/

namespace tlp {

  /// This interactor allows to move/add/delete EdgeBends
  class TLP_QT_SCOPE MouseEdgeBendEditor:public InteractorComponent {

  public:

    MouseEdgeBendEditor();
    ~MouseEdgeBendEditor();
    bool compute(GlMainWidget *glMainWidget);
    bool draw(GlMainWidget *);
    bool eventFilter(QObject *, QEvent *);
    InteractorComponent *clone() { return new MouseEdgeBendEditor(); }

  private:
    enum EditOperation { NONE_OP=0, TRANSLATE_OP, NEW_OP, DELETE_OP};
    enum OperationTarget { COORD = 0, SIZE, COORD_AND_SIZE};

    Graph *_graph;
    GlMainWidget* glMainWidget;
    LayoutProperty *_layout;
    BooleanProperty *_selection;
    DoubleProperty *_rotation;
    SizeProperty *_sizes;
    IntegerProperty *_shape;
    CoordVectorProperty *_coordsVectorProperty;
    LayoutProperty *_copyLayout;
    SizeProperty *_copySizes;
    DoubleProperty *_copyRotation;

    void initProxies(GlMainWidget *glMainWidget);
    void saveInfo();
    void restoreInfo();
    void initEdition();
    void undoEdition();
    void stopEdition();

    EditOperation operation;
    OperationTarget mode;

    Coord editPosition;

    GlLayer *layer;
    std::vector<tlp::GlCircle> circles;
    std::vector<Coord> coordinates;
    GlCircle basicCircle;
    GlTriangle targetTriangle;
    GlComplexPolygon sourceCross;
    GlComposite *circleString;
    std::vector <GlEntity * > select;
    bool edgeSelected;
    edge mEdge;
    node mNode;
    Coord start, end;
    std::string selectedEntity;
    bool belong(Coord, Coord, Coord, GlMainWidget*);
    bool haveSelection(GlMainWidget *);
    void computeSrcTgtEntities(GlMainWidget *);
    bool computeBendsCircles(GlMainWidget*);
    void mMouseTranslate(double, double, GlMainWidget*);
    void mMouseDelete();
    void mMouseCreate(double, double, GlMainWidget*);
  };

}
/*@}*/

#endif
