FILE(READ ${JSFILE} EMSCRIPTEN_GENERATED_CODE)
FILE(READ ${JSUTILSFILE} JSUTILS_CODE)

FILE(WRITE ${JSFILE} "
!function() {
  ${JSUTILS_CODE}
  ${EMSCRIPTEN_GENERATED_CODE}
  if(typeof tulip == 'undefined') tulip = {};
  if(typeof tulipConf == 'undefined') tulipConf = {};
  this.tulip = tulip;
  var scriptName = getScriptName();
  var scriptPath = getScriptPath();
  if(tulipConf.modulePrefixURL) tulip.modulePrefixURL = tulipConf.modulePrefixURL;
  if(tulipConf.filePackagePrefixURL) tulip.filePackagePrefixURL = tulipConf.filePackagePrefixURL;
  if(tulipConf.memoryInitializerPrefixURL) tulip.memoryInitializerPrefixURL = tulipConf.memoryInitializerPrefixURL;
  if(tulipConf.TOTAL_MEMORY) tulip.TOTAL_MEMORY = tulipConf.TOTAL_MEMORY;
  if(!tulip.modulePrefixURL) tulip.modulePrefixURL = scriptPath;
  if(!tulip.filePackagePrefixURL) tulip.filePackagePrefixURL = scriptPath;
  if(!tulip.memoryInitializerPrefixURL) tulip.memoryInitializerPrefixURL = scriptPath;
  tulip.loadingFailed = false;
  tulip.wasm = false;
  tulip.vizFeatures = false;
  tulip.isLoaded = function() {return false;};
")
IF(TULIP_VIZ_FEATURES)
FILE(APPEND ${JSFILE} "
  tulip.vizFeatures = true;
")
ENDIF(TULIP_VIZ_FEATURES)
IF(USE_WASM)
FILE(APPEND ${JSFILE} "
  tulip.wasm = true;
")
ENDIF(USE_WASM)
FILE(APPEND ${JSFILE} "
  tulip.init = function() {
    return new Promise(function(resolve, reject) {
      function checkTulipIsLoaded() {
        if (tulip.isLoaded()) {
          resolve();
        } else {
          setTimeout(checkTulipIsLoaded);
        }
      }
      if (!tulip.wasm) {
        try {
          tulip = tulipjs(tulip);
        } catch (e) {
          reject(e);
        }
      } else {
        var xhr = new XMLHttpRequest();
        xhr.open('GET', tulip.modulePrefixURL + 'tulip.wasm');
        xhr.responseType = 'arraybuffer';
        xhr.onload = function() {
          tulip.wasmBinary = xhr.response;
          try {
            tulip = tulipjs(tulip);
          } catch (e) {
            reject(e);
          }
        };
        xhr.send(null);
      }
      checkTulipIsLoaded();
    });
  };
  if (typeof define === 'function' && define.amd) define(tulip); else if (typeof module === 'object' && module.exports) module.exports = tulip;
}();")
