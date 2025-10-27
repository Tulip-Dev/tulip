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

  bool quickAccessBarVisible() const;
%Docstring
tlpgui.GlMainView.quickAccessBarVisible()

Returns the visibility state of the quick access bar.

:rtype: boolean
%End

// =======================================================

  void setQuickAccessBarVisible(bool);
%Docstring
tlpgui.GlMainView.setQuickAccessBarVisible(state)

Allows to control the quiack access bar visibility.

:param state: the visibility state of the quick access bar.
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

enable/disable the orthogonal projection for OpenGL rendering.
Perspective projection is used when orthogonal one is disabled.

:param state: the state of the orthogonal projection
:type state: boolean
%End

// =======================================================

  bool pickNodeEdge(int x, int y, tlp::node &n /Out/, tlp::edge &e /Out/, bool pickNode = true, bool pickEdge = true)/TypeHint="Tuple[bool, node, edge]"/ ;
%Docstring
tlpgui.GlMainView.pickNodeEdge(x, y, pickNode, pickEdge)

Returns a tuple of 3 elements. The first one is a bool indicating if a node/edge has been found found at the (x,y) position in the view, the second element is the node found, the third the edge found.

:param x: the x coordinate
:type x: integer
:param y: the y coordinate
:type y: integer
:param pickNode: enable/disable the node picking
:type pickNode: boolean
:param pickEdge: enable/disable the edge picking
:type pickNode: boolean
:rtype: (boolean, :class:`tulip.tlp.node`, :class:`tulip.tlp.edge`)
%End

%MethodCode
  *a2 = tlp::node();
  *a3 = tlp::edge();
  if (tlp::GlMainView::getNodeOrEdgeAtViewportPos(sipCpp->getGlMainWidget(), a0, a1, *a2, *a3)) {
    if ((a4 && a2->isValid()) || (a5 && a3->isValid()))
      sipRes = true;
  } else
    sipRes = false;
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

  PyObject *getRGBImage(int &width /Out/, int &height /Out/);
%Docstring
tlpgui.GlMainView.getRGBImage()

Return a tuple of 3 elements. The first element is the image in RGB format (3 bytes per pixel) as a bytearray object; the second is the width of the image and the third is its height.

:rtype: (:class:`bytearray`, int ,int)
%End

%MethodCode
   auto img = sipCpp->getRGBImage();
   sipRes = PyByteArray_FromStringAndSize((const char *) img.constBits(), 3 * img.width() * img.height());
   a0 = img.width();
   a1 = img.height();
%End
};

};
