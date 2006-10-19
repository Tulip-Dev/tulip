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

#include <tulip/MouseInteractors.h>
#include <tulip/MouseSelectionEditor.h>
#include <tulip/MouseNodeBuilder.h>
#include <tulip/MouseEdgeBuilder.h>
#include <tulip/MouseSelector.h>
#include <tulip/MouseMagicSelector.h>
#include <tulip/MouseBoxZoomer.h>
#include "ToolBar.h"
#include "Application.h"

// the vectors of interactors associated to each action
static vector<tlp::GWInteractor *>addEdgeInteractors;
static vector<tlp::GWInteractor *>addNodeInteractors;
static vector<tlp::GWInteractor *>deleteEltInteractors;
static vector<tlp::GWInteractor *>graphNavigateInteractors;
static vector<tlp::GWInteractor *>magicSelectionInteractors;
static vector<tlp::GWInteractor *>editSelectionInteractors;
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
  addEdgeInteractors.push_back(new MousePanNZoomNavigator());
  //addEdgeInteractors.push_back(new MouseNodeBuilder());
  addEdgeInteractors.push_back(new MouseEdgeBuilder());
  addNodeInteractors.push_back(new MousePanNZoomNavigator());
  addNodeInteractors.push_back(new MouseNodeBuilder());
  deleteEltInteractors.push_back(new MousePanNZoomNavigator());
  deleteEltInteractors.push_back(new MouseElementDeleter());
  graphNavigateInteractors.push_back(new MouseNKeysNavigator());
  magicSelectionInteractors.push_back(new MousePanNZoomNavigator());
  magicSelectionInteractors.push_back(new MouseMagicSelector());
  editSelectionInteractors.push_back(new MousePanNZoomNavigator());
  editSelectionInteractors.push_back(new MouseSelector());
  editSelectionInteractors.push_back(new MouseSelectionEditor());
  selectInteractors.push_back(new MousePanNZoomNavigator());
  selectionInteractors.push_back(new MousePanNZoomNavigator());
  selectionInteractors.push_back(new MouseSelector());
  zoomBoxInteractors.push_back(new MousePanNZoomNavigator());
  zoomBoxInteractors.push_back(new MouseBoxZoomer());
  // set the current one
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
  deleteInteractors(editSelectionInteractors);
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
  setCurrentInteractors(&editSelectionInteractors);
}
void ToolBar::setSelect() {
  setCurrentInteractors(&selectInteractors);
}
void ToolBar::setSelectInteractor(GWInteractor *interactor) {
  selectInteractors.push_back(interactor);
}
void ToolBar::setSelection() {
  setCurrentInteractors(&selectionInteractors);
}
void ToolBar::setZoomBox() {
  setCurrentInteractors(&zoomBoxInteractors);
}

