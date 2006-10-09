//-*-c++-*-
/**
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 20/08/2001
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/MouseObject.h>
#include <tulip/MouseMoveSelection.h>
#include <tulip/MouseAddNode.h>
#include <tulip/MouseAddEdge.h>
#include <tulip/MouseSelection.h>
#include <tulip/MouseMagicSelection.h>
//#include <tulip/MouseTreeFishEyes.h>
#include <tulip/MouseZoomBox.h>
#include "ToolBar.h"
#include "Application.h"

// the vectors of interactors associated to each action
static vector<tlp::GWInteractor *>addEdgeInteractors;
static vector<tlp::GWInteractor *>addNodeInteractors;
static vector<tlp::GWInteractor *>deleteEltInteractors;
static vector<tlp::GWInteractor *>graphNavigateInteractors;
static vector<tlp::GWInteractor *>magicSelectionInteractors;
static vector<tlp::GWInteractor *>moveSelectionInteractors;
static vector<tlp::GWInteractor *>selectInteractors;
static vector<tlp::GWInteractor *>selectionInteractors;
static vector<tlp::GWInteractor *>zoomBoxInteractors;

/* 
 *  Constructs a ToolBar which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ToolBar::ToolBar( QWidget* parent,  const char* name, Qt::WFlags fl )
  : ToolBarData(parent, name, fl) {
  // initialize the vectors of interactors associated to each action
  addEdgeInteractors.push_back(new MouseAddEdge());
  addNodeInteractors.push_back(new MouseAddNode());
  deleteEltInteractors.push_back(new MouseDelete());
  graphNavigateInteractors.push_back(new MouseGraphNavigate());
  magicSelectionInteractors.push_back(new MouseMagicSelection());
  moveSelectionInteractors.push_back(new MouseSelection());
  moveSelectionInteractors.push_back(new MouseMoveSelection());
  selectInteractors.push_back(new MouseSelect());
  selectionInteractors.push_back(new MouseSelection());
  zoomBoxInteractors.push_back(new MouseZoomBox());
  // initialize the current one
  currentInteractors = &graphNavigateInteractors;
}

/*  
 *  Destroys the object and frees any allocated resources
 */
ToolBar::~ToolBar() {
  deleteInteractors(addEdgeInteractors);
  deleteInteractors(addNodeInteractors);
  deleteInteractors(deleteEltInteractors);
  deleteInteractors(graphNavigateInteractors);
  deleteInteractors(magicSelectionInteractors);
  deleteInteractors(moveSelectionInteractors);
  deleteInteractors(selectInteractors);
  deleteInteractors(selectionInteractors);
  deleteInteractors(zoomBoxInteractors);  
}

vector <GWInteractor *> &ToolBar::getCurrentInteractors() const {
  return *currentInteractors;
}

void ToolBar::setCurrentInteractors(vector<tlp::GWInteractor *> *interactors) {
  if (currentInteractors == interactors)
    return;
  currentInteractors = interactors;
  emit interactorsChanged(*currentInteractors);
}

// deletion of registered interactors
void ToolBar::deleteInteractors(vector<tlp::GWInteractor *> &interactors) {
  for(vector<GWInteractor *>::iterator it =
	interactors.begin(); it != interactors.end(); ++it)
    delete *it;
}

/* 
 * public slots
 */
void ToolBar::setAddEdge() {
  setCurrentInteractors(&addEdgeInteractors);
}
void ToolBar::setAddNode() {
  setCurrentInteractors(&addNodeInteractors);
}
void ToolBar::setDelete() {
  setCurrentInteractors(&deleteEltInteractors);
}
void ToolBar::setGraphNavigate() {
  setCurrentInteractors(&graphNavigateInteractors);
}
void ToolBar::setMagicSelection() {
  setCurrentInteractors(&magicSelectionInteractors);
}
void ToolBar::setMoveSelection() {
  setCurrentInteractors(&moveSelectionInteractors);
}
void ToolBar::setSelect() {
  setCurrentInteractors(&selectInteractors);
}
void ToolBar::setSelection() {
  setCurrentInteractors(&selectionInteractors);
}
void ToolBar::setZoomBox() {
  setCurrentInteractors(&zoomBoxInteractors);
}

