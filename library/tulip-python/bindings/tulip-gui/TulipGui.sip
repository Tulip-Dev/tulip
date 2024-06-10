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
extern tlp::Workspace* tlpWorkspace(int& sipIsErr);
extern bool isViewOpened(const tlp::View *v, int& sipIsErr);
extern std::set<tlp::View *> createdView;
%End

%ModuleCode

std::set<tlp::View *> createdView;

tlp::Workspace* tlpWorkspace(int& sipIsErr) {
  tlp::Perspective *perspective = tlp::Perspective::instance();
  if (perspective)
    return perspective->mainWindow()->findChild<tlp::Workspace *>();

  sipIsErr = 1;
  PyErr_SetString(PyExc_Exception, "No Tulip workspace found. Are we running inside Tulip? This module is not designed to run outside the Tulip GUI");
  return nullptr;  
}

bool isViewOpened(const tlp::View *v, int& sipIsErr) {
  tlp::Workspace *w = tlpWorkspace(sipIsErr);
  if(w) {
    auto p = w->panels();
    if(std::find(p.begin(), p.end(), v)!= p.end()) {
      return true;
    }
    else {
      releaseSIPWrapper(const_cast<tlp::View*>(v), sipFindType("tlp::View"));
      sipIsErr = 1;
      PyErr_SetString(PyExc_Exception, "This view does not exist anymore");
    }
  }
  return false;
}

%End

namespace tlp {

std::list<std::string> getAvailableViews() const;
%Docstring
tlpgui.getAvailableViews()

.. versionadded:: 4.4

Returns a list of the names of the views currently available.

:rtype: list of strings
%End

%MethodCode
  sipRes = new std::list<std::string>(tlp::PluginLister::availablePlugins<tlp::View>());
%End

// =========================================================================================

std::vector<tlp::View *> getOpenedViews() const;
%Docstring
tlpgui.getOpenedViews()

.. versionadded:: 4.4

Returns a list of the views currently opened.

:rtype: list of :class:`tlpgui.View`
%End

%MethodCode
  tlp::Workspace *w = tlpWorkspace(sipIsErr);
  sipRes = w?(new std::vector<tlp::View *>(w->panels())):nullptr;
%End

// =========================================================================================

std::vector<tlp::View *> getOpenedViewsWithName(const std::string &viewName) const;
%Docstring
tlpgui.getOpenedViewsWithName(viewName)

.. versionadded:: 4.4

Returns a list of the views named viewName currently opened. To get the names of
the available views, use function :func:`tlpgui.getAvailableViews`.

:param viewName: the view name (e.g., "Node Link Diagram view")
:type viewName: string
:rtype: list of :class:`tlpgui.View`
%End

%MethodCode
  tlp::Workspace *w = tlpWorkspace(sipIsErr);
  std::vector<tlp::View *> ret;
  if(w) {
    for (tlp::View *v : w->panels()) {
      if (v->name() == *a0)
        ret.push_back(v);
      }
  }
  sipRes = w?(new std::vector<tlp::View *>(ret)):nullptr;
%End

// =========================================================================================

tlp::View *createView(const std::string &viewName, tlp::Graph *graph, tlp::DataSet dataSet = tlp::DataSet(), bool show = true);
%Docstring
tlpgui.createView(viewName, graph, dataSet={}, show=True)

.. versionadded:: 4.4

Creates and adds a new Tulip view. The view will be added in the Tulip workspace.
Returns an instance of the newly created view.

:param viewName: the name of the view to create (use :func:`tlpgui.getAvailableViews` to know which ones are available)
:type viewName: string
:param graph: the graph to set on the new view
:type graph: :class:`tulip.tlp.Graph`
:param dataSet: the optional parameters of the view
:type dataSet: a dictionary with string keys (parameters names) and parameters values
:param show: show the view if :const:`True`
:type show: boolean
:rtype: :class:`tlpgui.View`
%End

%MethodCode
  tlp::Workspace *w = tlpWorkspace(sipIsErr);
  if(!sipIsErr) {
    for (auto v : tlp::PluginLister::availablePlugins<tlp::View>()) {
      if (v == *a0) {
        tlp::View *viewobj = tlp::PluginLister::getPluginObject<tlp::View>(*a0);
        viewobj->setupUi();
        viewobj->setGraph(a1);
        viewobj->setState(*a2);
        if(a3)
          w->addPanel(viewobj);
        createdView.insert(viewobj);
        sipRes = viewobj;

        }
      }
    if(!sipRes) {
      sipIsErr = 1;
      std::ostringstream oss;
      oss << "Error : No Tulip view named \"" << *a0 << "\".";
      PyErr_SetString(PyExc_Exception, oss.str().c_str());
      }
  }
%End

// =========================================================================================

tlp::NodeLinkDiagramComponent *createNodeLinkDiagramView(tlp::Graph *graph, tlp::DataSet dataSet = tlp::DataSet(), bool show = true);
%Docstring
tlpgui.createNodeLinkDiagramView(graph, dataSet={}, show=True)

.. versionadded:: 4.4

Convenient function for creating the principal view offered by Tulip: a node link diagram.
Its call is equivalent to::

    nodeLinkView = tlpgui.createView("Node Link Diagram view", graph)

:param graph: the graph to visualize
:type graph: :class:`tulip.tlp.Graph`
:param dataSet: the optional parameters of the view
:type dataSet: :class:`tulip.tlp.DataSet`
:param show: show the view if :const:`True`
:type show: boolean
:rtype: :class:`tlpgui.NodeLinkDiagramComponent`
%End

%MethodCode
  tlp::Workspace *w = tlpWorkspace(sipIsErr);
  if(!sipIsErr) {
    tlp::View *viewobj = tlp::PluginLister::getPluginObject<tlp::View>("Node Link Diagram view");
    viewobj->setupUi();
    viewobj->setGraph(a0);
    viewobj->setState(*a1);
    if(a2)
      w->addPanel(viewobj);
    createdView.insert(viewobj);
    sipRes = static_cast<tlp::NodeLinkDiagramComponent *>(viewobj);
    }
%End

// =========================================================================================

void closeView(tlp::View *view);
%Docstring
tlpgui.closeView(view)

.. versionadded:: 4.4

Closes and destroys an opened view.

:param view: the view to close
:type view: :class:`tlpgui.View`
%End

%MethodCode
  bool viewOk = isViewOpened(a0, sipIsErr);
  if(viewOk) {
    tlpWorkspace(sipIsErr)->delView(a0);
    auto a = createdView.find(a0);
    if (a!=createdView.end()) {
      createdView.erase(a0);
    }
    releaseSIPWrapper(a0, sipFindType("tlp::View"));    
  }
%End

// =========================================================================================

std::vector<tlp::View*> getViewsOfGraph(tlp::Graph *graph);
%Docstring
tlpgui.getViewsOfGraph(graph)

.. versionadded:: 4.4

Returns a list of views associated to a particular graph.

:param graph: the graph on which to find views
:type graph: :class:`tulip.tlp.Graph`
:rtype: list of :class:`tlpgui.View`
%End

%MethodCode
  tlp::Workspace* w = tlpWorkspace(sipIsErr);
  std::vector<tlp::View *> ret;
  if(w) {
    for (tlp::View *v : w->panels()) {
      if (v->graph() == a0)
        ret.push_back(v);
    }
  }
  sipRes = w?(new std::vector<tlp::View *>(ret)):nullptr;
%End

// =========================================================================================

void closeAllViews();
%Docstring
tlpgui.closeAllViews()

.. versionadded:: 4.4

Closes and destroys all opened views.
%End

%MethodCode
  tlp::Workspace* w = tlpWorkspace(sipIsErr);
  if(w) {
    for(tlp::View* v:w->panels()) {
      releaseSIPWrapper(v, sipFindType("tlp::View"));
    }
    w->closeAll();
    createdView.clear();
    }
%End

// =========================================================================================

void closeViewsRelatedToGraph(tlp::Graph* graph);
%Docstring
tlpgui.closeViewsRelatedToGraph(graph)

.. versionadded:: 4.4

Closes and destroys all the views associated to a particular graph.

:param graph: the graph on which to close views
:type graph: :class:`tulip.tlp.Graph`
%End

%MethodCode
  tlp::Workspace* w = tlpWorkspace(sipIsErr);
  if(w) {
    for(tlp::View* v:w->panels()) {
      if(v->graph() == a0) {
        releaseSIPWrapper(v, sipFindType("tlp::View"));
        w->delView(v);
        auto a = createdView.find(v);
        if (a!=createdView.end()) {
          createdView.erase(v);
        }
      }
    }
  }
%End

};
