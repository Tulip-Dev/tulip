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
#include "NodeLinkDiagramComponentInteractor.h"

#include <QEvent>
#include <QMouseEvent>

#include <tulip/MouseInteractors.h>
#include <tulip/MouseNodeBuilder.h>
#include <tulip/LayoutProperty.h>
#include <tulip/NodeLinkDiagramComponent.h>
#include <tulip/Camera.h>
#include <tulip/GlComplexPolygon.h>
#include <tulip/GlMainWidget.h>
#include <tulip/MouseSelector.h>

using namespace tlp;

/** \brief Tulip interactor to add a node
 *
 */

class TLP_QT_SCOPE CustomMouseNodeBuilder : public InteractorComponent {

public:
  CustomMouseNodeBuilder() {}
  ~CustomMouseNodeBuilder() {}
  
  bool eventFilter(QObject* widget, QEvent* e) {
    if (e->type() == QEvent::MouseButtonDblClick) {
      QMouseEvent * qMouseEv = (QMouseEvent *) e;
      if (qMouseEv->button() == Qt::LeftButton) {
        GlMainWidget *glw = (GlMainWidget *) widget;

        Graph*_graph=glw->getScene()->getGlGraphComposite()->getInputData()->getGraph();
        LayoutProperty* mLayout=_graph->getProperty<LayoutProperty>(glw->getScene()->getGlGraphComposite()->getInputData()->getElementLayoutPropName());
        // allow to undo
        _graph->push();
        Observable::holdObservers();
        node newNode;
        newNode = _graph->addNode();
        Coord point((double) glw->width() - (double) qMouseEv->x(),
        (double) qMouseEv->y(),
        0);
        point = glw->getScene()->getCamera()->screenTo3DWorld(point);
        mLayout->setNodeValue(newNode, point);
        Observable::unholdObservers();
        NodeLinkDiagramComponent *nodeLinkView=(NodeLinkDiagramComponent *)view;
        nodeLinkView->elementSelectedSlot(newNode.id, true);
        //glw->redraw();
        return true;
      }
    }
    return false;
  }
  
  InteractorComponent *clone() { return new CustomMouseNodeBuilder(); }
};

class TLP_QT_SCOPE CustomMouseEdgeBuilder:public InteractorComponent {
  public:
    CustomMouseEdgeBuilder() :_isDragging(false), _currentNode(UINT_MAX), _mainWidget(NULL), _camera(NULL) {}
    ~CustomMouseEdgeBuilder() {}
    
    bool eventFilter(QObject*, QEvent* e) {
      _camera = _mainWidget->getScene()->getLayer("Main")->getCamera();
      bool result = false;
      QMouseEvent * qMouseEv = (QMouseEvent *) e;
      if (e->type() == QEvent::MouseButtonPress) {
        if (qMouseEv->button() == Qt::LeftButton && qMouseEv->modifiers() == Qt::CTRL) {
          _isDragging = true;
          result= true;
        }
      }
      else if(e->type() == QEvent::MouseButtonRelease && qMouseEv->modifiers() == Qt::CTRL) {
        _isDragging = false;
        _polygon.clear();
        _currentNode = node(UINT_MAX);
        _mainWidget->redraw();
        result= true;
      }
      else if(_isDragging && e->type() == QEvent::MouseMove) {
        Coord currentPointerScreenCoord = Coord(qMouseEv->x(), _mainWidget->height() - qMouseEv->y());
        _polygon.push_back(currentPointerScreenCoord);
        
        edge tmpEdge;
        node tmpNode;
        ElementType type;
        
        bool result = _mainWidget->doSelect(qMouseEv->x(),qMouseEv->y(),type,tmpNode,tmpEdge);
        if(result && type == NODE) {
          if(_currentNode.isValid()) {
            Graph* graph = _mainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph();
            if(!graph->existEdge(_currentNode, tmpNode, false))
              graph->addEdge(_currentNode, tmpNode);
            _currentNode = tmpNode;
            
          }
          else {
            _currentNode = tmpNode;
          }
        }
        
        _mainWidget->redraw();
        result = true;
      }
      
      return result;
    }
    
    virtual bool draw(GlMainWidget* ) {
      if (_polygon.size() > 0) {
        Camera camera2D(_camera->getScene(), false);

        glEnable(GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        camera2D.initGl();
        GlComplexPolygon complexPolygon(_polygon, Color(0,0,0,0), Color(0,255,0));
        complexPolygon.draw(0,0);
      }
      return true;
    }
    
    InteractorComponent *clone() { return new CustomMouseEdgeBuilder(); }
    
    virtual void setView(View* view) {
      GlMainView *glView = dynamic_cast<GlMainView *>(view);
      if (glView) {
        _mainWidget = glView->getGlMainWidget();
      }
    }
    
  private:
    bool _isDragging;
    tlp::node _currentNode;
    GlMainWidget* _mainWidget;
    Camera* _camera;
    std::vector<Coord> _polygon;
};

class GeneralPurposeInteractor  : public NodeLinkDiagramComponentInteractor {

public:

  /**
   * Default constructor
   */
  GeneralPurposeInteractor():NodeLinkDiagramComponentInteractor(":/i_open.png","general purpose"){
    setPriority(1);
    setConfigurationWidgetText(QString("<h3>general purpose interactor</h3>")+
                   "<br><b>Mouse left</b> double-click to add a node in the graph" + 
                   "<br><b>Alt + Mouse Left + Move Mouse</b> click and drag to create edges." + 
                   "<br><b>zoom and pan interactions</b>");
  }

  /**
   * Construct chain of responsibility
   */
  void construct(){
    pushInteractorComponent(new MouseNKeysNavigator);
    pushInteractorComponent(new CustomMouseEdgeBuilder());
    pushInteractorComponent(new CustomMouseNodeBuilder);
  }
};

INTERACTORPLUGIN(GeneralPurposeInteractor, "GeneralPurposeInteractor", "Tulip Team", "24/08/2010", "General purpose interactor : zoom and pan, add nodes, add edges.", "1.0");
