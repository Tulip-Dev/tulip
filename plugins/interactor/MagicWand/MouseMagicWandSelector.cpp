#include "MouseMagicWandSelector.h"

#include <iostream>
#include <QMouseEvent>
#include <tulip/GlMainWidget.h>
#include "MagicSelectionInteractorConfigWidget.h"

using namespace std;
using namespace tlp;

tlp::MouseMagicWandSelector::MouseMagicWandSelector(MagicSelectionInteractorConfigWidget* configWidget):x(0),y(0),w(0),h(0),graph(0),configWidget(configWidget)
{

}

bool tlp::MouseMagicWandSelector::eventFilter(QObject* widget, QEvent* e)
{
  QMouseEvent *qMouseEv = (QMouseEvent*)e;
  
  if(qMouseEv != NULL) {
    node tmpNode;
    edge tmpEdge;
    ElementType type;
    GlMainWidget *glMainWidget = (GlMainWidget *) widget;
    bool hoveringOverNode = glMainWidget->doSelect(qMouseEv->x(), qMouseEv->y(), type, tmpNode, tmpEdge) && type == NODE;
    if(e->type() == QEvent::MouseMove) {    
      if (hoveringOverNode) {
        glMainWidget->setCursor(QCursor(QPixmap(":/i_magic.png")));
      }
      else {
        glMainWidget->setCursor(Qt::ArrowCursor);
      }
    }
    if (e->type() == QEvent::MouseButtonPress) {
      if (qMouseEv->button()==Qt::LeftButton) {
        if (!hoveringOverNode) {
          return true;
        }
        Observable::holdObservers();
        
        GlGraphInputData *inputData=glMainWidget->getScene()->getGlGraphComposite()->getInputData();
        graph = inputData->getGraph();
        
        //the metric to check the value of in order to select the nodes or not
        DoubleProperty* metric = graph->getProperty<DoubleProperty>(configWidget->propertyName().toStdString());
        
        //the selection to add nodes to
        BooleanProperty* selection = graph->getProperty<BooleanProperty>("viewSelection");
        
        //temporary property to check whether the node has already been visited
        BooleanProperty* visited = graph->getProperty<BooleanProperty>("tmpVisited");
        
        //No node is visited in the initial state
        visited->setAllNodeValue(false);
        visited->setAllEdgeValue(false);
        
        switch(configWidget->selectionBehavior()) {
          case Replace:
            selection->setAllNodeValue(false);
            selection->setAllEdgeValue(false);
            break;  
          default:
            break;
        }
        
        double initValue = metric->getNodeValue(tmpNode);
        list <node> bfsFifo;
        bfsFifo.push_back(tmpNode);
        while(!bfsFifo.empty()) {
          node itn = bfsFifo.front();
          bfsFifo.pop_front();
          switch(configWidget->selectionBehavior()) {
            case Add:
            case Replace:
              selection->setNodeValue(itn, true);
              break;
            case Remove:
              selection->setNodeValue(itn, false);
              break;
            case Intersect:
              break;
          }
          visited->setNodeValue(itn,true);
         
          //let's add all the in/out nodes of this node to the list of nodes to check
          Iterator<node> *itN;
          if(configWidget->directedSelection()) {
            itN = graph->getOutNodes(itn);
          }
          else {
            itN = graph->getInOutNodes(itn);
          }
          
          while (itN->hasNext()) {
            node itv = itN->next();
            double curValue = metric->getNodeValue(itv);
            if ( shouldSelectvalue(initValue, curValue) && (!visited->getNodeValue(itv)) ) {
              bfsFifo.push_back(itv);
            }
          } delete itN;
        }
        
        Observable::unholdObservers();
        
        if(configWidget->selectionBehavior() == Intersect) {
          Iterator<node>* selectedNodes = selection->getNodesEqualTo(true);
          while(selectedNodes->hasNext()) {
            node n = selectedNodes->next();
            selection->setNodeValue(n, visited->getNodeValue(n));
          }
          delete selectedNodes;
        }
        
        return true;
      }
    }
  }
  return false;
}

void MouseMagicWandSelector::setView(View* view) {
  tlp::InteractorComponent::setView(view);
  if(view) {
    configWidget->setGraph(view->getGraph());
  }
}

bool MouseMagicWandSelector::shouldSelectvalue(double originalValue, double newValue) const {
  return newValue >= (originalValue - configWidget->lowerBound()) && newValue <= (originalValue + configWidget->upperBound());
}
