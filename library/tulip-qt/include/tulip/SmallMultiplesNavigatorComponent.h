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
#ifndef SMALLMULTIPLESNAVIGATORCOMPONENT_H
#define SMALLMULTIPLESNAVIGATORCOMPONENT_H

#include "tulip/InteractorComponent.h"

namespace tlp {

class MouseNKeysNavigator;

/**
  * @brief The default interactor for small multiples view implementations.
  * This interactor provides user with a way to easily siwtch between overview and specific modes.
  * When subclassing SmallMultiplesView, you should use the INTERACTORPLUGINVIEWEXTENSIONWITHPRIORITY macro to create a new interactor compatible with your view object.
  * For instance:
  * @code INTERACTORPLUGINVIEWEXTENSIONWITHPRIORITY(MyCustomSmallMultipleViewObject, "The name of the interactor for my custom view", "InteractorSmallMultiplesNavigation", "Name of my custom view", "Authors", "Date", "Desc", "Version", INT_MAX);
  * @warning the last parameter of the INTERACTORPLUGINVIEWEXTENSIONWITHPRIORITY defines the priority. This should always be set to INT_MAX.
  */
class TLP_QT_SCOPE SmallMultiplesNavigatorComponent: public InteractorComponent {
public:
  SmallMultiplesNavigatorComponent();
  ~SmallMultiplesNavigatorComponent();
  bool draw(GlMainWidget *);
  bool eventFilter(QObject *, QEvent *);
  InteractorComponent *clone() { return new SmallMultiplesNavigatorComponent(); }

private:
  MouseNKeysNavigator *_navigator;
};
}

#endif // SMALLMULTIPLESNAVIGATORCOMPONENT_H
