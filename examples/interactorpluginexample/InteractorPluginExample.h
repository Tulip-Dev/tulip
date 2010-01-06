#ifndef INTERACTORPLUGINEXAMPLE_H
#define INTERACTORPLUGINEXAMPLE_H

#include <tulip/tulipconf.h>

#include <tulip/Graph.h>
#include <tulip/InteractorChainOfResponsibility.h>

// For this example we construct an interactor who use the chain of responsibility pattern
//   In this case if first interactorComponent don't treat the event, this event is passed to second interactorComponent ...
class InteractorPluginExample  : public tlp::InteractorChainOfResponsibility {

  public :

  InteractorPluginExample();

  // This function is call when the interactor is activate 
  //   Construct the interactor chain of responsibility
  void construct();

  // This function is call to check if the interactor is compatible with the given view
  //   In this example : this function return true if view is Node Link Diagram View
  virtual bool isCompatible(const std::string &viewName) {
    if(viewName=="Node Link Diagram view")
      return true;
    return false;
  }

};

// Interactor Component class
class InteractorPluginComponent : public tlp::InteractorComponent {

 public :

  // Main function of this class
  bool eventFilter(QObject *, QEvent *);
  // This function must be implemented
  InteractorComponent *clone() { return new InteractorPluginComponent(); }

};

#endif
