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

#include "SOMViewInteractor.h"
#include <tulip/MouseInteractors.h>
#include <tulip/MouseSelector.h>
#include <tulip/MouseShowElementInfos.h>
#include "EditColorScaleInteractor.h"
#include "ThresholdInteractor.h"
using namespace std;
using namespace tlp;

PLUGIN(SOMViewNavigation)
PLUGIN(SOMViewSelection)
PLUGIN(SOMViewProperties)
PLUGIN(SOMViewThreshold)

SOMViewInteractor::SOMViewInteractor(const QString &iconPath, const QString &text) :
  GLInteractorComposite(QIcon(iconPath),text) {
}

SOMViewInteractor::~SOMViewInteractor() {
}

SOMViewNavigation::SOMViewNavigation(PluginContext *) :
  SOMViewInteractor(":/tulip/gui/icons/i_navigation.png", "Navigate") {
  //setPriority(3);
}
SOMViewNavigation::~SOMViewNavigation() {
}

void SOMViewNavigation::construct() {
  push_back(new MouseNKeysNavigator());
  push_back(new EditColorScaleInteractor());
}

QWidget *SOMViewNavigation::configurationWidget() const {
  return new QWidget;
}

SOMViewSelection::SOMViewSelection(PluginContext *) :
  SOMViewInteractor(":/tulip/gui/icons/i_selection.png", "Select") {
  //setPriority(2);
}

SOMViewSelection::~SOMViewSelection() {
}

void SOMViewSelection::construct() {
  push_back(new MouseSelector());
  push_back(new MousePanNZoomNavigator());
  push_back(new EditColorScaleInteractor());
}

QWidget *SOMViewSelection::configurationWidget() const {
  return new QWidget;
}

SOMViewProperties::SOMViewProperties(PluginContext *) :
  SOMViewInteractor(":/tulip/gui/icons/i_select.png", "Properties") {
  //setPriority(1);
}

SOMViewProperties::~SOMViewProperties() {
}

void SOMViewProperties::construct() {
  push_back(new MouseShowElementInfos());
  push_back(new MousePanNZoomNavigator());
  push_back(new EditColorScaleInteractor());
}

QWidget *SOMViewProperties::configurationWidget() const {
  return new QWidget;
}

SOMViewThreshold::SOMViewThreshold(PluginContext *) :
  SOMViewInteractor(":/i_slider.png", "Threshold Selection") {
  //setPriority(0);
}
SOMViewThreshold::~SOMViewThreshold() {

}
void SOMViewThreshold::construct() {
  push_back(new MouseNKeysNavigator());
  push_back(new ThresholdInteractor());
}

QWidget *SOMViewThreshold::configurationWidget() const {
  QLabel *_label = new QLabel("<H1>Threshold Interactor</H1><p>This interactor is used to select nodes with a value between those indicated by the two sliders</p><p>Move the each slider to change the bound.</p><p>Press the Ctrl button to add the new threshold selection to the current selection. If Ctrl is not pressed the old selection will be replaced</p>");
  _label->setWordWrap(true);
  _label->setAlignment(Qt::AlignTop);
  _label->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
  return _label;
}
