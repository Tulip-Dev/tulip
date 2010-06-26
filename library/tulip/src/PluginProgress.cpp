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
#include "tulip/PluginProgress.h"

using namespace tlp;

//====================================================
PluginProgress::PluginProgress() :
  _state(TLP_CONTINUE),
  _preview(false) {
}
//====================================================
PluginProgress::~PluginProgress(){
}
//====================================================
ProgressState PluginProgress::progress(int step, int max_step) {
  progress_handler(step,max_step);
  return _state;
}
//====================================================
void PluginProgress::stop() {
  _state=TLP_STOP;
}
//====================================================
void PluginProgress::cancel() {
  _state=TLP_CANCEL;
}
//====================================================
bool PluginProgress::isPreviewMode() const {
  return _preview;
}
//====================================================
void PluginProgress::setPreviewMode(bool b) {
  _preview = b;
}
//====================================================
void PluginProgress::showPreview(bool b) {
}
//====================================================
ProgressState PluginProgress::state() const {
  return _state;
}
//====================================================
void PluginProgress::progress_handler(int step, int max_step) {
}
//====================================================
