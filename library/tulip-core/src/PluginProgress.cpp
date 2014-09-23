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

#include <tulip/PluginProgress.h>

using namespace tlp;

PluginProgress::PluginProgress(): _previewHandler(NULL) {
}

PluginProgress::~PluginProgress() {
  delete _previewHandler;
}

void PluginProgress::setPreviewHandler(ProgressPreviewHandler* handler) {
  delete _previewHandler;
  _previewHandler = handler;
}
ProgressState PluginProgress::progress(int step, int max_step) {
  if (_previewHandler != NULL && isPreviewMode())
    _previewHandler->progressStateChanged(step,max_step);

  return TLP_CONTINUE;
}

ProgressPreviewHandler::~ProgressPreviewHandler() {
}
