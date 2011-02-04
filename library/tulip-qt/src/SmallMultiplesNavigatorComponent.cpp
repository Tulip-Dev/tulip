#include "tulip/SmallMultiplesNavigatorComponent.h"
#include "tulip/SmallMultiplesView.h"
#include "tulip/GlMainWidget.h"
#include "tulip/GlLODCalculator.h"
#include "tulip/MouseInteractors.h"

#include <QtCore/QEvent>
#include <QtGui/QMouseEvent>

#include <iostream>

using namespace std;

namespace tlp {

SmallMultiplesNavigatorComponent::SmallMultiplesNavigatorComponent(): _navigator(new MouseNKeysNavigator) {
}

SmallMultiplesNavigatorComponent::~SmallMultiplesNavigatorComponent() {
  delete _navigator;
}

bool SmallMultiplesNavigatorComponent::draw(GlMainWidget *) {
  return false;
}

bool SmallMultiplesNavigatorComponent::eventFilter(QObject *o, QEvent *e) {

  SmallMultiplesView *smView = dynamic_cast<SmallMultiplesView *>(view);
  if (!view)
    return false;

  if (e->type() == QEvent::MouseButtonDblClick) {
    if (smView->isOverviewVisible()) {
      QMouseEvent *mouseEv = static_cast<QMouseEvent *>(e);
      GlScene *scene = smView->overview()->getScene();
      vector<ulong> entities;
      scene->selectEntities(RenderingNodes, mouseEv->x(), mouseEv->y(), 1, 1, smView->overviewLayer(), entities);
      if (entities.size() > 0) {
        int id = smView->nodeItemId(node(entities[0]));
        if (id != -1)
          smView->selectItem(id);
      }
    }
    else
      smView->overviewSelected();
    return true;
  }
  else {
    GlMainWidget *w = dynamic_cast<GlMainWidget *>(smView->overview());
    if (w) return _navigator->eventFilter(o,e);
  }
  return false;
}

}
