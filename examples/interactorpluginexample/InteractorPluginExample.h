/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
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
