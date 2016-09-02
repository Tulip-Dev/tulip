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

var workerMode = typeof importScripts === 'function';
var nodejs = typeof process == "object";

function getScriptName() {

  if (nodejs) {
    var scriptPath = getScriptPath();
    var scriptName = __filename;
    scriptName = scriptName.substr(scriptPath.length);
    return scriptName;
  }

  var scriptName
  , source
  , lastStackFrameRegex = new RegExp(/.+\/(.*?):\d+[:]*\d*.*$/)
  , currentStackFrameRegex = new RegExp('getScriptName \\(.+\\/(.*?):\\d+[:]*\\d*.*\\)');

  try {
    var x = v43x76fr.name;
  } catch(error) {

    if((source = lastStackFrameRegex.exec(error.stack.trim())))
      scriptName = source[1];
    else if((source = currentStackFrameRegex.exec(error.stack.trim())))
      scriptName = source[1];
    else if(error.fileName != undefined)
      scriptName = error.fileName;
  }

  if (scriptName && scriptName.indexOf('?r=') != -1) {
    scriptName = scriptName.substr(0, scriptName.indexOf('?r='));
  }

  if (scriptName && scriptName.indexOf('?v=') != -1) {
    scriptName = scriptName.substr(0, scriptName.indexOf('?v='));
  }

  return scriptName;
}

function getScriptPath() {

  if (nodejs) {
    return __dirname + '/';
  }

  var scriptName = getScriptName();

  if (workerMode) {
    var location = self.location;
  } else {
    var location = window.location;
  }

  var origin = location.origin;
  if (location.protocol == "file:") {
    origin = "file://";
  }

  var scriptPath
  , source
  , lastStackFrameRegex = new RegExp('.*' + origin + '(.*)' + scriptName + '.*:\\d+[:]*\\d*.*$')
  , currentStackFrameRegex = new RegExp('getScriptPath \\(.*' + origin + '(.*)' + scriptName + '.*:\\d+[:]*\\d*.*\\)');

  try {
    var x = v43x76fr.name;
  } catch(error) {

    if((source = lastStackFrameRegex.exec(error.stack.trim())))
      scriptPath = source[1];
    else if((source = currentStackFrameRegex.exec(error.stack.trim())))
      scriptPath = source[1];

    if (location.protocol == "file:") {
      var scriptPathTokens = scriptPath.split('/');
      var pathnameTokens = location.pathname.split('/');
      for (var i = 0 ; i < scriptPathTokens.length ; ++i) {
        if (scriptPathTokens[i] != pathnameTokens[i]) {
          break;
        }
      }
      scriptPath = '';
      for (var j = i ; j < scriptPathTokens.length ; ++j) {
        if (scriptPathTokens[j] == "") {
          break;
        }
        scriptPath += (scriptPathTokens[j] + '/');
      }
    }
  }

  return scriptPath;
}
