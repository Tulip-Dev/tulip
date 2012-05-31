%ModuleHeaderCode
#include <tulip/MainController.h>
#include <tulip/TemplateFactory.h>
#include <QtGui/QWorkspace>

extern std::vector<std::string> getTulipViews();
%End

%ModuleCode
std::vector<std::string> getTulipViews() {
    std::vector<std::string> ret;
    tlp::TemplateFactory<tlp::ViewFactory, tlp::View, tlp::ViewContext>::ObjectCreator::const_iterator it;
    for (it=tlp::ViewFactory::factory->objMap.begin(); it != tlp::ViewFactory::factory->objMap.end(); ++it) {
        if (it->first != "Python Script view") {
            ret.push_back(it->first);
        }
    }
    return ret;
}
%End

namespace tlp {

std::vector<std::string> getAvailableViews() const;
%Docstring
tlp.getAvailableViews()

.. versionadded:: 3.8

Returns a list of the views names currently available in Tulip.

:rtype: list of string
%End

%MethodCode
    sipRes = new std::vector<std::string>(getTulipViews());
%End

// =========================================================================================

std::vector<tlp::View *> getOpenedViews() const;
%Docstring
tlp.getOpenedViews()

.. versionadded:: 3.8

Returns a list of the views (except the Python ones) currently opened in Tulip.

:rtype: list of :class:`tlp.View`
%End

%MethodCode
    std::vector<tlp::View *> views;
    tlp::MainController *mainController = dynamic_cast<tlp::MainController *>(tlp::Controller::getCurrentController());
    if (mainController) {
        std::vector<tlp::View *> viewsTmp;
        mainController->getViews(viewsTmp);
        for (size_t i = 0 ; i < viewsTmp.size() ; ++i) {
            if (mainController->getNameOfView(viewsTmp[i]) != "Python Script view") {
                views.push_back(viewsTmp[i]);
            }
        }
    }
    sipRes = new std::vector<tlp::View *>(views);
%End

// =========================================================================================

    tlp::View *addView(const std::string &viewName, tlp::Graph *graph, tlp::DataSet dataSet);
%Docstring
tlp.addView(viewName, graph, dataSet)

.. versionadded:: 3.8

Creates and add a new view in the Tulip workspace. Returns an instance on the newly created view.

:param viewName: the name of the view to create (use :func:`tlp.getAvailableViews` to know which ones are installed)
:type viewName: string
:param graph: the graph to set on the new view
:type graph: :class:`tulip.tlp.Graph`
:param dataSet: the parameters of the view
:type dataSet: :class:`tulip.tlp.Dataset`
:rtype: :class:`tlp.View`
%End

%MethodCode
    sipRes = NULL;
    tlp::MainController *mainController = dynamic_cast<tlp::MainController *>(tlp::Controller::getCurrentController());
    if (mainController) {
        std::vector<std::string> tlpViews = getTulipViews();
        if (std::find(tlpViews.begin(), tlpViews.end(), *a0) != tlpViews.end()) {
            sipRes = mainController->createView(*a0, a1, *a2);
        } else {
            sipIsErr = 1;
            std::ostringstream oss;
            oss << "Error : No Tulip view named \"" << *a0 << "\".";
            PyErr_SetString(PyExc_Exception, oss.str().c_str());
        }
    }
%End

// =========================================================================================

    void closeView(tlp::View *view);
%Docstring
tlp.closeView(view)

.. versionadded:: 3.8

Closes a view in the Tulip workspace.

:param view: the view to close
:type view: :class:`tlp.View`
%End

%MethodCode
    tlp::MainController *mainController = dynamic_cast<tlp::MainController *>(tlp::Controller::getCurrentController());
    if (mainController) {
        releaseSIPWrapper(a0, sipType_tlp_View);
        mainController->closeView(a0);
    }
%End

// =========================================================================================

    void tileViews();
%Docstring
tlp.tileViews()

.. versionadded:: 3.8

Tiles the opened views in the current workspace.
%End

%MethodCode
    tlp::MainController *mainController = dynamic_cast<tlp::MainController *>(tlp::Controller::getCurrentController());
    if (mainController) {
        mainController->getMainWindowFacade()->getWorkspace()->tile();
    }
%End

// =========================================================================================

    std::vector<tlp::View*> getViewsOfGraph(tlp::Graph *graph);
%Docstring
tlp.getViewsOfGraph(graph)

.. versionadded:: 3.8

Returns a list of views (except the Python ones) associated to a particular graph.

:param graph: the graph on which to find views
:type graph: :class:`tulip.tlp.Graph`
:rtype: list of :class:`tlp.View`
%End

%MethodCode
    std::vector<tlp::View*> ret;
    tlp::MainController *mainController = dynamic_cast<tlp::MainController *>(tlp::Controller::getCurrentController());
    if (mainController) {
        std::vector<tlp::View*> tmp = mainController->getViewsOfGraph(a0);
        for (size_t i = 0 ; i < tmp.size() ; ++i) {
            if (mainController->getNameOfView(tmp[i]) != "Python Script view") {
                ret.push_back(tmp[i]);
            }
        }
    }
    sipRes = new std::vector<tlp::View*>(ret);
%End

// =========================================================================================

    void closeAllViews();
%Docstring
tlp.closeAllViews()

.. versionadded:: 3.8

Closes all the views (except the Python ones) in the Tulip workspace.
%End

%MethodCode
    std::vector<tlp::View*> views;
    tlp::MainController *mainController = dynamic_cast<tlp::MainController *>(tlp::Controller::getCurrentController());
    if (mainController) {
        mainController->getViews(views);
        for (size_t i = 0 ; i < views.size() ; ++i) {
            if (mainController->getNameOfView(views[i]) != "Python Script view") {
                releaseSIPWrapper(views[i], sipType_tlp_View);
                mainController->closeView(views[i]);
            }
        }
    }
%End

// =========================================================================================

    void closeViewsRelatedToGraph(tlp::Graph* graph);
%Docstring
tlp.closeViewsRelatedToGraph(graph)

.. versionadded:: 3.8

Closes all the views (except the Python ones) associated to a particular graph.

:param graph: the graph on which to close views
:type graph: :class:`tulip.tlp.Graph`
%End

%MethodCode
    std::vector<tlp::View*> views;
    tlp::MainController *mainController = dynamic_cast<tlp::MainController *>(tlp::Controller::getCurrentController());
    if (mainController) {
        views = mainController->getViewsOfGraph(a0);
        for (size_t i = 0 ; i < views.size() ; ++i) {
            if (mainController->getNameOfView(views[i]) != "Python Script view") {
                if (views[i]->getGraph() == a0) {
                    releaseSIPWrapper(views[i], sipType_tlp_View);
                    mainController->closeView(views[i]);
                }
            }
        }
    }
%End

};
