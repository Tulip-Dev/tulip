/*
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
///@cond DOXYGEN_HIDDEN

#ifndef GLINTERACTOR_H
#define GLINTERACTOR_H

#include <tulip/InteractorComposite.h>

namespace tlp {

class GlMainWidget;

class TLP_QT_SCOPE GLInteractorComponent: public tlp::InteractorComponent {
  Q_OBJECT
public slots:
  virtual bool draw(tlp::GlMainWidget*) {
    return false;
  }
  virtual bool compute(tlp::GlMainWidget*) {
    return false;
  }
};

class TLP_QT_SCOPE GLInteractorComposite: public tlp::InteractorComposite {
  Q_OBJECT

public:
  GLInteractorComposite(const QIcon& icon, const QString& text="");

public slots:
  virtual void compute(tlp::GlMainWidget*);
  virtual void draw(tlp::GlMainWidget*);
};

}

#endif // GLINTERACTOR_H
///@endcond