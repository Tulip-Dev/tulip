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
#include <tulip/PythonIncludes.h>
#include <tulip/PythonInterpreter.h>
#include <QPixmap>
#include <QImage>
#include <QGraphicsView>
#include <iostream>
#include <stdint.h>
%End

namespace tlp {

class View  : tlp::Plugin /Abstract/ {
%TypeHeaderCode
#include <tulip/View.h>
%End

%ConvertToSubClassCode
   if (dynamic_cast<tlp::NodeLinkDiagramComponent*>(sipCpp)) {
        sipType = sipFindType("tlp::NodeLinkDiagramComponent");
   } else if (dynamic_cast<tlp::GlMainView*>(sipCpp)) {
        sipType = sipFindType("tlp::GlMainView");
   } else {
        sipType = NULL;
   }
%End

%Docstring
.. versionadded:: 4.4

Bases: :class:`tulip.tlp.Plugin`

This is the base class for Tulip data views. Use the following functions to get instances on such object :

    * :func:`tlpgui.createView`
    * :func:`tlpgui.getOpenedViews`
    * :func:`tlpgui.getViewsOfGraph`

%End

public:

// =========================================================================================================================

   View();

// =========================================================================================================================

   virtual ~View();

// =========================================================================================================================

   PyObject* viewQFrame() const;
%Docstring
tlpgui.View.viewQFrame()

Dynamically wraps the QFrame enclosing the view into a :class:`PyQt4.QtGui.QFrame` instance.
The purpose of that method is for developers that want to integrate a Tulip view in their PyQt4 application.
You need to have PyQt4 install for that feature to work linked against the same version
of Qt used to build Tulip and using the same version of SIP used to build the Tulip Python bindings
(meaning, you will surely have to compile Tulip and PyQt4 yourself, otherwise you are lucky).

:rtype: :class:`PyQt4.QtGui.QFrame`
%End

%MethodCode
   QString wrapViewQFrameFunc = ""
           "import sip\n"
           "import PyQt4.QtGui\n"

           "def wrapQFrame(address):\n"
           "    wrap = sip.wrapinstance(address, PyQt4.QtGui.QFrame)\n"
           "    sip.transferto(wrap, wrap)\n"
           "    return wrap\n"
           ;
   sipRes = NULL;
   if (tlp::PythonInterpreter::getInstance()->runString(wrapViewQFrameFunc)) {
       PyObject *wrapper = NULL;
       if (tlp::PythonInterpreter::getInstance()->callFunctionOneParamAndGetReturnValue(QString("__main__"), QString("wrapQFrame"), reinterpret_cast<uintptr_t>(sipCpp->graphicsView()->parent()), wrapper)) {
           sipRes = wrapper;
           Py_INCREF(sipRes);
           sipTransferTo(sipRes, NULL);
       }
   }
%End

// =========================================================================================================================

   PyObject* viewQGraphicsView() const;
%Docstring
tlpgui.View.viewQFrame()

Dynamically wraps the QGraphicsView used to display the view content into a :class:`PyQt4.QtGui.QGraphicsView` instance.
The purpose of that method is for developers that want to integrate a Tulip view in their PyQt4 application.
You need to have PyQt4 install for that feature to work linked against the same version
of Qt used to build Tulip and using the same version of SIP used to build the Tulip Python bindings
(meaning, you will surely have to compile Tulip and PyQt4 yourself, otherwise you are lucky).

:rtype: :class:`PyQt4.QtGui.QGraphicsView`
%End

%MethodCode
    QString wrapViewQGraphicsViewFunc = ""
        "import sip\n"
        "import PyQt4.QtGui\n"
        "def wrapQGraphicsView(address):\n"
        "    wrap = sip.wrapinstance(address, PyQt4.QtGui.QGraphicsView)\n"
        "    sip.transferto(wrap, wrap)\n"
        "    return wrap\n"
        ;
    sipRes = NULL;
    if (tlp::PythonInterpreter::getInstance()->runString(wrapViewQGraphicsViewFunc)) {
        PyObject *wrapper = NULL;
        if (tlp::PythonInterpreter::getInstance()->callFunctionOneParamAndGetReturnValue(QString("__main__"), QString("wrapQGraphicsView"), reinterpret_cast<uintptr_t>(sipCpp->graphicsView()), wrapper)) {
            sipRes = wrapper;
            Py_INCREF(sipRes);
            sipTransferTo(sipRes, NULL);
        }
    }
%End

// =========================================================================================================================

    virtual void setState(const tlp::DataSet &dataSet) = 0;
%Docstring
tlpgui.View.setState(dataSet)

Sets the parameters for the view.

:param dataSet: the view parameters
:type dataSet: :class:`tulip.tlp.DataSet`
%End

// =========================================================================================================================

    virtual void setGraph(tlp::Graph *graph) = 0;
%Docstring
tlpgui.View.setGraph(graph)

Sets the graph for the view. The current parameters will be kept.

:param graph: the graph from which the view will be drawn.
:type graph: :class:`tulip.tlp.Graph`
%End

%MethodCode
    tvm.graphModel()->addGraph(a0);
    if (!tvm.tlpWorkspace()) {
      a0->addListener(&tvm);
    }
    sipCpp->setGraph(a0);
%End

// =========================================================================================================================

    virtual tlp::DataSet state() const = 0;
%Docstring
tlpgui.View.state()

Returns the current parameters of the view.

:rtype: :class:`tulip.tlp.DataSet`
%End

// =========================================================================================================================

    virtual tlp::Graph *graph() = 0;
%Docstring
tlpgui.View.graph()

Returns the graph currently attached to the view.

:rtype: :class:`tulip.tlp.Graph`
%End

	virtual void draw() = 0;
%Docstring
tlpgui.View.draw()

Draws/Updates the view.
%End

%MethodCode
    sipCpp->draw();
    QApplication::processEvents();
%End

// =========================================================================================================================

    void setVisible(const bool visible);
%Docstring
tlpgui.View.setVisible(visible)

Hides / Shows the view window. That method has effects only when working trough the
classical Python interpreter.

:param visible: the state of the view window visibility
:type visible: boolean

%End

%MethodCode
    tvm.setViewVisible(sipCpp, a0);
%End

// =========================================================================================================================

    void resize(int width, int height);
%Docstring
tlpgui.View.resize(width, height)

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
tlpgui.View.setPos(x, y)

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

  bool saveSnapshot(const std::string& pictureName, int width=-1, int height=-1);
%Docstring
tlpgui.View.saveSnapshot(picturePath, width, height)

Takes a snapshot of the view and saves it to an image file on disk.
The view is not required to be visible for that feature to work.
Returns :const:`True` if the operation succeeds.

:param picturePath: the absolute path of the image file with extension (used to infer image type)
:type picturePath: string
:param width: the image width
:type width: integer
:param height: the image height
:type height: integer
:rtype: boolean
%End

%MethodCode
    QPixmap snapshot = sipCpp->snapshot(QSize(a1, a2));
    sipRes = snapshot.toImage().save(a0->c_str());
%End

// =========================================================================================================================

    std::list<tlp::Interactor*> interactors() const;
%Docstring
tlpgui.View.interactors()

Returns the interactors currently installed on the view.

:rtype: list of :class:`tlpgui.Interactor`
%End

%MethodCode
    std::list<tlp::Interactor*> ret;
    QList<tlp::Interactor*> interactors = sipCpp->interactors();
    for (int i = 0 ; i < interactors.count() ; ++i) {
        ret.push_back(interactors.at(i));
    }
    sipRes = new std::list<tlp::Interactor*>(ret);
%End

// =========================================================================================================================

    tlp::Interactor* currentInteractor() const;
%Docstring
tlpgui.Interactor.currentInteractor()

Returns the current active interactor of the view.

:rtype: :class:`tlpgui.Interactor`
%End


// =========================================================================================================================

    std::list<tlp::Observable*> triggers() const;
%MethodCode
    std::list<tlp::Observable*> ret;
    QSet<tlp::Observable*> obs = sipCpp->triggers();
    QSet<tlp::Observable *>::const_iterator i = obs.constBegin();
    for ( ; i != obs.constEnd() ; ++i) {
        ret.push_back(*i);
    }
    sipRes = new std::list<tlp::Observable*>(ret);
%End

// =========================================================================================================================

    void setInteractors(const std::list<tlp::Interactor*>&);
%Docstring
tlpgui.View.setInteractors(interactors)

Installs a new set of interactors on the view.

:param interactors: the new interactors to install
:type interactors: list of :class:`tlpgui.Interactor`
%End


%MethodCode
    QList<tlp::Interactor*> param;
    std::list<tlp::Interactor*>::const_iterator it = a0->begin();
    for (; it != a0->end() ; ++it) {
        param.append(*it);
    }
    sipCpp->setInteractors(param);
%End

// =========================================================================================================================

    void setCurrentInteractor(tlp::Interactor* currentInteractor);
%Docstring
tlpgui.View.setCurrentInteractor(interactor)

Set the current active interactor on the view.

:param interactor: the interactor to set active
:type interactor: :class:`tlpgui.Interactor`
%End

};
};
