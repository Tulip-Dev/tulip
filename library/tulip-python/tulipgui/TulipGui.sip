%ModuleHeaderCode
#include <TulipViewsUtils.h>
extern TulipViewsManager tvm;
%End

%ModuleCode
TulipViewsManager tvm;
%End

namespace tlp {

void loadGUIPlugins();
%Docstring
tlp.loadGUIPlugins()

Loads Tulip plugins related to the :mod:`tulipgui` module (views, interactors, glyphs).
%End

%MethodCode
 tlp::InteractorManager::getInst().loadPlugins();
 tlp::ViewPluginsManager::getInst().loadPlugins();
 tlp::GlyphManager::getInst().loadPlugins();
%End

std::vector<std::string> getAvailableViews() const;
%Docstring
tlp.getAvailableViews()

.. versionadded:: 3.8

Returns a list of the views names currently available in Tulip.

:rtype: list of string
%End

%MethodCode
    sipRes = new std::vector<std::string>(tvm.getTulipViews());
%End

// =========================================================================================

std::vector<tlp::View *> getOpenedViews() const;
%Docstring
tlp.getOpenedViews()

.. versionadded:: 3.8

Returns a list of the views currently opened.

:rtype: list of :class:`tlp.View`
%End

%MethodCode
    sipRes = new std::vector<tlp::View *>(tvm.getOpenedViews());
%End

// =========================================================================================

            tlp::View *addView(const std::string &viewName, tlp::Graph *graph, tlp::DataSet dataSet = tlp::DataSet());
%Docstring
tlp.addView(viewName, graph[, dataSet])

.. versionadded:: 3.8

Creates and adds a new Tulip view. If called through a Python Script editor in the main Tulip GUI, the
view will be added in the Tulip workspace. If called through the classical Python shell,
a dialog containing the view will be created and displayed.
Returns an instance of the newly created view.

:param viewName: the name of the view to create (use :func:`tlp.getAvailableViews` to know which ones are installed)
:type viewName: string
:param graph: the graph to set on the new view
:type graph: :class:`tulip.tlp.Graph`
:param dataSet: the optional parameters of the view
:type dataSet: :class:`tulip.tlp.Dataset`
:rtype: :class:`tlp.View`
%End

%MethodCode
    sipRes = NULL;
    std::vector<std::string> tlpViews = tvm.getTulipViews();
    if (std::find(tlpViews.begin(), tlpViews.end(), *a0) != tlpViews.end()) {
        sipRes = tvm.addView(*a0, a1, *a2);
    } else {
        sipIsErr = 1;
        std::ostringstream oss;
        oss << "Error : No Tulip view named \"" << *a0 << "\".";
        PyErr_SetString(PyExc_Exception, oss.str().c_str());
    }
%End

// =========================================================================================

tlp::NodeLinkDiagramComponent *addNodeLinkDiagramView(tlp::Graph *graph);
%Docstring
tlp.addNodeLinkDiagramView(graph)

.. versionadded:: 3.8

Convenient function for creating the principal view offered by Tulip : a node link diagram.
Its call is equivalent to::

    nodeLinkView = tlp.addView("Node Link Diagram view", graph)

:param graph: the graph to visualize
:type graph: :class:`tulip.tlp.Graph`
:rtype: :class:`tlp.NodeLinkDiagramComponent`
%End

%MethodCode
    sipRes = static_cast<tlp::NodeLinkDiagramComponent *>(tvm.addView("Node Link Diagram view", a0));
%End

// =========================================================================================

    void closeView(tlp::View *view);
%Docstring
tlp.closeView(view)

.. versionadded:: 3.8

Closes an opened view.

:param view: the view to close
:type view: :class:`tlp.View`
%End

%MethodCode
    tvm.closeView(a0);
%End

// =========================================================================================

    void tileViews();
%Docstring
tlp.tileViews()

.. versionadded:: 3.8

Tiles the opened views in the current workspace (this function has no effect when called
from the classical Python shell).
%End

%MethodCode
    tvm.tileViews();
%End

// =========================================================================================

    std::vector<tlp::View*> getViewsOfGraph(tlp::Graph *graph);
%Docstring
tlp.getViewsOfGraph(graph)

.. versionadded:: 3.8

Returns a list of views associated to a particular graph.

:param graph: the graph on which to find views
:type graph: :class:`tulip.tlp.Graph`
:rtype: list of :class:`tlp.View`
%End

%MethodCode
    sipRes = new std::vector<tlp::View*>(tvm.getViewsOfGraph(a0));
%End

// =========================================================================================

    void closeAllViews();
%Docstring
tlp.closeAllViews()

.. versionadded:: 3.8

Closes all opened views.
%End

%MethodCode
    tvm.closeAllViews();
%End

// =========================================================================================

    void closeViewsRelatedToGraph(tlp::Graph* graph);
%Docstring
tlp.closeViewsRelatedToGraph(graph)

.. versionadded:: 3.8

Closes all the views associated to a particular graph.

:param graph: the graph on which to close views
:type graph: :class:`tulip.tlp.Graph`
%End

%MethodCode
    tvm.closeViewsRelatedToGraph(a0);
%End

// =========================================================================================

    void runQtMainLoop();
%Docstring
tlp.runQtMainLoop()

.. versionadded:: 3.8

That function allows to run the main Qt event loop.
It should only be used when executing a script through the classical Python interpreter
(it has no effect when calling it from a Python script editor in the main Tulip GUI).
There is no need to call it when working in an interactive Python shell, as Qt events
are processed when the shell is waiting for input.
It will return only when there is no more Qt widgets displayed.
So it should be the last instruction of a script and it enables to keep the created Tulip
views displayed and functionnal (otherwise, the interpreter is closed and so are the views).
%End

%MethodCode
    tlp::MainController *mainController = dynamic_cast<tlp::MainController *>(tlp::Controller::getCurrentController());
    if (!mainController) {
        if (tvm.areViewsVisible())
            QApplication::exec();
    }
%End

};
