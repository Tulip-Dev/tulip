//-*-c++-*-
#ifndef MOUSEADDEDGE_H
#define MOUSEADDEDGE_H
#include <vector>
#include <tulip/Graph.h>
#include <tulip/Coord.h>
#include <tulip/GWInteractor.h>

namespace tlp {

/** \addtogroup Mouse_interactor */ 
/*@{*/
class TLP_QT_SCOPE MouseEdgeBuilder:public GWInteractor
{
 private:
  node source;
  bool started;
  Coord startPos,curPos;
  std::vector<Coord> bends;

 public:
  MouseEdgeBuilder();
  ~MouseEdgeBuilder(){}
  bool draw(GlGraphWidget *);
  bool eventFilter(QObject *, QEvent *);
  GWInteractor *clone() { return new MouseEdgeBuilder(); }
};
/*@}*/

}
#endif
