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
#include <tulip/GlMainWidget.h>
#include <tulip/RenderingParametersDialog.h>
#include <tulip/LayerManagerWidget.h>

extern void updateNodeLinkConfWidgets(tlp::NodeLinkDiagramComponent *nlv);

%End

%ModuleCode

void updateNodeLinkConfWidgets(tlp::NodeLinkDiagramComponent *nlv) {
    tlp::GlMainWidget *glWidget = nlv->getGlMainWidget();
    std::list<std::pair<QWidget *,std::string> > confWidgets = nlv->getConfigurationWidget();
    std::list<std::pair<QWidget *,std::string> >::iterator it;
    for (it = confWidgets.begin() ; it != confWidgets.end() ; ++it) {
        tlp::RenderingParametersDialog *rpDialog = dynamic_cast<tlp::RenderingParametersDialog *>(it->first);
        tlp::LayerManagerWidget *lmWidget = dynamic_cast<tlp::LayerManagerWidget *>(it->first);
        if (rpDialog) {
            rpDialog->setGlMainWidget(glWidget);
        } else if (lmWidget) {
            lmWidget->attachMainWidget(glWidget);
        }
    }
}

%End

namespace tlp {
class NodeLinkDiagramComponent : tlp::View /Abstract/ {
%TypeHeaderCode
#include <tulip/NodeLinkDiagramComponent.h>
%End

%Docstring
.. versionadded:: 3.8

Bases: :class:`tlp.View`

That class represents the main type of view offered by Tulip : a node link diagram.
%End

public:

	void centerView();
%Docstring
tlp.NodeLinkDiagramComponent.centerView()

Centers the view according to the current view window size and draws it.
%End

// =======================================================================================

    tlp::GlScene *getScene();
%MethodCode
    sipRes = sipCpp->getGlMainWidget()->getScene();
%End

// =======================================================================================

    tlp::GlGraphRenderingParameters getRenderingParameters();
%Docstring
tlp.NodeLinkDiagramComponent.getRenderingParameters()

Returns a copy of the current rendering parameters for the node link diagram view.

:rtype: :class:`tulipogl.tlp.GlGraphRenderingParameters`
%End

%MethodCode
    sipRes = new tlp::GlGraphRenderingParameters(sipCpp->getGlMainWidget()->getScene()->getGlGraphComposite()->getRenderingParameters());
%End

// =======================================================================================

    void setRenderingParameters(const tlp::GlGraphRenderingParameters &parameters);
%Docstring
tlp.NodeLinkDiagramComponent.setRenderingParameters(parameters)

Sets the rendering parameters for the node link diagram view.

:param parameters: the new rendering parameters
:type parameters: :class:`tulipogl.tlp.GlGraphRenderingParameters`
%End

%MethodCode
    sipCpp->getGlMainWidget()->getScene()->getGlGraphComposite()->setRenderingParameters(*a0);
    updateNodeLinkConfWidgets(sipCpp);
    tlp::MainController *mainController = dynamic_cast<tlp::MainController *>(tlp::Controller::getCurrentController());
    if (!mainController) {
        sipCpp->draw();
        QApplication::processEvents();
    }
%End

// =======================================================================================

    void setCenterViewOnSetGraph(bool initView);
%Docstring
tlp.NodeLinkDiagramComponent.setCenterViewOnSetGraph(center)

Activates / desactivates the automatic centering of the view when setting a new graph on it.

:param center: the state of the automatic centering
:type center: boolean
%End

%MethodCode
    sipCpp->setInitViewOnSetGraph(a0);
    updateNodeLinkConfWidgets(sipCpp);
%End

// =======================================================================================

    bool geCenterViewOnSetGraph();
%Docstring
tlp.NodeLinkDiagramComponent.getCenterViewOnSetGraph()

Returns the state of the automatic centering when setting a new graph on the view.

:rtype: boolean
%End

%MethodCode
    sipRes = sipCpp->getInitViewOnSetGraph();
%End

// =======================================================================================

    void hideOverview(bool hideOverview);
%Docstring
tlp.NodeLinkDiagramComponent.hideOverview(hideOverview)

Hides / displays the overview in the top left corner of the view.

:param hideOverview: whether to hide the overview
:type hideOverview: boolean
%End

};
};
