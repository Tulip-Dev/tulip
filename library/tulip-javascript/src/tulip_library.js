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

var LibraryTulipUtils = {

  $TulipUtils : {

    requestedAnimationFrame : false,
    fullScreenCanvasId : null,
    canvasFSOldWidth : 0,
    canvasFSOldHeight : 0,

    onFullScreenEventChange: function(event) {
      var width;
      var height;
      var tulipView = tulip.getViewForCanvasId(TulipUtils.fullScreenCanvasId);
      if (document["fullScreen"] || document["mozFullScreen"] || document["webkitIsFullScreen"]) {
        width = screen["width"];
        height = screen["height"];
        tulipView.fullScreenActivated = true;
      } else {
        width = TulipUtils.canvasFSOldWidth;
        height = TulipUtils.canvasFSOldHeight;
        document.removeEventListener('fullscreenchange', TulipUtils.onFullScreenEventChange, true);
        document.removeEventListener('mozfullscreenchange', TulipUtils.onFullScreenEventChange, true);
        document.removeEventListener('webkitfullscreenchange', TulipUtils.onFullScreenEventChange, true);
        tulipView.fullScreenActivated = false;
      }
      tulipView.resize(width, height);
      tulipView.draw();
    },

  },

  resizeWebGLCanvas : function(canvasId, width, height, sizeRelativeToContainer) {
    Module['canvas'] = document.getElementById(Pointer_stringify(canvasId));
    if (!sizeRelativeToContainer) {
      Browser.setCanvasSize(width, height);
    } else {
      Module['canvas'].width = width;
      Module['canvas'].height = height;
    }
  },

  requestFullScreenCanvas : function (canvasId) {
    TulipUtils.fullScreenCanvasId = Pointer_stringify(canvasId);
    var canvas = document.getElementById(TulipUtils.fullScreenCanvasId);
    TulipUtils.canvasFSOldWidth = canvas.width;
    TulipUtils.canvasFSOldHeight = canvas.height;
    document.addEventListener('fullscreenchange', TulipUtils.onFullScreenEventChange, true);
    document.addEventListener('mozfullscreenchange', TulipUtils.onFullScreenEventChange, true);
    document.addEventListener('webkitfullscreenchange', TulipUtils.onFullScreenEventChange, true);
    var RFS = canvas['requestFullscreen'] ||
        canvas['requestFullScreen'] ||
        canvas['mozRequestFullScreen'] ||
        canvas['webkitRequestFullScreen'] ||
        (function() {});
    RFS.apply(canvas, []);
  },

  refreshWebGLCanvas : function(drawFunc) {
    if (drawFunc && !TulipUtils.requestedAnimationFrame) {
      TulipUtils.requestedAnimationFrame = true;
      Browser.requestAnimationFrame(function() {
        Browser.mainLoop.runIter(function() {
          Runtime.dynCall('v', drawFunc);
          TulipUtils.requestedAnimationFrame = false;
        });
      });
    }
  },

  safeSetTimeout : function(msec, func, value) {
    Browser.safeSetTimeout(function() { Runtime.dynCall('vi', func, [value]); }, msec);
  },

  setProgressBarValueWorker : function(graphId, value) {
    tulip.sendProgressValue(graphId, value);
  },

  setProgressBarCommentWorker : function(graphId, text) {
    tulip.sendProgressComment(graphId, Pointer_stringify(text));
  },

  workerMode : function() {
    return tulip.workerMode;
  },

  domElementExists : function(elementId) {
    return document.getElementById(Pointer_stringify(elementId)) != null;
  },

  canXhrOnUrl : function(url) {
    if (window.location.protocol == "http:") {
      return true;
    }
    var fileOk = true;
    try {
      Browser.xhrLoad(Pointer_stringify(url), function(array) {}, function() {});
    } catch (e) {
      fileOk = false;
    }
    return fileOk;
  },

  createGlTextureFromCanvas : function(canvasId) {
    GLctx.pixelStorei(GLctx.UNPACK_FLIP_Y_WEBGL, true);
    var canvas = document.getElementById(Pointer_stringify(canvasId));
    GLctx.texImage2D(GLctx.TEXTURE_2D, 0, GLctx.RGBA, GLctx.RGBA, GLctx.UNSIGNED_BYTE, canvas);
    GLctx.pixelStorei(GLctx.UNPACK_FLIP_Y_WEBGL, false);
  },

  textureCanvas: null,

  loadImageFromUrl: function(url, imageLoadedCallback, errorCallback) {

    if (!TulipUtils.textureCanvas) {
      TulipUtils.textureCanvas = document.createElement("canvas");
    }

    var img = new Image,
        canvas = TulipUtils.textureCanvas,
        ctx = canvas.getContext("2d"),
        src = Pointer_stringify(url);

    img.crossOrigin = "anonymous";

    img.onload = function() {
      canvas.width = img.width;
      canvas.height = img.height;
      ctx.clearRect(0, 0, canvas.width, canvas.height);
      ctx.translate(0, img.height);
      ctx.scale(1, -1);
      ctx.drawImage( img, 0, 0 );
      var imageData = ctx.getImageData(0, 0, img.width, img.height);
      var nDataBytes = img.width * img.height * 4 * Uint8ClampedArray.BYTES_PER_ELEMENT;
      var dataPtr = Module._malloc(nDataBytes);
      var imageDataHeap = new Uint8ClampedArray(Module.HEAPU8.buffer, dataPtr, img.width * img.height * 4);
      imageDataHeap.set(imageData.data);
      Runtime.dynCall('viiii', imageLoadedCallback, [url, imageDataHeap.byteOffset, img.width, img.height]);
      Module._free(imageDataHeap.byteOffset);
    }
    img.onerror = function() {
      Runtime.dynCall('viii', errorCallback, [0, url, 0]);
    };
    img.src = src;
    if (img.complete || img.complete === undefined) {
      img.src = "data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///ywAAAAAAQABAAACAUwAOw==";
      img.src = src;
    }
  },

  isChrome: function() {
    return navigator.userAgent.toLowerCase().indexOf('chrome') > -1 &&
           navigator.vendor.toLowerCase().indexOf('google inc') > -1;
  },

  focusCanvas: function(canvasId) {
    var canvas = document.getElementById(Pointer_stringify(canvasId));
    if (!canvas) return;
    var oldPos = canvas.style.position;
    canvas.style.position = 'fixed';
    canvas.focus();
    canvas.style.position = oldPos;
  },

  blurCanvas: function(canvasId) {
    var canvas = document.getElementById(Pointer_stringify(canvasId));
    if (!canvas) return;
    canvas.blur();
  }

};

autoAddDeps(LibraryTulipUtils, '$TulipUtils');
mergeInto(LibraryManager.library, LibraryTulipUtils);
