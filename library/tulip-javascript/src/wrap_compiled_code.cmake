FILE(READ ${JSFILE} EMSCRIPTEN_GENERATED_CODE)
FILE(READ ${JSUTILSFILE} JSUTILS_CODE)

FILE(WRITE ${JSFILE} "!function() {")
FILE(APPEND ${JSFILE} "  ${JSUTILS_CODE}")
FILE(APPEND ${JSFILE} "  ${EMSCRIPTEN_GENERATED_CODE}")
FILE(APPEND ${JSFILE} "  if(typeof tulip == 'undefined') tulip = {};")
FILE(APPEND ${JSFILE} "  if(typeof tulipConf == 'undefined') tulipConf = {};")
FILE(APPEND ${JSFILE} "  this.tulip = tulip;")
FILE(APPEND ${JSFILE} "  var scriptName = getScriptName();")
FILE(APPEND ${JSFILE} "  var scriptPath = getScriptPath();")
FILE(APPEND ${JSFILE} "  if(tulipConf.modulePrefixURL) tulip.modulePrefixURL = tulipConf.modulePrefixURL;")
FILE(APPEND ${JSFILE} "  if(tulipConf.filePackagePrefixURL) tulip.filePackagePrefixURL = tulipConf.filePackagePrefixURL;")
FILE(APPEND ${JSFILE} "  if(tulipConf.memoryInitializerPrefixURL) tulip.memoryInitializerPrefixURL = tulipConf.memoryInitializerPrefixURL;")
FILE(APPEND ${JSFILE} "  if(tulipConf.TOTAL_MEMORY) tulip.TOTAL_MEMORY = tulipConf.TOTAL_MEMORY;")
FILE(APPEND ${JSFILE} "  if(!tulip.modulePrefixURL) tulip.modulePrefixURL = scriptPath;")
FILE(APPEND ${JSFILE} "  if(!tulip.filePackagePrefixURL) tulip.filePackagePrefixURL = scriptPath;")
FILE(APPEND ${JSFILE} "  if(!tulip.memoryInitializerPrefixURL) tulip.memoryInitializerPrefixURL = scriptPath;")
IF(TULIP_VIZ_FEATURES)
  FILE(APPEND ${JSFILE} "  tulip.vizFeatures = true;")
ELSE(TULIP_VIZ_FEATURES)
  FILE(APPEND ${JSFILE} "  tulip.vizFeatures = false;")
ENDIF(TULIP_VIZ_FEATURES)
IF(USE_WASM)
  FILE(APPEND ${JSFILE} "  tulip.isLoaded = function() {return false;};")
  FILE(APPEND ${JSFILE} "  var xhr = new XMLHttpRequest();")
  FILE(APPEND ${JSFILE} "  xhr.open('GET', tulip.modulePrefixURL + 'tulip.wasm');")
  FILE(APPEND ${JSFILE} "  xhr.responseType = 'arraybuffer';")
  FILE(APPEND ${JSFILE} "  xhr.onload = function() {")
  FILE(APPEND ${JSFILE} "    tulip.wasmBinary = xhr.response;")
ENDIF(USE_WASM)
FILE(APPEND ${JSFILE} "    tulip = tulipjs(tulip);")
FILE(APPEND ${JSFILE} "    if (typeof define === 'function' && define.amd) define(tulip); else if (typeof module === 'object' && module.exports) module.exports = tulip;")
IF(USE_WASM)
  FILE(APPEND ${JSFILE} "  };")
  FILE(APPEND ${JSFILE} "  xhr.send(null);")
ENDIF(USE_WASM)
FILE(APPEND ${JSFILE} "}();")








