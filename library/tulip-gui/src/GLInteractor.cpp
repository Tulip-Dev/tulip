/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
