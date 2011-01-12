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
