//-*-c++-*-
#ifndef MOUSEADDNODE_H
#define MOUSEADDNODE_H

#include <tulip/InteractorComponent.h>

namespace tlp {

/** \addtogroup Mouse_interactor */
/*@{*/
class TLP_QT_SCOPE MouseNodeBuilder:public InteractorComponent
{

public:
  MouseNodeBuilder() {}
  ~MouseNodeBuilder() {}
  bool eventFilter(QObject *, QEvent *);
  InteractorComponent *clone() { return new MouseNodeBuilder(); }};
/*@}*/

}
#endif
