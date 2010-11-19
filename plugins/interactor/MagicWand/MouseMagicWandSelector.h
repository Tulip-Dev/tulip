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
