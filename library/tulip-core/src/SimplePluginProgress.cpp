/**
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

#include <tulip/SimplePluginProgress.h>
using namespace tlp;
using namespace std;
SimplePluginProgress::SimplePluginProgress(): _state(TLP_CONTINUE),
  _preview(false) {
}
ProgressState SimplePluginProgress::progress(int step, int max_step) {
  PluginProgress::progress(step,max_step);
  progress_handler(step,max_step);
  return _state;
}
//====================================================
void SimplePluginProgress::stop() {
  _state=TLP_STOP;
}
//====================================================
void SimplePluginProgress::cancel() {
  _state=TLP_CANCEL;
}
//====================================================
bool SimplePluginProgress::isPreviewMode() const {
  return _preview;
}
//====================================================
void SimplePluginProgress::setPreviewMode(bool b) {
  if (b == _preview) return;

  _preview = b;
  preview_handler(_preview);
}
//====================================================
void SimplePluginProgress::showPreview(bool) {
}
//====================================================
ProgressState SimplePluginProgress::state() const {
  return _state;
}
//====================================================
void SimplePluginProgress::progress_handler(int, int) {
}
//====================================================
void SimplePluginProgress::preview_handler(bool) {
}
//====================================================
