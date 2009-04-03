#ifndef _Tulip_INTERACTORCHAINOFRESPONSIBILITY_H_
#define _Tulip_INTERACTORCHAINOFRESPONSIBILITY_H_

#include "Interactor.h"
#include "InteractorComponent.h"

namespace tlp {


  /** \brief Tulip interactor abstract class with chain of responsibility
   *
   */
  class TLP_QT_SCOPE InteractorChainOfResponsibility  : public Interactor {

  public:

    /**
     * Default constructor
     */
    InteractorChainOfResponsibility(const QString &iconPath,const QString &text);

    /**
     * Default destructor
     */
    ~InteractorChainOfResponsibility();

    /**
     * Set the view attached with this interactor
     */
    virtual void setView(View *view);

    /**
     * Install eventFilters of interactor on given widget
     */
    virtual void install(QWidget *);

    /**
     * Remove eventFilters of interactor
     */
    virtual void remove();

    /**
     * return QAction of this interactor
     */
    virtual InteractorAction* getAction();

    /**
     * Compute InteractorComponents include in this interactor
     */
    virtual void compute(GlMainWidget *);

    /**
     * Draw InteractorComponents include in this interactor
     */
    virtual void draw(GlMainWidget *);

    /**
     * Construct chain of responsibility
     */
    virtual void construct() {std::cout << "bad construct" << std::endl; ;}

  protected :

    /**
     * Push an interactor component on the top of the chain of responsibility
     * Now interactor component destruction is the responsibility of InteractorChainOfResponsibility
     */
    void pushInteractorComponent(InteractorComponent *component);

    View *view;
    QString interactorIconPath;
    QString interactorText;
    InteractorAction *action;
    std::vector<InteractorComponent *> interactorComponents;
    std::vector<InteractorComponent *> installedInteractorComponents;


  };

}

#endif






