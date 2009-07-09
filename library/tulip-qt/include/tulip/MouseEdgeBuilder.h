//-*-c++-*-
#ifndef MOUSEADDEDGE_H
#define MOUSEADDEDGE_H
#include <vector>
#include <tulip/Graph.h>
#include <tulip/Coord.h>
#include <tulip/InteractorComponent.h>

namespace tlp {

/** \addtogroup Mouse_interactor */
/*@{*/
class TLP_QT_SCOPE MouseEdgeBuilder:public InteractorComponent
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
  InteractorComponent *clone() { return new MouseEdgeBuilder(); }
};
/*@}*/

}
#endif
