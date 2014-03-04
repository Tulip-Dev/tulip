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

namespace tlp {

class GlMainView : tlp::View /Abstract/ {
%TypeHeaderCode
#include <tulip/GlMainView.h>
%End

%Docstring
Bases: :class:`tlpgui.View`

This is the based class of Tulip OpenGL views.
%End

public:

  GlMainView();
  virtual ~GlMainView();

// =======================================================

//  tlp::GlScene *getScene();
//%MethodCode
//    sipRes = sipCpp->getGlMainWidget()->getScene();
//%End

// =======================================================

  bool overviewVisible() const;
%Docstring
tlpgui.GlMainView.overviewVisible()

Returns the visibility state of the overview window.

:rtype: boolean
%End

// =======================================================

  void setOverviewVisible(bool);
%Docstring
tlpgui.GlMainView.setOverviewVisible(state)

Sets the visibility state of the overview window.

:param state: the visibility state of the overview window.
:type state: boolean
%End

// =======================================================

  virtual void drawOverview(bool generatePixmap=true);
%Docstring
tlpgui.GlMainView.drawOverview()

Forces the redraw of the overview window.
%End

// =======================================================

  virtual void centerView(bool graphChanged = false);
%Docstring
tlpgui.GlMainView.centerView()

Centers the OpenGL scene in the view window.
%End


// =======================================================

  void setViewOrtho(bool);
%Docstring
tlpgui.GlMainView.setViewOrtho(state)

Activates/desactivate the orthogonal projection for OpenGL rendering.
Perspective projection is used when orthogonal one is disabled.

:param state: the state of the orthogonal projection
:type state: boolean
%End

};

};
