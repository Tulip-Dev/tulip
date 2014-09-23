/*
 *
 * This file is part of Tulip (www.tulip-software.org)
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
///@cond DOXYGEN_HIDDEN

#ifndef MOUSEMOVESELECTION_H
#define MOUSEMOVESELECTION_H

#include <tulip/GlCircle.h>
#include <tulip/GLInteractor.h>
#include <tulip/GlComplexPolygon.h>
#include <tulip/GlRect.h>

namespace tlp {

class LayoutProperty;
class BooleanProperty;
class DoubleProperty;
class SizeProperty;

/// This interactor allows to move/rotate/stretch the current selection layout
class TLP_QT_SCOPE MouseSelectionEditor:public GLInteractorComponent {

public:

  MouseSelectionEditor();
  ~MouseSelectionEditor();
  void clear();
  bool compute(GlMainWidget *glMainWidget);
  bool draw(GlMainWidget *);
  bool eventFilter(QObject *, QEvent *);

private:
  enum EditOperation { NONE=0, ROTATE_Z, ROTATE_XY, STRETCH_X, STRETCH_Y, STRETCH_XY, TRANSLATE, ALIGN_TOP, ALIGN_BOTTOM, ALIGN_LEFT, ALIGN_RIGHT, ALIGN_VERTICALLY, ALIGN_HORIZONTALLY};
  enum OperationTarget { COORD = 0, SIZE, COORD_AND_SIZE};

  GlMainWidget *glMainWidget;
  DoubleProperty *_rotation;

  void initProxies(GlMainWidget *glMainWidget);
  void initEdition();
  void undoEdition();
  void stopEdition();

  Coord ffdCenter;

  GlLayer *layer;
  GlComposite *composite;
  GlCircle _controls[8];
  GlComplexPolygon _advControls[6];
  GlRect centerRect;
  GlRect advRect;
  Coord _layoutCenter;

  bool computeFFD(GlMainWidget*);
  void getOperation(GlEntity *select);

protected:

  EditOperation operation;
  OperationTarget mode;

  Coord editCenter;
  Coord editPosition;
  Graph *_graph;
  LayoutProperty *_layout;
  BooleanProperty *_selection;
  SizeProperty *_sizes;
  Coord editLayoutCenter;

  virtual void mMouseTranslate(double, double, GlMainWidget*);
  virtual void mMouseRotate(double, double, GlMainWidget*);
  virtual void mMouseStretchAxis(double, double, GlMainWidget*);
  virtual void mAlign(EditOperation operation,GlMainWidget*);


};

}


#endif
///@endcond
