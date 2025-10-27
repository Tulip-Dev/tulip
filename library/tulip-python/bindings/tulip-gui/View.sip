/*
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

// +-------------------------------------------------------------------------+
// | Tulip Python Bindings                                                   |
// | inspired from bindings by the Booggie project development team          |
// | (https://sourceforge.net/projects/booggie/)                             |
// +-------------------------------------------------------------------------+

%ModuleHeaderCode
#include <tulip/NodeLinkDiagramComponent.h>
#include <tulip/GlMainView.h>
#include <tulip/View.h>
#include <QPixmap>
#include <QImage>
#include <QGraphicsView>
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
        sipType = nullptr;
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
   virtual ~View();

// =========================================================================================================================

    virtual void setState(const tlp::DataSet &dataSet) = 0;
%Docstring
tlpgui.View.setState(dataSet)

Sets the parameters for the view.

:param dataSet: the view parameters
:type dataSet: :class:`tulip.tlp.DataSet`
%End
%MethodCode
  bool viewOk = isViewOpened(sipCpp, sipIsErr);
  if(viewOk)
    sipCpp->setState(*a0);
%End


// =========================================================================================================================

void setGraph(tlp::Graph *graph);
%Docstring
tlpgui.View.setGraph(graph)

Sets the graph for the view. The current parameters will be kept.

:param graph: the graph from which the view will be drawn.
:type graph: :class:`tulip.tlp.Graph`
%End

%MethodCode
  bool viewOk = isViewOpened(sipCpp, sipIsErr);
  if(viewOk)
    sipCpp->setGraph(a0);
%End

// =========================================================================================================================

virtual tlp::DataSet state() const = 0;
%Docstring
tlpgui.View.state()

Returns the current parameters of the view.

:rtype: :class:`tulip.tlp.DataSet`
%End
%MethodCode
bool viewOk = isViewOpened(sipCpp, sipIsErr);
if(viewOk) {
  sipRes = new tlp::DataSet(sipCpp->state());
}
%End

// =========================================================================================================================

tlp::Graph *graph() const;
%Docstring
tlpgui.View.graph()

Returns the graph currently attached to the view.

:rtype: :class:`tulip.tlp.Graph`
%End
%MethodCode
bool viewOk = isViewOpened(sipCpp, sipIsErr);
if(viewOk)
  sipRes = sipCpp->graph();
%End
// =========================================================================================================================
virtual void draw() = 0;
%Docstring
tlpgui.View.draw()

Draws/Updates the view.
%End
%MethodCode
bool viewOk = isViewOpened(sipCpp, sipIsErr);
if(viewOk)
  sipCpp->draw();
%End

// =========================================================================================================================
void setVisible(const bool visible=true) /Deprecated/;
%Docstring
tlpgui.View.setVisible(visible=True)

  .. deprecated:: 6.0.0
    Use :func:`tlpgui.View.show` instead. To close a view use :func:`tlpgui.closeView`.

  Shows the view.

  :param visible: this parameter does nothing.
  :type visible: Boolean

%End

%MethodCode
  tlp::Workspace *w = tlpWorkspace(sipIsErr);
  if(w)
    w->addPanel(sipCpp);
%End

// =========================================================================================================================
void show();
%Docstring
tlpgui.View.show()

.. versionadded:: 6.0.0

Shows the view.

%End

%MethodCode
  tlp::Workspace *w = tlpWorkspace(sipIsErr);
  if(w) {
    auto a = createdView.find(sipCpp);
    if (a!=createdView.end()) {
      w->addPanel(sipCpp);
    }
    else {
      sipIsErr = 1;
      PyErr_SetString(PyExc_Exception, "This view does not exist or was created from the GUI.");
    }
  }

%End

// =========================================================================================================================

void resize(int width, int height) /Deprecated/;
%Docstring
tlpgui.View.resize(width, height)

.. deprecated:: 6.0.0

This function does nothing.

:param width: width
:type width: integer
:param height: height
:type height: integer
%End

%MethodCode
  sipIsErr = 1;
  PyErr_SetString(PyExc_Exception, "This function is deprecated and does nothing since Tulip 6.0.0");
%End

// =========================================================================================================================

void setPos(int x, int y)  /Deprecated/;
%Docstring
tlpgui.View.setPos(x, y)

.. deprecated:: 6.0.0

This function does nothing.

:param x: x
:type x: integer
:param y: y
:type y: integer
%End

%MethodCode
  sipIsErr = 1;
  PyErr_SetString(PyExc_Exception, "This function is deprecated and does nothing since Tulip 6.0.0");
%End

// =========================================================================================================================

  bool saveSnapshot(const std::string& pictureName, int width=-1, int height=-1);
%Docstring
tlpgui.View.saveSnapshot(picturePath, width, height)

Takes a snapshot of the view and saves it to an image file on disk.
The view is not required to be visible for that feature to work.
Returns :const:`True` if the operation succeeded.

:param picturePath: the absolute path of the image file with extension (used to infer image type)
:type picturePath: string
:param width: the image width
:type width: integer
:param height: the image height
:type height: integer
:rtype: boolean
%End

%MethodCode

bool viewOk = isViewOpened(sipCpp, sipIsErr);
if(viewOk) {
    QPixmap snapshot = sipCpp->snapshot(QSize(a1, a2));
    sipRes = snapshot.toImage().save(a0->c_str());
}
%End

// =========================================================================================================================

    std::list<tlp::Interactor*> interactors() const;
%Docstring
tlpgui.View.interactors()

Returns the interactors currently installed on the view.

:rtype: list of :class:`tlpgui.Interactor`
%End

%MethodCode
  bool viewOk = isViewOpened(sipCpp, sipIsErr);
  if(viewOk)
    sipRes = new std::list<tlp::Interactor*>(sipCpp->interactors());
%End

// =========================================================================================================================

tlp::Interactor* currentInteractor() const;
%Docstring
tlpgui.Interactor.currentInteractor()

Returns the current active interactor of the view.

:rtype: :class:`tlpgui.Interactor`
%End
%MethodCode
bool viewOk = isViewOpened(sipCpp, sipIsErr);
if(viewOk)
  sipRes = sipCpp->currentInteractor();
%End

// =========================================================================================================================

std::list<tlp::Observable*> triggers() const;
%MethodCode
bool viewOk = isViewOpened(sipCpp, sipIsErr);
if(viewOk) {
    sipRes = new std::list<tlp::Observable*>(sipCpp->triggers());
  }
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
bool viewOk = isViewOpened(sipCpp, sipIsErr);
if(viewOk)
    sipCpp->setInteractors(*a0);
%End

// =========================================================================================================================
void setCurrentInteractor(tlp::Interactor* currentInteractor);
%Docstring
tlpgui.View.setCurrentInteractor(interactor)

Set the current active interactor on the view.

:param interactor: the interactor to set active
:type interactor: :class:`tlpgui.Interactor`
%End
%MethodCode
  bool viewOk = isViewOpened(sipCpp, sipIsErr);
  if(viewOk)
    sipCpp->setCurrentInteractor(a0);
%End

};
};
