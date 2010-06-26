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
#include "PreferenceDialog.h"

#include <QtCore/QSettings>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>

#include <tulip/PreferenceManager.h>

using namespace std;
using namespace tlp;

PreferenceDialog::PreferenceDialog( QWidget* parent)
    : QDialog( parent),modified(false) {
  setupUi(this);

  PreferenceManager &preferenceManager=PreferenceManager::getInst();
  bool networkConnection=preferenceManager.getNetworkConnection();
  bool autoLoadController=preferenceManager.getAutoLoadController();
  Color selectionColor=preferenceManager.getSelectionColor();

  networkRadioButton->setChecked(networkConnection);
  autoLoadControllerButton->setChecked(autoLoadController);
}


PreferenceDialog::~PreferenceDialog()
{

}

void PreferenceDialog::loadPreference(){
  QSettings settings("TulipSoftware","Tulip");
  settings.beginGroup("Preference");

  Color selectionColor=Color(255,0,255,255);
  selectionColor[0] = settings.value("selectionColorR",255).toUInt();
  selectionColor[1] = settings.value("selectionColorG",0).toUInt();
  selectionColor[2] = settings.value("selectionColorB",255).toUInt();
  selectionColor[3] = settings.value("selectionColorA",255).toUInt();

  bool networkConnection= settings.value("networkConnection",true).toBool();
  bool autoLoadController = settings.value("autoLoadController",false).toBool();

  settings.endGroup();

  PreferenceManager &preferenceManager=PreferenceManager::getInst();
  preferenceManager.setNetworkConnection(networkConnection);
  preferenceManager.setSelectionColor(selectionColor);
  preferenceManager.setAutoLoadController(autoLoadController);
}
void PreferenceDialog::savePreference(){
  QSettings settings("TulipSoftware","Tulip");
  settings.beginGroup("Preference");

  // Check modification
  modified = modified || (settings.value("networkConnection",true).toBool()!=networkRadioButton->isChecked());
  modified = modified || (settings.value("autoLoadController",true).toBool()!=autoLoadControllerButton->isChecked());

  // Store data
  settings.setValue("networkConnection",networkRadioButton->isChecked());
  settings.setValue("autoLoadController",autoLoadControllerButton->isChecked());

  settings.endGroup();
}

void PreferenceDialog::accept() {
  savePreference();

  QDialog::accept();
}

