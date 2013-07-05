%ModuleHeaderCode
#include <TulipViewsUtils.h>
extern TulipViewsManager tvm;
%End

%ModuleCode
TulipViewsManager tvm;
%End

namespace tlp {

std::vector<std::string> getAvailableViews() const;
%Docstring
tlpgui.getAvailableViews()

.. versionadded:: 4.4

Returns a list of the views names currently available in Tulip.

:rtype: list of string
%End

%MethodCode
    sipRes = new std::vector<std::string>(tvm.getTulipViews());
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
    sipRes = new std::vector<tlp::View *>(tvm.getOpenedViews());
%End

// =========================================================================================

std::vector<tlp::View *> getOpenedViewsWithName(const std::string &viewName) const;
%Docstring
tlpgui.getOpenedViewsWithName(viewName)

.. versionadded:: 4.4

Returns a list of the views named viewName currently opened. To get the names of
the available views, use function :func:`tlpgui.getAvailableViews`.

:param viewName: the view name (e.g. "Node Link Diagram view")
:type viewName: string
:rtype: list of :class:`tlpgui.View`
%End

%MethodCode
    sipRes = new std::vector<tlp::View *>(tvm.getOpenedViewsWithName(*a0));
%End

// =========================================================================================

            tlp::View *createView(const std::string &viewName, tlp::Graph *graph, tlp::DataSet dataSet = tlp::DataSet(), bool show = true);
%Docstring
        tlpgui.addView(viewName, graph[, dataSet = tlp.DataSet(), show = True])

.. versionadded:: 4.4

Creates and adds a new Tulip view. If called through a Python Script editor in the main Tulip GUI, the
view will be added in the Tulip workspace. If called through the classical Python shell,
a dialog containing the view will be created and displayed.
Returns an instance of the newly created view.

:param viewName: the name of the view to create (use :func:`tlpgui.getAvailableViews` to know which ones are installed)
:type viewName: string
:param graph: the graph to set on the new view
:type graph: :class:`tulip.tlp.Graph`
:param dataSet: the optional parameters of the view
:type dataSet: :class:`tulip.tlp.DataSet`
:rtype: :class:`tlpgui.View`
%End

%MethodCode
    sipRes = NULL;
    std::vector<std::string> tlpViews = tvm.getTulipViews();
    if (std::find(tlpViews.begin(), tlpViews.end(), *a0) != tlpViews.end()) {
        if (tvm.tlpWorkspace()) {
            PyObject *pyObj = sipGetPyObject(a1, sipFindType("tlp::Graph"));
            if (pyObj) {
                sipTransferTo(pyObj, pyObj);
            }
        }
        sipRes = tvm.addView(*a0, a1, *a2, a3);
    } else {
        sipIsErr = 1;
        std::ostringstream oss;
        oss << "Error : No Tulip view named \"" << *a0 << "\".";
        PyErr_SetString(PyExc_Exception, oss.str().c_str());
    }
%End

// =========================================================================================

tlp::NodeLinkDiagramComponent *createNodeLinkDiagramView(tlp::Graph *graph, tlp::DataSet dataSet = tlp::DataSet(), bool show = true);
%Docstring
tlpgui.addNodeLinkDiagramView(graph)

.. versionadded:: 4.4

Convenient function for creating the principal view offered by Tulip : a node link diagram.
Its call is equivalent to::

    nodeLinkView = tlp.createView("Node Link Diagram view", graph)

:param graph: the graph to visualize
:type graph: :class:`tulip.tlp.Graph`
:rtype: :class:`tlpgui.NodeLinkDiagramComponent`
%End

%MethodCode
    sipRes = static_cast<tlp::NodeLinkDiagramComponent *>(tvm.addView("Node Link Diagram view", a0, *a1, a2));
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
    tvm.closeView(a0);
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
    sipRes = new std::vector<tlp::View*>(tvm.getViewsOfGraph(a0));
%End

// =========================================================================================

    void closeAllViews();
%Docstring
tlpgui.closeAllViews()

.. versionadded:: 4.4

Closes and destroys all opened views.
%End

%MethodCode
    tvm.closeAllViews();
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
    tvm.closeViewsRelatedToGraph(a0);
%End

// =========================================================================================

    void runQtMainLoop();
%MethodCode
    if (tvm.areViewsVisible()) {
      QApplication::exec();
    }

%End

};
