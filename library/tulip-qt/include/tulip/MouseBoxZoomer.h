//-*-c++-*-
#ifndef MOUSEZOOMBOX_H
#define MOUSEZOOMBOX_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include <tulip/GWInteractor.h>

class QMouseEvent;
class QKeyEvent;

namespace tlp {

class Graph;
/** \addtogroup Mouse_interactor */ 
/*@{*/
class TLP_QT_SCOPE MouseBoxZoomer : public Interactor {
private:
  Qt::MouseButton mButton;
  Qt::KeyboardModifier kModifier;
public:
  MouseBoxZoomer(Qt::MouseButton button = Qt::LeftButton,
		 Qt::KeyboardModifier modifier = Qt::NoModifier);
  ~MouseBoxZoomer();
  bool draw(GlMainWidget *);
  bool eventFilter(QObject *, QEvent *);
  Interactor *clone() { return new MouseBoxZoomer(mButton, kModifier); }
private:
  unsigned int x, y;
  int w, h;
  bool started;
  Graph *graph;
};
/*@}*/

}
#endif //MOUSEZOOMBOX_H
