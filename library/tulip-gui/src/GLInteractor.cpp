#include "tulip/GLInteractor.h"

using namespace tlp;

GLInteractorComposite::GLInteractorComposite(const QIcon &icon, const QString &text)
  : InteractorComposite(icon,text) {
}

void GLInteractorComposite::draw(tlp::GlMainWidget *w) {
  foreach(tlp::InteractorComponent* i,_components) {
    GLInteractorComponent* cmp = dynamic_cast<GLInteractorComponent*>(i);

    if (cmp != NULL)
      cmp->draw(w);
  }
}

void GLInteractorComposite::compute(tlp::GlMainWidget *w) {
  foreach(tlp::InteractorComponent* i,_components) {
    GLInteractorComponent* cmp = dynamic_cast<GLInteractorComponent*>(i);

    if (cmp != NULL)
      cmp->compute(w);
  }
}
