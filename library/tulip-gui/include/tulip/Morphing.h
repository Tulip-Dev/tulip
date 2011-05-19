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
#ifndef MORPHING_H
#define MORPHING_H

#ifndef DOXYGEN_NOTFOR_USER
#include <vector>
#include <tulip/Camera.h>
#include <tulip/Coord.h>
#include <tulip/Edge.h>

#include <QtGui/qevent.h>
#include <QtCore/qobject.h>
#include <tulip/tulipconf.h>

namespace tlp {

  class GlMainWidget;
  class Graph;
  class LayoutProperty;
  class SizeProperty;
  class ColorProperty;
  //=====================================
  struct TLP_QT_SCOPE GraphState {
    Graph *g;
    LayoutProperty *layout;
    SizeProperty *size;
    ColorProperty *color;
    Camera *camera;
    std::vector<std::vector<Coord> > augPoints;
    std::vector<Coord> augCenters;
    std::vector<std::vector<Coord> > curInterpolation;
    std::vector<Coord> interpolateCenters;

    GraphState( GlMainWidget * glgw );
    GraphState(Graph *graph, LayoutProperty *layout, SizeProperty *size, ColorProperty *color, Camera *camera);
    ~GraphState();
    static bool setupDiff( Graph * inG, GraphState * inGS0, GraphState * inGS1 );
    void EdgeEnds( Coord & outC0, Coord & outC1, edge e );
  };
  //=====================================
  class TLP_QT_SCOPE Morphing : public QObject {
	  Q_OBJECT
  public:
    Morphing();
    bool init(GlMainWidget * outGlgw, 
	      GraphState * inG0, 
	      GraphState * inG1);
    void interpolate( GlMainWidget *, float inT);
    bool start(GlMainWidget * outGlgw, unsigned int morphingDurationSeconds = 10);

  public slots:

	void interpolationSlot(qreal t);
    
  protected:
    void stop();

  private: 
    GraphState *g0, *g1;
    LayoutProperty *e0, *e1;
    GlMainWidget *glWidget;
    int frameCpt; 
    float t;
  };
  //=====================================
}

#endif //DOXYGEN_NOTFOR_USER
#endif //MORPHING_H
