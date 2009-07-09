#ifndef _Tulip_INTERACTORCOMPONENT_H_
#define _Tulip_INTERACTORCOMPONENT_H_

#include <tulip/tulipconf.h>

#include <QtCore/QObject>

namespace tlp {

  class GlMainWidget;
  class View;


  /** \brief Tulip interactor component main class
   *
   */
  class TLP_QT_SCOPE InteractorComponent : public QObject{

  public:
    typedef unsigned int ID;

  protected:
    ID id;
    View *view;

  public:
    /*
     * Defaulkt destructor
     */
    virtual ~InteractorComponent() {}

    /**
     * This function compute the interactor visual feedback
     */
    virtual bool compute(GlMainWidget *glMainWidget) { return false; }
    /**
     * This function draw interactor
     */
    virtual bool draw(GlMainWidget *glMainWidget){ return false; }

    /**
     * Set interactor connected view
     */
    virtual void setView(View *view) {this->view=view;}
    /**
     * \return the interactor connected view
     */
    View *getView() {return view;}

    /**
     * Clone this interactor component
     */
    virtual InteractorComponent *clone() =0;

    /**
     * \return the id of this interactor
     */
    ID getID() { return id; }
    /**
     * Set the id of this interactor
     */
    void setID(ID i) { id = i; }
    static const ID invalidID = 0;

  };

}

#endif //_Tulip_INTERACTORCOMPONENT_H_






