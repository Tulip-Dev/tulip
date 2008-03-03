//-*-c++-*-
#ifndef MOUSEADDNODE_H
#define MOUSEADDNODE_H

#include <tulip/GWInteractor.h>

namespace tlp {

/** \addtogroup Mouse_interactor */ 
/*@{*/
class TLP_QT_SCOPE MouseNodeBuilder:public GWInteractor
{
public:
  MouseNodeBuilder(){}
  ~MouseNodeBuilder(){}
  bool eventFilter(QObject *, QEvent *);
  GWInteractor *clone() { return new MouseNodeBuilder(); }};
/*@}*/

}
#endif
