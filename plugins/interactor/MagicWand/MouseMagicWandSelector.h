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
#ifndef MOUSEMAGICWANDSELECTOR_H
#define MOUSEMAGICWANDSELECTOR_H

#include <tulip/InteractorComponent.h>

class MagicSelectionInteractorConfigWidget;

namespace tlp {
class Graph;

class MouseMagicWandSelector : public tlp::InteractorComponent
{
  private:
    unsigned int x,y;
    int w,h;
    tlp::Graph *graph;
    MagicSelectionInteractorConfigWidget* configWidget;
    
    bool shouldSelectvalue(double originalValue, double newValue) const;
    
  public:
    MouseMagicWandSelector(MagicSelectionInteractorConfigWidget* configWidget);
    ~MouseMagicWandSelector(){}
    bool eventFilter(QObject *, QEvent *);
    InteractorComponent *clone() { return new MouseMagicWandSelector(configWidget); }
    virtual void setView(View* view);
};

}
#endif // MOUSEMAGICWANDSELECTOR_H
