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
%End

namespace tlp {
class NodeLinkDiagramComponent : tlp::GlMainView  /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/NodeLinkDiagramComponent.h>
#include <tulip/GlGraphComposite.h>
%End

%Docstring
            
.. versionadded:: 4.4

Bases: :class:`tlpgui.GlMainView`

That class represents the main type of view offered by Tulip : a node link diagram.
%End

public:

  NodeLinkDiagramComponent(const tlp::PluginContext* context = NULL);
  virtual ~NodeLinkDiagramComponent();

// =======================================================================================

    tlp::GlGraphInputData *getInputData();
%Docstring
tlpgui.NodeLinkDiagramComponent.getInputData()

Returns the current rendering input data for the node link diagram view.

:rtype: :class:`tulipogl.tlpogl.GlGraphInputData`
%End

%MethodCode
    sipRes = sipCpp->getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData();
%End

// =======================================================================================

    tlp::GlGraphRenderingParameters getRenderingParameters();
%Docstring
tlpgui.NodeLinkDiagramComponent.getRenderingParameters()

Returns a copy of the current rendering parameters for the node link diagram view.

:rtype: :class:`tulipogl.tlpogl.GlGraphRenderingParameters`
%End

%MethodCode
    sipRes = new tlp::GlGraphRenderingParameters(sipCpp->getGlMainWidget()->getScene()->getGlGraphComposite()->getRenderingParameters());
%End

// =======================================================================================

    void setRenderingParameters(const tlp::GlGraphRenderingParameters &parameters);
%Docstring
tlpgui.NodeLinkDiagramComponent.setRenderingParameters(parameters)

Sets the rendering parameters for the node link diagram view.

:param parameters: the new rendering parameters
:type parameters: :class:`tulipogl.tlpogl.GlGraphRenderingParameters`
%End

%MethodCode
    sipCpp->getGlMainWidget()->getScene()->getGlGraphComposite()->setRenderingParameters(*a0);
    sipCpp->draw();
    QApplication::processEvents();
%End

// =======================================================================================

protected :

    void setQuickAccessBarVisible(bool);
%Docstring
tlpgui.NodeLinkDiagramComponent.setQuickAccessBarVisible(state)

Sets the visibility state of the quick access bar.

:param state: the visibility state of the quick access bar
:type state: boolean
%End

// =======================================================================================

    bool quickAccessBarVisible() const;
%Docstring
tlpgui.NodeLinkDiagramComponent.quickAccessBarVisible()

Returns the visibility state of the quick access bar.

:rtype: boolean
%End


};

};
