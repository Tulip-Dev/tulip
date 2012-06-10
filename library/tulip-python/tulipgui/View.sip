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

%ModuleHeaderCode
#include <tulip/NodeLinkDiagramComponent.h>
#include <tulip/GlMainView.h>
#include <tulip/View.h>
#include <tulip/MainController.h>
%End

namespace tlp {

class View /Abstract/ {
%TypeHeaderCode
#include <tulip/View.h>
%End

%ConvertToSubClassCode
   if (dynamic_cast<tlp::NodeLinkDiagramComponent*>(sipCpp)) {
   		sipType = sipType_tlp_NodeLinkDiagramComponent;
   } else {
   		sipType = NULL;
   } 
%End

%Docstring
.. versionadded:: 3.8

This is the base class for Tulip data views. Use the following functions to get instances on such object :

    * :func:`tlp.addView`
    * :func:`tlp.getOpenedViews`
    * :func:`tlp.getViewsOfGraph`

%End

public:

	virtual void setData(tlp::Graph *graph, tlp::DataSet dataSet) = 0;
%Docstring
tlp.View.setData(graph, dataSet)

Sets the graph and parameters for the view.

:param graph: the graph from which the view will be drawn.
:type graph: :class:`tulip.tlp.Graph`
:param dataSet: the view parameters
:type dataSet: :class:`tulip.tlp.DataSet`
%End

%MethodCode
    tlp::MainController *mainController = dynamic_cast<tlp::MainController *>(tlp::Controller::getCurrentController());
    if (mainController) {
        if (!sipCpp->getGraph()->getRoot()->isDescendantGraph(a0) && a0 != sipCpp->getGraph()->getRoot()) {
            sipIsErr = 1;
            std::ostringstream oss;
            oss << "Error : the graph you are trying to set on a view does not belong to the same sub-graphs hierarchy as the one currently loaded." << std::endl;
            oss << "<graph " << a0->getName() << " (id " << a0->getId() << ")>";
            oss << "is not a descendant of <graph " << sipCpp->getGraph()->getName() << " (id " << sipCpp->getGraph()->getId() << ")>";
            PyErr_SetString(PyExc_Exception, oss.str().c_str());
         } else {
            mainController->setDataOfView(sipCpp, a0, *a1);
         }
    } else {
        sipCpp->setData(a0, *a1);
    }
%End

// =========================================================================================================================

    virtual void setGraph(tlp::Graph *graph) = 0;
%Docstring
tlp.View.setGraph(graph)

Sets the graph for the view. The current parameters will be kept.

:param graph: the graph from which the view will be drawn.
:type graph: :class:`tulip.tlp.Graph`
%End

%MethodCode
    tlp::MainController *mainController = dynamic_cast<tlp::MainController *>(tlp::Controller::getCurrentController());
    if (mainController) {
        if (!sipCpp->getGraph()->getRoot()->isDescendantGraph(a0) && a0 != sipCpp->getGraph()->getRoot()) {
            sipIsErr = 1;
            std::ostringstream oss;
            oss << "Error : the graph you are trying to set on a view does not belong to the same sub-graphs hierarchy as the one currently loaded." << std::endl;
            oss << "<graph " << a0->getName() << " (id " << a0->getId() << ")>";
            oss << "is not a descendant of <graph " << sipCpp->getGraph()->getName() << " (id " << sipCpp->getGraph()->getId() << ")>";
            PyErr_SetString(PyExc_Exception, oss.str().c_str());
        } else {
            mainController->setGraphOfView(sipCpp, a0);
        }
    } else {
        sipCpp->setGraph(a0);
    }
%End

// =========================================================================================================================

    tlp::DataSet getData();
%Docstring
tlp.View.getData()

Returns the current parameters of the view.

:rtype: :class:`tulip.tlp.DataSet`
%End

%MethodCode
    tlp::Graph *g = NULL;
    tlp::DataSet data;
    sipCpp->getData(&g, &data);
    sipRes = new tlp::DataSet(data);
%End

// =========================================================================================================================

	virtual tlp::Graph *getGraph() = 0;
%Docstring
tlp.View.getGraph()

Returns the graph currently attached to the view.

:rtype: :class:`tulip.tlp.Graph`
%End

// =========================================================================================================================

    virtual bool savePicture(const std::string& pictureName, int width, int height, bool center);
%Docstring
tlp.View.savePicture(picturePath, width, height, center)

Takes a snapshot of the view (if the view supports it), and saves it to a picture on disk.
Returns :const:`True` if the operation succeeds.

:param picturePath: the absolute path of the picture file with extension (used to infer image type)
:type picturePath: string
:param width: the picture width
:type width: integer
:param height: the picture height
:type height: integer
:param center: if :const:`True`, center the view before taking the snapshot, otherwise use the current zoom and pan level
:type center: boolean
:rtype: boolean
%End

// =========================================================================================================================

	virtual void draw() = 0;
%Docstring
tlp.View.draw()

Draws/Updates the view.
%End

%MethodCode
    sipCpp->draw();
    tlp::MainController *mainController = dynamic_cast<tlp::MainController *>(tlp::Controller::getCurrentController());
    if (!mainController) {
        QApplication::processEvents();
    }
%End

// =========================================================================================================================

    std::string getName();
%Docstring
tlp.View.getName()

Returns the name of the view.

:rtype: string
%End

%MethodCode
    sipRes = new std::string(tvm.getNameOfView(sipCpp));
%End

// =========================================================================================================================

    void setVisible(const bool visible);
%Docstring
tlp.View.setVisible(visible)

Hides / Shows the view window. That method has effects only when working trough the
classical Python interpreter. It can be usefull for scripts that only needs.
to save visualizations snaphots to disk. View windows can be hidden and the script will
terminate directly instead of waiting for the views windows to be closed by the user.
However, the view window needs to be displayed at least once in order to correctly
initialize the OpenGL context.

:param visible: the state of the view window visibility
:type visible: boolean

%End

%MethodCode
    tvm.setViewVisible(sipCpp, a0);
%End

// =========================================================================================================================

    void resize(int width, int height);
%Docstring
tlp.View.resize(width, height)

Resizes the view windows. That method has effects only when working trough the
classical Python interpreter.

:param width: the new width of the view window
:type width: integer
:param height: the new height of the view window
:type height: integer
%End

%MethodCode
    tvm.resizeView(sipCpp, a0, a1);
%End

// =========================================================================================================================

    void setPos(int x, int y);
%Docstring
tlp.View.setPos(x, y)

Set the position on screen of the view windows. That method has effects only when working trough the
classical Python interpreter.

:param x: the new x position of the view window
:type x: integer
:param y: the new y position of the view window
:type y: integer
%End

%MethodCode
    tvm.setViewPos(sipCpp, a0, a1);
%End

// =========================================================================================================================

    void setOptionsWidgetsVisible(const bool visible);
%Docstring
tlp.View.setOptionsWidgetsVisible(visible)

Shows / hides the options widgets located on the left side of the view.
That method has effects only when working trough the
classical Python interpreter.

:param visible: the state of the options widgets visibility
:type visible: boolean
%End

%MethodCode
    tvm.setViewOptionsWidgetsVisible(sipCpp, a0);
%End

};
};
