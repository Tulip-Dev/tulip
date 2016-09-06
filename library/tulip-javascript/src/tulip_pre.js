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

Module.workerMode = workerMode;

if (workerMode) {

  Module.print = function(msg) {
    self.postMessage({eventType: 'print', text: msg});
  };

  Module.printErr = function(msg) {
    self.postMessage({eventType: 'print', text: msg});
  };
}

Module.noExitRuntime = true;

if (nodejs) {
  Module.preRun = function() {
    FS.mkdir('root');
    FS.mount(NODEFS, { root: process.cwd() }, 'root');
    FS.chdir('root');
  }
}
