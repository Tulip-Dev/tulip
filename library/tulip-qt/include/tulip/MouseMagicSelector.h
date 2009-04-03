//-*-c++-*-
#ifndef MOUSEMAGICSELECTION_H
#define MOUSEMAGICSELECTION_H
#include <vector>
#include <tulip/Graph.h>
#include <tulip/Coord.h>
#include <tulip/InteractorComponent.h>
namespace tlp {

/** \addtogroup Mouse_interactor */
/*@{*/
class TLP_QT_SCOPE MouseMagicSelector:public InteractorComponent
{
private:
  unsigned int x,y;
  int w,h;
  Graph *graph;
public:
  MouseMagicSelector();
  ~MouseMagicSelector(){}
  bool eventFilter(QObject *, QEvent *);
  InteractorComponent *clone() { return new MouseMagicSelector(); }};
/*@}*/

}
#endif
