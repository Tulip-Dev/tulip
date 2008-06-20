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
class TLP_QT_SCOPE MouseEdgeBuilder:public Interactor
{
 private:
  node source;
  bool started;
  Coord startPos,curPos;
  std::vector<Coord> bends;

 public:
  MouseEdgeBuilder();
  ~MouseEdgeBuilder(){}
  bool draw(GlMainWidget *);
  bool eventFilter(QObject *, QEvent *);
  Interactor *clone() { return new MouseEdgeBuilder(); }
};
/*@}*/

}
#endif
