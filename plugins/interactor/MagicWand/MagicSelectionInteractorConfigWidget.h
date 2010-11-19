#ifndef MAGICSELECTIONINTERACTORCONFIGWIDGET_H
#define MAGICSELECTIONINTERACTORCONFIGWIDGET_H

#include <QtGui/QWidget>
#include "MagicSelectionInteractorConfigWidgetData.h"
#include <tulip/Observable.h>

namespace tlp {
  class Graph;
class PropertyInterface;
}

enum Selectionbehavior {
  Replace = 0,
  Add = 1,
  Remove = 2,
  Intersect = 3
};

class MagicSelectionInteractorConfigWidget : public QWidget, public Ui_MagicSelectionInteractorConfigWidget, public tlp::Observer {
  Q_OBJECT
  public:
    MagicSelectionInteractorConfigWidget(QWidget* parent = 0);
    
    void setGraph(tlp::Graph* graph);
    
    virtual void observableDestroyed(tlp::Observable* ) {}
    virtual void update(std::set< tlp::Observable* >::iterator begin, std::set< tlp::Observable* >::iterator end);
    
    QString propertyName() const;
    Selectionbehavior selectionBehavior() const;
    bool directedSelection() const;
    double upperBound() const;
    double lowerBound() const;
    
  private:
    tlp::Graph* _graph;
    tlp::PropertyInterface* _currentProperty;
    void updateAvailableProperties();
public slots:
    void pushButtontoggled(bool);
    void selectedPropertyChanged(const QString&);
};

#endif // MAGICSELECTIONINTERACTORCONFIGWIDGET_H
