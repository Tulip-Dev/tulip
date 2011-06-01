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
  currentMouse = new MouseGraphNavigate();
}

/*  
 *  Destroys the object and frees any allocated resources
 */
ToolBar::~ToolBar() {
  if (currentMouse != NULL) delete currentMouse;
}

MouseInterface *ToolBar::getCurrentMouse() const {return currentMouse;}

void ToolBar::setCurrentMouse(MouseInterface *m) {
  if (currentMouse != NULL) {
    if (typeid(*currentMouse) == typeid(*m)) {
      delete m;
      return;
    }
    else
      delete currentMouse;
  }
  currentMouse = m;
  emit mouseChanged(currentMouse);
}
/* 
 * public slots
 */
void ToolBar::setSelect() { setCurrentMouse(new MouseSelect()); }
void ToolBar::setAddEdge() { setCurrentMouse(new MouseAddEdge()); }
void ToolBar::setAddNode() { setCurrentMouse(new MouseAddNode()); }
void ToolBar::setDel() { setCurrentMouse(new MouseDel()); }
void ToolBar::setZoomBox() { setCurrentMouse(new MouseZoomBox()); }
//void ToolBar::setDelSelection() { setCurrentMouse(new MouseTreeFishEyes()); }
void ToolBar::setMoveSelection() { setCurrentMouse(new MouseMoveSelection()); }
void ToolBar::setSelection() { setCurrentMouse(new MouseSelection()); }
void ToolBar::setMagicSelection() { setCurrentMouse(new MouseMagicSelection()); }
void ToolBar::setGraphNavigate() { setCurrentMouse(new MouseGraphNavigate()); }
