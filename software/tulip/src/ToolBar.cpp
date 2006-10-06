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

/* 
 *  Constructs a ToolBar which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ToolBar::ToolBar( QWidget* parent,  const char* name, Qt::WFlags fl )
  : ToolBarData(parent, name, fl) {
  currentInteractor = new MouseGraphNavigate();
}

/*  
 *  Destroys the object and frees any allocated resources
 */
ToolBar::~ToolBar() {
  if (currentInteractor != NULL) delete currentInteractor;
}

GWInteractor *ToolBar::getCurrentInteractor() const {return currentInteractor;}

void ToolBar::setCurrentInteractor(GWInteractor *m) {
  if (currentInteractor != NULL) {
    if (typeid(*currentInteractor) == typeid(*m)) {
      delete m;
      return;
    }
    else
      delete currentInteractor;
  }
  currentInteractor = m;
  emit interactorChanged(currentInteractor);
}
/* 
 * public slots
 */
void ToolBar::setSelect() { setCurrentInteractor(new MouseSelect()); }
void ToolBar::setAddEdge() { setCurrentInteractor(new MouseAddEdge()); }
void ToolBar::setAddNode() { setCurrentInteractor(new MouseAddNode()); }
void ToolBar::setDel() { setCurrentInteractor(new MouseDel()); }
void ToolBar::setZoomBox() { setCurrentInteractor(new MouseZoomBox()); }
//void ToolBar::setDelSelection() { setCurrentInteractor(new MouseTreeFishEyes()); }
void ToolBar::setMoveSelection() { setCurrentInteractor(new MouseMoveSelection()); }
void ToolBar::setSelection() { setCurrentInteractor(new MouseSelection()); }
void ToolBar::setMagicSelection() { setCurrentInteractor(new MouseMagicSelection()); }
void ToolBar::setGraphNavigate() { setCurrentInteractor(new MouseGraphNavigate()); }
