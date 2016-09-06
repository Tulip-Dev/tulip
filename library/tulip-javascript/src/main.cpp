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

/**
 *
 * tulip.js : port of the Tulip framework to JavaScript through emscripten
 * Copyright (c) 2016 Antoine Lambert, Thales Services SAS
 * antoine-e.lambert@thalesgroup.com / antoine.lambert33@gmail.com
 *
 */

#include <tulip/TlpTools.h>
#include <tulip/GlyphsManager.h>

#include <emscripten/emscripten.h>

int EMSCRIPTEN_KEEPALIVE main(int /* argc */, char ** /* argv */) {

  tlp::initTulipLib();
  tlp::GlyphsManager::instance()->loadGlyphPlugins();

  emscripten_run_script("if (typeof window != 'undefined' && typeof window.tulip != 'undefined') { window.tulip.mainCalled = true; }");

  emscripten_run_script("if (typeof tulip != 'undefined' && tulip.workerMode) { self.postMessage({eventType : 'tulipWorkerInit'}); }");

  return 0;
}
