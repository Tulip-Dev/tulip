//-*-c++-*-
#ifndef MOUSEMAGICSELECTION_H
#define MOUSEMAGICSELECTION_H 
#include <vector>
#include <tulip/Graph.h>
#include <tulip/Coord.h>
#include <tulip/GWInteractor.h>
namespace tlp {

/** \addtogroup Mouse_interactor */ 
/*@{*/
class TLP_QT_SCOPE MouseMagicSelector:public GWInteractor
{
private:
  unsigned int x,y;
  int w,h;
  bool started;
  Graph *graph;
public:
  MouseMagicSelector();
  ~MouseMagicSelector(){}
  bool eventFilter(QObject *, QEvent *);
  GWInteractor *clone() { return new MouseMagicSelector(); }};
/*@}*/

}
#endif
