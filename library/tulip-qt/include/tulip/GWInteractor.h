#ifndef _GL_INTERACTOR_H
#define _GL_INTERACTOR_H

#if (QT_REL == 3)
#include <qobject.h>
#else
#include <Qt/qobject.h>
#endif

class GlGraphWidget;

namespace tlp {

  /// The class uses to manage event filters on a GlGraphWidget
  class GWInteractor : public QObject {
  public:
    typedef unsigned int ID;
  protected:
    ID id;
  public:
    GWInteractor() {}
    virtual ~GWInteractor() {}
    /// an interactor may do some drawing feedback on the GlGraphWidget it interacts with
    virtual bool draw(GlGraphWidget *){ return false; }
    virtual GWInteractor *clone() = 0;
    ID getID() { return id; }
    void setID(ID i) { id = i; }
    static const ID invalidID = 0;
};

}

#endif






