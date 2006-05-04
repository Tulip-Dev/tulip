#ifndef MORPHING_H
#define MORPHING_H

#include <tulip/Camera.h>
#include <tulip/Coord.h>
#include <tulip/Edge.h>

#include <qevent.h>
#include <qdatetime.h>
#include <qobject.h>
#include <tulip/tulipconf.h>

#define	MORPHING_MAX_FPS	30

class GlGraphWidget;
class Graph;
class LayoutProperty;
class SizeProperty;
class ColorProperty;

struct TLP_QT_SCOPE GraphState {
  Graph *g;
  LayoutProperty *layout;
  SizeProperty *size;
  ColorProperty *color;
  Camera camera;
  Coord sceneT;
  Coord sceneR;
  
  GraphState( GlGraphWidget * glgw );
  ~GraphState();
  static bool setupDiff( Graph * inG, GraphState * inGS0, GraphState * inGS1 );
  void EdgeEnds( Coord & outC0, Coord & outC1, edge e );
};


class TLP_QT_SCOPE Morphing : public QObject{
 public:
  Morphing();
  bool start( GlGraphWidget * outGlgw, GraphState * inG0, GraphState * inG1);
  void interpolate( GlGraphWidget *, float inT);
  
 protected:
  void timerEvent( QTimerEvent * );
  void stop();
  float fps();

 private: 
  GraphState *g0, *g1;
  LayoutProperty *e0, *e1;
  GlGraphWidget *glWidget;
  QTime qt0;
  int frameCpt; 
  int tid;
  float t;
};

#endif //MORPHING_H
