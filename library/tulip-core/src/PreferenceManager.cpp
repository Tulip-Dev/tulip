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
#include <tulip/PreferenceManager.h>

#include <QtCore/QSettings>
#include <tulip/PropertyTypes.h>

tlp::PreferenceManager* tlp::PreferenceManager::inst=NULL;

using namespace std;
using namespace tlp;

// FIXME: pretty dirty since this is duplicated in TulipSettings and should belong to it
const Color& PreferenceManager::getSelectionColor() {
  QSettings settings("TulipSoftware","Tulip");
  QString val = settings.value("graph/defaults/selectioncolor/","(23, 81, 228)").toString();
  ColorType::fromString(selectionColor,val.toStdString());
  return selectionColor;
}
