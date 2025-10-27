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
#include <tulip/GlMainWidget.h>
%End

namespace tlp {
class NodeLinkDiagramComponent : tlp::GlMainView  /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/NodeLinkDiagramComponent.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlComplexPolygon.h>
#include <tulip/GlCompositeHierarchyManager.h>
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

:rtype: :class:`tlpgui.GlGraphInputData`
%End

%MethodCode
    sipRes = sipCpp->getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData();
%End

// =======================================================================================

    tlp::GlGraphRenderingParameters getRenderingParameters();
%Docstring
tlpgui.NodeLinkDiagramComponent.getRenderingParameters()

Returns a copy of the current rendering parameters for the node link diagram view.

:rtype: :class:`tlpgui.GlGraphRenderingParameters`
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
:type parameters: :class:`tlpgui.GlGraphRenderingParameters`
%End

%MethodCode
    sipCpp->getGlMainWidget()->getScene()->getGlGraphComposite()->setRenderingParameters(*a0);
    sipCpp->draw();
%End

// =======================================================================================


void displayConvexHulls(const bool visible=true, const bool withTextures=false);
%Docstring

    .. versionadded:: 6.0

    tlpgui.NodeLinkDiagramComponent.displayConvexHulls(visible=True, withTextures=False)

    Display or hide subgraphs convex hulls
%End
%MethodCode
  auto h = sipCpp->getGlMainWidget()->getScene()->getLayer("Main")->findGlEntity("Hulls");

  h->setVisible(a0);

tlp::GlComplexPolygon *pl = dynamic_cast<tlp::GlComplexPolygon *>(h);
  if (pl)
    pl->setTextureActivation(a1);
  else {
    tlp::GlConvexGraphHullsComposite *composite = dynamic_cast<tlp::GlConvexGraphHullsComposite *>(h);
    if (composite)
      composite->setHullsTextureActivation(a1);
sipCpp->draw();
}
%End

// =======================================================================================

void setQuickAccessBarVisible(bool);
%Docstring

.. versionadded:: 6.0

tlpgui.NodeLinkDiagramComponent.setQuickAccessBarVisible(state)

Sets the visibility state of the quick access bar.

:param state: the visibility state of the quick access bar
:type state: boolean
%End

// =======================================================================================

bool quickAccessBarVisible() const;
%Docstring
  .. versionadded:: 6.0

tlpgui.NodeLinkDiagramComponent.quickAccessBarVisible()

Returns the visibility state of the quick access bar.

:rtype: boolean
%End

// =======================================================================================

void setOverviewVisible(bool display);
%Docstring

  .. versionadded:: 6.0

  tlpgui.NodeLinkDiagramComponent.setOverviewVisible(state)

  Sets the visibility state of the overview display.

  :param state: the visibility state of the overview display
  :type state: boolean
%End

bool overviewVisible() const;
%Docstring

  .. versionadded:: 6.0

  tlpgui.NodeLinkDiagramComponent.overviewVisible()

  Returns the visibility state of the overview display.

  :rtype: boolean
%End

};

};
