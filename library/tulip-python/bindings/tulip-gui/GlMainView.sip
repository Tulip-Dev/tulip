/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

namespace tlp {

class GlMainView : tlp::View /Abstract/ {
%TypeHeaderCode
#include <tulip/GlMainView.h>
#include <vector>
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

// =======================================================

  bool pickNodeEdge(int x, int y, tlp::node &n, tlp::edge &e, bool pickNode = true, bool pickEdge = true);
%Docstring
tlpgui.GlMainView.pickNodeEdge(x, y, node, edge, pickNode, pickEdge)
 Returns True if a node/edge is found at the (x,y) position in the view
 The node/edge found is returned in the node or edge parameter.

:param x: the x coordinate
:type x: integer
:param y: the y coordinate
:type y: integer
:param node: on return holds the node found under the (x,y) position, node.isValid() return False if none has been found
:type node: :class:`tlp.node`
:param edge: on return holds the edge found under the (x,y) position, edge.isValid() return False if none has been found
:type edge: :class:`tlp.edge`
:param pickNode: enable/disable the node picking
:type pickNode: boolean
:param pickEdge: enable/disable the edge picking
:type pickNode: boolean
:rtype: boolean
%End

// =======================================================

  void rotateCamera(int x, int y, int z);
%Docstring
tlpgui.GlMainView.rotateCamera(x, y, z)
 Rotate the view camera by (x, y, z)

:param x: the angle of the rotation around X axis in degree
:type x: integer
:param y: the angle of the rotation around Y axis in degree
:type y: integer
:param z: the angle of the rotation around Z axis in degree
:type z: integer
%End

// =======================================================

  void translateCamera(int x, int y, int z);
%Docstring
tlpgui.GlMainView.translateCamera(x, y, z)
 Translate the view camera by (x, y, z)

:param x: the translation offset along the X axis
:type x: integer
:param y: the translation offset along the Y axis
:type y: integer
:param z: the translation offset along the Z axis
:type z: integer
%End

// =======================================================

  Coord viewToWorld(const Coord &vpos);
%Docstring
tlpgui.GlMainView.viewToWorld(vpos)
 Return the 3D world position for the given view position

:param vpos: the view position
:type vpos: :class:`tlp.Coord`
%End

// =======================================================

  Coord worldToView(const Coord &wpos);
%Docstring
tlpgui.GlMainView.viewToWorld(wpos)
 Return the view position for the given 3D position

:param wpos: the world position
:type wpos: :class:`tlp.Coord`
%End

// =======================================================

  void zoomXY(int step, int x, int y);
%Docstring
tlpgui.GlMainView.zoomXY(step, x, y)
 Zoom by step to the given (x,y) view position

:param step: the zoom step
:type step: integer
:param x: the x coordinate of the view position
:type x: integer
:param y: the y coordinate of the view position
:type y: integer
%End

// =======================================================

  void zoom(int step);
%Docstring
tlpgui.GlMainView.zoom(step)
 Zoom by step

:param step: the zoom step
:type step: integer
%End

// =======================================================

  void zoomFactor(float factor);
%Docstring
tlpgui.GlMainView.zoom(factor)
 Zoom by factor

:param factor: the zoom factor
:type factor: float
%End

// =======================================================

  void zoomAndPanAnimation(const tlp::BoundingBox &bb, double duration = 1000.);
%Docstring
tlpgui.GlMainView.zoomAndPanAnimation(bb, duration)
 Do a zoom and pan animation during a specific duration.
 At the end of the animation, the view will be zoomed and centered on the content of the given bounding box.

:param bb: the bounding box in scene coordinates on which the view has to be zoomed and panned. If it is not valid, the scene bounding box will be used.
:type bb: :class: `tlp.BoundingBox`
:param duration: the animation duration in msecs
:type duration: float
%End

// =======================================================

  PyObject *getRGBImage();
%Docstring
tlpgui.GlMainView.getRGBImage(w, h)
  return the RGB image of the viewport scene as a bytes object.
:rtype: :class: `bytes`
%End

%MethodCode
   auto vp = sipCpp->getGlMainWidget()->getScene()->getViewport();
   auto img = sipCpp->getGlMainWidget()->getScene()->getImage();
   sipRes = PyByteArray_FromStringAndSize((const char *) img, 3 * vp[2] * vp[3]);
   free(img);
%End

// =======================================================

  std::vector<int> getViewportSize();
%Docstring
tlpgui.GlMainView.getViewportSize(w, h)
  return the width and height of the viewport scene.
:rtype: list of 2 integers
%End

%MethodCode
   auto vp = sipCpp->getGlMainWidget()->getScene()->getViewport();
   sipRes = new std::vector<int>(2);
   (*sipRes)[0] = vp[2], (*sipRes)[1] = vp[3];
%End
};

};
