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

var tulip = Module;

var UINT_MAX = 0xffffffff;

function typeOf(value) {
  var s = typeof value;
  if (s === 'object') {
    if (value) {
      if (Object.prototype.toString.call(value) == '[object Array]') {
        s = 'array';
      }
    } else {
      s = 'null';
    }
  }
  return s;
}

function allocArrayInEmHeap(ArrayType, size) {
  var nDataBytes = size * ArrayType.BYTES_PER_ELEMENT;
  var dataPtr = Module._malloc(nDataBytes);
  return new ArrayType(Module.HEAPU8.buffer, dataPtr, size);
}

function freeArrayInEmHeap(arrayHeap) {
  Module._free(arrayHeap.byteOffset);
}

function bytesTypedArrayToStringArray(bytesArray, offsetArray, nbStrings) {
  var ret = [];
  var start = 0;
  for (var i = 0 ; i < nbStrings ; ++i) {
    ret.push(Module.UTF8ArrayToString(bytesArray.subarray(start, start+offsetArray[i]), 0));
    start += offsetArray[i];
  }
  return ret;
}

function stringArrayToBytesAndOffsetsTypedArray(stringArray) {
  var nbBytes = 0;
  var uintArray = allocArrayInEmHeap(Uint32Array, stringArray.length);
  for (var i = 0 ; i < stringArray.length ; ++i) {
    var strNbBytes = Module.lengthBytesUTF8(stringArray[i]) + 1;
    nbBytes += strNbBytes;
    uintArray[i] = strNbBytes;
  }
  var ucharArray = allocArrayInEmHeap(Uint8Array, nbBytes);
  var offset = 0;
  for (var i = 0 ; i < stringArray.length ; ++i) {
    Module.stringToUTF8Array(stringArray[i], ucharArray, offset, uintArray[i]);
    offset += uintArray[i];
  }
  return {bytesArray: ucharArray, offsetsArray: uintArray};
}

Function.prototype.inheritsFrom = function(parentClassOrObject) {
  this.prototype = Object.create(parentClassOrObject.prototype);
  this.prototype.constructor = this;
}

if (typeOf(String.prototype.startsWith) == 'undefined') {

  String.prototype.startsWith = function(prefix) {
    return this.indexOf(prefix) === 0;
  }

  String.prototype.endsWith = function(suffix) {
    return this.match(suffix+"$") == suffix;
  };

}

function createObject(ObjectType, caller) {
  return ObjectType.prototype.isPrototypeOf(caller) ? caller : new ObjectType();
}

tulip.mainCalled = false;
if (nodejs) {
  tulip.mainCalled = true;
}

tulip.debugChecks = tulip.debugChecks || true;
if (workerMode) {
  tulip.debugChecks = false;
}

function checkArgumentsTypes(argList, typeList, nbRequiredArguments) {
  if (!tulip.debugChecks) return;
  var callerName = checkArgumentsTypes.caller.name;
  if (callerName.startsWith("tulip_")) {
    callerName = callerName.replace(/_/g, '.');
  }
  var nbArgs = argList.length;
  if (nbRequiredArguments && nbRequiredArguments > nbArgs) nbArgs = nbRequiredArguments;
  for (var i = 0 ; i < nbArgs ; ++i) {
    if (typeOf(typeList[i]) == "string" && typeOf(argList[i]) != typeList[i]) {
      throw new TypeError("Error when calling function '" + callerName + "', parameter "+ i + " must be of type '"+ typeList[i] +"' (found '" + typeOf(argList[i]) + "' instead)");
    } else if (typeOf(typeList[i]) == "function") {
      if (typeOf(argList[i]) != "object" || !(argList[i] instanceof typeList[i])) {
        var typename = typeList[i].name;
        if (typename.startsWith("tulip_")) {
          typename = typename.replace(/_/g, '.');
        }
        throw new TypeError("Error when calling function '" + callerName + "', parameter "+ i + " must be an instance of a '"+ typename +"' object (found '" + typeOf(argList[i]) + "' instead)");
      }
    } else if (typeOf(typeList[i]) == "array") {
      var typeOk = false;
      for (var j = 0 ; j < typeList[i].length ; ++j) {
        if (typeOf(typeList[i][j]) == "string" && typeOf(argList[i]) == typeList[i][j]) {
          typeOk = true;
          break;
        } else if (typeOf(typeList[i][j]) == "function") {
          if (typeOf(argList[i]) == "object" && argList[i] instanceof typeList[i][j]) {
            typeOk = true;
            break;
          }
        }
      }
      if (!typeOk) {
        var errMsg = "Error when calling function '" + callerName + "', parameter "+ i + " must be of one of the following types : ";
        for (var j = 0 ; j < typeList[i].length ; ++j) {
          if (typeOf(typeList[i][j]) == "string") {
            errMsg += typeList[i][j];
          } else if (typeOf(typeList[i][j]) == "function") {
            var typename = typeList[i][j].name;
            if (typename.startsWith("tulip_")) {
              typename = typename.replace(/_/g, '.');
            }
            errMsg += "an instance of a '"+ typename +"' object";
          }
          if (j != typeList[i].length - 1) {
            errMsg += ", ";
          }
        }
        errMsg += ", found '" + typeOf(argList[i]) + "' instead";
        throw new TypeError(errMsg);
      }
    }
  }
}

function checkArrayOfType(array, type, i) {
  if (!tulip.debugChecks) return;
  var callerName = checkArrayOfType.caller.name;
  if (callerName.startsWith("tulip_")) {
    callerName = callerName.replace(/_/g, '.');
  }
  var types = [];
  for (var j = 0 ; j < array.length ; ++j) {
    types.push(type);
  }
  try {
    checkArgumentsTypes(array, types);
  } catch (e) {
    var typename = type;
    if (typeOf(typename) == "function") {
      typename = type.name;
      if (typename.startsWith("tulip_")) {
        typename = typename.replace(/_/g, '.');
      }
    }
    throw new TypeError("Error when calling function '" + callerName + "', parameter "+ i + " must be an array of "+ typename);
  }
}

var _isPointerDeleted = Module.cwrap('isPointerDeleted', 'number', ['number']);

function checkWrappedCppPointer(cppPointer) {
  //if (!tulip.debugChecks) return;
  if (cppPointer == 0 || _isPointerDeleted(cppPointer)) {
    var callerName = checkWrappedCppPointer.caller.name;
    if (callerName.startsWith("tulip_")) {
      callerName = callerName.replace(/_/g, '.');
    }
    throw "Runtime error when calling function '" + callerName +"' : wrapped C++ object is null or has been deleted";
  }
}

function getArrayOfTulipType(arraySize, arrayFillFunc, tulipType) {
  var result = allocArrayInEmHeap(Uint32Array, arraySize);
  arrayFillFunc(result.byteOffset);
  var tulipTypeArray = new Array();
  for (var i = 0 ; i < arraySize ; ++i) {
    if (tulipType == tulip.Node || tulipType == tulip.Edge) {
      if (result[i] == UINT_MAX) {
        break;
      }
    }
    tulipTypeArray.push(tulipType(result[i]));
  }
  freeArrayInEmHeap(result);
  return tulipTypeArray;
};

tulip.CppObjectWrapper = function(cppPointer, wrappedTypename) {
  this.cppPointer = cppPointer;
  this.wrappedTypename = wrappedTypename;
};

tulip.CppObjectWrapper.prototype.getCppPointer = function() {
  return this.cppPointer;
};

tulip.CppObjectWrapper.prototype.cppPointerValid = function() {
  return this.cppPointer != 0 && !_isPointerDeleted(this.cppPointer);
};

tulip.CppObjectWrapper.prototype.getWrappedTypename = function() {
  return this.wrappedTypename;
};

tulip.CppObjectWrapper.prototype.setWrappedTypename = function(typename) {
  this.wrappedTypename = typename;
};

// ==================================================================================================================

var _PropertyInterface_delete = Module.cwrap('PropertyInterface_delete', null, ["number"]);
var _PropertyInterface_getName = Module.cwrap('PropertyInterface_getName', 'string', ['number']);
var _PropertyInterface_getTypename = Module.cwrap('PropertyInterface_getTypename', 'string', ['number']);
var _PropertyInterface_getGraph = Module.cwrap('PropertyInterface_getGraph', 'number', ['number']);
var _PropertyInterface_getNodeStringValue = Module.cwrap('PropertyInterface_getNodeStringValue', 'string', ['number', 'number']);
var _PropertyInterface_getEdgeStringValue = Module.cwrap('PropertyInterface_getEdgeStringValue', 'string', ['number', 'number']);
var _PropertyInterface_copy = Module.cwrap('PropertyInterface_copy', null, ["number", "number"]);

/**
* This is the description for the tulip.node class.
*
* @class PropertyInterface
*/
tulip.PropertyInterface = function tulip_PropertyInterface(cppPointer, graphManaged) {
  var newObject = createObject(tulip.PropertyInterface, this);
  if (tulip_PropertyInterface.caller == null || tulip_PropertyInterface.caller.name != "createObject") {
    tulip.CppObjectWrapper.call(newObject, cppPointer, "tlp::PropertyInterface");
    newObject.graphManaged = graphManaged;
  }
  return newObject;
};
tulip.PropertyInterface.inheritsFrom(tulip.CppObjectWrapper);
tulip.PropertyInterface.prototype.destroy = function tulip_PropertyInterface_prototype_destroy() {
  checkWrappedCppPointer(this.cppPointer);
  if (!this.graphManaged) {
    _PropertyInterface_delete(this.cppPointer);
    this.cppPointer = 0;
  } else {
    console.log("Not destroying property named '" + this.getName() + "' as it is managed by the graph named '" + this.getGraph().getName() + "'");
  }
};
tulip.PropertyInterface.prototype.getName = function tulip_PropertyInterface_prototype_getName() {
  checkWrappedCppPointer(this.cppPointer);
  return _PropertyInterface_getName(this.cppPointer);
};
tulip.PropertyInterface.prototype.getTypename = function tulip_PropertyInterface_prototype_getTypename() {
  checkWrappedCppPointer(this.cppPointer);
  return _PropertyInterface_getTypename(this.cppPointer);
};
tulip.PropertyInterface.prototype.getNodeStringValue = function tulip_PropertyInterface_prototype_getNodeStringValue(node) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node], 1);
  return _PropertyInterface_getNodeStringValue(this.cppPointer, node.id);
};
tulip.PropertyInterface.prototype.getEdgeStringValue = function tulip_PropertyInterface_prototype_getEdgeStringValue(edge) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge], 1);
  return _PropertyInterface_getEdgeStringValue(this.cppPointer, edge.id);
};
tulip.PropertyInterface.prototype.copy = function tulip_PropertyInterface_prototype_copy(propertyToCopy) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.PropertyInterface], 1);
  _PropertyInterface_copy(this.cppPointer, propertyToCopy.cppPointer);
};
/**
* Returns the graph that have created that property
*
* @method getGraph
* @return {Graph}
*/
tulip.PropertyInterface.prototype.getGraph = function tulip_PropertyInterface_prototype_getGraph() {
  checkWrappedCppPointer(this.cppPointer);
  return tulip.Graph(_PropertyInterface_getGraph(this.cppPointer));
};

// ==================================================================================================================

function createPropertyProxyIfAvailable(prop) {
  if (typeof Proxy == 'function') {
    var handler = {
      get: function(target, p) {
        if (typeOf(p) == 'string' && p.startsWith('[Node')) {
          return target.getNodeValue(tulip.Node(parseInt(p.substr(6, p.length-7))));
        } else if (typeOf(p) == 'string' && p.startsWith('[Edge')) {
          return target.getEdgeValue(tulip.Edge(parseInt(p.substr(6, p.length-7))));
        } else {
          return target[p];
        }
      },
      set: function(target, p, value){
        if (typeOf(p) == 'string' && p.startsWith('[Node')) {
          target.setNodeValue(tulip.Node(parseInt(p.substr(6, p.length-7))), value);
        } else if (typeOf(p) == 'string' && p.startsWith('[Edge')) {
          target.setEdgeValue(tulip.Edge(parseInt(p.substr(6, p.length-7))), value);
        } else {
          target[p] = value;
        }
      }
    };
    return new Proxy(prop, handler);
  } else {
    return prop;
  }
}

// ==================================================================================================================

var _createBooleanProperty = Module.cwrap('createBooleanProperty', 'number', ['number', 'string']);
var _BooleanProperty_setAllNodeValue = Module.cwrap('BooleanProperty_setAllNodeValue', null, ['number', 'number']);
var _BooleanProperty_getNodeDefaultValue = Module.cwrap('BooleanProperty_getNodeDefaultValue', 'number', ['number']);
var _BooleanProperty_setNodeValue = Module.cwrap('BooleanProperty_setNodeValue', null, ['number', 'number', 'number']);
var _BooleanProperty_getNodeValue = Module.cwrap('BooleanProperty_getNodeValue', 'number', ['number', 'number']);
var _BooleanProperty_setAllEdgeValue = Module.cwrap('BooleanProperty_setAllEdgeValue', null, ['number', 'number']);
var _BooleanProperty_setEdgeValue = Module.cwrap('BooleanProperty_setEdgeValue', null, ['number', 'number', 'number']);
var _BooleanProperty_getEdgeDefaultValue = Module.cwrap('BooleanProperty_getEdgeDefaultValue', 'number', ['number']);
var _BooleanProperty_getEdgeValue = Module.cwrap('BooleanProperty_getEdgeValue', 'number', ['number', 'number']);
var _BooleanProperty_getNodesEqualTo = Module.cwrap('BooleanProperty_getNodesEqualTo', null, ['number', 'number', 'number', 'number']);
var _BooleanProperty_getEdgesEqualTo = Module.cwrap('BooleanProperty_getEdgesEqualTo', null, ['number', 'number', 'number', 'number']);

tulip.BooleanProperty = function tulip_BooleanProperty() {
  var newObject = createObject(tulip.BooleanProperty, this);
  if (tulip_BooleanProperty.caller == null || tulip_BooleanProperty.caller.name != "createObject") {
    var cppPointer = 0;
    var graphManaged = false;
    if (arguments.length == 1 && typeOf(arguments[0]) == "number") {
      cppPointer = arguments[0];
      graphManaged = true;
    } else {
      checkArgumentsTypes(arguments, [tulip.Graph, "string"], 1);
      var propName = "";
      if (arguments.length > 1) propName = arguments[1];
      cppPointer = _createBooleanProperty(arguments[0].cppPointer, propName);
    }
    tulip.PropertyInterface.call(newObject, cppPointer, graphManaged);
    newObject.setWrappedTypename("tlp::BooleanProperty");
  }
  return createPropertyProxyIfAvailable(newObject);
};
tulip.BooleanProperty.inheritsFrom(tulip.PropertyInterface);
tulip.BooleanProperty.prototype.getNodeDefaultValue = function tulip_BooleanProperty_prototype_getNodeDefaultValue() {
  checkWrappedCppPointer(this.cppPointer);
  return _BooleanProperty_getNodeDefaultValue(this.cppPointer) > 0;
};
tulip.BooleanProperty.prototype.getNodeValue = function tulip_BooleanProperty_prototype_getNodeValue(node) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node]);
  return _BooleanProperty_getNodeValue(this.cppPointer, node.id) > 0;
};
tulip.BooleanProperty.prototype.setNodeValue = function tulip_BooleanProperty_prototype_setNodeValue(node, val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node, "boolean"]);
  _BooleanProperty_setNodeValue(this.cppPointer, node.id, val);
};
tulip.BooleanProperty.prototype.getEdgeDefaultValue = function tulip_BooleanProperty_prototype_getEdgeDefaultValue() {
  checkWrappedCppPointer(this.cppPointer);
  return _BooleanProperty_getEdgeDefaultValue(this.cppPointer) > 0;
};
tulip.BooleanProperty.prototype.getEdgeValue = function tulip_BooleanProperty_prototype_getEdgeValue(edge) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge]);
  return _BooleanProperty_getEdgeValue(this.cppPointer, edge.id) > 0;
};
tulip.BooleanProperty.prototype.setEdgeValue = function tulip_BooleanProperty_prototype_setEdgeValue(edge, val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge, "boolean"]);
  _BooleanProperty_setEdgeValue(this.cppPointer, edge.id, val);
};
tulip.BooleanProperty.prototype.setAllNodeValue = function tulip_BooleanProperty_prototype_setAllNodeValue(val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["boolean"]);
  _BooleanProperty_setAllNodeValue(this.cppPointer, val);
};
tulip.BooleanProperty.prototype.setAllEdgeValue = function tulip_BooleanProperty_prototype_setAllEdgeValue(val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["boolean"]);
  _BooleanProperty_setAllEdgeValue(this.cppPointer, val);
};
tulip.BooleanProperty.prototype.getNodesEqualTo = function tulip_BooleanProperty_prototype_getNodesEqualTo(val, graph) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["boolean", tulip.Graph]);
  var propObject = this;
  return getArrayOfTulipType(propObject.getGraph().numberOfNodes(), function(byteOffset){_BooleanProperty_getNodesEqualTo(propObject.cppPointer, val, graph ? graph.cppPointer : 0, byteOffset)}, tulip.Node);
};
tulip.BooleanProperty.prototype.getEdgesEqualTo = function tulip_BooleanProperty_prototype_getEdgesEqualTo(val, graph) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["boolean", tulip.Graph]);
  var propObject = this;
  return getArrayOfTulipType(propObject.getGraph().numberOfEdges(), function(byteOffset){_BooleanProperty_getEdgesEqualTo(propObject.cppPointer, val, graph ? graph.cppPointer : 0, byteOffset)}, tulip.Edge);
};
// ==================================================================================================================

var _createBooleanVectorProperty = Module.cwrap('createBooleanVectorProperty', 'number', ['number', 'string']);

var _BooleanVectorProperty_setAllNodeValue = Module.cwrap('BooleanVectorProperty_setAllNodeValue', null, ['number', 'number', 'number']);
var _BooleanVectorProperty_setNodeValue = Module.cwrap('BooleanVectorProperty_setNodeValue', null, ['number', 'number', 'number', 'number']);
var _BooleanVectorProperty_getNodeDefaultVectorSize = Module.cwrap('BooleanVectorProperty_getNodeDefaultVectorSize', 'number', ['number']);
var _BooleanVectorProperty_getNodeDefaultValue = Module.cwrap('BooleanVectorProperty_getNodeDefaultValue', null, ['number', 'number']);
var _BooleanVectorProperty_getNodeVectorSize = Module.cwrap('BooleanVectorProperty_getNodeVectorSize', 'number', ['number', 'number']);
var _BooleanVectorProperty_getNodeValue = Module.cwrap('BooleanVectorProperty_getNodeValue', null, ['number', 'number', 'number']);

var _BooleanVectorProperty_setAllEdgeValue = Module.cwrap('BooleanVectorProperty_setAllEdgeValue', null, ['number', 'number', 'number']);
var _BooleanVectorProperty_setEdgeValue = Module.cwrap('BooleanVectorProperty_setEdgeValue', null, ['number', 'number', 'number', 'number']);
var _BooleanVectorProperty_getEdgeDefaultVectorSize = Module.cwrap('BooleanVectorProperty_getEdgeDefaultVectorSize', 'number', ['number']);
var _BooleanVectorProperty_getEdgeDefaultValue = Module.cwrap('BooleanVectorProperty_getEdgeDefaultValue', null, ['number', 'number']);
var _BooleanVectorProperty_getEdgeVectorSize = Module.cwrap('BooleanVectorProperty_getEdgeVectorSize', 'number', ['number', 'number']);
var _BooleanVectorProperty_getEdgeValue = Module.cwrap('BooleanVectorProperty_getEdgeValue', null, ['number', 'number', 'number']);

tulip.BooleanVectorProperty = function tulip_BooleanVectorProperty() {
  var newObject = createObject(tulip.BooleanVectorProperty, this);
  if (tulip_BooleanVectorProperty.caller == null || tulip_BooleanVectorProperty.caller.name != "createObject") {
    var cppPointer = 0;
    var graphManaged = false;
    if (arguments.length == 1 && typeOf(arguments[0]) == "number") {
      cppPointer = arguments[0];
      graphManaged = true;
    } else {
      checkArgumentsTypes(arguments, [tulip.Graph, "string"], 1);
      var propName = "";
      if (arguments.length > 1) propName = arguments[1];
      cppPointer = _createBooleanVectorProperty(arguments[0].cppPointer, propName);
    }
    tulip.PropertyInterface.call(newObject, cppPointer, graphManaged);
    newObject.setWrappedTypename("tlp::BooleanVectorProperty");
  }
  return createPropertyProxyIfAvailable(newObject);
};
tulip.BooleanVectorProperty.inheritsFrom(tulip.PropertyInterface);
tulip.BooleanVectorProperty.prototype.getNodeDefaultValue = function tulip_BooleanVectorProperty_prototype_getNodeDefaultValue() {
  checkWrappedCppPointer(this.cppPointer);
  var size = _BooleanVectorProperty_getNodeDefaultVectorSize(this.cppPointer);
  var booleanArray = allocArrayInEmHeap(Uint8Array, size);
  _BooleanVectorProperty_getNodeDefaultValue(this.cppPointer, booleanArray.byteOffset);
  var ret = Array.prototype.slice.call(booleanArray).map(Boolean);
  freeArrayInEmHeap(booleanArray);
  return ret;
};
tulip.BooleanVectorProperty.prototype.getNodeValue = function tulip_BooleanVectorProperty_prototype_getNodeValue(node) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node], 1);
  var size = _BooleanVectorProperty_getNodeVectorSize(this.cppPointer, node.id);
  var booleanArray = allocArrayInEmHeap(Uint8Array, size);
  _BooleanVectorProperty_getNodeValue(this.cppPointer, node.id, booleanArray.byteOffset);
  var ret = Array.prototype.slice.call(booleanArray).map(Boolean);
  freeArrayInEmHeap(booleanArray);
  return ret;
};
tulip.BooleanVectorProperty.prototype.setNodeValue = function tulip_BooleanVectorProperty_prototype_setNodeValue(node, val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node, "array"], 2);
  checkArrayOfType(val, "boolean", 1);
  var booleanArray = allocArrayInEmHeap(Uint8Array, val.length);
  for (var i = 0 ; i < val.length ; ++i) {
    booleanArray[i] = val[i];
  }
  _BooleanVectorProperty_setNodeValue(this.cppPointer, node.id, booleanArray.byteOffset, val.length);
  freeArrayInEmHeap(booleanArray);
};
tulip.BooleanVectorProperty.prototype.getEdgeDefaultValue = function tulip_BooleanVectorProperty_prototype_getEdgeDefaultValue() {
  checkWrappedCppPointer(this.cppPointer);
  var size = _BooleanVectorProperty_getEdgeDefaultVectorSize(this.cppPointer);
  var booleanArray = allocArrayInEmHeap(Uint8Array, size);
  _BooleanVectorProperty_getEdgeDefaultValue(this.cppPointer, booleanArray.byteOffset);
  var ret = Array.prototype.slice.call(booleanArray).map(Boolean);
  freeArrayInEmHeap(booleanArray);
  return ret;
};
tulip.BooleanVectorProperty.prototype.getEdgeValue = function tulip_BooleanVectorProperty_prototype_getEdgeValue(edge) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge], 1);
  var size = _BooleanVectorProperty_getEdgeVectorSize(this.cppPointer, edge.id);
  var booleanArray = allocArrayInEmHeap(Uint8Array, size);
  _BooleanVectorProperty_getEdgeValue(this.cppPointer, edge.id, booleanArray.byteOffset);
  var ret = Array.prototype.slice.call(booleanArray).map(Boolean);
  freeArrayInEmHeap(booleanArray);
  return ret;
};
tulip.BooleanVectorProperty.prototype.setEdgeValue = function tulip_BooleanVectorProperty_prototype_setEdgeValue(edge, val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge, "array"], 2);
  checkArrayOfType(val, "boolean", 1);
  var booleanArray = allocArrayInEmHeap(Uint8Array, val.length);
  for (var i = 0 ; i < val.length ; ++i) {
    booleanArray[i] = val[i];
  }
  _BooleanVectorProperty_setEdgeValue(this.cppPointer, edge.id, booleanArray.byteOffset, val.length);
  freeArrayInEmHeap(booleanArray);
};
tulip.BooleanVectorProperty.prototype.setAllNodeValue = function tulip_BooleanVectorProperty_prototype_setAllNodeValue(val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["array"], 1);
  checkArrayOfType(val, "boolean", 0);
  var booleanArray = allocArrayInEmHeap(Uint8Array, val.length);
  for (var i = 0 ; i < val.length ; ++i) {
    booleanArray[i] = val[i];
  }
  _BooleanVectorProperty_setAllNodeValue(this.cppPointer, booleanArray.byteOffset, val.length);
  freeArrayInEmHeap(booleanArray);
};
tulip.BooleanVectorProperty.prototype.setAllEdgeValue = function tulip_BooleanVectorProperty_prototype_setAllEdgeValue(val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["array"], 1);
  checkArrayOfType(val, "boolean", 0);
  var booleanArray = allocArrayInEmHeap(Uint8Array, val.length);
  for (var i = 0 ; i < val.length ; ++i) {
    booleanArray[i] = val[i];
  }
  _BooleanVectorProperty_setAllEdgeValue(this.cppPointer, booleanArray.byteOffset, val.length);
  freeArrayInEmHeap(booleanArray);
};
// ==================================================================================================================

var _createDoubleProperty = Module.cwrap('createDoubleProperty', 'number', ['number', 'string']);
var _DoubleProperty_setAllNodeValue = Module.cwrap('DoubleProperty_setAllNodeValue', null, ['number', 'number']);
var _DoubleProperty_setNodeValue = Module.cwrap('DoubleProperty_setNodeValue', null, ['number', 'number', 'number']);
var _DoubleProperty_getNodeDefaultValue = Module.cwrap('DoubleProperty_getNodeDefaultValue', 'number', ['number']);
var _DoubleProperty_getNodeValue = Module.cwrap('DoubleProperty_getNodeValue', 'number', ['number', 'number']);
var _DoubleProperty_setAllEdgeValue = Module.cwrap('DoubleProperty_setAllEdgeValue', null, ['number', 'number']);
var _DoubleProperty_setEdgeValue = Module.cwrap('DoubleProperty_setEdgeValue', null, ['number', 'number', 'number']);
var _DoubleProperty_getEdgeDefaultValue = Module.cwrap('DoubleProperty_getEdgeDefaultValue', 'number', ['number']);
var _DoubleProperty_getEdgeValue = Module.cwrap('DoubleProperty_getEdgeValue', 'number', ['number', 'number']);
var _DoubleProperty_getSortedEdges = Module.cwrap('DoubleProperty_getSortedEdges', null, ['number', 'number', 'number', 'number']);

tulip.DoubleProperty = function tulip_DoubleProperty() {
  var newObject = createObject(tulip.DoubleProperty, this);
  if (tulip_DoubleProperty.caller == null || tulip_DoubleProperty.caller.name != "createObject") {
    var cppPointer = 0;
    var graphManaged = false;
    if (arguments.length == 1 && typeOf(arguments[0]) == "number") {
      cppPointer = arguments[0];
      graphManaged = true;
    } else {
      checkArgumentsTypes(arguments, [tulip.Graph, "string"], 1);
      var propName = "";
      if (arguments.length > 1) propName = arguments[1];
      cppPointer = _createDoubleProperty(arguments[0].cppPointer, propName);
    }
    tulip.PropertyInterface.call(newObject, cppPointer, graphManaged);
    newObject.setWrappedTypename("tlp::DoubleProperty");
  }
  return createPropertyProxyIfAvailable(newObject);
};
tulip.DoubleProperty.inheritsFrom(tulip.PropertyInterface);
tulip.DoubleProperty.prototype.getNodeDefaultValue = function tulip_DoubleProperty_prototype_getNodeDefaultValue() {
  checkWrappedCppPointer(this.cppPointer);
  return _DoubleProperty_getNodeDefaultValue(this.cppPointer);
};
tulip.DoubleProperty.prototype.getNodeValue = function tulip_DoubleProperty_prototype_getNodeValue(node) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node]);
  return _DoubleProperty_getNodeValue(this.cppPointer, node.id);
};
tulip.DoubleProperty.prototype.setNodeValue = function tulip_DoubleProperty_prototype_setNodeValue(node, val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node, "number"]);
  _DoubleProperty_setNodeValue(this.cppPointer, node.id, val);
};
tulip.DoubleProperty.prototype.getEdgeDefaultValue = function tulip_DoubleProperty_prototype_getEdgeDefaultValue() {
  checkWrappedCppPointer(this.cppPointer);
  return _DoubleProperty_getEdgeDefaultValue(this.cppPointer);
};
tulip.DoubleProperty.prototype.getEdgeValue = function tulip_DoubleProperty_prototype_getEdgeValue(edge) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge]);
  return _DoubleProperty_getEdgeValue(this.cppPointer, edge.id);
};
tulip.DoubleProperty.prototype.setEdgeValue = function tulip_DoubleProperty_prototype_setEdgeValue(edge, val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge, "number"]);
  _DoubleProperty_setEdgeValue(this.cppPointer, edge.id, val);
};
tulip.DoubleProperty.prototype.setAllNodeValue = function tulip_DoubleProperty_prototype_setAllNodeValue(val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["number"]);
  _DoubleProperty_setAllNodeValue(this.cppPointer, val);
};
tulip.DoubleProperty.prototype.setAllEdgeValue = function tulip_DoubleProperty_prototype_setAllEdgeValue(val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["number"]);
  _DoubleProperty_setAllEdgeValue(this.cppPointer, val);
};
tulip.DoubleProperty.prototype.getSortedEdges = function tulip_DoubleProperty_prototype_getSortedEdges(sg, ascendingOrder) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Graph, "boolean"]);
  if (typeOf(sg) == "undefined") sg = this.getGraph();
  if (typeOf(ascendingOrder) == "undefined") ascendingOrder = true;
  var propertyObject = this;
  return getArrayOfTulipType(sg.numberOfEdges(), function(byteOffset){_DoubleProperty_getSortedEdges(propertyObject.cppPointer, sg.cppPointer, ascendingOrder, byteOffset)}, tulip.Edge);
};

// ==================================================================================================================

var _createDoubleVectorProperty = Module.cwrap('createDoubleVectorProperty', 'number', ['number', 'string']);

var _DoubleVectorProperty_setAllNodeValue = Module.cwrap('DoubleVectorProperty_setAllNodeValue', null, ['number', 'number', 'number']);
var _DoubleVectorProperty_setNodeValue = Module.cwrap('DoubleVectorProperty_setNodeValue', null, ['number', 'number', 'number', 'number']);
var _DoubleVectorProperty_getNodeDefaultVectorSize = Module.cwrap('DoubleVectorProperty_getNodeDefaultVectorSize', 'number', ['number']);
var _DoubleVectorProperty_getNodeDefaultValue = Module.cwrap('DoubleVectorProperty_getNodeDefaultValue', null, ['number', 'number']);
var _DoubleVectorProperty_getNodeVectorSize = Module.cwrap('DoubleVectorProperty_getNodeVectorSize', 'number', ['number', 'number']);
var _DoubleVectorProperty_getNodeValue = Module.cwrap('DoubleVectorProperty_getNodeValue', null, ['number', 'number', 'number']);

var _DoubleVectorProperty_setAllEdgeValue = Module.cwrap('DoubleVectorProperty_setAllEdgeValue', null, ['number', 'number', 'number']);
var _DoubleVectorProperty_setEdgeValue = Module.cwrap('DoubleVectorProperty_setEdgeValue', null, ['number', 'number', 'number', 'number']);
var _DoubleVectorProperty_getEdgeDefaultVectorSize = Module.cwrap('DoubleVectorProperty_getEdgeDefaultVectorSize', 'number', ['number']);
var _DoubleVectorProperty_getEdgeDefaultValue = Module.cwrap('DoubleVectorProperty_getEdgeDefaultValue', null, ['number', 'number']);
var _DoubleVectorProperty_getEdgeVectorSize = Module.cwrap('DoubleVectorProperty_getEdgeVectorSize', 'number', ['number', 'number']);
var _DoubleVectorProperty_getEdgeValue = Module.cwrap('DoubleVectorProperty_getEdgeValue', null, ['number', 'number', 'number']);

tulip.DoubleVectorProperty = function tulip_DoubleVectorProperty() {
  var newObject = createObject(tulip.DoubleVectorProperty, this);
  if (tulip_DoubleVectorProperty.caller == null || tulip_DoubleVectorProperty.caller.name != "createObject") {
    var cppPointer = 0;
    var graphManaged = false;
    if (arguments.length == 1 && typeOf(arguments[0]) == "number") {
      cppPointer = arguments[0];
      graphManaged = true;
    } else {
      checkArgumentsTypes(arguments, [tulip.Graph, "string"], 1);
      var propName = "";
      if (arguments.length > 1) propName = arguments[1];
      cppPointer = _createDoubleVectorProperty(arguments[0].cppPointer, propName);
    }
    tulip.PropertyInterface.call(newObject, cppPointer, graphManaged);
    newObject.setWrappedTypename("tlp::DoubleVectorProperty");
  }
  return createPropertyProxyIfAvailable(newObject);
};
tulip.DoubleVectorProperty.inheritsFrom(tulip.PropertyInterface);
tulip.DoubleVectorProperty.prototype.getNodeDefaultValue = function tulip_DoubleVectorProperty_prototype_getNodeDefaultValue() {
  checkWrappedCppPointer(this.cppPointer);
  var size = _DoubleVectorProperty_getNodeDefaultVectorSize(this.cppPointer);
  var doubleArray = allocArrayInEmHeap(Float64Array, size);
  _DoubleVectorProperty_getNodeDefaultValue(this.cppPointer, doubleArray.byteOffset);
  var ret = Array.prototype.slice.call(doubleArray);
  freeArrayInEmHeap(doubleArray);
  return ret;
};
tulip.DoubleVectorProperty.prototype.getNodeValue = function tulip_DoubleVectorProperty_prototype_getNodeValue(node) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node], 1);
  var size = _DoubleVectorProperty_getNodeVectorSize(this.cppPointer, node.id);
  var doubleArray = allocArrayInEmHeap(Float64Array, size);
  _DoubleVectorProperty_getNodeValue(this.cppPointer, node.id, doubleArray.byteOffset);
  var ret = Array.prototype.slice.call(doubleArray);
  freeArrayInEmHeap(doubleArray);
  return ret;
};
tulip.DoubleVectorProperty.prototype.setNodeValue = function tulip_DoubleVectorProperty_prototype_setNodeValue(node, val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node, "array"], 2);
  checkArrayOfType(val, "number", 1);
  var doubleArray = allocArrayInEmHeap(Float64Array, val.length);
  for (var i = 0 ; i < val.length ; ++i) {
    doubleArray[i] = val[i];
  }
  _DoubleVectorProperty_setNodeValue(this.cppPointer, node.id, doubleArray.byteOffset, val.length);
  freeArrayInEmHeap(doubleArray);
};
tulip.DoubleVectorProperty.prototype.getEdgeDefaultValue = function tulip_DoubleVectorProperty_prototype_getEdgeDefaultValue() {
  checkWrappedCppPointer(this.cppPointer);
  var size = _DoubleVectorProperty_getEdgeDefaultVectorSize(this.cppPointer);
  var doubleArray = allocArrayInEmHeap(Float64Array, size);
  _DoubleVectorProperty_getEdgeDefaultValue(this.cppPointer, doubleArray.byteOffset);
  var ret = Array.prototype.slice.call(doubleArray);
  freeArrayInEmHeap(doubleArray);
  return ret;
};
tulip.DoubleVectorProperty.prototype.getEdgeValue = function tulip_DoubleVectorProperty_prototype_getEdgeValue(edge) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge], 1);
  var size = _DoubleVectorProperty_getEdgeVectorSize(this.cppPointer, edge.id);
  var doubleArray = allocArrayInEmHeap(Float64Array, size);
  _DoubleVectorProperty_getEdgeValue(this.cppPointer, edge.id, doubleArray.byteOffset);
  var ret = Array.prototype.slice.call(doubleArray);
  freeArrayInEmHeap(doubleArray);
  return ret;
};
tulip.DoubleVectorProperty.prototype.setEdgeValue = function tulip_DoubleVectorProperty_prototype_setEdgeValue(edge, val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge, "array"], 2);
  checkArrayOfType(val, "number", 1);
  var doubleArray = allocArrayInEmHeap(Float64Array, val.length);
  for (var i = 0 ; i < val.length ; ++i) {
    doubleArray[i] = val[i];
  }
  _DoubleVectorProperty_setEdgeValue(this.cppPointer, edge.id, doubleArray.byteOffset, val.length);
  freeArrayInEmHeap(doubleArray);
};
tulip.DoubleVectorProperty.prototype.setAllNodeValue = function tulip_DoubleVectorProperty_prototype_setAllNodeValue(val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["array"], 1);
  checkArrayOfType(val, "number", 0);
  var doubleArray = allocArrayInEmHeap(Float64Array, val.length);
  for (var i = 0 ; i < val.length ; ++i) {
    doubleArray[i] = val[i];
  }
  _DoubleVectorProperty_setAllNodeValue(this.cppPointer, doubleArray.byteOffset, val.length);
  freeArrayInEmHeap(doubleArray);
};
tulip.DoubleVectorProperty.prototype.setAllEdgeValue = function tulip_DoubleVectorProperty_prototype_setAllEdgeValue(val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["array"], 1);
  checkArrayOfType(val, "number", 0);
  var doubleArray = allocArrayInEmHeap(Float64Array, val.length);
  for (var i = 0 ; i < val.length ; ++i) {
    doubleArray[i] = val[i];
  }
  _DoubleVectorProperty_setAllEdgeValue(this.cppPointer, doubleArray.byteOffset, val.length);
  freeArrayInEmHeap(doubleArray);
};

// ==================================================================================================================

var _createIntegerProperty = Module.cwrap('createIntegerProperty', 'number', ['number', 'string']);
var _IntegerProperty_setAllNodeValue = Module.cwrap('IntegerProperty_setAllNodeValue', null, ['number', 'number']);
var _IntegerProperty_setNodeValue = Module.cwrap('IntegerProperty_setNodeValue', null, ['number', 'number', 'number']);
var _IntegerProperty_getNodeDefaultValue = Module.cwrap('IntegerProperty_getNodeDefaultValue', 'number', ['number']);
var _IntegerProperty_getNodeValue = Module.cwrap('IntegerProperty_getNodeValue', 'number', ['number', 'number']);
var _IntegerProperty_setAllEdgeValue = Module.cwrap('IntegerProperty_setAllEdgeValue', null, ['number', 'number']);
var _IntegerProperty_setEdgeValue = Module.cwrap('IntegerProperty_setEdgeValue', null, ['number', 'number', 'number']);
var _IntegerProperty_getEdgeDefaultValue = Module.cwrap('IntegerProperty_getEdgeDefaultValue', 'number', ['number']);
var _IntegerProperty_getEdgeValue = Module.cwrap('IntegerProperty_getEdgeValue', 'number', ['number', 'number']);

tulip.IntegerProperty = function tulip_IntegerProperty() {
  var newObject = createObject(tulip.IntegerProperty, this);
  if (tulip_IntegerProperty.caller == null || tulip_IntegerProperty.caller.name != "createObject") {
    var cppPointer = 0;
    var graphManaged = false;
    if (arguments.length == 1 && typeOf(arguments[0]) == "number") {
      cppPointer = arguments[0];
      graphManaged = true;
    } else {
      checkArgumentsTypes(arguments, [tulip.Graph, "string"], 1);
      var propName = "";
      if (arguments.length > 1) propName = arguments[1];
      cppPointer = _createIntegerProperty(arguments[0].cppPointer, propName);
    }
    tulip.PropertyInterface.call(newObject, cppPointer, graphManaged);
    newObject.setWrappedTypename("tlp::IntegerProperty");
  }
  return createPropertyProxyIfAvailable(newObject);
};
tulip.IntegerProperty.inheritsFrom(tulip.PropertyInterface);
tulip.IntegerProperty.prototype.getNodeDefaultValue = function tulip_IntegerProperty_prototype_getNodeDefaultValue() {
  checkWrappedCppPointer(this.cppPointer);
  return _IntegerProperty_getNodeDefaultValue(this.cppPointer);
};
tulip.IntegerProperty.prototype.getNodeValue = function tulip_IntegerProperty_prototype_getNodeValue(node) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node]);
  return _IntegerProperty_getNodeValue(this.cppPointer, node.id);
};
tulip.IntegerProperty.prototype.setNodeValue = function tulip_IntegerProperty_prototype_setNodeValue(node, val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node, "number"]);
  _IntegerProperty_setNodeValue(this.cppPointer, node.id, val);
};
tulip.IntegerProperty.prototype.getEdgeDefaultValue = function tulip_IntegerProperty_prototype_getEdgeDefaultValue() {
  checkWrappedCppPointer(this.cppPointer);
  return _IntegerProperty_getEdgeDefaultValue(this.cppPointer);
};
tulip.IntegerProperty.prototype.getEdgeValue = function tulip_IntegerProperty_prototype_getEdgeValue(edge) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge]);
  return _IntegerProperty_getEdgeValue(this.cppPointer, edge.id);
};
tulip.IntegerProperty.prototype.setEdgeValue = function tulip_IntegerProperty_prototype_setEdgeValue(edge, val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge, "number"]);
  _IntegerProperty_setEdgeValue(this.cppPointer, edge.id, val);
};
tulip.IntegerProperty.prototype.setAllNodeValue = function tulip_IntegerProperty_prototype_setAllNodeValue(val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["number"]);
  _IntegerProperty_setAllNodeValue(this.cppPointer, val);
};
tulip.IntegerProperty.prototype.setAllEdgeValue = function tulip_IntegerProperty_prototype_setAllEdgeValue(val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["number"]);
  _IntegerProperty_setAllEdgeValue(this.cppPointer, val);
};

// ==================================================================================================================

var _createIntegerVectorProperty = Module.cwrap('createIntegerVectorProperty', 'number', ['number', 'string']);

var _IntegerVectorProperty_setAllNodeValue = Module.cwrap('IntegerVectorProperty_setAllNodeValue', null, ['number', 'number', 'number']);
var _IntegerVectorProperty_setNodeValue = Module.cwrap('IntegerVectorProperty_setNodeValue', null, ['number', 'number', 'number', 'number']);
var _IntegerVectorProperty_getNodeDefaultVectorSize = Module.cwrap('IntegerVectorProperty_getNodeDefaultVectorSize', 'number', ['number']);
var _IntegerVectorProperty_getNodeDefaultValue = Module.cwrap('IntegerVectorProperty_getNodeDefaultValue', null, ['number', 'number']);
var _IntegerVectorProperty_getNodeVectorSize = Module.cwrap('IntegerVectorProperty_getNodeVectorSize', 'number', ['number', 'number']);
var _IntegerVectorProperty_getNodeValue = Module.cwrap('IntegerVectorProperty_getNodeValue', null, ['number', 'number', 'number']);

var _IntegerVectorProperty_setAllEdgeValue = Module.cwrap('IntegerVectorProperty_setAllEdgeValue', null, ['number', 'number', 'number']);
var _IntegerVectorProperty_setEdgeValue = Module.cwrap('IntegerVectorProperty_setEdgeValue', null, ['number', 'number', 'number', 'number']);
var _IntegerVectorProperty_getEdgeDefaultVectorSize = Module.cwrap('IntegerVectorProperty_getEdgeDefaultVectorSize', 'number', ['number']);
var _IntegerVectorProperty_getEdgeDefaultValue = Module.cwrap('IntegerVectorProperty_getEdgeDefaultValue', null, ['number', 'number']);
var _IntegerVectorProperty_getEdgeVectorSize = Module.cwrap('IntegerVectorProperty_getEdgeVectorSize', 'number', ['number', 'number']);
var _IntegerVectorProperty_getEdgeValue = Module.cwrap('IntegerVectorProperty_getEdgeValue', null, ['number', 'number', 'number']);

tulip.IntegerVectorProperty = function tulip_IntegerVectorProperty() {
  var newObject = createObject(tulip.IntegerVectorProperty, this);
  if (tulip_IntegerVectorProperty.caller == null || tulip_IntegerVectorProperty.caller.name != "createObject") {
    var cppPointer = 0;
    var graphManaged = false;
    if (arguments.length == 1 && typeOf(arguments[0]) == "number") {
      cppPointer = arguments[0];
      graphManaged = true;
    } else {
      checkArgumentsTypes(arguments, [tulip.Graph, "string"], 1);
      var propName = "";
      if (arguments.length > 1) propName = arguments[1];
      cppPointer = _createIntegerVectorProperty(arguments[0].cppPointer, propName);
    }
    tulip.PropertyInterface.call(newObject, cppPointer, graphManaged);
    newObject.setWrappedTypename("tlp::IntegerVectorProperty");
  }
  return createPropertyProxyIfAvailable(newObject);
};
tulip.IntegerVectorProperty.inheritsFrom(tulip.PropertyInterface);
tulip.IntegerVectorProperty.prototype.getNodeDefaultValue = function tulip_IntegerVectorProperty_prototype_getNodeDefaultValue() {
  checkWrappedCppPointer(this.cppPointer);
  var size = _IntegerVectorProperty_getNodeDefaultVectorSize(this.cppPointer);
  var integerArray = allocArrayInEmHeap(Int32Array, size);
  _IntegerVectorProperty_getNodeDefaultValue(this.cppPointer, integerArray.byteOffset);
  var ret = Array.prototype.slice.call(integerArray);
  freeArrayInEmHeap(integerArray);
  return ret;
};
tulip.IntegerVectorProperty.prototype.getNodeValue = function tulip_IntegerVectorProperty_prototype_getNodeValue2(node) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node], 1);
  var size = _IntegerVectorProperty_getNodeVectorSize(this.cppPointer, node.id);
  var integerArray = allocArrayInEmHeap(Int32Array, size);
  _IntegerVectorProperty_getNodeValue(this.cppPointer, node.id, integerArray.byteOffset);
  var ret = Array.prototype.slice.call(integerArray);
  freeArrayInEmHeap(integerArray);
  return ret;
};
tulip.IntegerVectorProperty.prototype.setNodeValue = function tulip_IntegerVectorProperty_prototype_setNodeValue(node, val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node, "array"], 2);
  checkArrayOfType(val, "number", 1);
  var integerArray = allocArrayInEmHeap(Int32Array, val.length);
  for (var i = 0 ; i < val.length ; ++i) {
    integerArray[i] = val[i];
  }
  _IntegerVectorProperty_setNodeValue(this.cppPointer, node.id, integerArray.byteOffset, val.length);
  freeArrayInEmHeap(integerArray);
};
tulip.IntegerVectorProperty.prototype.getEdgeDefaultValue = function tulip_IntegerVectorProperty_prototype_getEdgeDefaultValue() {
  checkWrappedCppPointer(this.cppPointer);
  var size = _IntegerVectorProperty_getEdgeDefaultVectorSize(this.cppPointer);
  var integerArray = allocArrayInEmHeap(Int32Array, size);
  _IntegerVectorProperty_getEdgeDefaultValue(this.cppPointer, integerArray.byteOffset);
  var ret = Array.prototype.slice.call(integerArray);
  freeArrayInEmHeap(integerArray);
  return ret;
};
tulip.IntegerVectorProperty.prototype.getEdgeValue = function tulip_IntegerVectorProperty_prototype_getEdgeValue(edge) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge], 1);
  var size = _IntegerVectorProperty_getEdgeVectorSize(this.cppPointer, edge.id);
  var integerArray = allocArrayInEmHeap(Int32Array, size);
  _IntegerVectorProperty_getEdgeValue(this.cppPointer, edge.id, integerArray.byteOffset);
  var ret = Array.prototype.slice.call(integerArray);
  freeArrayInEmHeap(integerArray);
  return ret;
};
tulip.IntegerVectorProperty.prototype.setEdgeValue = function tulip_IntegerVectorProperty_prototype_setEdgeValue(edge, val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge, "array"], 2);
  checkArrayOfType(val, "number", 1);
  var integerArray = allocArrayInEmHeap(Int32Array, val.length);
  for (var i = 0 ; i < val.length ; ++i) {
    integerArray[i] = val[i];
  }
  _IntegerVectorProperty_setEdgeValue(this.cppPointer, edge.id, integerArray.byteOffset, val.length);
  freeArrayInEmHeap(integerArray);
};
tulip.IntegerVectorProperty.prototype.setAllNodeValue = function tulip_IntegerVectorProperty_prototype_setAllNodeValue(val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["array"], 1);
  checkArrayOfType(val, "number", 0);
  var integerArray = allocArrayInEmHeap(Int32Array, val.length);
  for (var i = 0 ; i < val.length ; ++i) {
    integerArray[i] = val[i];
  }
  _IntegerVectorProperty_setAllNodeValue(this.cppPointer, integerArray.byteOffset, val.length);
  freeArrayInEmHeap(integerArray);
};
tulip.IntegerVectorProperty.prototype.setAllEdgeValue = function tulip_IntegerVectorProperty_prototype_setAllEdgeValue(val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["array"], 1);
  checkArrayOfType(val, "number", 0);
  var integerArray = allocArrayInEmHeap(Int32Array, val.length);
  for (var i = 0 ; i < val.length ; ++i) {
    integerArray[i] = val[i];
  }
  _IntegerVectorProperty_setAllEdgeValue(this.cppPointer, integerArray.byteOffset, val.length);
  freeArrayInEmHeap(integerArray);
};

// ==================================================================================================================

var _createStringProperty = Module.cwrap('createStringProperty', 'number', ['number', 'string']);
var _StringProperty_setAllNodeValue = Module.cwrap('StringProperty_setAllNodeValue', null, ['number', 'string']);
var _StringProperty_setNodeValue = Module.cwrap('StringProperty_setNodeValue', null, ['number', 'number', 'string']);
var _StringProperty_getNodeDefaultValue = Module.cwrap('StringProperty_getNodeDefaultValue', 'string', ['number']);
var _StringProperty_getNodeValue = Module.cwrap('StringProperty_getNodeValue', 'string', ['number', 'number']);
var _StringProperty_setAllEdgeValue = Module.cwrap('StringProperty_setAllEdgeValue', null, ['number', 'string']);
var _StringProperty_setEdgeValue = Module.cwrap('StringProperty_setEdgeValue', null, ['number', 'number', 'string']);
var _StringProperty_getEdgeDefaultValue = Module.cwrap('StringProperty_getEdgeDefaultValue', 'string', ['number']);
var _StringProperty_getEdgeValue = Module.cwrap('StringProperty_getEdgeValue', 'string', ['number', 'number']);

tulip.StringProperty = function tulip_StringProperty() {
  var newObject = createObject(tulip.StringProperty, this);
  if (tulip_StringProperty.caller == null || tulip_StringProperty.caller.name != "createObject") {
    var cppPointer = 0;
    var graphManaged = false;
    if (arguments.length == 1 && typeOf(arguments[0]) == "number") {
      cppPointer = arguments[0];
      graphManaged = true;
    } else {
      checkArgumentsTypes(arguments, [tulip.Graph, "string"], 1);
      var propName = "";
      if (arguments.length > 1) propName = arguments[1];
      cppPointer = _createStringProperty(arguments[0].cppPointer, propName);
    }
    tulip.PropertyInterface.call(newObject, cppPointer, graphManaged);
    newObject.setWrappedTypename("tlp::StringProperty");
  }
  return createPropertyProxyIfAvailable(newObject);
};
tulip.StringProperty.inheritsFrom(tulip.PropertyInterface);
tulip.StringProperty.prototype.getNodeDefaultValue = function tulip_StringProperty_prototype_getNodeDefaultValue() {
  checkWrappedCppPointer(this.cppPointer);
  return _StringProperty_getNodeDefaultValue(this.cppPointer);
};
tulip.StringProperty.prototype.getNodeValue = function tulip_StringProperty_prototype_getNodeValue(node) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node]);
  return _StringProperty_getNodeValue(this.cppPointer, node.id);
};
tulip.StringProperty.prototype.setNodeValue = function tulip_StringProperty_prototype_setNodeValue(node, val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node, "string"]);
  _StringProperty_setNodeValue(this.cppPointer, node.id, val);
};
tulip.StringProperty.prototype.getEdgeDefaultValue = function tulip_StringProperty_prototype_getEdgeDefaultValue() {
  checkWrappedCppPointer(this.cppPointer);
  return _StringProperty_getEdgeDefaultValue(this.cppPointer);
};
tulip.StringProperty.prototype.getEdgeValue = function tulip_StringProperty_prototype_getEdgeValue(edge) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge]);
  return _StringProperty_getEdgeValue(this.cppPointer, edge.id);
};
tulip.StringProperty.prototype.setEdgeValue = function tulip_StringProperty_prototype_setEdgeValue(edge, val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge, "string"]);
  _StringProperty_setEdgeValue(this.cppPointer, edge.id, val);
};
tulip.StringProperty.prototype.setAllNodeValue = function tulip_StringProperty_prototype_setAllNodeValue(val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string"]);
  _StringProperty_setAllNodeValue(this.cppPointer, val);
};
tulip.StringProperty.prototype.setAllEdgeValue = function tulip_StringProperty_prototype_setAllEdgeValue(val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string"]);
  _StringProperty_setAllEdgeValue(this.cppPointer, val);
};

// ==================================================================================================================

var _createStringVectorProperty = Module.cwrap('createStringVectorProperty', 'number', ['number', 'string']);
var _StringVectorProperty_setAllNodeValue = Module.cwrap('StringVectorProperty_setAllNodeValue', null, ['number', 'number', 'number', 'number']);
var _StringVectorProperty_setNodeValue = Module.cwrap('StringVectorProperty_setNodeValue', null, ['number', 'number', 'number', 'number', 'number']);
var _StringVectorProperty_setAllEdgeValue = Module.cwrap('StringVectorProperty_setAllEdgeValue', null, ['number', 'number', 'number', 'number']);
var _StringVectorProperty_setEdgeValue = Module.cwrap('StringVectorProperty_setEdgeValue', null, ['number', 'number', 'number', 'number', 'number']);

var _StringVectorProperty_getNodeDefaultVectorSize = Module.cwrap('StringVectorProperty_getNodeDefaultVectorSize', 'number', ['number']);
var _StringVectorProperty_getNodeDefaultStringsLengths = Module.cwrap('StringVectorProperty_getNodeDefaultStringsLengths', 'number', ['number', 'number']);
var _StringVectorProperty_getNodeDefaultValue = Module.cwrap('StringVectorProperty_getNodeDefaultValue', null, ['number', 'number']);

var _StringVectorProperty_getNodeVectorSize = Module.cwrap('StringVectorProperty_getNodeVectorSize', 'number', ['number', 'number']);
var _StringVectorProperty_getNodeStringsLengths = Module.cwrap('StringVectorProperty_getNodeStringsLengths', 'number', ['number', 'number', 'number']);
var _StringVectorProperty_getNodeValue = Module.cwrap('StringVectorProperty_getNodeValue', null, ['number', 'number', 'number']);

var _StringVectorProperty_getEdgeVectorSize = Module.cwrap('StringVectorProperty_getEdgeVectorSize', 'number', ['number', 'number']);
var _StringVectorProperty_getEdgeStringsLengths = Module.cwrap('StringVectorProperty_getEdgeStringsLengths', 'number', ['number', 'number', 'number']);
var _StringVectorProperty_getEdgeValue = Module.cwrap('StringVectorProperty_getEdgeValue', null, ['number', 'number', 'number']);

var _StringVectorProperty_getEdgeDefaultVectorSize = Module.cwrap('StringVectorProperty_getEdgeDefaultVectorSize', 'number', ['number']);
var _StringVectorProperty_getEdgeDefaultStringsLengths = Module.cwrap('StringVectorProperty_getEdgeDefaultStringsLengths', 'number', ['number', 'number']);
var _StringVectorProperty_getEdgeDefaultValue = Module.cwrap('StringVectorProperty_getEdgeDefaultValue', null, ['number', 'number']);

tulip.StringVectorProperty = function tulip_StringVectorProperty() {
  var newObject = createObject(tulip.StringVectorProperty, this);
  if (tulip_StringVectorProperty.caller == null || tulip_StringVectorProperty.caller.name != "createObject") {
    var cppPointer = 0;
    var graphManaged = false;
    if (arguments.length == 1 && typeOf(arguments[0]) == "number") {
      cppPointer = arguments[0];
      graphManaged = true;
    } else {
      checkArgumentsTypes(arguments, [tulip.Graph, "string"], 1);
      var propName = "";
      if (arguments.length > 1) propName = arguments[1];
      cppPointer = _createStringVectorProperty(arguments[0].cppPointer, propName);
    }
    tulip.PropertyInterface.call(newObject, cppPointer, graphManaged);
    newObject.setWrappedTypename("tlp::StringVectorProperty");
  }
  return createPropertyProxyIfAvailable(newObject);
};
tulip.StringVectorProperty.inheritsFrom(tulip.PropertyInterface);

tulip.StringVectorProperty.prototype.getNodeDefaultValue = function tulip_StringVectorProperty_prototype_getNodeDefaultValue() {
  checkWrappedCppPointer(this.cppPointer);
  var nbStrings = _StringVectorProperty_getNodeDefaultVectorSize(this.cppPointer);
  var uintArray = allocArrayInEmHeap(Uint32Array, nbStrings);
  var nbBytes = _StringVectorProperty_getNodeDefaultStringsLengths(this.cppPointer, uintArray.byteOffset);
  var ucharArray = allocArrayInEmHeap(Uint8Array, nbBytes);
  _StringVectorProperty_getNodeDefaultValue(this.cppPointer, ucharArray.byteOffset);
  var ret = bytesTypedArrayToStringArray(ucharArray, uintArray, nbStrings);
  freeArrayInEmHeap(uintArray);
  freeArrayInEmHeap(ucharArray);
  return ret;
};
tulip.StringVectorProperty.prototype.getNodeValue = function tulip_StringVectorProperty_prototype_getNodeValue(node) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node]);
  var nbStrings = _StringVectorProperty_getNodeVectorSize(this.cppPointer, node.id);
  var uintArray = allocArrayInEmHeap(Uint32Array, nbStrings);
  var nbBytes = _StringVectorProperty_getNodeStringsLengths(this.cppPointer, node.id, uintArray.byteOffset);
  var ucharArray = allocArrayInEmHeap(Uint8Array, nbBytes);
  _StringVectorProperty_getNodeValue(this.cppPointer, node.id, ucharArray.byteOffset);
  var ret = bytesTypedArrayToStringArray(ucharArray, uintArray, nbStrings);
  freeArrayInEmHeap(uintArray);
  freeArrayInEmHeap(ucharArray);
  return ret;
};
tulip.StringVectorProperty.prototype.setNodeValue = function tulip_StringVectorProperty_prototype_setNodeValue(node, val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node, "array"]);
  checkArrayOfType(val, "string", 1);
  var data = stringArrayToBytesAndOffsetsTypedArray(val);
  _StringVectorProperty_setNodeValue(this.cppPointer, node.id, data.bytesArray.byteOffset, data.offsetsArray.byteOffset, val.length);
  freeArrayInEmHeap(data.bytesArray);
  freeArrayInEmHeap(data.offsetsArray);
};
tulip.StringVectorProperty.prototype.getEdgeDefaultValue = function tulip_StringVectorProperty_prototype_getEdgeDefaultValue() {
  checkWrappedCppPointer(this.cppPointer);
  var nbStrings = _StringVectorProperty_getEdgeDefaultVectorSize(this.cppPointer);
  var uintArray = allocArrayInEmHeap(Uint32Array, nbStrings);
  var nbBytes = _StringVectorProperty_getEdgeDefaultStringsLengths(this.cppPointer, uintArray.byteOffset);
  var ucharArray = allocArrayInEmHeap(Uint8Array, nbBytes);
  _StringVectorProperty_getEdgeDefaultValue(this.cppPointer, ucharArray.byteOffset);
  var ret = bytesTypedArrayToStringArray(ucharArray, uintArray, nbStrings);
  freeArrayInEmHeap(uintArray);
  freeArrayInEmHeap(ucharArray);
  return ret;
};
tulip.StringVectorProperty.prototype.getEdgeValue = function tulip_StringVectorProperty_prototype_getEdgeValue(edge) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge]);
  var nbStrings = _StringVectorProperty_getEdgeVectorSize(this.cppPointer, edge.id);
  var uintArray = allocArrayInEmHeap(Uint32Array, nbStrings);
  var nbBytes = _StringVectorProperty_getEdgeStringsLengths(this.cppPointer, edge.id, uintArray.byteOffset);
  var ucharArray = allocArrayInEmHeap(Uint8Array, nbBytes);
  _StringVectorProperty_getEdgeValue(this.cppPointer, edge.id, ucharArray.byteOffset);
  var ret = bytesTypedArrayToStringArray(ucharArray, uintArray, nbStrings);
  freeArrayInEmHeap(uintArray);
  freeArrayInEmHeap(ucharArray);
  return ret;
};
tulip.StringVectorProperty.prototype.setEdgeValue = function tulip_StringVectorProperty_prototype_setEdgeValue(edge, val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge, "array"]);
  checkArrayOfType(val, "string", 1);
  var data = stringArrayToBytesAndOffsetsTypedArray(val);
  _StringVectorProperty_setEdgeValue(this.cppPointer, edge.id, data.bytesArray.byteOffset, data.offsetsArray.byteOffset, val.length);
  freeArrayInEmHeap(data.bytesArray);
  freeArrayInEmHeap(data.offsetsArray);
};
tulip.StringVectorProperty.prototype.setAllNodeValue = function tulip_StringVectorProperty_prototype_setAllNodeValue(val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["array"]);
  checkArrayOfType(val, "string", 0);
  var data = stringArrayToBytesAndOffsetsTypedArray(val);
  _StringVectorProperty_setAllNodeValue(this.cppPointer, data.bytesArray.byteOffset, data.offsetsArray.byteOffset, val.length);
  freeArrayInEmHeap(data.bytesArray);
  freeArrayInEmHeap(data.offsetsArray);
};
tulip.StringVectorProperty.prototype.setAllEdgeValue = function tulip_StringVectorProperty_prototype_setAllEdgeValue(val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["array"]);
  checkArrayOfType(val, "string", 0);
  var data = stringArrayToBytesAndOffsetsTypedArray(val);
  _StringVectorProperty_setAllEdgeValue(this.cppPointer, data.bytesArray.byteOffset, data.offsetsArray.byteOffset, val.length);
  freeArrayInEmHeap(data.bytesArray);
  freeArrayInEmHeap(data.offsetsArray);
};

// ==================================================================================================================

tulip.Color = function tulip_Color() {
  var newObject = createObject(tulip.Color, this);
  newObject.r = newObject.g = newObject.b = newObject.a = 0;
  if (arguments.length > 0) {
    if (arguments.length == 1 && typeOf(arguments[0]) == 'string') {
      var hexColorRegexp = new RegExp('^#[A-Fa-f0-9]{6}$');
      if (hexColorRegexp.test(arguments[0])) {
        newObject.r = parseInt(arguments[0].substr(1, 2), 16);
        newObject.g = parseInt(arguments[0].substr(3, 2), 16);
        newObject.b = parseInt(arguments[0].substr(5, 2), 16);
        newObject.a = 255;
      }
    } else {
      checkArgumentsTypes(arguments, ["number", "number", "number", "number"], 3);
      newObject.r = arguments[0];
      newObject.g = arguments[1];
      newObject.b = arguments[2];
      if (arguments.length < 4) {
        newObject.a = 255;
      } else {
        newObject.a = arguments[3];
      }
    }
  }
  return newObject;
};

tulip.Color.prototype.rgbHexStr = function tulip_Color_prototype_rgbHexStr() {
  return '#' + this.r.toString(16) + this.g.toString(16) + this.b.toString(16);
};

// ==================================================================================================================

var _createColorProperty = Module.cwrap('createColorProperty', 'number', ['number', 'string']);
var _ColorProperty_setAllNodeValue = Module.cwrap('ColorProperty_setAllNodeValue', null, ['number', 'number', 'number', 'number', 'number']);
var _ColorProperty_setNodeValue = Module.cwrap('ColorProperty_setNodeValue', null, ['number', 'number', 'number', 'number', 'number', 'number']);
var _ColorProperty_getNodeDefaultValue = Module.cwrap('ColorProperty_getNodeDefaultValue', null, ['number', 'number']);
var _ColorProperty_getNodeValue = Module.cwrap('ColorProperty_getNodeValue', null, ['number', 'number', 'number']);
var _ColorProperty_setAllEdgeValue = Module.cwrap('ColorProperty_setAllEdgeValue', null, ['number', 'number', 'number', 'number', 'number']);
var _ColorProperty_setEdgeValue = Module.cwrap('ColorProperty_setEdgeValue', null, ['number', 'number', 'number', 'number', 'number', 'number']);
var _ColorProperty_getEdgeDefaultValue = Module.cwrap('ColorProperty_getEdgeDefaultValue', null, ['number', 'number']);
var _ColorProperty_getEdgeValue = Module.cwrap('ColorProperty_getEdgeValue', null, ['number', 'number', 'number']);

tulip.ColorProperty = function tulip_ColorProperty() {
  var newObject = createObject(tulip.ColorProperty, this);
  if (tulip_ColorProperty.caller == null || tulip_ColorProperty.caller.name != "createObject") {
    var cppPointer = 0;
    var graphManaged = false;
    if (arguments.length == 1 && typeOf(arguments[0]) == "number") {
      cppPointer = arguments[0];
      graphManaged = true;
    } else {
      checkArgumentsTypes(arguments, [tulip.Graph, "string"], 1);
      var propName = "";
      if (arguments.length > 1) propName = arguments[1];
      cppPointer = _createColorProperty(arguments[0].cppPointer, propName);
    }
    tulip.PropertyInterface.call(newObject, cppPointer, graphManaged);
    newObject.setWrappedTypename("tlp::ColorProperty");
  }
  return createPropertyProxyIfAvailable(newObject);
};
tulip.ColorProperty.inheritsFrom(tulip.PropertyInterface);
tulip.ColorProperty.prototype.getNodeDefaultValue = function tulip_ColorProperty_prototype_getNodeDefaultValue() {
  checkWrappedCppPointer(this.cppPointer);
  var ucharArray = allocArrayInEmHeap(Uint8Array, 4);
  _ColorProperty_getNodeDefaultValue(this.cppPointer, ucharArray.byteOffset);
  var ret = tulip.Color(ucharArray[0], ucharArray[1], ucharArray[2], ucharArray[3]);
  freeArrayInEmHeap(ucharArray);
  return ret;
};
tulip.ColorProperty.prototype.getNodeValue = function tulip_ColorProperty_prototype_getNodeValue(node) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node], 1);
  var ucharArray = allocArrayInEmHeap(Uint8Array, 4);
  _ColorProperty_getNodeValue(this.cppPointer, node.id, ucharArray.byteOffset);
  var ret = tulip.Color(ucharArray[0], ucharArray[1], ucharArray[2], ucharArray[3]);
  freeArrayInEmHeap(ucharArray);
  return ret;
};
tulip.ColorProperty.prototype.setNodeValue = function tulip_ColorProperty_prototype_setNodeValue(node, color) {
  checkArgumentsTypes(arguments, [tulip.Node, tulip.Color], 2);
  _ColorProperty_setNodeValue(this.cppPointer, node.id, color.r, color.g, color.b, color.a);
};
tulip.ColorProperty.prototype.getEdgeDefaultValue = function tulip_ColorProperty_prototype_getEdgeDefaultValue() {
  checkWrappedCppPointer(this.cppPointer);
  var ucharArray = allocArrayInEmHeap(Uint8Array, 4);
  _ColorProperty_getEdgeDefaultValue(this.cppPointer, ucharArray.byteOffset);
  var ret = tulip.Color(ucharArray[0], ucharArray[1], ucharArray[2], ucharArray[3]);
  freeArrayInEmHeap(ucharArray);
  return ret;
};
tulip.ColorProperty.prototype.getEdgeValue = function tulip_ColorProperty_prototype_getEdgeValue(edge) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge], 1);
  var ucharArray = allocArrayInEmHeap(Uint8Array, 4);
  _ColorProperty_getEdgeValue(this.cppPointer, edge.id, ucharArray.byteOffset);
  var ret = tulip.Color(ucharArray[0], ucharArray[1], ucharArray[2], ucharArray[3]);
  freeArrayInEmHeap(ucharArray);
  return ret;
};
tulip.ColorProperty.prototype.setEdgeValue = function tulip_ColorProperty_prototype_setEdgeValue(edge, color) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge, tulip.Color], 2);
  _ColorProperty_setEdgeValue(this.cppPointer, edge.id, color.r, color.g, color.b, color.a);
};
tulip.ColorProperty.prototype.setAllNodeValue = function tulip_ColorProperty_prototype_setAllNodeValue(color) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Color], 1);
  _ColorProperty_setAllNodeValue(this.cppPointer, color.r, color.g, color.b, color.a);
};
tulip.ColorProperty.prototype.setAllEdgeValue = function tulip_ColorProperty_prototype_setAllEdgeValue(color) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Color], 1);
  _ColorProperty_setAllEdgeValue(this.cppPointer, color.r, color.g, color.b, color.a);
};

// ==================================================================================================================

var _createColorVectorProperty = Module.cwrap('createColorVectorProperty', 'number', ['number', 'string']);

var _ColorVectorProperty_setAllNodeValue = Module.cwrap('ColorVectorProperty_setAllNodeValue', null, ['number', 'number', 'number']);
var _ColorVectorProperty_setNodeValue = Module.cwrap('ColorVectorProperty_setNodeValue', null, ['number', 'number', 'number', 'number']);
var _ColorVectorProperty_getNodeDefaultVectorSize = Module.cwrap('ColorVectorProperty_getNodeDefaultVectorSize', 'number', ['number']);
var _ColorVectorProperty_getNodeDefaultValue = Module.cwrap('ColorVectorProperty_getNodeDefaultValue', null, ['number', 'number']);
var _ColorVectorProperty_getNodeVectorSize = Module.cwrap('ColorVectorProperty_getNodeVectorSize', 'number', ['number', 'number']);
var _ColorVectorProperty_getNodeValue = Module.cwrap('ColorVectorProperty_getNodeValue', null, ['number', 'number', 'number']);

var _ColorVectorProperty_setAllEdgeValue = Module.cwrap('ColorVectorProperty_setAllEdgeValue', null, ['number', 'number', 'number']);
var _ColorVectorProperty_setEdgeValue = Module.cwrap('ColorVectorProperty_setEdgeValue', null, ['number', 'number', 'number', 'number']);
var _ColorVectorProperty_getEdgeDefaultVectorSize = Module.cwrap('ColorVectorProperty_getEdgeDefaultVectorSize', 'number', ['number']);
var _ColorVectorProperty_getEdgeDefaultValue = Module.cwrap('ColorVectorProperty_getEdgeDefaultValue', null, ['number', 'number']);
var _ColorVectorProperty_getEdgeVectorSize = Module.cwrap('ColorVectorProperty_getEdgeVectorSize', 'number', ['number', 'number']);
var _ColorVectorProperty_getEdgeValue = Module.cwrap('ColorVectorProperty_getEdgeValue', null, ['number', 'number', 'number']);

tulip.ColorVectorProperty = function tulip_ColorVectorProperty() {
  var newObject = createObject(tulip.ColorVectorProperty, this);
  if (tulip_ColorVectorProperty.caller == null || tulip_ColorVectorProperty.caller.name != "createObject") {
    var cppPointer = 0;
    var graphManaged = false;
    if (arguments.length == 1 && typeOf(arguments[0]) == "number") {
      cppPointer = arguments[0];
      graphManaged = true;
    } else {
      checkArgumentsTypes(arguments, [tulip.Graph, "string"], 1);
      var propName = "";
      if (arguments.length > 1) propName = arguments[1];
      cppPointer = _createColorVectorProperty(arguments[0].cppPointer, propName);
    }
    tulip.PropertyInterface.call(newObject, cppPointer, graphManaged);
    newObject.setWrappedTypename("tlp::ColorVectorProperty");
  }
  return createPropertyProxyIfAvailable(newObject);
};
tulip.ColorVectorProperty.inheritsFrom(tulip.PropertyInterface);
tulip.ColorVectorProperty.prototype.getNodeDefaultValue = function tulip_ColorVectorProperty_prototype_getNodeDefaultValue() {
  checkWrappedCppPointer(this.cppPointer);
  var size = _ColorVectorProperty_getNodeDefaultVectorSize(this.cppPointer);
  var colorArray = allocArrayInEmHeap(Uint8Array, size*4);
  _ColorVectorProperty_getNodeDefaultValue(this.cppPointer, colorArray.byteOffset);
  var ret = [];
  for (var i = 0 ; i < size ; ++i) {
    ret.push(tulip.Color(colorArray[4*i],colorArray[4*i+1],colorArray[4*i+2],colorArray[4*i+3]));
  }
  freeArrayInEmHeap(colorArray);
  return ret;
};
tulip.ColorVectorProperty.prototype.getNodeValue = function tulip_ColorVectorProperty_prototype_getNodeValue(node) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node], 1);
  var size = _ColorVectorProperty_getNodeVectorSize(this.cppPointer, node.id);
  var colorArray = allocArrayInEmHeap(Uint8Array, size*4);
  _ColorVectorProperty_getNodeValue(this.cppPointer, node.id, colorArray.byteOffset);
  var ret = [];
  for (var i = 0 ; i < size ; ++i) {
    ret.push(tulip.Color(colorArray[4*i],colorArray[4*i+1],colorArray[4*i+2],colorArray[4*i+3]));
  }
  freeArrayInEmHeap(colorArray);
  return ret;
};
tulip.ColorVectorProperty.prototype.setNodeValue = function tulip_ColorVectorProperty_prototype_setNodeValue(node, val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node, "array"], 2);
  checkArrayOfType(val, tulip.Color, 1);
  var colorArray = allocArrayInEmHeap(Uint8Array, val.length*4);
  for (var i = 0 ; i < val.length ; ++i) {
    colorArray[4*i] = val[i].r;
    colorArray[4*i+1] = val[i].g;
    colorArray[4*i+2] = val[i].b;
    colorArray[4*i+3] = val[i].a;
  }
  _ColorVectorProperty_setNodeValue(this.cppPointer, node.id, colorArray.byteOffset, val.length);
  freeArrayInEmHeap(colorArray);
};
tulip.ColorVectorProperty.prototype.getEdgeDefaultValue = function tulip_ColorVectorProperty_prototype_getEdgeDefaultValue() {
  checkWrappedCppPointer(this.cppPointer);
  var size = _ColorVectorProperty_getEdgeDefaultVectorSize(this.cppPointer);
  var colorArray = allocArrayInEmHeap(Uint8Array, size*4);
  _ColorVectorProperty_getEdgeDefaultValue(this.cppPointer, colorArray.byteOffset);
  var ret = [];
  for (var i = 0 ; i < size ; ++i) {
    ret.push(tulip.Color(colorArray[4*i],colorArray[4*i+1],colorArray[4*i+2],colorArray[4*i+3]));
  }
  freeArrayInEmHeap(colorArray);
  return ret;
};
tulip.ColorVectorProperty.prototype.getEdgeValue = function tulip_ColorVectorProperty_prototype_getEdgeValue(edge) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge], 1);
  var size = _ColorVectorProperty_getEdgeVectorSize(this.cppPointer, edge.id);
  var colorArray = allocArrayInEmHeap(Uint8Array, size*4);
  _ColorVectorProperty_getEdgeValue(this.cppPointer, edge.id, colorArray.byteOffset);
  var ret = [];
  for (var i = 0 ; i < size ; ++i) {
    ret.push(tulip.Color(colorArray[4*i],colorArray[4*i+1],colorArray[4*i+2],colorArray[4*i+3]));
  }
  freeArrayInEmHeap(colorArray);
  return ret;
};
tulip.ColorVectorProperty.prototype.setEdgeValue = function tulip_ColorVectorProperty_prototype_setEdgeValue(edge, val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge, "array"], 2);
  checkArrayOfType(val, tulip.Color, 1);
  var colorArray = allocArrayInEmHeap(Uint8Array, val.length*4);
  for (var i = 0 ; i < val.length ; ++i) {
    colorArray[4*i] = val[i].r;
    colorArray[4*i+1] = val[i].g;
    colorArray[4*i+2] = val[i].b;
    colorArray[4*i+3] = val[i].a;
  }
  _ColorVectorProperty_setEdgeValue(this.cppPointer, edge.id, colorArray.byteOffset, val.length);
  freeArrayInEmHeap(colorArray);
};
tulip.ColorVectorProperty.prototype.setAllNodeValue = function tulip_ColorVectorProperty_prototype_setAllNodeValue(val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["array"], 1);
  checkArrayOfType(val, tulip.Color, 0);
  var colorArray = allocArrayInEmHeap(Uint8Array, val.length*4);
  for (var i = 0 ; i < val.length ; ++i) {
    colorArray[4*i] = val[i].r;
    colorArray[4*i+1] = val[i].g;
    colorArray[4*i+2] = val[i].b;
    colorArray[4*i+3] = val[i].a;
  }
  _ColorVectorProperty_setAllNodeValue(this.cppPointer, colorArray.byteOffset, val.length);
  freeArrayInEmHeap(colorArray);
};
tulip.ColorVectorProperty.prototype.setAllEdgeValue = function tulip_ColorVectorProperty_prototype_setAllEdgeValue(val) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["array"], 1);
  checkArrayOfType(val, "color", 0);
  var colorArray = allocArrayInEmHeap(Uint8Array, val.length*4);
  for (var i = 0 ; i < val.length ; ++i) {
    colorArray[4*i] = val[i].r;
    colorArray[4*i+1] = val[i].g;
    colorArray[4*i+2] = val[i].b;
    colorArray[4*i+3] = val[i].a;
  }
  _ColorVectorProperty_setAllEdgeValue(this.cppPointer, colorArray.byteOffset, val.length);
  freeArrayInEmHeap(colorArray);
};
// ==================================================================================================================

tulip.Vec3f = function tulip_Vec3f(x, y, z) {
  checkArgumentsTypes(arguments, ["number", "number", "number"])
  var newObject = createObject(tulip.Vec3f, this);
  newObject.x = 0;
  newObject.y = 0;
  newObject.z = 0;
  if (arguments.length == 1) {
    newObject.x = newObject.y = newObject.z = x;
  } else if (arguments.length == 2) {
    newObject.x = x;
    newObject.y = y;
  } else if (arguments.length == 3) {
    newObject.x = x;
    newObject.y = y;
    newObject.z = z;
  }
  return newObject;
};
tulip.Vec3f.prototype.add = function tulip_Vec3f_prototype_add() {
  var types = [];
  for (var i = 0 ; i < arguments.length ; ++i) {
    types.push(['number', tulip.Vec3f]);
  }
  checkArgumentsTypes(arguments, types, 1);
  for (var i = 0 ; i < arguments.length ; ++i) {
    if (typeOf(arguments[i]) == 'number') {
      this.x += arguments[i];
      this.y += arguments[i];
      this.z += arguments[i];
    } else {
      this.x += arguments[i].x;
      this.y += arguments[i].y;
      this.z += arguments[i].z;
    }
  }
  return this;
};
tulip.Vec3f.add = function tulip_Vec3f_add() {
  var types = [];
  types.push(tulip.Vec3f);
  for (var i = 1 ; i < arguments.length ; ++i) {
    types.push(['number', tulip.Vec3f]);
  }
  checkArgumentsTypes(arguments, types, 2);
  var p = arguments[0];
  for (var i = 1 ; i < arguments.length ; ++i) {
    if (typeOf(arguments[i]) == 'number') {
      p.x += arguments[i];
      p.y += arguments[i];
      p.z += arguments[i];
    } else {
      p.x += arguments[i].x;
      p.y += arguments[i].y;
      p.z += arguments[i].z;
    }
  }
  return p;
};
tulip.Vec3f.prototype.sub = function tulip_Vec3f_prototype_sub() {
  var types = [];
  for (var i = 0 ; i < arguments.length ; ++i) {
    types.push(['number', tulip.Vec3f]);
  }
  checkArgumentsTypes(arguments, types, 1);
  for (var i = 0 ; i < arguments.length ; ++i) {
    this.x -= arguments[i].x;
    this.y -= arguments[i].y;
    this.z -= arguments[i].z;
  }
  return this;
};
tulip.Vec3f.sub = function tulip_Vec3f_sub() {
  var types = [];
  types.push(tulip.Vec3f);
  for (var i = 1 ; i < arguments.length ; ++i) {
    types.push(['number', tulip.Vec3f]);
  }
  checkArgumentsTypes(arguments, types, 2);
  var p = arguments[0];
  for (var i = 1 ; i < arguments.length ; ++i) {
    if (typeOf(arguments[i]) == 'number') {
      p.x -= arguments[i];
      p.y -= arguments[i];
      p.z -= arguments[i];
    } else {
      p.x -= arguments[i].x;
      p.y -= arguments[i].y;
      p.z -= arguments[i].z;
    }
  }
  return p;
};
tulip.Vec3f.prototype.mul = function tulip_Vec3f_prototype_mul() {
  var types = [];
  for (var i = 0 ; i < arguments.length ; ++i) {
    types.push(['number', tulip.Vec3f]);
  }
  checkArgumentsTypes(arguments, types, 1);
  for (var i = 0 ; i < arguments.length ; ++i) {
    if (typeOf(arguments[i]) == 'number') {
      this.x *= arguments[i];
      this.y *= arguments[i];
      this.z *= arguments[i];
    } else {
      this.x *= arguments[i].x;
      this.y *= arguments[i].y;
      this.z *= arguments[i].z;
    }
  }
  return this;
};
tulip.Vec3f.mul = function tulip_Vec3f_mul() {
  var types = [];
  types.push(tulip.Vec3f);
  for (var i = 1 ; i < arguments.length ; ++i) {
    types.push(['number', tulip.Vec3f]);
  }
  checkArgumentsTypes(arguments, types, 2);
  var p = arguments[0];
  for (var i = 1 ; i < arguments.length ; ++i) {
    if (typeOf(arguments[i]) == 'number') {
      p.x *= arguments[i];
      p.y *= arguments[i];
      p.z *= arguments[i];
    } else {
      p.x *= arguments[i].x;
      p.y *= arguments[i].y;
      p.z *= arguments[i].z;
    }
  }
  return p;
};
tulip.Vec3f.prototype.div = function tulip_Vec3f_prototype_div() {
  var types = [];
  for (var i = 0 ; i < arguments.length ; ++i) {
    types.push(['number', tulip.Vec3f]);
  }
  checkArgumentsTypes(arguments, types, 1);
  for (var i = 0 ; i < arguments.length ; ++i) {
    if (typeOf(arguments[i]) == 'number') {
      this.x /= arguments[i];
      this.y /= arguments[i];
      this.z /= arguments[i];
    } else {
      this.x /= arguments[i].x;
      this.y /= arguments[i].y;
      this.z /= arguments[i].z;
    }
  }
  return this;
};
tulip.Vec3f.div = function tulip_Vec3f_div() {
  var types = [];
  types.push(tulip.Vec3f);
  for (var i = 1 ; i < arguments.length ; ++i) {
    types.push(['number', tulip.Vec3f]);
  }
  checkArgumentsTypes(arguments, types, 2);
  var p = arguments[0];
  for (var i = 1 ; i < arguments.length ; ++i) {
    p.x *= arguments[i].x;
    p.y *= arguments[i].y;
    p.z *= arguments[i].z;
  }
  return p;
};
tulip.Vec3f.prototype.norm = function tulip_Vec3f_prototype_norm() {
  return Math.sqrt(this.x*this.x+this.y*this.y+this.z*this.z);
};
tulip.Vec3f.prototype.normalize = function tulip_Vec3f_prototype_normalize() {
  var n = this.norm();
  if (n != 0) {
    this.div(n);
  }
};
tulip.Vec3f.prototype.dist = function tulip_Vec3f_prototype_dist(c) {
  checkArgumentsTypes(arguments, [tulip.Vec3f]);
  return tulip.Vec3f.dist(this, c);
};
tulip.Vec3f.dist = function tulip_Vec3f_dist(c1, c2) {
  checkArgumentsTypes(arguments, [tulip.Vec3f, tulip.Vec3f]);
  return tulip.Vec3f.sub(c1, c2).norm();
};
tulip.Vec3f.min = function tulip_Vec3f_min() {
  var types = [];
  for (var i = 0 ; i < arguments.length ; ++i) {
    types.push(tulip.Vec3f);
  }
  checkArgumentsTypes(arguments, types, 2);
  var ret = arguments[0];
  for (var i = 1 ; i < arguments.length ; ++i) {
    ret.x = Math.min(ret.x, arguments[i].x);
    ret.y = Math.min(ret.y, arguments[i].y);
    ret.z = Math.min(ret.z, arguments[i].z);
  }
  return ret;
};
tulip.Vec3f.max = function tulip_Vec3f_max() {
  var types = [];
  for (var i = 0 ; i < arguments.length ; ++i) {
    types.push(tulip.Vec3f);
  }
  checkArgumentsTypes(arguments, types, 2);
  var ret = arguments[0];
  for (var i = 1 ; i < arguments.length ; ++i) {
    ret.x = Math.max(ret.x, arguments[i].x);
    ret.y = Math.max(ret.y, arguments[i].y);
    ret.z = Math.max(ret.z, arguments[i].z);
  }
  return ret;
};
tulip.Vec3f.lt = function tulip_Vec3f_lt(v1, v2) {
  checkArgumentsTypes(arguments, [tulip.Vec3f, tulip.Vec3f], 2);
  return v1.x < v2.x && v1.y < v2.y && v1.z < v2.z;
};
tulip.Vec3f.prototype.lt = function tulip_Vec3f_prototype_lt(v) {
  checkArgumentsTypes(arguments, [tulip.Vec3f], 1);
  return tulip.Vec3f.lt(this, v);
};
tulip.Vec3f.leq = function tulip_Vec3f_leq(v1, v2) {
  checkArgumentsTypes(arguments, [tulip.Vec3f, tulip.Vec3f], 2);
  return v1.x <= v2.x && v1.y <= v2.y && v1.z <= v2.z;
};
tulip.Vec3f.prototype.leq = function tulip_Vec3f_prototype_leq(v) {
  checkArgumentsTypes(arguments, [tulip.Vec3f], 1);
  return tulip.Vec3f.leq(this, v);
};
tulip.Vec3f.gt = function tulip_Vec3f_gt(v1, v2) {
  checkArgumentsTypes(arguments, [tulip.Vec3f, tulip.Vec3f], 2);
  return v1.x > v2.x && v1.y > v2.y && v1.z > v2.z;
};
tulip.Vec3f.prototype.gt = function tulip_Vec3f_prototype_gt(v) {
  checkArgumentsTypes(arguments, [tulip.Vec3f], 1);
  return tulip.Vec3f.gt(this, v);
};
tulip.Vec3f.geq = function tulip_Vec3f_geq(v1, v2) {
  checkArgumentsTypes(arguments, [tulip.Vec3f, tulip.Vec3f], 2);
  return v1.x >= v2.x && v1.y >= v2.y && v1.z >= v2.z;
};
tulip.Vec3f.prototype.geq = function tulip_Vec3f_prototype_geq(v) {
  checkArgumentsTypes(arguments, [tulip.Vec3f], 1);
  return tulip.Vec3f.geq(this, v);
};
tulip.Vec3f.dot = function tulip_Vec3f_dot(v1, v2) {
  checkArgumentsTypes(arguments, [tulip.Vec3f], 2);
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
};
tulip.Vec3f.prototype.dot = function tulip_Vec3f_prototype_dot(v) {
  checkArgumentsTypes(arguments, [tulip.Vec3f], 1);
  return tulip.Vec3f.dot(this, v);
};
tulip.Vec3f.cross = function tulip_Vec3f_cross(v1, v2) {
  checkArgumentsTypes(arguments, [tulip.Vec3f], 2);
  var x = v1.y * v2.z - v1.z * v2.y;
  var y = v1.z * v2.x - v1.x * v2.z;
  var z = v1.x * v2.y - v1.y * v2.x;
  return tulip.Vec3f(x, y, z);
};
tulip.Vec3f.prototype.cross = function tulip_Vec3f_prototype_cross(v) {
  checkArgumentsTypes(arguments, [tulip.Vec3f], 1);
  return tulip.Vec3f.cross(this, v);
};
tulip.Vec3f.prototype.getX = function tulip_Vec3f_prototype_getX() {
  return this.x;
};
tulip.Vec3f.prototype.getY = function tulip_Vec3f_prototype_getY() {
  return this.y;
};
tulip.Vec3f.prototype.getZ = function tulip_Vec3f_prototype_getZ() {
  return this.z;
};
tulip.Vec3f.prototype.setX = function tulip_Vec3f_prototype_setX(x) {
  checkArgumentsTypes(arguments, ["number"], 1);
  this.x = x;
};
tulip.Vec3f.prototype.setY = function tulip_Vec3f_prototype_setY(y) {
  checkArgumentsTypes(arguments, ["number"], 1);
  this.y = y;
};
tulip.Vec3f.prototype.setZ = function tulip_Vec3f_prototype_setZ(z) {
  checkArgumentsTypes(arguments, ["number"], 1);
  this.z = z;
};
tulip.Vec3f.prototype.getWidth = function tulip_Vec3f_prototype_getWidth() {
  return this.x;
};
tulip.Vec3f.prototype.getHeight = function tulip_Vec3f_prototype_getHeight() {
  return this.y;
};
tulip.Vec3f.prototype.getDepth = function tulip_Vec3f_prototype_getDepth() {
  return this.z;
};
tulip.Vec3f.prototype.setWidth = function tulip_Vec3f_prototype_setWidth(w) {
  checkArgumentsTypes(arguments, ["number"], 1);
  this.x = w;
};
tulip.Vec3f.prototype.setHeight = function tulip_Vec3f_prototype_setHeight(h) {
  checkArgumentsTypes(arguments, ["number"], 1);
  this.y = h;
};
tulip.Vec3f.prototype.setDepth = function tulip_Vec3f_prototype_setDepth(d) {
  checkArgumentsTypes(arguments, ["number"], 1);
  this.z = d;
};


tulip.Coord = tulip.Vec3f;
tulip.Size = tulip.Vec3f;

// ==================================================================================================================

var _createLayoutProperty = Module.cwrap('createLayoutProperty', 'number', ['number', 'string']);
var _LayoutProperty_setNodeValue = Module.cwrap('LayoutProperty_setNodeValue', null, ['number', 'number', 'number', 'number', 'number']);
var _LayoutProperty_setAllNodeValue = Module.cwrap('LayoutProperty_setAllNodeValue', null, ['number', 'number', 'number', 'number']);
var _LayoutProperty_getNodeDefaultValue = Module.cwrap('LayoutProperty_getNodeDefaultValue', null, ['number', 'number']);
var _LayoutProperty_getNodeValue = Module.cwrap('LayoutProperty_getNodeValue', null, ['number', 'number', 'number']);
var _LayoutProperty_getEdgeDefaultNumberOfBends = Module.cwrap('LayoutProperty_getEdgeDefaultNumberOfBends', 'number', ['number']);
var _LayoutProperty_getEdgeDefaultValue = Module.cwrap('LayoutProperty_getEdgeDefaultValue', null, ['number', 'number']);
var _LayoutProperty_getEdgeNumberOfBends = Module.cwrap('LayoutProperty_getEdgeNumberOfBends', 'number', ['number', 'number']);
var _LayoutProperty_getEdgeValue = Module.cwrap('LayoutProperty_getEdgeValue', null, ['number', 'number', 'number']);
var _LayoutProperty_setEdgeValue = Module.cwrap('LayoutProperty_setEdgeValue', null, ['number', 'number', 'number', 'number']);
var _LayoutProperty_setAllEdgeValue = Module.cwrap('LayoutProperty_setAllEdgeValue', null, ['number', 'number', 'number']);
var _LayoutProperty_getMin = Module.cwrap('LayoutProperty_getMin', null, ['number', 'number', 'number']);
var _LayoutProperty_getMax = Module.cwrap('LayoutProperty_getMax', null, ['number', 'number', 'number']);
var _LayoutProperty_translate = Module.cwrap('LayoutProperty_translate', null, ['number', 'number', 'number', 'number', 'number']);
var _LayoutProperty_scale = Module.cwrap('LayoutProperty_scale', null, ['number', 'number', 'number', 'number', 'number']);
var _LayoutProperty_rotateX = Module.cwrap('LayoutProperty_rotateX', null, ['number', 'number', 'number']);
var _LayoutProperty_rotateY = Module.cwrap('LayoutProperty_rotateY', null, ['number', 'number', 'number']);
var _LayoutProperty_rotateZ = Module.cwrap('LayoutProperty_rotateZ', null, ['number', 'number', 'number']);
var _LayoutProperty_center = Module.cwrap('LayoutProperty_center', null, ['number', 'number']);
var _LayoutProperty_center2 = Module.cwrap('LayoutProperty_center2', null, ['number', 'number', 'number', 'number', 'number']);
var _LayoutProperty_normalize = Module.cwrap('LayoutProperty_normalize', null, ['number', 'number']);
var _LayoutProperty_perfectAspectRatio = Module.cwrap('LayoutProperty_perfectAspectRatio', null, ['number']);

tulip.LayoutProperty = function tulip_LayoutProperty() {
  var newObject = createObject(tulip.LayoutProperty, this);
  if (tulip_LayoutProperty.caller == null || tulip_LayoutProperty.caller.name != "createObject") {
    var cppPointer = 0;
    var graphManaged = false;
    if (arguments.length == 1 && typeOf(arguments[0]) == "number") {
      cppPointer = arguments[0];
      graphManaged = true;
    } else {
      checkArgumentsTypes(arguments, [tulip.Graph, "string"], 1);
      var propName = "";
      if (arguments.length > 1) propName = arguments[1];
      cppPointer = _createLayoutProperty(arguments[0].cppPointer, propName);
    }
    tulip.PropertyInterface.call(newObject, cppPointer, graphManaged);
    newObject.setWrappedTypename("tlp::LayoutProperty");
  }
  return createPropertyProxyIfAvailable(newObject);
};
tulip.LayoutProperty.inheritsFrom(tulip.PropertyInterface);
tulip.LayoutProperty.prototype.getNodeDefaultValue = function tulip_LayoutProperty_prototype_getNodeDefaultValue() {
  checkWrappedCppPointer(this.cppPointer);
  var floatArray = allocArrayInEmHeap(Float32Array, 3);
  _LayoutProperty_getNodeDefaultValue(this.cppPointer, floatArray.byteOffset);
  var ret = tulip.Coord(floatArray[0], floatArray[1], floatArray[2]);
  freeArrayInEmHeap(floatArray);
  return ret;
};
tulip.LayoutProperty.prototype.getNodeValue = function tulip_LayoutProperty_prototype_getNodeValue(node) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node], 1);
  var floatArray = allocArrayInEmHeap(Float32Array, 3);
  _LayoutProperty_getNodeValue(this.cppPointer, node.id, floatArray.byteOffset);
  var ret = tulip.Coord(floatArray[0], floatArray[1], floatArray[2]);
  freeArrayInEmHeap(floatArray);
  return ret;
};
tulip.LayoutProperty.prototype.setNodeValue = function tulip_LayoutProperty_prototype_setNodeValue(node, coord) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node, tulip.Coord], 2);
  _LayoutProperty_setNodeValue(this.cppPointer, node.id, coord.x, coord.y, coord.z);
};
tulip.LayoutProperty.prototype.setAllNodeValue = function tulip_LayoutProperty_prototype_setAllNodeValue(coord) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Coord], 1);
  _LayoutProperty_setAllNodeValue(this.cppPointer, coord.x, coord.y, coord.z);
};
tulip.LayoutProperty.prototype.getEdgeDefaultValue = function tulip_LayoutProperty_prototype_getEdgeDefaultValue() {
  checkWrappedCppPointer(this.cppPointer);
  var nbBends = _LayoutProperty_getEdgeDefaultNumberOfBends(this.cppPointer);
  var floatArray = allocArrayInEmHeap(Float32Array, nbBends*3);
  _LayoutProperty_getEdgeDefaultValue(this.cppPointer, floatArray.byteOffset);
  var ret = [];
  for (var i = 0 ; i < nbBends ; ++i) {
    ret.push(tulip.Coord(floatArray[3*i], floatArray[3*i+1], floatArray[3*i+2]));
  }
  freeArrayInEmHeap(floatArray);
  return ret;
};

tulip.LayoutProperty.prototype.getEdgeValue = function tulip_LayoutProperty_prototype_getEdgeValue(edge) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge], 1);
  var nbBends = _LayoutProperty_getEdgeNumberOfBends(this.cppPointer, edge.id);
  var floatArray = allocArrayInEmHeap(Float32Array, nbBends*3);
  _LayoutProperty_getEdgeValue(this.cppPointer, edge.id, floatArray.byteOffset);
  var ret = [];
  for (var i = 0 ; i < nbBends ; ++i) {
    ret.push(tulip.Coord(floatArray[3*i], floatArray[3*i+1], floatArray[3*i+2]));
  }
  freeArrayInEmHeap(floatArray);
  return ret;
};
tulip.LayoutProperty.prototype.setEdgeValue = function tulip_LayoutProperty_prototype_setEdgeValue(edge, bends) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge, "array"]);
  checkArrayOfType(bends, tulip.Coord, 1);
  if (bends.length == 0) return;
  var floatArray = allocArrayInEmHeap(Float32Array, bends.length*3);
  for (var i = 0 ; i < bends.length ; ++i) {
    floatArray[3*i] = bends[i].x;
    floatArray[3*i+1] = bends[i].y;
    floatArray[3*i+2] = bends[i].z;
  }
  _LayoutProperty_setEdgeValue(this.cppPointer, edge.id, floatArray.byteOffset, bends.length);
  freeArrayInEmHeap(floatArray);
};
tulip.LayoutProperty.prototype.setAllEdgeValue = function tulip_LayoutProperty_prototype_setAllEdgeValue(bends) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["array"]);
  if (bends.length == 0) return;
  checkArrayOfType(bends, tulip.Coord, 0);
  var floatArray = allocArrayInEmHeap(Float32Array, bends.length*3);
  for (var i = 0 ; i < bends.length ; ++i) {
    floatArray[3*i] = bends[i].x;
    floatArray[3*i+1] = bends[i].y;
    floatArray[3*i+2] = bends[i].z;
  }
  _LayoutProperty_setAllEdgeValue(this.cppPointer, floatArray.byteOffset, bends.length);
  freeArrayInEmHeap(floatArray);
};
tulip.LayoutProperty.prototype.getMin = function tulip_LayoutProperty_prototype_getMin(subgraph) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Graph]);
  var sgPointer = 0;
  if (arguments.length == 1) {
    sgPointer = subgraph.cppPointer;
  }
  var floatArray = allocArrayInEmHeap(Float32Array, 3);
  _LayoutProperty_getMin(this.cppPointer, sgPointer, floatArray.byteOffset);
  var ret = tulip.Coord(floatArray[0], floatArray[1], floatArray[2]);
  freeArrayInEmHeap(floatArray);
  return ret;
};
tulip.LayoutProperty.prototype.getMax = function tulip_LayoutProperty_prototype_getMax(subgraph) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Graph]);
  var sgPointer = 0;
  if (arguments.length == 1) {
    sgPointer = subgraph.cppPointer;
  }
  var floatArray = allocArrayInEmHeap(Float32Array, 3);
  _LayoutProperty_getMax(this.cppPointer, sgPointer, floatArray.byteOffset);
  var ret = tulip.Coord(floatArray[0], floatArray[1], floatArray[2]);
  freeArrayInEmHeap(floatArray);
  return ret;
};
tulip.LayoutProperty.prototype.translate = function tulip_LayoutProperty_prototype_translate(move, subgraph) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Coord, tulip.Graph], 1);
  var sgPointer = 0;
  if (arguments.length == 2) {
    sgPointer = subgraph.cppPointer;
  }
  _LayoutProperty_translate(this.cppPointer, move.x, move.y, move.z, sgPointer);
};
tulip.LayoutProperty.prototype.scale = function tulip_LayoutProperty_prototype_scale(scaleFactors, subgraph) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Coord, tulip.Graph], 1);
  var sgPointer = 0;
  if (arguments.length == 2) {
    sgPointer = subgraph.cppPointer;
  }
  _LayoutProperty_scale(this.cppPointer, scaleFactors.x, scaleFactors.y, scaleFactors.z, sgPointer);
};
tulip.LayoutProperty.prototype.rotateX = function tulip_LayoutProperty_prototype_rotateX(alpha, subgraph) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["number", tulip.Graph], 1);
  var sgPointer = 0;
  if (arguments.length == 2) {
    sgPointer = subgraph.cppPointer;
  }
  _LayoutProperty_rotateX(this.cppPointer, alpha, sgPointer);
};
tulip.LayoutProperty.prototype.rotateY = function tulip_LayoutProperty_prototype_rotateY(alpha, subgraph) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["number", tulip.Graph], 1);
  var sgPointer = 0;
  if (arguments.length == 2) {
    sgPointer = subgraph.cppPointer;
  }
  _LayoutProperty_rotateY(this.cppPointer, alpha, sgPointer);
};
tulip.LayoutProperty.prototype.rotateZ = function tulip_LayoutProperty_prototype_rotateZ(alpha, subgraph) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["number", tulip.Graph], 1);
  var sgPointer = 0;
  if (arguments.length == 2) {
    sgPointer = subgraph.cppPointer;
  }
  _LayoutProperty_rotateZ(this.cppPointer, alpha, sgPointer);
};
tulip.LayoutProperty.prototype.center = function tulip_LayoutProperty_prototype_center() {
  checkWrappedCppPointer(this.cppPointer);
  if (arguments.length == 0) {
    _LayoutProperty_center(this.cppPointer, 0);
  } else if (arguments.length == 1) {
    checkArgumentsTypes(arguments, [[tulip.Coord, tulip.Graph]]);
    if (arguments[0] instanceof tulip.Coord) {
      _LayoutProperty_center2(this.cppPointer, arguments[0].x, arguments[0].y, arguments[0].z, 0);
    } else {
      _LayoutProperty_center(this.cppPointer, arguments[0].cppPointer);
    }
  } else {
    checkArgumentsTypes(arguments, [tulip.Coord, tulip.Graph], 2);
    _LayoutProperty_center2(this.cppPointer, arguments[0].x, arguments[0].y, arguments[0].z, arguments[1].cppPointer);
  }
};
tulip.LayoutProperty.prototype.normalize = function tulip_LayoutProperty_prototype_normalize(subgraph) {
  checkWrappedCppPointer(this.cppPointer);
  var sgPointer = 0;
  checkArgumentsTypes(arguments, [tulip.Graph]);
  if (arguments.length == 1) {
    sgPointer = subgraph.cppPointer;
  }
  _LayoutProperty_normalize(this.cppPointer, sgPointer);
};
tulip.LayoutProperty.prototype.perfectAspectRatio = function tulip_LayoutProperty_prototype_perfectAspectRatio() {
  checkWrappedCppPointer(this.cppPointer);
  _LayoutProperty_perfectAspectRatio(this.cppPointer);
};
// ==================================================================================================================

var _createSizeProperty = Module.cwrap('createSizeProperty', 'number', ['number', 'string']);
var _SizeProperty_setNodeValue = Module.cwrap('SizeProperty_setNodeValue', null, ['number', 'number', 'number', 'number', 'number']);
var _SizeProperty_setAllNodeValue = Module.cwrap('SizeProperty_setAllNodeValue', null, ['number', 'number', 'number', 'number']);
var _SizeProperty_getNodeDefaultValue = Module.cwrap('SizeProperty_getNodeDefaultValue', null, ['number', 'number']);
var _SizeProperty_getNodeValue = Module.cwrap('SizeProperty_getNodeValue', null, ['number', 'number', 'number']);
var _SizeProperty_getEdgeDefaultValue = Module.cwrap('SizeProperty_getEdgeDefaultValue', null, ['number', 'number']);
var _SizeProperty_getEdgeValue = Module.cwrap('SizeProperty_getEdgeValue', null, ['number', 'number', 'number']);
var _SizeProperty_setEdgeValue = Module.cwrap('SizeProperty_setAllEdgeValue', null, ['number', 'number', 'number', 'number', 'number']);
var _SizeProperty_setAllEdgeValue = Module.cwrap('SizeProperty_setAllEdgeValue', null, ['number', 'number', 'number', 'number']);
var _SizeProperty_scale = Module.cwrap('SizeProperty_scale', null, ['number', 'number', 'number', 'number', 'number']);
var _SizeProperty_getMin = Module.cwrap('SizeProperty_getMin', null, ['number', 'number', 'number']);
var _SizeProperty_getMax = Module.cwrap('SizeProperty_getMax', null, ['number', 'number', 'number']);

tulip.SizeProperty = function tulip_SizeProperty() {
  var newObject = createObject(tulip.SizeProperty, this);
  if (tulip_SizeProperty.caller == null || tulip_SizeProperty.caller.name != "createObject") {
    var cppPointer = 0;
    var graphManaged = false;
    if (arguments.length == 1 && typeOf(arguments[0]) == "number") {
      cppPointer = arguments[0];
      graphManaged = true;
    } else {
      checkArgumentsTypes(arguments, [tulip.Graph, "string"], 1);
      var propName = "";
      if (arguments.length > 1) propName = arguments[1];
      cppPointer = _createSizeProperty(arguments[0].cppPointer, propName);
    }
    tulip.PropertyInterface.call(newObject, cppPointer, graphManaged);
    newObject.setWrappedTypename("tlp::SizeProperty");
  }
  return createPropertyProxyIfAvailable(newObject);
};
tulip.SizeProperty.inheritsFrom(tulip.PropertyInterface);
tulip.SizeProperty.prototype.getNodeDefaultValue = function tulip_SizeProperty_prototype_getNodeDefaultValue() {
  checkWrappedCppPointer(this.cppPointer);
  var floatArray = allocArrayInEmHeap(Float32Array, 3);
  _SizeProperty_getNodeDefaultValue(this.cppPointer, floatArray.byteOffset);
  var ret = tulip.Size(floatArray[0], floatArray[1], floatArray[2]);
  freeArrayInEmHeap(floatArray);
  return ret;
};
tulip.SizeProperty.prototype.getNodeValue = function tulip_SizeProperty_prototype_getNodeValue(node) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node], 1);
  var floatArray = allocArrayInEmHeap(Float32Array, 3);
  _SizeProperty_getNodeValue(this.cppPointer, node.id, floatArray.byteOffset);
  var ret = tulip.Size(floatArray[0], floatArray[1], floatArray[2]);
  freeArrayInEmHeap(floatArray);
  return ret;
};
tulip.SizeProperty.prototype.setNodeValue = function tulip_SizeProperty_prototype_setNodeValue(node, size) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node, tulip.Size]);
  _SizeProperty_setNodeValue(this.cppPointer, node.id, size.getWidth(), size.getHeight(), size.getDepth());
};
tulip.SizeProperty.prototype.setAllNodeValue = function tulip_SizeProperty_prototype_setAllNodeValue(size) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Size]);
  _SizeProperty_setAllNodeValue(this.cppPointer, size.getWidth(), size.getHeight(), size.getDepth());
};
tulip.SizeProperty.prototype.getEdgeDefaultValue = function tulip_SizeProperty_prototype_getEdgeDefaultValue() {
  checkWrappedCppPointer(this.cppPointer);
  var floatArray = allocArrayInEmHeap(Float32Array, 3);
  _SizeProperty_getEdgeDefaultValue(this.cppPointer, floatArray.byteOffset);
  var ret = tulip.Size(floatArray[0], floatArray[1], floatArray[2]);
  freeArrayInEmHeap(floatArray);
  return ret;
};
tulip.SizeProperty.prototype.getEdgeValue = function tulip_SizeProperty_prototype_getEdgeValue(edge) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge], 1);
  var floatArray = allocArrayInEmHeap(Float32Array, 3);
  _SizeProperty_getEdgeValue(this.cppPointer, edge.id, floatArray.byteOffset);
  var ret = tulip.Size(floatArray[0], floatArray[1], floatArray[2]);
  freeArrayInEmHeap(floatArray);
  return ret;
};
tulip.SizeProperty.prototype.setEdgeValue = function tulip_SizeProperty_prototype_setEdgeValue(edge, size) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge, tulip.Size]);
  _SizeProperty_setEdgeValue(this.cppPointer, edge.id, size.getWidth(), size.getHeight(), size.getDepth());
};
tulip.SizeProperty.prototype.setAllEdgeValue = function tulip_SizeProperty_prototype_setAllEdgeValue(size) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Size]);
  _SizeProperty_setAllEdgeValue(this.cppPointer, size.getWidth(), size.getHeight(), size.getDepth());
};
tulip.SizeProperty.prototype.scale = function tulip_SizeProperty_prototype_scale(sizeFactor, subgraph) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Size, tulip.Graph], 1);
  var sgPointer = 0;
  if (arguments.length > 1) sgPointer = subgraph.cppPointer;
  _SizeProperty_scale(this.cppPointer, sizeFactor.getWidth(), sizeFactor.getHeight(), sizeFactor.getDepth(), sgPointer);
};
tulip.SizeProperty.prototype.getMin = function tulip_SizeProperty_prototype_getMin(subgraph) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Graph]);
  var sgPointer= 0;
  if (arguments.length == 1) {
    sgPointer = subgraph.cppPointer;
  }
  var floatArray = allocArrayInEmHeap(Float32Array, 3);
  _SizeProperty_getMin(this.cppPointer, sgPointer, floatArray.byteOffset);
  var ret = tulip.Size(floatArray[0], floatArray[1], floatArray[2]);
  freeArrayInEmHeap(floatArray);
  return ret;
};
tulip.SizeProperty.prototype.getMax = function tulip_SizeProperty_prototype_getMax(subgraph) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Graph]);
  var sgPointer= 0;
  if (arguments.length == 1) {
    sgPointer = subgraph.cppPointer;
  }
  var floatArray = allocArrayInEmHeap(Float32Array, 3);
  _SizeProperty_getMax(this.cppPointer, sgPointer, floatArray.byteOffset);
  var ret = tulip.Size(floatArray[0], floatArray[1], floatArray[2]);
  freeArrayInEmHeap(floatArray);
  return ret;
};

// ==================================================================================================================

tulip.BoundingBox = function tulip_BoundingBox() {
  var newObject = createObject(tulip.BoundingBox, this);
  if (arguments.length == 0) {
    newObject.min = tulip.Coord(1,1,1);
    newObject.max = tulip.Coord(-1,-1,-1);
  } else {
    checkArgumentsTypes(arguments, [tulip.Coord, tulip.Coord], 2);
    newObject.min = arguments[0];
    newObject.max = arguments[1];
  }
  return newObject;
};
tulip.BoundingBox.prototype.isValid = function tulip_BoundingBox_prototype_isValid() {
  return this.min.x <= this.max.x && this.min.y <= this.max.y && this.min.z <= this.min.z;
};
tulip.BoundingBox.prototype.center = function tulip_BoundingBox_prototype_center() {
  return tulip.Coord.add(this.min, this.max).div(2);
};
tulip.BoundingBox.prototype.width = function tulip_BoundingBox_prototype_width() {
  return this.max.x - this.min.x;
};
tulip.BoundingBox.prototype.height = function tulip_BoundingBox_prototype_height() {
  return this.max.y - this.min.y;
};
tulip.BoundingBox.prototype.depth = function tulip_BoundingBox_prototype_depth() {
  return this.max.z - this.min.z;
};
tulip.BoundingBox.prototype.expand = function tulip_BoundingBox_prototype_depth(coord) {
  checkArgumentsTypes(arguments, [tulip.Coord], 1);
  if (!this.isValid()) {
    this.min = arguments[0];
    this.max = arguments[0];
  }
  this.min = tulip.Coord.min.apply(null, [this.min].concat(Array.slice(arguments)));
  this.max = tulip.Coord.max.apply(null, [this.max].concat(Array.slice(arguments)));
};
tulip.BoundingBox.prototype.translate = function tulip_BoundingBox_prototype_translate(vec) {
  checkArgumentsTypes(arguments, [tulip.Coord], 1);
  this.min.add(vec);
  this.max.add(vec);
};
tulip.BoundingBox.prototype.scale = function tulip_BoundingBox_prototype_scale(vec) {
  checkArgumentsTypes(arguments, [tulip.Coord], 1);
  this.min.mul(vec);
  this.max.mul(vec);
};
tulip.BoundingBox.prototype.contains = function tulip_BoundingBox_prototype_contains(obj) {
  checkArgumentsTypes(arguments, [[tulip.Coord, tulip.BoundingBox]], 1);
  if (obj instanceof tulip.Coord) {
    if (this.isValid()) {
      return coord.x >= this.min.x && coord.y >= this.min.y && coord.z >= this.min.z && coord.x <= this.max.x && coord.y <= this.max.y && coord.z <= this.max.z;
    } else {
      return false;
    }
  } else {
    if (this.isValid() && obj.isValid()) {
      return this.contains(obj.min) && this.contains(obj.max);
    } else {
      return false;
    }
  }
};
tulip.BoundingBox.prototype.intersect = function tulip_BoundingBox_prototype_intersect(bb) {
  checkArgumentsTypes(arguments, [tulip.BoundingBox], 1);
  if (!this.isValid() || !bb.isValid()) {
    return false;
  }
  if (this.max.x < bb.min.x) return false;
  if (bb.max.x < this.min.x) return false;
  if (this.max.y < bb.min.y) return false;
  if (bb.max.y < this.min.y) return false;
  if (this.max.z < bb.min.z) return false;
  if (bb.max.z < this.min.z) return false;
  return true;
};


// ==================================================================================================================
/**
* This is the description for the tulip.node class.
*
* @class Node
* @constructor
*/
tulip.Node = function tulip_Node(id) {
  checkArgumentsTypes(arguments, ["number"]);
  var newObject = createObject(tulip.Node, this);
  if (arguments.length == 0) {
    newObject.id = UINT_MAX;
  } else {
    newObject.id = id;
  }
  return newObject;
};
tulip.Node.prototype.isValid = function() {
  return this.id != UINT_MAX;
};
tulip.Node.prototype.toString = function() {
  return '[Node ' + this.id + ']';
};

// ==================================================================================================================
/**
* This is the description for the tulip.edge class.
*
* @class Edge
* @constructor
*/
tulip.Edge = function tulip_Edge(id) {
  checkArgumentsTypes(arguments, ["number"]);
  var newObject = createObject(tulip.Edge, this);
  if (arguments.length == 0) {
    newObject.id = UINT_MAX;
  } else {
    newObject.id = id;
  }
  return newObject;
};
tulip.Edge.prototype.isValid = function() {
  return this.id != UINT_MAX;
};
tulip.Edge.prototype.toString = function() {
  return '[Edge ' + this.id + ']';
};

// ==================================================================================================================

var _getJSONGraph = Module.cwrap('getJSONGraph', 'string', ['number']);
var _loadGraph = Module.cwrap('loadGraph', 'number', ['string', 'number']);
var _saveGraph = Module.cwrap('saveGraph', 'number', ['number', 'string', 'number']);
var _getDefaultAlgorithmParametersJSON = Module.cwrap('getDefaultAlgorithmParametersJSON', 'string', ['string', 'number']);





tulip.loadGraph = function tulip_loadGraph(filename, notifyProgress) {
  checkArgumentsTypes(arguments, ["string", "boolean"]);
  var graphPointer = _loadGraph(filename, notifyProgress);
  if (graphPointer) {
    return tulip.Graph(graphPointer);
  } else {
    return null;
  }
};

tulip.loadGraphAsync = function tulip_loadGraphAsync(graphFileUrl, graphLoadedCallback) {
  checkArgumentsTypes(arguments, ["string", "function"]);
  var graphReq = new XMLHttpRequest();
  graphReq.open("GET", graphFileUrl, true);
  graphReq.responseType = "arraybuffer";
  graphReq.onload = function (oEvent) {
    var arrayBuffer = graphReq.response;
    var paths = graphFileUrl.split('/');

    var file = FS.findObject(graphFileUrl);
    if (!file) {
      var filePath = "/";
      for (var i = 0; i < paths.length - 1; ++i) {
        filePath += paths[i];
        filePath += "/";
      }
      FS.createPath('/', filePath, true, true);
      FS.createFile('/', graphFileUrl, {}, true, true);
    }
    FS.writeFile(graphFileUrl, new Uint8Array(arrayBuffer), {'encoding' : 'binary'});
    var graph = tulip.loadGraph(graphFileUrl, false);
    FS.unlink(graphFileUrl);
    if (graphLoadedCallback) {
      graphLoadedCallback(graph);
    }
  };
  graphReq.send(null);
};

tulip.saveGraph = function tulip_saveGraph(graph, filename, notifyProgress) {
  checkArgumentsTypes(arguments, [tulip.Graph, "string"]);
  return _saveGraph(graph.cppPointer, filename, notifyProgress) > 0;
};

tulip.getDefaultAlgorithmParameters = function tulip_getDefaultAlgorithmParameters(algoName, graph) {
  checkArgumentsTypes(arguments, ["string", tulip.Graph], 1);
  var gPointer = 0;
  if (graph) {
    gPointer = graph.cppPointer;
  }
  if (!tulip.pluginExists(algoName)) {
    console.log("Error : no Tulip algorithm named '" + algoName + "'");
    return {};
  }
  var params = JSON.parse(_getDefaultAlgorithmParametersJSON(algoName, gPointer));
  for (var property in params) {
    if (params.hasOwnProperty(property)) {
      if (typeOf(params[property]) == "object") {
        if (params[property].type == "property") {
          params[property] = graph.getProperty(params[property].name);
        }
      }
    }
  }
  return params;
};

// ==================================================================================================================

var _algorithmExists = Module.cwrap('algorithmExists', 'number', ['string']);
var _numberOfAlgorithms = Module.cwrap('numberOfAlgorithms', 'number', []);
var _algorithmsNamesLengths = Module.cwrap('algorithmsNamesLengths', 'number', ['number']);
var _algorithmsList = Module.cwrap('algorithmsList', null, ['number']);
var _numberOfPlugins = Module.cwrap('numberOfPlugins', 'number', []);
var _pluginsNamesLengths = Module.cwrap('pluginsNamesLengths', 'number', ['number']);
var _pluginsList = Module.cwrap('pluginsList', null, ['number']);
var _propertyAlgorithmExists = Module.cwrap('propertyAlgorithmExists', 'number', ['string']);
var _booleanAlgorithmExists = Module.cwrap('booleanAlgorithmExists', 'number', ['string']);
var _colorAlgorithmExists = Module.cwrap('colorAlgorithmExists', 'number', ['string']);
var _doubleAlgorithmExists = Module.cwrap('doubleAlgorithmExists', 'number', ['string']);
var _integerAlgorithmExists = Module.cwrap('integerAlgorithmExists', 'number', ['string']);
var _layoutAlgorithmExists = Module.cwrap('layoutAlgorithmExists', 'number', ['string']);
var _stringAlgorithmExists = Module.cwrap('stringAlgorithmExists', 'number', ['string']);
var _sizeAlgorithmExists = Module.cwrap('sizeAlgorithmExists', 'number', ['string']);
var _importPluginExists = Module.cwrap('importPluginExists', 'number', ['string']);

tulip.pluginsList = function() {
  var nbPlugins = _numberOfPlugins();
  var uintArray = allocArrayInEmHeap(Uint32Array, nbPlugins);
  var nbBytes = _pluginsNamesLengths(uintArray.byteOffset);
  var ucharArray = allocArrayInEmHeap(Uint8Array, nbBytes);
  _pluginsList(ucharArray.byteOffset);
  var ret = bytesTypedArrayToStringArray(ucharArray, uintArray, nbPlugins);
  freeArrayInEmHeap(ucharArray);
  freeArrayInEmHeap(uintArray);
  return ret;
};

tulip.algorithmsList = function() {
  var nbAlgorithms = _numberOfAlgorithms();
  var uintArray = allocArrayInEmHeap(Uint32Array, nbAlgorithms);
  var nbBytes = _pluginsNamesLengths(uintArray.byteOffset);
  var ucharArray = allocArrayInEmHeap(Uint8Array, nbBytes);
  _algorithmsList(ucharArray.byteOffset);
  var ret = bytesTypedArrayToStringArray(ucharArray, uintArray, nbAlgorithms);
  freeArrayInEmHeap(ucharArray);
  freeArrayInEmHeap(uintArray);
  return ret;
};

tulip.pluginExists = function tulip_pluginExists(pluginName) {
  return tulip.pluginsList().indexOf(pluginName) != -1;
};

tulip.algorithmExists = function tulip_algoritmExists(algoName) {
  return _algorithmExists(algoName) > 0;
};

tulip.propertyAlgorithmExists = function(algoName) {
  return _propertyAlgorithmExists(algoName) > 0;
};

tulip.booleanAlgorithmExists = function(algoName) {
  return _booleanAlgorithmExists(algoName) > 0;
};

tulip.colorAlgorithmExists = function(algoName) {
  return _colorAlgorithmExists(algoName) > 0;
};

tulip.doubleAlgorithmExists = function(algoName) {
  return _doubleAlgorithmExists(algoName) > 0;
};

tulip.integerAlgorithmExists = function(algoName) {
  return _integerAlgorithmExists(algoName) > 0;
};

tulip.layoutAlgorithmExists = function(algoName) {
  return _layoutAlgorithmExists(algoName) > 0;
};

tulip.stringAlgorithmExists = function(algoName) {
  return _stringAlgorithmExists(algoName) > 0;
};

tulip.sizeAlgorithmExists = function(algoName) {
  return _sizeAlgorithmExists(algoName) > 0;
};

tulip.importPluginExists = function(algoName) {
  return _importPluginExists(algoName) > 0;
};

// ==================================================================================================================

var _Graph_newGraph = Module.cwrap('Graph_newGraph', 'number', []);
var _Graph_delete = Module.cwrap('Graph_delete', null, ['number']);
var _Graph_applyAlgorithm = Module.cwrap('Graph_applyAlgorithm', 'number', ['number', 'string', 'string', 'number']);
var _Graph_clear = Module.cwrap('Graph_clear', null, ['number']);
var _Graph_addSubGraph1 = Module.cwrap('Graph_addSubGraph1', 'number', ['number', 'number', 'string']);
var _Graph_addSubGraph2 = Module.cwrap('Graph_addSubGraph2', 'number', ['number', 'string']);
var _Graph_addCloneSubGraph = Module.cwrap('Graph_addCloneSubGraph', 'number', ['number', 'string', 'number']);
var _Graph_inducedSubGraph = Module.cwrap('Graph_inducedSubGraph', 'number', ['number', 'number', 'number']);
var _Graph_delSubGraph = Module.cwrap('Graph_delSubGraph', null, ['number', 'number']);
var _Graph_delAllSubGraphs = Module.cwrap('Graph_delAllSubGraphs', null, ['number', 'number']);
var _Graph_getSuperGraph = Module.cwrap('Graph_getSuperGraph', 'number', ['number']);
var _Graph_getRoot = Module.cwrap('Graph_getRoot', 'number', ['number']);
var _Graph_getSubGraphs = Module.cwrap('Graph_getSubGraphs', null, ['number', 'number']);
var _Graph_getNthSubGraph = Module.cwrap('Graph_getNthSubGraph', 'number', ['number', 'number']);
var _Graph_numberOfSubGraphs = Module.cwrap('Graph_numberOfSubGraphs', 'number', ['number']);
var _Graph_numberOfDescendantGraphs = Module.cwrap('Graph_numberOfDescendantGraphs', 'number', ['number']);
var _Graph_isSubGraph = Module.cwrap('Graph_isSubGraph', 'number', ['number', 'number']);
var _Graph_isDescendantGraph = Module.cwrap('Graph_isDescendantGraph', 'number', ['number', 'number']);
var _Graph_getSubGraph1 = Module.cwrap('Graph_getSubGraph1', 'number', ['number', 'number']);
var _Graph_getSubGraph2 = Module.cwrap('Graph_getSubGraph2', 'number', ['number', 'string']);
var _Graph_getDescendantGraph1 = Module.cwrap('Graph_getDescendantGraph1', 'number', ['number', 'number']);
var _Graph_getDescendantGraph2 = Module.cwrap('Graph_getDescendantGraph2', 'number', ['number', 'string']);
var _Graph_getDescendantGraphs = Module.cwrap('Graph_getDescendantGraphs', null, ['number', 'number']);

var _Graph_addNode1 = Module.cwrap('Graph_addNode1', 'number', ['number']);
var _Graph_addNode2 = Module.cwrap('Graph_addNode2', null, ['number', 'number']);
var _Graph_addNodes1 = Module.cwrap('Graph_addNodes1', null, ['number', 'number', 'number']);
var _Graph_addNodes2 = Module.cwrap('Graph_addNodes2', null, ['number', 'number', 'number']);
var _Graph_delNode = Module.cwrap('Graph_delNode', null, ['number', 'number', 'number']);
var _Graph_delNodes = Module.cwrap('Graph_delNodes', null, ['number', 'number', 'number', 'number']);

var _Graph_addEdge1 = Module.cwrap('Graph_addEdge1', 'number', ['number', 'number', 'number']);
var _Graph_addEdge2 = Module.cwrap('Graph_addEdge2', null, ['number', 'number']);
var _Graph_addEdges1 = Module.cwrap('Graph_addEdges1', null, ['number', 'number', 'number', 'number']);
var _Graph_addEdges2 = Module.cwrap('Graph_addEdges2', null, ['number', 'number', 'number']);
var _Graph_delEdge = Module.cwrap('Graph_delEdge', null, ['number', 'number', 'number']);
var _Graph_delEdges = Module.cwrap('Graph_delEdges', null, ['number', 'number', 'number', 'number']);
var _Graph_setEdgeOrder = Module.cwrap('Graph_setEdgeOrder', null, ['number', 'number', 'number', 'number']);
var _Graph_swapEdgeOrder = Module.cwrap('Graph_swapEdgeOrder', null, ['number', 'number', 'number', 'number']);
var _Graph_setSource = Module.cwrap('Graph_setSource', null, ['number', 'number', 'number']);
var _Graph_setTarget = Module.cwrap('Graph_setTarget', null, ['number', 'number', 'number']);
var _Graph_setEnds = Module.cwrap('Graph_setEnds', null, ['number', 'number', 'number', 'number']);
var _Graph_reverse = Module.cwrap('Graph_reverse', null, ['number', 'number']);
var _Graph_getSource = Module.cwrap('Graph_getSource', 'number', ['number']);

var _Graph_getNodes = Module.cwrap('Graph_getNodes', null, ['number', 'number']);
var _Graph_getInNodes = Module.cwrap('Graph_getInNodes', null, ['number', 'number', 'number']);
var _Graph_getOutNodes = Module.cwrap('Graph_getOutNodes', null, ['number', 'number', 'number']);
var _Graph_getInOutNodes = Module.cwrap('Graph_getInOutNodes', null, ['number', 'number', 'number']);
var _Graph_bfs = Module.cwrap('Graph_bfs', null, ['number', 'number', 'number']);
var _Graph_dfs = Module.cwrap('Graph_dfs', null, ['number', 'number', 'number']);
var _Graph_getEdges = Module.cwrap('Graph_getEdges', null, ['number', 'number']);
var _Graph_getOutEdges = Module.cwrap('Graph_getOutEdges', null, ['number', 'number', 'number']);
var _Graph_getInOutEdges = Module.cwrap('Graph_getInOutEdges', null, ['number', 'number', 'number']);
var _Graph_getInEdges = Module.cwrap('Graph_getInEdges', null, ['number', 'number', 'number']);
var _Graph_getRandomNode = Module.cwrap('Graph_getRandomNode', 'number', ['number']);
var _Graph_getRandomEdge = Module.cwrap('Graph_getRandomEdge', 'number', ['number']);

var _Graph_getId = Module.cwrap('Graph_getId', 'number', ['number']);
var _Graph_numberOfNodes = Module.cwrap('Graph_numberOfNodes', 'number', ['number']);
var _Graph_numberOfEdges = Module.cwrap('Graph_numberOfEdges', 'number', ['number']);
var _Graph_deg = Module.cwrap('Graph_deg', 'number', ['number', 'number']);
var _Graph_indeg = Module.cwrap('Graph_indeg', 'number', ['number', 'number']);
var _Graph_outdeg = Module.cwrap('Graph_outdeg', 'number', ['number', 'number']);
var _Graph_source = Module.cwrap('Graph_source', 'number', ['number', 'number']);
var _Graph_target = Module.cwrap('Graph_target', 'number', ['number', 'number']);
var _Graph_ends = Module.cwrap('Graph_ends', null, ['number', 'number', 'number']);
var _Graph_opposite = Module.cwrap('Graph_opposite', 'number', ['number', 'number', 'number']);
var _Graph_isNodeElement = Module.cwrap('Graph_isNodeElement', 'number', ['number', 'number']);
var _Graph_isEdgeElement = Module.cwrap('Graph_isEdgeElement', 'number', ['number', 'number']);
var _Graph_hasEdge = Module.cwrap('Graph_hasEdge', 'number', ['number', 'number', 'number', 'number']);
var _Graph_existEdge = Module.cwrap('Graph_existEdge', 'number', ['number', 'number', 'number', 'number']);
var _Graph_getEdges2 = Module.cwrap('Graph_getEdges2', null, ['number', 'number', 'number', 'number', 'number']);

var _Graph_getName = Module.cwrap('Graph_getName', 'string', ['number']);
var _Graph_setName = Module.cwrap('Graph_setName', null, ['number', 'string']);

var _Graph_numberOfProperties = Module.cwrap('Graph_numberOfProperties', 'number', ['number']);
var _Graph_propertiesNamesLengths = Module.cwrap('Graph_propertiesNamesLengths', 'number', ['number', 'number']);
var _Graph_getProperties = Module.cwrap('Graph_getProperties', null, ['number', 'number']);

var _Graph_getProperty = Module.cwrap('Graph_getProperty', 'number', ['number', 'string']);
var _Graph_getBooleanProperty = Module.cwrap('Graph_getBooleanProperty', 'number', ['number', 'string']);
var _Graph_getLocalBooleanProperty = Module.cwrap('Graph_getLocalBooleanProperty', 'number', ['number', 'string']);
var _Graph_getColorProperty = Module.cwrap('Graph_getColorProperty', 'number', ['number', 'string']);
var _Graph_getLocalColorProperty = Module.cwrap('Graph_getLocalColorProperty', 'number', ['number', 'string']);
var _Graph_getDoubleProperty = Module.cwrap('Graph_getDoubleProperty', 'number', ['number', 'string']);
var _Graph_getLocalDoubleProperty = Module.cwrap('Graph_getLocalDoubleProperty', 'number', ['number', 'string']);
var _Graph_getIntegerProperty = Module.cwrap('Graph_getIntegerProperty', 'number', ['number', 'string']);
var _Graph_getLocalIntegerProperty = Module.cwrap('Graph_getLocalIntegerProperty', 'number', ['number', 'string']);
var _Graph_getLayoutProperty = Module.cwrap('Graph_getLayoutProperty', 'number', ['number', 'string']);
var _Graph_getLocalLayoutProperty = Module.cwrap('Graph_getLocalLayoutProperty', 'number', ['number', 'string']);
var _Graph_getSizeProperty = Module.cwrap('Graph_getSizeProperty', 'number', ['number', 'string']);
var _Graph_getLocalSizeProperty = Module.cwrap('Graph_getLocalSizeProperty', 'number', ['number', 'string']);
var _Graph_getStringProperty = Module.cwrap('Graph_getStringProperty', 'number', ['number', 'string']);
var _Graph_getLocalStringProperty = Module.cwrap('Graph_getLocalStringProperty', 'number', ['number', 'string']);
var _Graph_getBooleanVectorProperty = Module.cwrap('Graph_getBooleanVectorProperty', 'number', ['number', 'string']);
var _Graph_getLocalBooleanVectorProperty = Module.cwrap('Graph_getLocalBooleanVectorProperty', 'number', ['number', 'string']);
var _Graph_getColorVectorProperty = Module.cwrap('Graph_getColorVectorProperty', 'number', ['number', 'string']);
var _Graph_getLocalColorVectorProperty = Module.cwrap('Graph_getLocalColorVectorProperty', 'number', ['number', 'string']);
var _Graph_getDoubleVectorProperty = Module.cwrap('Graph_getDoubleVectorProperty', 'number', ['number', 'string']);
var _Graph_getLocalDoubleVectorProperty = Module.cwrap('Graph_getLocalDoubleVectorProperty', 'number', ['number', 'string']);
var _Graph_getIntegerVectorProperty = Module.cwrap('Graph_getIntegerVectorProperty', 'number', ['number', 'string']);
var _Graph_getLocalIntegerVectorProperty = Module.cwrap('Graph_getLocalIntegerVectorProperty', 'number', ['number', 'string']);
var _Graph_getCoordVectorProperty = Module.cwrap('Graph_getCoordVectorProperty', 'number', ['number', 'string']);
var _Graph_getLocalCoordVectorProperty = Module.cwrap('Graph_getLocalCoordVectorProperty', 'number', ['number', 'string']);
var _Graph_getSizeVectorProperty = Module.cwrap('Graph_getSizeVectorProperty', 'number', ['number', 'string']);
var _Graph_getLocalSizeVectorProperty = Module.cwrap('Graph_getLocalSizeVectorProperty', 'number', ['number', 'string']);
var _Graph_getStringVectorProperty = Module.cwrap('Graph_getStringVectorProperty', 'number', ['number', 'string']);
var _Graph_getLocalStringVectorProperty = Module.cwrap('Graph_getLocalStringVectorProperty', 'number', ['number', 'string']);
var _Graph_delLocalProperty = Module.cwrap('Graph_delLocalProperty', null, ['number', 'string']);
var _Graph_applyPropertyAlgorithm = Module.cwrap('Graph_applyPropertyAlgorithm', 'number', ['number', 'string', 'number', 'string', 'number']);
var _Graph_push = Module.cwrap('Graph_push', null, ['number']);
var _Graph_pop = Module.cwrap('Graph_pop', null, ['number']);
var _Graph_setEventsActivated = Module.cwrap('Graph_setEventsActivated', null, ['number', 'number']);
var _Graph_getAttributesJSON = Module.cwrap('Graph_getAttributesJSON', 'string', ['number']);
var _Graph_isMetaNode = Module.cwrap('Graph_isMetaNode', 'number', ['number', 'number']);
var _Graph_openMetaNode = Module.cwrap('Graph_openMetaNode', null, ['number', 'number']);

var _Graph_isConnected = Module.cwrap('Graph_isConnected', 'number', ['number']);

/**
* This is the description for the tulip.Graph class.
*
* @class Graph
* @constructor
*/
tulip.Graph = function tulip_Graph(cppPointer) {
  var newObject = createObject(tulip.Graph, this);
  if (arguments.length == 0) {
    tulip.CppObjectWrapper.call(newObject, _Graph_newGraph(), "tlp::Graph");
  } else {
    tulip.CppObjectWrapper.call(newObject, cppPointer, "tlp::Graph");
  }
  if (!workerMode) {
    graphHierarchyIdToWrapper[newObject.getCppPointer()] = newObject;
  }
  return newObject;
};
tulip.Graph.inheritsFrom(tulip.CppObjectWrapper);
tulip.Graph.prototype.destroy = function() {
  _Graph_delete(this.cppPointer);
  this.cppPointer = 0;
};
tulip.Graph.prototype.applyAlgorithm = function tulip_Graph_prototype_applyAlgorithm(algorithmName, algoParameters, notifyProgress) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string", "object", "boolean"], 1);
  if (!tulip.algorithmExists(algorithmName)) {
    console.log("Error : no Tulip algorithm named '" + algorithmName + "'");
    return false;
  }
  if (algoParameters == undefined) algoParameters = {};
  if (notifyProgress == undefined) notifyProgress = false;
  return _Graph_applyAlgorithm(this.cppPointer, algorithmName, JSON.stringify(algoParameters), notifyProgress) > 0;
};
tulip.Graph.prototype.clear = function tulip_Graph_prototype_clear() {
  checkWrappedCppPointer(this.cppPointer);
  _Graph_clear(this.cppPointer);
};
tulip.Graph.prototype.addSubGraph = function tulip_Graph_prototype_addSubGraph() {
  checkWrappedCppPointer(this.cppPointer);
  // addSubGraph(selection, name)
  if (arguments.length == 2) {
    checkArgumentsTypes(arguments, [tulip.BooleanProperty, "string"]);
    return tulip.Graph(_Graph_addSubGraph1(this.cppPointer, arguments[0].cppPointer, arguments[1]));
    // addSubGraph(name)
  } else if (arguments.length == 1) {
    checkArgumentsTypes(arguments, ["string"]);
    return tulip.Graph(_Graph_addSubGraph2(this.cppPointer, arguments[0]));
  } else if (arguments.length == 0) {
    return tulip.Graph(_Graph_addSubGraph2(this.cppPointer, ""));
  } else {
    return null;
  }
};
tulip.Graph.prototype.addCloneSubGraph = function tulip_Graph_prototype_addCloneSubGraph(name, addSibling) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string", "boolean"]);
  return tulip.Graph(_Graph_addCloneSubGraph(this.cppPointer, name, addSibling));
};
tulip.Graph.prototype.inducedSubGraph = function tulip_Graph_prototype_inducedSubGraph(nodes, parentSubGraph) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["array", tulip.Graph]);
  checkArrayOfType(nodes, tulip.Node, 0);
  var nodesIdsArray = allocArrayInEmHeap(Uint32Array, nodes.length+1);
  for (var i = 0 ; i < nodes.length ; ++i) {
    nodesIdsArray[i] = nodes[i].id;
  }
  nodesIdsArray[nodes.length] = UINT_MAX;
  var sg = tulip.Graph(_Graph_inducedSubGraph(this.cppPointer, nodesIdsArray.byteOffset, parentSubGraph ? parentSubGraph.cppPointer : 0));
  freeArrayInEmHeap(nodesIdsArray);
  return sg;
};
tulip.Graph.prototype.delSubGraph = function tulip_Graph_prototype_delSubGraph(sg) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Graph]);
  _Graph_delSubGraph(this.cppPointer, sg.cppPointer);
};
tulip.Graph.prototype.delAllSubGraphs = function tulip_Graph_prototype_delAllSubGraphs(sg) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Graph]);
  _Graph_delAllSubGraphs(this.cppPointer, sg.cppPointer);
};
tulip.Graph.prototype.getSuperGraph = function tulip_Graph_prototype_getSuperGraph() {
  checkWrappedCppPointer(this.cppPointer);
  return tulip.Graph(_Graph_getSuperGraph(this.cppPointer));
};
tulip.Graph.prototype.getRoot = function tulip_Graph_prototype_getRoot() {
  checkWrappedCppPointer(this.cppPointer);
  return tulip.Graph(_Graph_getRoot(this.cppPointer));
};
tulip.Graph.prototype.getSubGraphs = function tulip_Graph_prototype_getSubGraphs() {
  checkWrappedCppPointer(this.cppPointer);
  var graphObject = this;
  return getArrayOfTulipType(graphObject.numberOfSubGraphs(), function(byteOffset){_Graph_getSubGraphs(graphObject.cppPointer, byteOffset)}, tulip.Graph);
};
tulip.Graph.prototype.getNthSubGraph = function tulip_Graph_prototype_getNthSubGraph(n) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["number"]);
  return tulip.Graph(_Graph_getNthSubGraph(this.cppPointer, n));
};
tulip.Graph.prototype.numberOfSubGraphs = function tulip_Graph_prototype_numberOfSubGraphs() {
  checkWrappedCppPointer(this.cppPointer);
  return _Graph_numberOfSubGraphs(this.cppPointer);
};
tulip.Graph.prototype.numberOfDescendantGraphs = function tulip_Graph_prototype_numberOfDescendantGraphs() {
  checkWrappedCppPointer(this.cppPointer);
  return _Graph_numberOfDescendantGraphs(this.cppPointer);
};
tulip.Graph.prototype.isSubGraph = function tulip_Graph_prototype_isSubGraph(subGraph) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Graph], 1);
  return _Graph_isSubGraph(this.cppPointer, subGraph.cppPointer);
};
tulip.Graph.prototype.isDescendantGraph = function tulip_Graph_prototype_isDescendantGraph(subGraph) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Graph], 1);
  return _Graph_isDescendantGraph(this.cppPointer, subGraph.cppPointer);
};
tulip.Graph.prototype.getSubGraph = function tulip_Graph_prototype_getSubGraph() {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [["number", "string"]], 1);
  if (typeOf(arguments[0]) == "string") {
    var sgPointer = _Graph_getSubGraph2(this.cppPointer, arguments[0]);
    return sgPointer ? tulip.Graph(sgPointer) : null;
  } else {
    var sgPointer = _Graph_getSubGraph1(this.cppPointer, arguments[0]);
    return sgPointer ? tulip.Graph(sgPointer) : null;
  }
};
tulip.Graph.prototype.getDescendantGraph = function tulip_Graph_prototype_getDescendantGraph() {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [["number", "string"]], 1);
  if (typeOf(arguments[0]) == "string") {
    var sgPointer = _Graph_getDescendantGraph2(this.cppPointer, arguments[0]);
    return sgPointer ? tulip.Graph(sgPointer) : null;
  } else {
    var sgPointer = _Graph_getDescendantGraph1(this.cppPointer, arguments[0]);
    return sgPointer ? tulip.Graph(sgPointer) : null;
  }
};
tulip.Graph.prototype.getDescendantGraphs = function tulip_Graph_prototype_getDescendantGraphs() {
  checkWrappedCppPointer(this.cppPointer);
  var graphObject = this;
  return getArrayOfTulipType(graphObject.numberOfDescendantGraphs(), function(byteOffset){_Graph_getDescendantGraphs(graphObject.cppPointer, byteOffset)}, tulip.Graph);
};
tulip.Graph.prototype.addNode = function tulip_Graph_prototype_addNode() {
  checkWrappedCppPointer(this.cppPointer);
  if (arguments.length == 0) {
    return tulip.Node(_Graph_addNode1(this.cppPointer));
  } else {
    checkArgumentsTypes(arguments, [tulip.Node], 1);
    _Graph_addNode2(this.cppPointer, arguments[0].id);
  }
};
tulip.Graph.prototype.addNodes = function tulip_Graph_prototype_addNodes() {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [['number', 'array']], 1);
  // addNodes(nbNodes) -> array of tulip.Node
  if (typeOf(arguments[0]) == 'number') {
    var nbNodes = arguments[0];
    var graphObject = this;
    return getArrayOfTulipType(nbNodes, function(byteOffset){_Graph_addNodes1(graphObject.cppPointer, nbNodes, byteOffset)}, tulip.Node);

    // addNodes(array of tulip.Node)
  } else {
    var nodes = arguments[0];
    checkArrayOfType(nodes, tulip.Node, 0);
    var nodesIdsArray = allocArrayInEmHeap(Uint32Array, nodes.length);
    for (var i = 0 ; i < nodes.length ; ++i) {
      nodesIdsArray[i] = nodes[i].id;
    }
    _Graph_addNodes2(this.cppPointer, nodes.length, nodesIdsArray.byteOffset);
    freeArrayInEmHeap(nodesIdsArray);
  }
};
tulip.Graph.prototype.delNode = function tulip_Graph_prototype_delNode(node, deleteInAllGraphs) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node, "boolean"], 1);
  if (arguments.length == 1) {
    deleteInAllGraphs = false;
  }
  _Graph_delNode(this.cppPointer, node.id, deleteInAllGraphs);
};
tulip.Graph.prototype.delNodes = function tulip_Graph_prototype_delNodes(nodes, deleteInAllGraphs) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["array", "boolean"], 1);
  checkArrayOfType(nodes, tulip.Node, 0);
  if (arguments.length == 1) {
    deleteInAllGraphs = false;
  }
  var nodesIdsArray = allocArrayInEmHeap(Uint32Array, nodes.length);
  for (var i = 0 ; i < nodes.length ; ++i) {
    nodesIdsArray[i] = nodes[i].id;
  }
  _Graph_delNodes(this.cppPointer, nodes.length, nodesIdsArray.byteOffset, deleteInAllGraphs);
  freeArrayInEmHeap(nodesIdsArray);
};
/**
* Adds a new edge in the graph
*
* @method addEdge
* @param {Node} src the source node
* @param {Node} tgt the target node
* @return {Edge} Returns a new graph edge
*/
tulip.Graph.prototype.addEdge = function tulip_Graph_prototype_addEdge() {
  checkWrappedCppPointer(this.cppPointer);
  // addEdge(tulip.Edge)
  if (arguments.length <= 1) {
    checkArgumentsTypes(arguments, [tulip.Edge], 1);
    var edge = arguments[0];
    _Graph_addEdge2(this.cppPointer, edge.id);
    // addEdge(tulip.Node, tulip.Node) -> tulip.Edge
  } else {
    checkArgumentsTypes(arguments, [tulip.Node, tulip.Node], 2);
    var src = arguments[0];
    var tgt = arguments[1];
    return tulip.Edge(_Graph_addEdge1(this.cppPointer, src.id, tgt.id));
  }
};
tulip.Graph.prototype.addEdges = function tulip_Graph_prototype_addEdges() {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["array"], 1);
  try {
    // addEdge(array of [tulip.Node, tulip.Node])
    checkArrayOfType(arguments[0], "array");
    try {
      var nodes = arguments[0];
      for (var i = 0 ; i < nodes.length ; ++i) {
        if (nodes[i].length != 2) {
          throw "error";
        } else {
          checkArrayOfType(nodes[i], tulip.Node);
        }
      }
      var nodesIdsArray = allocArrayInEmHeap(Uint32Array, nodes.length*2);
      for (var i = 0 ; i < nodes.length ; ++i) {
        nodesIdsArray[2*i] = nodes[i][0].id;
        nodesIdsArray[2*i+1] = nodes[i][1].id;
      }
      var graphObject = this;
      var edges = getArrayOfTulipType(nodes.length, function(byteOffset){_Graph_addEdges1(graphObject.cppPointer, nodes.length, nodesIdsArray.byteOffset, byteOffset)}, tulip.Edge);
      freeArrayInEmHeap(nodesIdsArray);
      return edges;
    } catch (err) {
      throw new TypeError("Error when calling function 'tulip.Graph.prototype.addEdges', parameter 0 must be an array of arrays containing two instances of tulip.Node objects");
    }
  } catch (err) {
    // addEdge(array of tulip.Edge)
    checkArrayOfType(arguments[0], tulip.Edge);
    var edges = arguments[0];
    var edgesIdsArray = allocArrayInEmHeap(Uint32Array, edges.length);
    for (var i = 0 ; i < edges.length ; ++i) {
      edgesIdsArray[i] = edges[i].id;
    }
    _Graph_addEdges2(this.cppPointer, edges.length, edgesIdsArray.byteOffset);
    freeArrayInEmHeap(edgesIdsArray);
  }
};
tulip.Graph.prototype.delEdge = function tulip_Graph_prototype_delEdge(edge, deleteInAllGraphs) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge, "boolean"], 1);
  if (arguments.length == 1) {
    deleteInAllGraphs = false;
  }
  _Graph_delEdge(this.cppPointer, edge.id, deleteInAllGraphs);
};
tulip.Graph.prototype.delEdges = function tulip_Graph_prototype_delEdges(edges, deleteInAllGraphs) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["array", "boolean"], 1);
  checkArrayOfType(edges, tulip.Edge, 0);
  if (arguments.length == 1) {
    deleteInAllGraphs = false;
  }
  var edgesIdsArray = allocArrayInEmHeap(Uint32Array, edges.length);
  for (var i = 0 ; i < edges.length ; ++i) {
    edgesIdsArray[i] = edges[i].id;
  }
  _Graph_delEdges(this.cppPointer, edges.length, edgesIdsArray.byteOffset, deleteInAllGraphs);
  freeArrayInEmHeap(edgesIdsArray);
};
tulip.Graph.prototype.setEdgeOrder = function tulip_Graph_prototype_setEdgeOrder(node, edges) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node, "array"], 2);
  checkArrayOfType(edges, tulip.Edge);
  var edgesIdsArray = allocArrayInEmHeap(Uint32Array, edges.length);
  for (var i = 0 ; i < edges.length ; ++i) {
    edgesIdsArray[i] = edges[i].id;
  }
  _Graph_setEdgeOrder(this.cppPointer, node.id, edges.length, edgesIdsArray.byteOffset);
  freeArrayInEmHeap(edgesIdsArray);
};
tulip.Graph.prototype.swapEdgeOrder = function tulip_Graph_prototype_swapEdgeOrder(node, edge1, edge2) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node, tulip.Edge, tulip.Edge], 3);
  _Graph_swapEdgeOrder(this.cppPointer, node.id, edge1.id, edge2.id);
};
tulip.Graph.prototype.setSource = function tulip_Graph_prototype_setSource(edge, node) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge, tulip.Node], 2);
  _Graph_setSource(this.cppPointer, edge.id, node.id);
};
tulip.Graph.prototype.setTarget = function tulip_Graph_prototype_setTarget(edge, node) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge, tulip.Node], 2);
  _Graph_setTarget(this.cppPointer, edge.id, node.id);
};
tulip.Graph.prototype.setEnds = function tulip_Graph_prototype_setEnds(edge, source, target) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge, tulip.Node, tulip.Node], 3);
  _Graph_setEnds(this.cppPointer, edge.id, source.id, target.id);
};
tulip.Graph.prototype.reverse = function tulip_Graph_prototype_reverse(edge) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge], 1);
  _Graph_reverse(this.cppPointer, edge.id);
};
tulip.Graph.prototype.getSource = function tulip_Graph_prototype_getSource() {
  checkWrappedCppPointer(this.cppPointer);
  return tulip.Node(_Graph_getSource(this.cppPointer));
};
tulip.Graph.prototype.getNodes = function tulip_Graph_prototype_getNodes() {
  checkWrappedCppPointer(this.cppPointer);
  var graphObject = this;
  return getArrayOfTulipType(graphObject.numberOfNodes(), function(byteOffset){_Graph_getNodes(graphObject.cppPointer, byteOffset)}, tulip.Node);
};
tulip.Graph.prototype.getRandomNode = function tulip_Graph_prototype_getRandomNode() {
  checkWrappedCppPointer(this.cppPointer);
  return tulip.Node(_Graph_getRandomNode(this.cppPointer));
};
tulip.Graph.prototype.getInNodes = function tulip_Graph_prototype_getInNodes(node) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node]);
  var graphObject = this;
  return getArrayOfTulipType(graphObject.indeg(node), function(byteOffset){_Graph_getInNodes(graphObject.cppPointer, node.id, byteOffset)}, tulip.Node);
};
tulip.Graph.prototype.getOutNodes = function tulip_Graph_prototype_getOutNodes(node) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node]);
  var graphObject = this;
  return getArrayOfTulipType(graphObject.outdeg(node), function(byteOffset){_Graph_getOutNodes(graphObject.cppPointer, node.id, byteOffset)}, tulip.Node);
};
tulip.Graph.prototype.getInOutNodes = function tulip_Graph_prototype_getInOutNodes(node) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node]);
  var graphObject = this;
  return getArrayOfTulipType(graphObject.deg(node), function(byteOffset){_Graph_getInOutNodes(graphObject.cppPointer, node.id, byteOffset)}, tulip.Node);
};
tulip.Graph.prototype.bfs = function tulip_Graph_prototype_bfs(root) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node]);
  var graphObject = this;
  if (arguments.length == 0) {
    return getArrayOfTulipType(graphObject.numberOfNodes(), function(byteOffset){_Graph_bfs(graphObject.cppPointer, UINT_MAX, byteOffset)}, tulip.Node);
  } else {
    return getArrayOfTulipType(graphObject.numberOfNodes(), function(byteOffset){_Graph_bfs(graphObject.cppPointer, root.id, byteOffset)}, tulip.Node);
  }
};
tulip.Graph.prototype.dfs = function tulip_Graph_prototype_dfs(root) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node]);
  var graphObject = this;
  if (arguments.length == 0) {
    return getArrayOfTulipType(graphObject.numberOfNodes(), function(byteOffset){_Graph_dfs(graphObject.cppPointer, UINT_MAX, byteOffset)}, tulip.Node);
  } else {
    return getArrayOfTulipType(graphObject.numberOfNodes(), function(byteOffset){_Graph_dfs(graphObject.cppPointer, root.id, byteOffset)}, tulip.Node);
  }
};
tulip.Graph.prototype.getEdges = function tulip_Graph_prototype_getEdges() {
  checkWrappedCppPointer(this.cppPointer);
  // getEdges()
  if (arguments.length == 0) {
    var graphObject = this;
    return getArrayOfTulipType(graphObject.numberOfEdges(), function(byteOffset){_Graph_getEdges(graphObject.cppPointer, byteOffset)}, tulip.Edge);
    // getEdges(tulip.Node, tulip.Node, boolean)
  } else {
    checkArgumentsTypes(arguments, [tulip.Node, tulip.Node, "boolean"], 2);
    var directed = true;
    if (arguments.length > 2) directed = arguments[2];
    return getArrayOfTulipType(graphObject.deg(arguments[0]), function(byteOffset){_Graph_getEdges2(this.cppPointer, arguments[0].id, arguments[1].id, directed, byteOffset)}, tulip.Edge);
  }
};
tulip.Graph.prototype.getRandomEdge = function tulip_Graph_prototype_getRandomEdge() {
  checkWrappedCppPointer(this.cppPointer);
  return tulip.Edge(_Graph_getRandomEdge(this.cppPointer));
};
tulip.Graph.prototype.getOutEdges = function tulip_Graph_prototype_getOutEdges(node) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node]);
  var graphObject = this;
  return getArrayOfTulipType(graphObject.outdeg(node), function(byteOffset){_Graph_getOutEdges(graphObject.cppPointer, node.id, byteOffset)}, tulip.Edge);
};
tulip.Graph.prototype.getInOutEdges = function tulip_Graph_prototype_getInOutEdges(node) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node]);
  var graphObject = this;
  return getArrayOfTulipType(graphObject.deg(node), function(byteOffset){_Graph_getInOutEdges(graphObject.cppPointer, node.id, byteOffset)}, tulip.Edge);
};
tulip.Graph.prototype.getInEdges = function tulip_Graph_prototype_getInEdges(node) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node]);
  var graphObject = this;
  return getArrayOfTulipType(graphObject.indeg(node), function(byteOffset){_Graph_getInEdges(graphObject.cppPointer, node.id, byteOffset)}, tulip.Edge);
};
tulip.Graph.prototype.getId = function tulip_Graph_prototype_getId() {
  checkWrappedCppPointer(this.cppPointer);
  return _Graph_getId(this.cppPointer);
};
tulip.Graph.prototype.numberOfNodes = function tulip_Graph_prototype_numberOfNodes() {
  checkWrappedCppPointer(this.cppPointer);
  return _Graph_numberOfNodes(this.cppPointer);
};
tulip.Graph.prototype.numberOfEdges = function tulip_Graph_prototype_numberOfEdges() {
  checkWrappedCppPointer(this.cppPointer);
  return _Graph_numberOfEdges(this.cppPointer);
};
tulip.Graph.prototype.deg = function tulip_Graph_prototype_deg(node) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node]);
  return _Graph_deg(this.cppPointer, node.id);
};
tulip.Graph.prototype.indeg = function tulip_Graph_prototype_indeg(node) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node]);
  return _Graph_indeg(this.cppPointer, node.id);
};
tulip.Graph.prototype.outdeg = function tulip_Graph_prototype_outdeg(node) {
  checkArgumentsTypes(arguments, [tulip.Node]);
  return _Graph_outdeg(this.cppPointer, node.id);
};
tulip.Graph.prototype.source = function tulip_Graph_prototype_source(edge) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge]);
  return tulip.Node(_Graph_source(this.cppPointer, edge.id));
};
tulip.Graph.prototype.target = function tulip_Graph_prototype_target(edge) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge]);
  return tulip.Node(_Graph_target(this.cppPointer, edge.id));
};
tulip.Graph.prototype.ends = function tulip_Graph_prototype_ends(edge) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge]);
  return getArrayOfTulipType(2, function(byteOffset){_Graph_ends(graphObject.cppPointer, edge.id, byteOffset)}, tulip.Node);
};
tulip.Graph.prototype.opposite = function tulip_Graph_prototype_opposite(edge, node) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge, tulip.Node]);
  return tulip.Node(_Graph_opposite(this.cppPointer, edge.id, node.id));
};
tulip.Graph.prototype.isElement = function tulip_Graph_prototype_isElement(elem) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [[tulip.Edge, tulip.Node]]);
  if (elem instanceof tulip.Node) {
    return _Graph_isNodeElement(this.cppPointer, elem.id) > 0;
  } else {
    return _Graph_isEdgeElement(this.cppPointer, elem.id) > 0;
  }
};
tulip.Graph.prototype.hasEdge = function tulip_Graph_prototype_hasEdge(sourceNode, targetNode, directed) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node, tulip.Node, "boolean"], 2);
  if (directed == undefined) directed = true;
  return _Graph_hasEdge(this.cppPointer, sourceNode.id, targetNode.id, directed) > 0;
};
tulip.Graph.prototype.existEdge = function tulip_Graph_prototype_existEdge(sourceNode, targetNode, directed) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node, tulip.Node, "boolean"], 2);
  if (directed == undefined) directed = true;
  return tulip.Edge(_Graph_existEdge(this.cppPointer, sourceNode.id, targetNode.id, directed));
};
tulip.Graph.prototype.getName = function tulip_Graph_prototype_getName() {
  checkWrappedCppPointer(this.cppPointer);
  return _Graph_getName(this.cppPointer);
};
tulip.Graph.prototype.setName = function tulip_Graph_prototype_setName(name) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string"]);
  _Graph_setName(this.cppPointer, name);
};
tulip.Graph.prototype.getProperties = function tulip_Graph_prototype_getProperties() {
  checkWrappedCppPointer(this.cppPointer);
  var nbProperties = _Graph_numberOfProperties(this.cppPointer);
  var uintArray = allocArrayInEmHeap(Uint32Array, nbProperties);
  var nbBytes = _Graph_propertiesNamesLengths(this.cppPointer, uintArray.byteOffset);
  var ucharArray = allocArrayInEmHeap(Uint8Array, nbBytes);
  _Graph_getProperties(this.cppPointer, ucharArray.byteOffset);
  var ret = bytesTypedArrayToStringArray(ucharArray, uintArray, nbProperties);
  freeArrayInEmHeap(uintArray);
  freeArrayInEmHeap(ucharArray);
  return ret;
};
tulip.Graph.prototype.getProperty = function tulip_Graph_prototype_getPropery(name) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string"]);
  var typedProperty = null;
  var propertyPointer = _Graph_getProperty(this.cppPointer, name);
  if (propertyPointer) {
    var prop = tulip.PropertyInterface(propertyPointer, true);
    var propertyType = prop.getTypename();
    switch (propertyType) {
    case 'bool':
      typedProperty = this.getBooleanProperty(name);
      break;
    case 'color':
      typedProperty = this.getColorProperty(name);
      break;
    case 'double':
      typedProperty = this.getDoubleProperty(name);
      break;
    case 'int' :
      typedProperty = this.getIntegerProperty(name);
      break;
    case 'layout':
      typedProperty = this.getLayoutProperty(name);
      break;
    case 'size':
      typedProperty = this.getSizeProperty(name);
      break;
    case 'string':
      typedProperty = this.getStringProperty(name);
      break;
    case 'vector<bool>':
      typedProperty = this.getBooleanVectorProperty(name);
      break;
    case 'vector<color>':
      typedProperty = this.getColorVectorProperty(name);
      break;
    case 'vector<double>':
      typedProperty = this.getDoubleVectorProperty(name);
      break;
    case 'vector<int>' :
      typedProperty = this.getIntegerVectorProperty(name);
      break;
    case 'vector<coord>':
      typedProperty = this.getCoordVectorProperty(name);
      break;
    case 'vector<size>':
      typedProperty = this.getSizeVectorProperty(name);
      break;
    case 'vector<string>':
      typedProperty = this.getStringVectorProperty(name);
      break;
    default:
      break;
    }
  }
  return typedProperty;
};
tulip.Graph.prototype.getBooleanProperty = function tulip_Graph_prototype_getBooleanProperty(name) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string"]);
  return tulip.BooleanProperty(_Graph_getBooleanProperty(this.cppPointer, name));
};
tulip.Graph.prototype.getLocalBooleanProperty = function tulip_Graph_prototype_getLocalBooleanProperty(name) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string"]);
  return tulip.BooleanProperty(_Graph_getLocalBooleanProperty(this.cppPointer, name));
};
tulip.Graph.prototype.getBooleanVectorProperty = function tulip_Graph_prototype_getBooleanVectorProperty(name) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string"]);
  return tulip.BooleanVectorProperty(_Graph_getBooleanVectorProperty(this.cppPointer, name));
};
tulip.Graph.prototype.getLocalBooleanVectorProperty = function tulip_Graph_prototype_getLocalBooleanVectorProperty(name) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string"]);
  return tulip.BooleanVectorProperty(_Graph_getLocalBooleanVectorProperty(this.cppPointer, name));
};
tulip.Graph.prototype.getColorProperty = function tulip_Graph_prototype_getColorProperty(name) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string"]);
  return tulip.ColorProperty(_Graph_getColorProperty(this.cppPointer, name));
};
tulip.Graph.prototype.getLocalColorProperty = function tulip_Graph_prototype_getLocalColorProperty(name) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string"]);
  return tulip.ColorProperty(_Graph_getLocalColorProperty(this.cppPointer, name));
};
tulip.Graph.prototype.getColorVectorProperty = function tulip_Graph_prototype_getColorVectorProperty(name) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string"]);
  return tulip.ColorVectorProperty(_Graph_getColorVectorProperty(this.cppPointer, name));
};
tulip.Graph.prototype.getLocalColorVectorProperty = function tulip_Graph_prototype_getLocalColorVectorProperty(name) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string"]);
  return tulip.ColorVectorProperty(_Graph_getLocalColorVectorProperty(this.cppPointer, name));
};
tulip.Graph.prototype.getDoubleProperty = function tulip_Graph_prototype_getDoubleProperty(name) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string"]);
  return tulip.DoubleProperty(_Graph_getDoubleProperty(this.cppPointer, name));
};
tulip.Graph.prototype.getLocalDoubleProperty = function tulip_Graph_prototype_getLocalDoubleProperty(name) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string"]);
  return tulip.DoubleProperty(_Graph_getLocalDoubleProperty(this.cppPointer, name));
};
tulip.Graph.prototype.getDoubleVectorProperty = function tulip_Graph_prototype_getDoubleVectorProperty(name) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string"]);
  return tulip.DoubleVectorProperty(_Graph_getDoubleVectorProperty(this.cppPointer, name));
};
tulip.Graph.prototype.getLocalDoubleVectorProperty = function tulip_Graph_prototype_getLocalDoubleVectorProperty(name) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string"]);
  return tulip.DoubleVectorProperty(_Graph_getLocalDoubleVectorProperty(this.cppPointer, name));
};
tulip.Graph.prototype.getIntegerProperty = function tulip_Graph_prototype_getIntegerProperty(name) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string"]);
  return tulip.IntegerProperty(_Graph_getIntegerProperty(this.cppPointer, name));
};
tulip.Graph.prototype.getLocalIntegerProperty = function tulip_Graph_prototype_getLocalIntegerProperty(name) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string"]);
  return tulip.IntegerProperty(_Graph_getLocalIntegerProperty(this.cppPointer, name));
};
tulip.Graph.prototype.getIntegerVectorProperty = function tulip_Graph_prototype_getIntegerVectorProperty(name) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string"]);
  return tulip.IntegerVectorProperty(_Graph_getIntegerVectorProperty(this.cppPointer, name));
};
tulip.Graph.prototype.getLocalIntegerVectorProperty = function tulip_Graph_prototype_getLocalIntegerVectorProperty(name) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string"]);
  return tulip.IntegerVectorProperty(_Graph_getLocalIntegerVectorProperty(this.cppPointer, name));
};
tulip.Graph.prototype.getLayoutProperty = function tulip_Graph_prototype_getLayoutProperty(name) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string"]);
  return tulip.LayoutProperty(_Graph_getLayoutProperty(this.cppPointer, name));
};
tulip.Graph.prototype.getLocalLayoutProperty = function tulip_Graph_prototype_getLocalLayoutProperty(name) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string"]);
  return tulip.LayoutProperty(_Graph_getLocalLayoutProperty(this.cppPointer, name));
};
tulip.Graph.prototype.getCoordVectorProperty = function tulip_Graph_prototype_getCoordVectorProperty(name) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string"]);
  return tulip.CoordVectorProperty(_Graph_getCoordVectorProperty(this.cppPointer, name));
};
tulip.Graph.prototype.getLocalCoordVectorProperty = function tulip_Graph_prototype_getLocalCoordVectorProperty(name) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string"]);
  return tulip.CoordVectorProperty(_Graph_getLocalCoordVectorProperty(this.cppPointer, name));
};
tulip.Graph.prototype.getSizeProperty = function tulip_Graph_prototype_getSizeProperty(name) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string"]);
  return tulip.SizeProperty(_Graph_getSizeProperty(this.cppPointer, name));
};
tulip.Graph.prototype.getLocalSizeProperty = function tulip_Graph_prototype_getLocalSizeProperty(name) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string"]);
  return tulip.SizeProperty(_Graph_getLocalSizeProperty(this.cppPointer, name));
};
tulip.Graph.prototype.getSizeVectorProperty = function tulip_Graph_prototype_getSizeVectorProperty(name) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string"]);
  return tulip.SizeVectorProperty(_Graph_getSizeVectorProperty(this.cppPointer, name));
};
tulip.Graph.prototype.getLocalSizeVectorProperty = function tulip_Graph_prototype_getLocalSizeVectorProperty(name) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string"]);
  return tulip.SizeVectorProperty(_Graph_getLocalSizeVectorProperty(this.cppPointer, name));
};
tulip.Graph.prototype.getStringProperty = function tulip_Graph_prototype_getStringProperty(name) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string"]);
  return tulip.StringProperty(_Graph_getStringProperty(this.cppPointer, name));
};
tulip.Graph.prototype.getLocalStringProperty = function tulip_Graph_prototype_getLocalStringProperty(name) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string"]);
  return tulip.StringProperty(_Graph_getLocalStringProperty(this.cppPointer, name));
};
tulip.Graph.prototype.getStringVectorProperty = function tulip_Graph_prototype_getStringVectorProperty(name) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string"]);
  return tulip.StringVectorProperty(_Graph_getStringVectorProperty(this.cppPointer, name));
};
tulip.Graph.prototype.getLocalStringVectorProperty = function tulip_Graph_prototype_getLocalStringVectorProperty(name) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string"]);
  return tulip.StringVectorProperty(_Graph_getLocalStringVectorProperty(this.cppPointer, name));
};
tulip.Graph.prototype.delLocalProperty = function tulip_Graph_prototype_delLocalProperty(name) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string"], 1);
  _Graph_delLocalProperty(this.cppPointer, name);
};
tulip.Graph.prototype.toJSON = function tulip_Graph_prototype_toJSON() {
  checkWrappedCppPointer(this.cppPointer);
  return _getJSONGraph(this.cppPointer);
};
tulip.Graph.prototype.getNodePropertiesValues = function tulip_Graph_prototype_getNodePropertiesValues(node) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node], 1);
  var ret = {};
  var properties = this.getProperties();
  for (var i = 0 ; i < properties.length ; ++i) {
    var propName = properties[i];
    if (propName == "viewMetaGraph") {
      continue;
    }
    var prop = graph.getProperty(propName);
    ret[propName] = prop.getNodeValue(node);
  }
  return ret;
};
tulip.Graph.prototype.setNodePropertiesValues = function tulip_Graph_prototype_setNodePropertiesValues(node, values) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node, "object"], 2);
  for (var propertyName in values) {
    if (values.hasOwnProperty(propertyName)) {
      var prop = this.getProperty(propertyName);
      if (!prop) {
        console.log("Error : no graph property named '" + propertyName + "'");
      }
      prop.setNodeValue(node, values[propertyName]);
    }
  }
};
tulip.Graph.prototype.setAllNodePropertiesValues = function tulip_Graph_prototype_setAllNodePropertiesValues(values) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["object"], 2);
  for (var propertyName in values) {
    if (values.hasOwnProperty(propertyName)) {
      var prop = this.getProperty(propertyName);
      if (!prop) {
        console.log("Error : no graph property named '" + propertyName + "'");
      }
      prop.setAllNodeValue(values[propertyName]);
    }
  }
};
tulip.Graph.prototype.getEdgePropertiesValues = function tulip_Graph_prototype_getEdgePropertiesValues(edge) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge], 1);
  var ret = {};
  var properties = this.getProperties();
  for (var i = 0 ; i < properties.length ; ++i) {
    var propName = properties[i];
    if (propName == "viewMetaGraph") {
      continue;
    }
    var prop = graph.getProperty(propName);
    ret[propName] = prop.getEdgeValue(edge);
  }
  return ret;
};
tulip.Graph.prototype.setEdgePropertiesValues = function tulip_Graph_prototype_setEdgePropertiesValues(edge, values) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Edge, "object"], 2);
  for (var propertyName in values) {
    if (values.hasOwnProperty(propertyName)) {
      var prop = this.getProperty(propertyName);
      if (!prop) {
        console.log("Error : no graph property named '" + propertyName + "'");
      }
      prop.setEdgeValue(edge, values[propertyName]);
    }
  }
};
tulip.Graph.prototype.setAllEdgePropertiesValues = function tulip_Graph_prototype_setAllEdgePropertiesValues(values) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["object"], 2);
  for (var propertyName in values) {
    if (values.hasOwnProperty(propertyName)) {
      var prop = this.getProperty(propertyName);
      if (!prop) {
        console.log("Error : no graph property named '" + propertyName + "'");
      }
      prop.setAllEdgeValue(values[propertyName]);
    }
  }
};
tulip.Graph.prototype.applyPropertyAlgorithm = function tulip_Graph_prototype_applyPropertyAlgorithm(algorithmName, resultProperty, algoParameters, notifyProgress) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string", tulip.PropertyInterface, "object", "boolean"], 2);
  if (!tulip.propertyAlgorithmExists(algorithmName)) {
    console.log("Error : no Tulip property algorithm named '" + algorithmName + "'");
    return false;
  }
  if (algoParameters === undefined) algoParameters = {};
  if (notifyProgress == undefined) notifyProgress = false;
  return _Graph_applyPropertyAlgorithm(this.cppPointer, algorithmName, resultProperty.cppPointer, JSON.stringify(algoParameters), notifyProgress) > 0;
};

tulip.Graph.prototype.applyBooleanAlgorithm = function tulip_Graph_prototype_applyBooleanAlgorithm(algorithmName, resultProperty, algoParameters, notifyProgress) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string", tulip.BooleanProperty, "object", "boolean"], 2);
  if (!tulip.booleanAlgorithmExists(algorithmName)) {
    console.log("Error : no Tulip boolean algorithm named '" + algorithmName + "'");
    return false;
  }
  if (algoParameters === undefined) algoParameters = {};
  if (notifyProgress == undefined) notifyProgress = false;
  return this.applyPropertyAlgorithm(algorithmName, resultProperty, algoParameters, notifyProgress);
};

tulip.Graph.prototype.applyColorAlgorithm = function tulip_Graph_prototype_applyColorAlgorithm(algorithmName, resultProperty, algoParameters, notifyProgress) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string", tulip.ColorProperty, "object", "boolean"], 2);
  if (!tulip.colorAlgorithmExists(algorithmName)) {
    console.log("Error : no Tulip color algorithm named '" + algorithmName + "'");
    return false;
  }
  if (algoParameters === undefined) algoParameters = {};
  if (notifyProgress == undefined) notifyProgress = false;
  return this.applyPropertyAlgorithm(algorithmName, resultProperty, algoParameters, notifyProgress);
};

tulip.Graph.prototype.applyDoubleAlgorithm = function tulip_Graph_prototype_applyDoubleAlgorithm(algorithmName, resultProperty, algoParameters, notifyProgress) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string", tulip.DoubleProperty, "object", "boolean"], 2);
  if (!tulip.doubleAlgorithmExists(algorithmName)) {
    console.log("Error : no Tulip double algorithm named '" + algorithmName + "'");
    return false;
  }
  if (algoParameters === undefined) algoParameters = {};
  if (notifyProgress == undefined) notifyProgress = false;
  return this.applyPropertyAlgorithm(algorithmName, resultProperty, algoParameters, notifyProgress);
};

tulip.Graph.prototype.applyLayoutAlgorithm = function tulip_Graph_prototype_applyLayoutAlgorithm(algorithmName, resultProperty, algoParameters, notifyProgress) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string", tulip.LayoutProperty, "object", "boolean"], 2);
  if (!tulip.layoutAlgorithmExists(algorithmName)) {
    console.log("Error : no Tulip layout algorithm named '" + algorithmName + "'");
    return false;
  }
  if (algoParameters === undefined) algoParameters = {};
  if (notifyProgress == undefined) notifyProgress = false;
  return this.applyPropertyAlgorithm(algorithmName, resultProperty, algoParameters, notifyProgress);
};

tulip.Graph.prototype.applySizeAlgorithm = function tulip_Graph_prototype_applySizeAlgorithm(algorithmName, resultProperty, algoParameters, notifyProgress) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["string", tulip.SizeProperty, "object", "boolean"], 2);
  if (!tulip.sizeAlgorithmExists(algorithmName)) {
    console.log("Error : no Tulip size algorithm named '" + algorithmName + "'");
    return false;
  }
  if (algoParameters === undefined) algoParameters = {};
  if (notifyProgress == undefined) notifyProgress = false;
  return this.applyPropertyAlgorithm(algorithmName, resultProperty, algoParameters, notifyProgress);
};

tulip.Graph.prototype.push = function tulip_Graph_prototype_push() {
  checkWrappedCppPointer(this.cppPointer);
  _Graph_push(this.cppPointer);
};

tulip.Graph.prototype.pop = function tulip_Graph_prototype_pop() {
  checkWrappedCppPointer(this.cppPointer);
  _Graph_pop(this.cppPointer);
};

tulip.Graph.prototype.setEventsActivated = function tulip_Graph_prototype_setEventsActivated(eventsActivated) {
  checkWrappedCppPointer(this.cppPointer);
  _Graph_setEventsActivated(this.cppPointer, eventsActivated);
};

tulip.Graph.prototype.getAttributes = function tulip_Graph_prototype_getAttributes() {
  checkWrappedCppPointer(this.cppPointer);
  return JSON.parse(_Graph_getAttributesJSON(this.cppPointer));
};

tulip.Graph.prototype.getAttribute = function tulip_Graph_prototype_getAttribute(name) {
  checkWrappedCppPointer(this.cppPointer);
  var attributes = this.getAttributes();
  if (name in attributes) {
    return attributes[name];
  } else {
    return undefined;
  }
};

tulip.Graph.prototype.isMetaNode = function tulip_Graph_prototype_isMetaNode(n) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node], 1);
  return _Graph_isMetaNode(this.cppPointer, n.id) > 0;
};

tulip.Graph.prototype.openMetaNode = function tulip_Graph_prototype_openMetaNode(n) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, [tulip.Node], 1);
  _Graph_openMetaNode(this.cppPointer, n.id);
};

tulip.Graph.prototype.isConnected = function tulip_Graph_prototype_isConnected() {
  checkWrappedCppPointer(this.cppPointer);
  return _Graph_isConnected(this.cppPointer) > 0;
};

if (!nodejs) {

var _computeGraphHullVertices = Module.cwrap('computeGraphHullVertices', null, ['number', 'number']);

tulip.Graph.prototype.computeGraphHullVertices = function(withHoles) {
  if (!withHoles) {
    withHoles = false;
  }
  _computeGraphHullVertices(this.cppPointer, withHoles);
};

}

// ==================================================================================================================

var _ColorScale_newColorScale = Module.cwrap('ColorScale_newColorScale', 'number', []);
var _ColorScale_setColorScale = Module.cwrap('ColorScale_setColorScale', null, ['number', 'number', 'number']);
var _ColorScale_setColorAtPos = Module.cwrap('ColorScale_setColorAtPos', null, ['number', 'number', 'number', 'number', 'number', 'number']);
var _ColorScale_getColorAtPos = Module.cwrap('ColorScale_getColorAtPos', null, ['number', 'number', 'number']);
var _ColorScale_numberOfColors = Module.cwrap('ColorScale_numberOfColors', 'number', ['number']);
var _ColorScale_getOffsets = Module.cwrap('ColorScale_getOffsets', null, ['number', 'number']);
var _ColorScale_getColors = Module.cwrap('ColorScale_getColors', null, ['number', 'number']);

tulip.ColorScale = function tulip_ColorScale() {
  var newObject = createObject(tulip.ColorScale, this);
  tulip.CppObjectWrapper.call(newObject, _ColorScale_newColorScale(), "tlp::ColorScale");
  if (arguments.length == 1) {
    checkArgumentsTypes(arguments, ["array"]);
    checkArrayOfType(arguments[0], tulip.Color);
    newObject.setColorScale(arguments[0]);
  }

  return newObject;
};

tulip.ColorScale.prototype.setColorScale = function tulip_ColorScale_prototype_setColorScale(colors) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["array"], 1);
  checkArrayOfType(colors, tulip.Color, 0);
  var ucharArray = allocArrayInEmHeap(Uint8Array, colors.length * 4);
  for (var i = 0 ; i < colors.length ; ++i) {
    ucharArray[4*i] = colors[i].r;
    ucharArray[4*i+1] = colors[i].g;
    ucharArray[4*i+2] = colors[i].b;
    ucharArray[4*i+3] = colors[i].a;
  }
  _ColorScale_setColorScale(this.cppPointer, ucharArray.byteOffset, colors.length);
  freeArrayInEmHeap(ucharArray);
};

tulip.ColorScale.prototype.setColorAtPos = function tulip_ColorScale_prototype_setColorAtPos(pos, color) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["number", tulip.Color], 2);
  _ColorScale_setColorAtPos(this.cppPointer, pos, color.r, color.g, color.b, color.a);
};

tulip.ColorScale.prototype.getColorAtPos = function tulip_ColorScale_prototype_setColorAtPos(pos) {
  checkWrappedCppPointer(this.cppPointer);
  checkArgumentsTypes(arguments, ["number"], 1);
  var ucharArray = allocArrayInEmHeap(Uint8Array, 4);
  _ColorScale_getColorAtPos(this.cppPointer, pos, ucharArray.byteOffset);
  var ret = tulip.Color(ucharArray[0], ucharArray[1], ucharArray[2], ucharArray[3]);
  freeArrayInEmHeap(ucharArray);
  return ret;
};

tulip.ColorScale.prototype.getColorMap = function() {
  checkWrappedCppPointer(this.cppPointer);
  var nbColors = _ColorScale_numberOfColors(this.cppPointer);
  var floatArray = allocArrayInEmHeap(Float32Array, nbColors);
  var ucharArray = allocArrayInEmHeap(Uint8Array, nbColors*4);
  _ColorScale_getOffsets(this.cppPointer, floatArray.byteOffset);
  _ColorScale_getColors(this.cppPointer, ucharArray.byteOffset);
  var ret = {};
  for (var i = 0 ; i < nbColors ; ++i) {
    ret[floatArray[i]] = tulip.Color(ucharArray[4*i], ucharArray[4*i+1], ucharArray[4*i+2], ucharArray[4*i+3]);
  }
  freeArrayInEmHeap(floatArray);
  freeArrayInEmHeap(ucharArray);
  return ret;
};

// ==================================================================================================================

var _holdObservers = Module.cwrap('holdObservers', null, []);
var _unholdObservers = Module.cwrap('unholdObservers', null, []);

tulip.holdObservers = function() {
  _holdObservers();
};

tulip.unholdObservers = function() {
  _unholdObservers();
};

// ==================================================================================================================

tulip.NodeShape = {
  Circle : 14,
  ChristmasTree : 28,
  Cone : 3,
  Cross : 8,
  Cube : 0,
  CubeOutlined : 1,
  CubeOutlinedTransparent : 9,
  Cylinder : 6,
  Diamond : 5,
  HalfCylinder : 10,
  Hexagon : 13,
  Pentagon : 12,
  Ring : 15,
  RoundedBox : 18,
  Sphere : 2,
  Square : 4,
  Triangle : 11,
  Star : 19,
  FontAwesomeIcon : 20,
  MaterialDesignIcon : 21
};

tulip.EdgeShape  = {
  Polyline : 0,
  BezierCurve : 4,
  CatmullRomCurve : 8,
  CubicBSplineCurve : 16
};

tulip.EdgeExtremityShape = {
  None : 1,
  Arrow : 50,
  Circle : 14,
  Cone : 3,
  Cross : 8,
  Cube : 0,
  CubeOutlinedTransparent : 9,
  Cylinder : 6,
  Diamond : 5,
  GlowSphere : 16,
  Hexagon : 13,
  Pentagon : 12,
  Ring : 15,
  Sphere : 2 ,
  Square : 4,
  Star : 19,
  FontAwesomeIcon : 20,
  MaterialDesignIcon : 21
};

tulip.FontAwesomeIcon = {
  Adjust : "adjust",
  Adn : "adn",
  AlignCenter : "align-center",
  AlignJustify : "align-justify",
  AlignLeft : "align-left",
  AlignRight : "align-right",
  Amazon : "amazon",
  Ambulance : "ambulance",
  Anchor : "anchor",
  Android : "android",
  Angellist : "angellist",
  AngleDoubleDown : "angle-double-down",
  AngleDoubleLeft : "angle-double-left",
  AngleDoubleRight : "angle-double-right",
  AngleDoubleUp : "angle-double-up",
  AngleDown : "angle-down",
  AngleLeft : "angle-left",
  AngleRight : "angle-right",
  AngleUp : "angle-up",
  Apple : "apple",
  Archive : "archive",
  AreaChart : "area-chart",
  ArrowCircleDown : "arrow-circle-down",
  ArrowCircleLeft : "arrow-circle-left",
  ArrowCircleODown : "arrow-circle-o-down",
  ArrowCircleOLeft : "arrow-circle-o-left",
  ArrowCircleORight : "arrow-circle-o-right",
  ArrowCircleOUp : "arrow-circle-o-up",
  ArrowCircleRight : "arrow-circle-right",
  ArrowCircleUp : "arrow-circle-up",
  ArrowDown : "arrow-down",
  ArrowLeft : "arrow-left",
  ArrowRight : "arrow-right",
  ArrowUp : "arrow-up",
  Arrows : "arrows",
  ArrowsAlt : "arrows-alt",
  ArrowsH : "arrows-h",
  ArrowsV : "arrows-v",
  Asterisk : "asterisk",
  At : "at",
  Automobile : "automobile",
  Backward : "backward",
  BalanceScale : "balance-scale",
  Ban : "ban",
  Bank : "bank",
  BarChart : "bar-chart",
  BarChartO : "bar-chart-o",
  Barcode : "barcode",
  Bars : "bars",
  Battery0 : "battery-0",
  Battery1 : "battery-1",
  Battery2 : "battery-2",
  Battery3 : "battery-3",
  Battery4 : "battery-4",
  BatteryEmpty : "battery-empty",
  BatteryFull : "battery-full",
  BatteryHalf : "battery-half",
  BatteryQuarter : "battery-quarter",
  BatteryThreeQuarters : "battery-three-quarters",
  Bed : "bed",
  Beer : "beer",
  Behance : "behance",
  BehanceSquare : "behance-square",
  Bell : "bell",
  BellO : "bell-o",
  BellSlash : "bell-slash",
  BellSlashO : "bell-slash-o",
  Bicycle : "bicycle",
  Binoculars : "binoculars",
  BirthdayCake : "birthday-cake",
  Bitbucket : "bitbucket",
  BitbucketSquare : "bitbucket-square",
  Bitcoin : "bitcoin",
  BlackTie : "black-tie",
  Bluetooth : "bluetooth",
  BluetoothB : "bluetooth-b",
  Bold : "bold",
  Bolt : "bolt",
  Bomb : "bomb",
  Book : "book",
  Bookmark : "bookmark",
  BookmarkO : "bookmark-o",
  Briefcase : "briefcase",
  Btc : "btc",
  Bug : "bug",
  Building : "building",
  BuildingO : "building-o",
  Bullhorn : "bullhorn",
  Bullseye : "bullseye",
  Bus : "bus",
  Buysellads : "buysellads",
  Cab : "cab",
  Calculator : "calculator",
  Calendar : "calendar",
  CalendarCheckO : "calendar-check-o",
  CalendarMinusO : "calendar-minus-o",
  CalendarO : "calendar-o",
  CalendarPlusO : "calendar-plus-o",
  CalendarTimesO : "calendar-times-o",
  Camera : "camera",
  CameraRetro : "camera-retro",
  Car : "car",
  CaretDown : "caret-down",
  CaretLeft : "caret-left",
  CaretRight : "caret-right",
  CaretSquareODown : "caret-square-o-down",
  CaretSquareOLeft : "caret-square-o-left",
  CaretSquareORight : "caret-square-o-right",
  CaretSquareOUp : "caret-square-o-up",
  CaretUp : "caret-up",
  CartArrowDown : "cart-arrow-down",
  CartPlus : "cart-plus",
  Cc : "cc",
  CcAmex : "cc-amex",
  CcDinersClub : "cc-diners-club",
  CcDiscover : "cc-discover",
  CcJcb : "cc-jcb",
  CcMastercard : "cc-mastercard",
  CcPaypal : "cc-paypal",
  CcStripe : "cc-stripe",
  CcVisa : "cc-visa",
  Certificate : "certificate",
  Chain : "chain",
  ChainBroken : "chain-broken",
  Check : "check",
  CheckCircle : "check-circle",
  CheckCircleO : "check-circle-o",
  CheckSquare : "check-square",
  CheckSquareO : "check-square-o",
  ChevronCircleDown : "chevron-circle-down",
  ChevronCircleLeft : "chevron-circle-left",
  ChevronCircleRight : "chevron-circle-right",
  ChevronCircleUp : "chevron-circle-up",
  ChevronDown : "chevron-down",
  ChevronLeft : "chevron-left",
  ChevronRight : "chevron-right",
  ChevronUp : "chevron-up",
  Child : "child",
  Chrome : "chrome",
  Circle : "circle",
  CircleO : "circle-o",
  CircleONotch : "circle-o-notch",
  CircleThin : "circle-thin",
  Clipboard : "clipboard",
  ClockO : "clock-o",
  Clone : "clone",
  Close : "close",
  Cloud : "cloud",
  CloudDownload : "cloud-download",
  CloudUpload : "cloud-upload",
  Cny : "cny",
  Code : "code",
  CodeFork : "code-fork",
  Codepen : "codepen",
  Codiepie : "codiepie",
  Coffee : "coffee",
  Cog : "cog",
  Cogs : "cogs",
  Columns : "columns",
  Comment : "comment",
  CommentO : "comment-o",
  Commenting : "commenting",
  CommentingO : "commenting-o",
  Comments : "comments",
  CommentsO : "comments-o",
  Compass : "compass",
  Compress : "compress",
  Connectdevelop : "connectdevelop",
  Contao : "contao",
  Copy : "copy",
  Copyright : "copyright",
  CreativeCommons : "creative-commons",
  CreditCard : "credit-card",
  CreditCardAlt : "credit-card-alt",
  Crop : "crop",
  Crosshairs : "crosshairs",
  Css3 : "css3",
  Cube : "cube",
  Cubes : "cubes",
  Cut : "cut",
  Cutlery : "cutlery",
  Dashboard : "dashboard",
  Dashcube : "dashcube",
  Database : "database",
  Dedent : "dedent",
  Delicious : "delicious",
  Desktop : "desktop",
  Deviantart : "deviantart",
  Diamond : "diamond",
  Digg : "digg",
  Dollar : "dollar",
  DotCircleO : "dot-circle-o",
  Download : "download",
  Dribbble : "dribbble",
  Dropbox : "dropbox",
  Drupal : "drupal",
  Edge : "edge",
  Edit : "edit",
  Eject : "eject",
  EllipsisH : "ellipsis-h",
  EllipsisV : "ellipsis-v",
  Empire : "empire",
  Envelope : "envelope",
  EnvelopeO : "envelope-o",
  EnvelopeSquare : "envelope-square",
  Eraser : "eraser",
  Eur : "eur",
  Euro : "euro",
  Exchange : "exchange",
  Exclamation : "exclamation",
  ExclamationCircle : "exclamation-circle",
  ExclamationTriangle : "exclamation-triangle",
  Expand : "expand",
  Expeditedssl : "expeditedssl",
  ExternalLink : "external-link",
  ExternalLinkSquare : "external-link-square",
  Eye : "eye",
  EyeSlash : "eye-slash",
  Eyedropper : "eyedropper",
  Facebook : "facebook",
  FacebookF : "facebook-f",
  FacebookOfficial : "facebook-official",
  FacebookSquare : "facebook-square",
  FastBackward : "fast-backward",
  FastForward : "fast-forward",
  Fax : "fax",
  Feed : "feed",
  Female : "female",
  FighterJet : "fighter-jet",
  File : "file",
  FileArchiveO : "file-archive-o",
  FileAudioO : "file-audio-o",
  FileCodeO : "file-code-o",
  FileExcelO : "file-excel-o",
  FileImageO : "file-image-o",
  FileMovieO : "file-movie-o",
  FileO : "file-o",
  FilePdfO : "file-pdf-o",
  FilePhotoO : "file-photo-o",
  FilePictureO : "file-picture-o",
  FilePowerpointO : "file-powerpoint-o",
  FileSoundO : "file-sound-o",
  FileText : "file-text",
  FileTextO : "file-text-o",
  FileVideoO : "file-video-o",
  FileWordO : "file-word-o",
  FileZipO : "file-zip-o",
  FilesO : "files-o",
  Film : "film",
  Filter : "filter",
  Fire : "fire",
  FireExtinguisher : "fire-extinguisher",
  Firefox : "firefox",
  Flag : "flag",
  FlagCheckered : "flag-checkered",
  FlagO : "flag-o",
  Flash : "flash",
  Flask : "flask",
  Flickr : "flickr",
  FloppyO : "floppy-o",
  Folder : "folder",
  FolderO : "folder-o",
  FolderOpen : "folder-open",
  FolderOpenO : "folder-open-o",
  Font : "font",
  Fonticons : "fonticons",
  FortAwesome : "fort-awesome",
  Forumbee : "forumbee",
  Forward : "forward",
  Foursquare : "foursquare",
  FrownO : "frown-o",
  FutbolO : "futbol-o",
  Gamepad : "gamepad",
  Gavel : "gavel",
  Gbp : "gbp",
  Ge : "ge",
  Gear : "gear",
  Gears : "gears",
  Genderless : "genderless",
  GetPocket : "get-pocket",
  Gg : "gg",
  GgCircle : "gg-circle",
  Gift : "gift",
  Git : "git",
  GitSquare : "git-square",
  Github : "github",
  GithubAlt : "github-alt",
  GithubSquare : "github-square",
  Gittip : "gittip",
  Glass : "glass",
  Globe : "globe",
  Google : "google",
  GooglePlus : "google-plus",
  GooglePlusSquare : "google-plus-square",
  GoogleWallet : "google-wallet",
  GraduationCap : "graduation-cap",
  Gratipay : "gratipay",
  Group : "group",
  HSquare : "h-square",
  HackerNews : "hacker-news",
  HandGrabO : "hand-grab-o",
  HandLizardO : "hand-lizard-o",
  HandODown : "hand-o-down",
  HandOLeft : "hand-o-left",
  HandORight : "hand-o-right",
  HandOUp : "hand-o-up",
  HandPaperO : "hand-paper-o",
  HandPeaceO : "hand-peace-o",
  HandPointerO : "hand-pointer-o",
  HandRockO : "hand-rock-o",
  HandScissorsO : "hand-scissors-o",
  HandSpockO : "hand-spock-o",
  HandStopO : "hand-stop-o",
  Hashtag : "hashtag",
  HddO : "hdd-o",
  Header : "header",
  Headphones : "headphones",
  Heart : "heart",
  HeartO : "heart-o",
  Heartbeat : "heartbeat",
  History : "history",
  Home : "home",
  HospitalO : "hospital-o",
  Hotel : "hotel",
  Hourglass : "hourglass",
  Hourglass1 : "hourglass-1",
  Hourglass2 : "hourglass-2",
  Hourglass3 : "hourglass-3",
  HourglassEnd : "hourglass-end",
  HourglassHalf : "hourglass-half",
  HourglassO : "hourglass-o",
  HourglassStart : "hourglass-start",
  Houzz : "houzz",
  Html5 : "html5",
  ICursor : "i-cursor",
  Ils : "ils",
  Image : "image",
  Inbox : "inbox",
  Indent : "indent",
  Industry : "industry",
  Info : "info",
  InfoCircle : "info-circle",
  Inr : "inr",
  Instagram : "instagram",
  Institution : "institution",
  InternetExplorer : "internet-explorer",
  Intersex : "intersex",
  Ioxhost : "ioxhost",
  Italic : "italic",
  Joomla : "joomla",
  Jpy : "jpy",
  Jsfiddle : "jsfiddle",
  Key : "key",
  KeyboardO : "keyboard-o",
  Krw : "krw",
  Language : "language",
  Laptop : "laptop",
  Lastfm : "lastfm",
  LastfmSquare : "lastfm-square",
  Leaf : "leaf",
  Leanpub : "leanpub",
  Legal : "legal",
  LemonO : "lemon-o",
  LevelDown : "level-down",
  LevelUp : "level-up",
  LifeBouy : "life-bouy",
  LifeBuoy : "life-buoy",
  LifeRing : "life-ring",
  LifeSaver : "life-saver",
  LightbulbO : "lightbulb-o",
  LineChart : "line-chart",
  Link : "link",
  Linkedin : "linkedin",
  LinkedinSquare : "linkedin-square",
  Linux : "linux",
  List : "list",
  ListAlt : "list-alt",
  ListOl : "list-ol",
  ListUl : "list-ul",
  LocationArrow : "location-arrow",
  Lock : "lock",
  LongArrowDown : "long-arrow-down",
  LongArrowLeft : "long-arrow-left",
  LongArrowRight : "long-arrow-right",
  LongArrowUp : "long-arrow-up",
  Magic : "magic",
  Magnet : "magnet",
  MailForward : "mail-forward",
  MailReply : "mail-reply",
  MailReplyAll : "mail-reply-all",
  Male : "male",
  Map : "map",
  MapMarker : "map-marker",
  MapO : "map-o",
  MapPin : "map-pin",
  MapSigns : "map-signs",
  Mars : "mars",
  MarsDouble : "mars-double",
  MarsStroke : "mars-stroke",
  MarsStrokeH : "mars-stroke-h",
  MarsStrokeV : "mars-stroke-v",
  Maxcdn : "maxcdn",
  Meanpath : "meanpath",
  Medium : "medium",
  Medkit : "medkit",
  MehO : "meh-o",
  Mercury : "mercury",
  Microphone : "microphone",
  MicrophoneSlash : "microphone-slash",
  Minus : "minus",
  MinusCircle : "minus-circle",
  MinusSquare : "minus-square",
  MinusSquareO : "minus-square-o",
  Mixcloud : "mixcloud",
  Mobile : "mobile",
  MobilePhone : "mobile-phone",
  Modx : "modx",
  Money : "money",
  MoonO : "moon-o",
  MortarBoard : "mortar-board",
  Motorcycle : "motorcycle",
  MousePointer : "mouse-pointer",
  Music : "music",
  Navicon : "navicon",
  Neuter : "neuter",
  NewspaperO : "newspaper-o",
  ObjectGroup : "object-group",
  ObjectUngroup : "object-ungroup",
  Odnoklassniki : "odnoklassniki",
  OdnoklassnikiSquare : "odnoklassniki-square",
  Opencart : "opencart",
  Openid : "openid",
  Opera : "opera",
//  OptinMonster : "optin-monster",
  Outdent : "outdent",
  Pagelines : "pagelines",
  PaintBrush : "paint-brush",
  PaperPlane : "paper-plane",
  PaperPlaneO : "paper-plane-o",
  Paperclip : "paperclip",
  Paragraph : "paragraph",
  Paste : "paste",
  Pause : "pause",
  PauseCircle : "pause-circle",
  PauseCircleO : "pause-circle-o",
  Paw : "paw",
  Paypal : "paypal",
  Pencil : "pencil",
  PencilSquare : "pencil-square",
  PencilSquareO : "pencil-square-o",
  Percent : "percent",
  Phone : "phone",
  PhoneSquare : "phone-square",
  Photo : "photo",
  PictureO : "picture-o",
  PieChart : "pie-chart",
  PiedPiper : "pied-piper",
  PiedPiperAlt : "pied-piper-alt",
  Pinterest : "pinterest",
  PinterestP : "pinterest-p",
  PinterestSquare : "pinterest-square",
  Plane : "plane",
  Play : "play",
  PlayCircle : "play-circle",
  PlayCircleO : "play-circle-o",
  Plug : "plug",
  Plus : "plus",
  PlusCircle : "plus-circle",
  PlusSquare : "plus-square",
  PlusSquareO : "plus-square-o",
  PowerOff : "power-off",
  Print : "print",
  ProductHunt : "product-hunt",
  PuzzlePiece : "puzzle-piece",
  Px500 : "500px",
  Qq : "qq",
  Qrcode : "qrcode",
  Question : "question",
  QuestionCircle : "question-circle",
  QuoteLeft : "quote-left",
  QuoteRight : "quote-right",
  Ra : "ra",
  Random : "random",
  Rebel : "rebel",
  Recycle : "recycle",
  Reddit : "reddit",
  RedditAlien : "reddit-alien",
  RedditSquare : "reddit-square",
  Refresh : "refresh",
  Registered : "registered",
  Remove : "remove",
  Renren : "renren",
  Reorder : "reorder",
  Repeat : "repeat",
  Reply : "reply",
  ReplyAll : "reply-all",
  Retweet : "retweet",
  Rmb : "rmb",
  Road : "road",
  Rocket : "rocket",
  RotateLeft : "rotate-left",
  RotateRight : "rotate-right",
  Rouble : "rouble",
  Rss : "rss",
  RssSquare : "rss-square",
  Rub : "rub",
  Ruble : "ruble",
  Rupee : "rupee",
  Safari : "safari",
  Save : "save",
  Scissors : "scissors",
  Scribd : "scribd",
  Search : "search",
  SearchMinus : "search-minus",
  SearchPlus : "search-plus",
  Sellsy : "sellsy",
  Send : "send",
  SendO : "send-o",
  Server : "server",
  Share : "share",
  ShareAlt : "share-alt",
  ShareAltSquare : "share-alt-square",
  ShareSquare : "share-square",
  ShareSquareO : "share-square-o",
  Shekel : "shekel",
  Sheqel : "sheqel",
  Shield : "shield",
  Ship : "ship",
  Shirtsinbulk : "shirtsinbulk",
  ShoppingBag : "shopping-bag",
  ShoppingBasket : "shopping-basket",
  ShoppingCart : "shopping-cart",
  SignIn : "sign-in",
  SignOut : "sign-out",
  Signal : "signal",
  Simplybuilt : "simplybuilt",
  Sitemap : "sitemap",
  Skyatlas : "skyatlas",
  Skype : "skype",
  Slack : "slack",
  Sliders : "sliders",
  Slideshare : "slideshare",
  SmileO : "smile-o",
  SoccerBallO : "soccer-ball-o",
  Sort : "sort",
  SortAlphaAsc : "sort-alpha-asc",
  SortAlphaDesc : "sort-alpha-desc",
  SortAmountAsc : "sort-amount-asc",
  SortAmountDesc : "sort-amount-desc",
  SortAsc : "sort-asc",
  SortDesc : "sort-desc",
  SortDown : "sort-down",
  SortNumericAsc : "sort-numeric-asc",
  SortNumericDesc : "sort-numeric-desc",
  SortUp : "sort-up",
  Soundcloud : "soundcloud",
  SpaceShuttle : "space-shuttle",
  Spinner : "spinner",
  Spoon : "spoon",
  Spotify : "spotify",
  Square : "square",
  SquareO : "square-o",
  StackExchange : "stack-exchange",
  StackOverflow : "stack-overflow",
  Star : "star",
  StarHalf : "star-half",
  StarHalfEmpty : "star-half-empty",
  StarHalfFull : "star-half-full",
  StarHalfO : "star-half-o",
  StarO : "star-o",
  Steam : "steam",
  SteamSquare : "steam-square",
  StepBackward : "step-backward",
  StepForward : "step-forward",
  Stethoscope : "stethoscope",
  StickyNote : "sticky-note",
  StickyNoteO : "sticky-note-o",
  Stop : "stop",
  StopCircle : "stop-circle",
  StopCircleO : "stop-circle-o",
  StreetView : "street-view",
  Strikethrough : "strikethrough",
  Stumbleupon : "stumbleupon",
  StumbleuponCircle : "stumbleupon-circle",
  Subscript : "subscript",
  Subway : "subway",
  Suitcase : "suitcase",
  SunO : "sun-o",
  Superscript : "superscript",
  Support : "support",
  Table : "table",
  Tablet : "tablet",
  Tachometer : "tachometer",
  Tag : "tag",
  Tags : "tags",
  Tasks : "tasks",
  Taxi : "taxi",
  Television : "television",
  TencentWeibo : "tencent-weibo",
  Terminal : "terminal",
  TextHeight : "text-height",
  TextWidth : "text-width",
  Th : "th",
  ThLarge : "th-large",
  ThList : "th-list",
  ThumbTack : "thumb-tack",
  ThumbsDown : "thumbs-down",
  ThumbsODown : "thumbs-o-down",
  ThumbsOUp : "thumbs-o-up",
  ThumbsUp : "thumbs-up",
  Ticket : "ticket",
  Times : "times",
  TimesCircle : "times-circle",
  TimesCircleO : "times-circle-o",
  Tint : "tint",
  ToggleDown : "toggle-down",
  ToggleLeft : "toggle-left",
  ToggleOff : "toggle-off",
  ToggleOn : "toggle-on",
  ToggleRight : "toggle-right",
  ToggleUp : "toggle-up",
  Trademark : "trademark",
  Train : "train",
  Transgender : "transgender",
  TransgenderAlt : "transgender-alt",
  Trash : "trash",
  TrashO : "trash-o",
  Tree : "tree",
  Trello : "trello",
  Tripadvisor : "tripadvisor",
  Trophy : "trophy",
  Truck : "truck",
  Try : "try",
  Tty : "tty",
  Tumblr : "tumblr",
  TumblrSquare : "tumblr-square",
  TurkishLira : "turkish-lira",
  Tv : "tv",
  Twitch : "twitch",
  Twitter : "twitter",
  TwitterSquare : "twitter-square",
  Umbrella : "umbrella",
  Underline : "underline",
  Undo : "undo",
  University : "university",
  Unlink : "unlink",
  Unlock : "unlock",
  UnlockAlt : "unlock-alt",
  Unsorted : "unsorted",
  Upload : "upload",
  Usb : "usb",
  Usd : "usd",
  User : "user",
  UserMd : "user-md",
  UserPlus : "user-plus",
  UserSecret : "user-secret",
  UserTimes : "user-times",
  Users : "users",
  Venus : "venus",
  VenusDouble : "venus-double",
  VenusMars : "venus-mars",
  Viacoin : "viacoin",
  VideoCamera : "video-camera",
  Vimeo : "vimeo",
  VimeoSquare : "vimeo-square",
  Vine : "vine",
  Vk : "vk",
  VolumeDown : "volume-down",
  VolumeOff : "volume-off",
  VolumeUp : "volume-up",
  Warning : "warning",
  Wechat : "wechat",
  Weibo : "weibo",
  Weixin : "weixin",
  Whatsapp : "whatsapp",
  Wheelchair : "wheelchair",
  Wifi : "wifi",
  WikipediaW : "wikipedia-w",
  Windows : "windows",
  Won : "won",
  Wordpress : "wordpress",
  Wrench : "wrench",
  Xing : "xing",
  XingSquare : "xing-square",
  YCombinator : "y-combinator",
  YCombinatorSquare : "y-combinator-square",
  Yahoo : "yahoo",
  Yc : "yc",
  YcSquare : "yc-square",
  Yelp : "yelp",
  Yen : "yen",
  Youtube : "youtube",
  YoutubePlay : "youtube-play",
  YoutubeSquare : "youtube-square"

};

tulip.MaterialDesignIcons = {
  AccessPoint : "access-point",
  AccessPointNetwork : "access-point-network",
  Account : "account",
  AccountAlert : "account-alert",
  AccountBox : "account-box",
  AccountBoxOutline : "account-box-outline",
  AccountCardDetails : "account-card-details",
  AccountCheck : "account-check",
  AccountCircle : "account-circle",
  AccountConvert : "account-convert",
  AccountKey : "account-key",
  AccountLocation : "account-location",
  AccountMinus : "account-minus",
  AccountMultiple : "account-multiple",
  AccountMultipleMinus : "account-multiple-minus",
  AccountMultipleOutline : "account-multiple-outline",
  AccountMultiplePlus : "account-multiple-plus",
  AccountNetwork : "account-network",
  AccountOff : "account-off",
  AccountOutline : "account-outline",
  AccountPlus : "account-plus",
  AccountRemove : "account-remove",
  AccountSearch : "account-search",
  AccountSettings : "account-settings",
  AccountSettingsVariant : "account-settings-variant",
  AccountStar : "account-star",
  AccountStarVariant : "account-star-variant",
  AccountSwitch : "account-switch",
  Adjust : "adjust",
  AirConditioner : "air-conditioner",
  Airballoon : "airballoon",
  Airplane : "airplane",
  AirplaneLanding : "airplane-landing",
  AirplaneOff : "airplane-off",
  AirplaneTakeoff : "airplane-takeoff",
  Airplay : "airplay",
  Alarm : "alarm",
  AlarmCheck : "alarm-check",
  AlarmMultiple : "alarm-multiple",
  AlarmOff : "alarm-off",
  AlarmPlus : "alarm-plus",
  AlarmSnooze : "alarm-snooze",
  Album : "album",
  Alert : "alert",
  AlertBox : "alert-box",
  AlertCircle : "alert-circle",
  AlertCircleOutline : "alert-circle-outline",
  AlertOctagon : "alert-octagon",
  AlertOutline : "alert-outline",
  Alpha : "alpha",
  Alphabetical : "alphabetical",
  Altimeter : "altimeter",
  Amazon : "amazon",
  AmazonClouddrive : "amazon-clouddrive",
  Ambulance : "ambulance",
  Amplifier : "amplifier",
  Anchor : "anchor",
  Android : "android",
  AndroidDebugBridge : "android-debug-bridge",
  AndroidStudio : "android-studio",
  Angular : "angular",
  Animation : "animation",
  Apple : "apple",
  AppleFinder : "apple-finder",
  AppleIos : "apple-ios",
  AppleKeyboardCaps : "apple-keyboard-caps",
  AppleKeyboardCommand : "apple-keyboard-command",
  AppleKeyboardControl : "apple-keyboard-control",
  AppleKeyboardOption : "apple-keyboard-option",
  AppleKeyboardShift : "apple-keyboard-shift",
  AppleMobileme : "apple-mobileme",
  AppleSafari : "apple-safari",
  Application : "application",
  Appnet : "appnet",
  Apps : "apps",
  Archive : "archive",
  ArrangeBringForward : "arrange-bring-forward",
  ArrangeBringToFront : "arrange-bring-to-front",
  ArrangeSendBackward : "arrange-send-backward",
  ArrangeSendToBack : "arrange-send-to-back",
  ArrowAll : "arrow-all",
  ArrowBottomLeft : "arrow-bottom-left",
  ArrowBottomRight : "arrow-bottom-right",
  ArrowCompress : "arrow-compress",
  ArrowCompressAll : "arrow-compress-all",
  ArrowDown : "arrow-down",
  ArrowDownBold : "arrow-down-bold",
  ArrowDownBoldCircle : "arrow-down-bold-circle",
  ArrowDownBoldCircleOutline : "arrow-down-bold-circle-outline",
  ArrowDownBoldHexagonOutline : "arrow-down-bold-hexagon-outline",
  ArrowDownDropCircle : "arrow-down-drop-circle",
  ArrowDownDropCircleOutline : "arrow-down-drop-circle-outline",
  ArrowExpand : "arrow-expand",
  ArrowExpandAll : "arrow-expand-all",
  ArrowLeft : "arrow-left",
  ArrowLeftBold : "arrow-left-bold",
  ArrowLeftBoldCircle : "arrow-left-bold-circle",
  ArrowLeftBoldCircleOutline : "arrow-left-bold-circle-outline",
  ArrowLeftBoldHexagonOutline : "arrow-left-bold-hexagon-outline",
  ArrowLeftDropCircle : "arrow-left-drop-circle",
  ArrowLeftDropCircleOutline : "arrow-left-drop-circle-outline",
  ArrowRight : "arrow-right",
  ArrowRightBold : "arrow-right-bold",
  ArrowRightBoldCircle : "arrow-right-bold-circle",
  ArrowRightBoldCircleOutline : "arrow-right-bold-circle-outline",
  ArrowRightBoldHexagonOutline : "arrow-right-bold-hexagon-outline",
  ArrowRightDropCircle : "arrow-right-drop-circle",
  ArrowRightDropCircleOutline : "arrow-right-drop-circle-outline",
  ArrowTopLeft : "arrow-top-left",
  ArrowTopRight : "arrow-top-right",
  ArrowUp : "arrow-up",
  ArrowUpBold : "arrow-up-bold",
  ArrowUpBoldCircle : "arrow-up-bold-circle",
  ArrowUpBoldCircleOutline : "arrow-up-bold-circle-outline",
  ArrowUpBoldHexagonOutline : "arrow-up-bold-hexagon-outline",
  ArrowUpDropCircle : "arrow-up-drop-circle",
  ArrowUpDropCircleOutline : "arrow-up-drop-circle-outline",
  Assistant : "assistant",
  At : "at",
  Attachment : "attachment",
  Audiobook : "audiobook",
  AutoFix : "auto-fix",
  AutoUpload : "auto-upload",
  Autorenew : "autorenew",
  AvTimer : "av-timer",
  Baby : "baby",
  BabyBuggy : "baby-buggy",
  Backburger : "backburger",
  Backspace : "backspace",
  BackupRestore : "backup-restore",
  Bandcamp : "bandcamp",
  Bank : "bank",
  Barcode : "barcode",
  BarcodeScan : "barcode-scan",
  Barley : "barley",
  Barrel : "barrel",
  Basecamp : "basecamp",
  Basket : "basket",
  BasketFill : "basket-fill",
  BasketUnfill : "basket-unfill",
  Battery : "battery",
  Battery10 : "battery-10",
  Battery20 : "battery-20",
  Battery30 : "battery-30",
  Battery40 : "battery-40",
  Battery50 : "battery-50",
  Battery60 : "battery-60",
  Battery70 : "battery-70",
  Battery80 : "battery-80",
  Battery90 : "battery-90",
  BatteryAlert : "battery-alert",
  BatteryCharging : "battery-charging",
  BatteryCharging100 : "battery-charging-100",
  BatteryCharging20 : "battery-charging-20",
  BatteryCharging30 : "battery-charging-30",
  BatteryCharging40 : "battery-charging-40",
  BatteryCharging60 : "battery-charging-60",
  BatteryCharging80 : "battery-charging-80",
  BatteryCharging90 : "battery-charging-90",
  BatteryMinus : "battery-minus",
  BatteryNegative : "battery-negative",
  BatteryOutline : "battery-outline",
  BatteryPlus : "battery-plus",
  BatteryPositive : "battery-positive",
  BatteryUnknown : "battery-unknown",
  Beach : "beach",
  Beaker : "beaker",
  Beats : "beats",
  Beer : "beer",
  Behance : "behance",
  Bell : "bell",
  BellOff : "bell-off",
  BellOutline : "bell-outline",
  BellPlus : "bell-plus",
  BellRing : "bell-ring",
  BellRingOutline : "bell-ring-outline",
  BellSleep : "bell-sleep",
  Beta : "beta",
  Bible : "bible",
  Bike : "bike",
  Bing : "bing",
  Binoculars : "binoculars",
  Bio : "bio",
  Biohazard : "biohazard",
  Bitbucket : "bitbucket",
  BlackMesa : "black-mesa",
  Blackberry : "blackberry",
  Blender : "blender",
  Blinds : "blinds",
  BlockHelper : "block-helper",
  Blogger : "blogger",
  Bluetooth : "bluetooth",
  BluetoothAudio : "bluetooth-audio",
  BluetoothConnect : "bluetooth-connect",
  BluetoothOff : "bluetooth-off",
  BluetoothSettings : "bluetooth-settings",
  BluetoothTransfer : "bluetooth-transfer",
  Blur : "blur",
  BlurLinear : "blur-linear",
  BlurOff : "blur-off",
  BlurRadial : "blur-radial",
  Bomb : "bomb",
  Bone : "bone",
  Book : "book",
  BookMinus : "book-minus",
  BookMultiple : "book-multiple",
  BookMultipleVariant : "book-multiple-variant",
  BookOpen : "book-open",
  BookOpenPageVariant : "book-open-page-variant",
  BookOpenVariant : "book-open-variant",
  BookPlus : "book-plus",
  BookVariant : "book-variant",
  Bookmark : "bookmark",
  BookmarkCheck : "bookmark-check",
  BookmarkMusic : "bookmark-music",
  BookmarkOutline : "bookmark-outline",
  BookmarkPlus : "bookmark-plus",
  BookmarkPlusOutline : "bookmark-plus-outline",
  BookmarkRemove : "bookmark-remove",
  Boombox : "boombox",
  BorderAll : "border-all",
  BorderBottom : "border-bottom",
  BorderColor : "border-color",
  BorderHorizontal : "border-horizontal",
  BorderInside : "border-inside",
  BorderLeft : "border-left",
  BorderNone : "border-none",
  BorderOutside : "border-outside",
  BorderRight : "border-right",
  BorderStyle : "border-style",
  BorderTop : "border-top",
  BorderVertical : "border-vertical",
  BowTie : "bow-tie",
  Bowl : "bowl",
  Bowling : "bowling",
  Box : "box",
  BoxCutter : "box-cutter",
  BoxShadow : "box-shadow",
  Bridge : "bridge",
  Briefcase : "briefcase",
  BriefcaseCheck : "briefcase-check",
  BriefcaseDownload : "briefcase-download",
  BriefcaseUpload : "briefcase-upload",
  Brightness1 : "brightness-1",
  Brightness2 : "brightness-2",
  Brightness3 : "brightness-3",
  Brightness4 : "brightness-4",
  Brightness5 : "brightness-5",
  Brightness6 : "brightness-6",
  Brightness7 : "brightness-7",
  BrightnessAuto : "brightness-auto",
  Broom : "broom",
  Brush : "brush",
  Buffer : "buffer",
  Bug : "bug",
  BulletinBoard : "bulletin-board",
  Bullhorn : "bullhorn",
  Bullseye : "bullseye",
  BurstMode : "burst-mode",
  Bus : "bus",
  Cached : "cached",
  Cake : "cake",
  CakeLayered : "cake-layered",
  CakeVariant : "cake-variant",
  Calculator : "calculator",
  Calendar : "calendar",
  CalendarBlank : "calendar-blank",
  CalendarCheck : "calendar-check",
  CalendarClock : "calendar-clock",
  CalendarMultiple : "calendar-multiple",
  CalendarMultipleCheck : "calendar-multiple-check",
  CalendarPlus : "calendar-plus",
  CalendarQuestion : "calendar-question",
  CalendarRange : "calendar-range",
  CalendarRemove : "calendar-remove",
  CalendarText : "calendar-text",
  CalendarToday : "calendar-today",
  CallMade : "call-made",
  CallMerge : "call-merge",
  CallMissed : "call-missed",
  CallReceived : "call-received",
  CallSplit : "call-split",
  Camcorder : "camcorder",
  CamcorderBox : "camcorder-box",
  CamcorderBoxOff : "camcorder-box-off",
  CamcorderOff : "camcorder-off",
  Camera : "camera",
  CameraBurst : "camera-burst",
  CameraEnhance : "camera-enhance",
  CameraFront : "camera-front",
  CameraFrontVariant : "camera-front-variant",
  CameraIris : "camera-iris",
  CameraOff : "camera-off",
  CameraPartyMode : "camera-party-mode",
  CameraRear : "camera-rear",
  CameraRearVariant : "camera-rear-variant",
  CameraSwitch : "camera-switch",
  CameraTimer : "camera-timer",
  Candle : "candle",
  Candycane : "candycane",
  Car : "car",
  CarBattery : "car-battery",
  CarConnected : "car-connected",
  CarWash : "car-wash",
  Cards : "cards",
  CardsOutline : "cards-outline",
  CardsPlayingOutline : "cards-playing-outline",
  Carrot : "carrot",
  Cart : "cart",
  CartOff : "cart-off",
  CartOutline : "cart-outline",
  CartPlus : "cart-plus",
  CaseSensitiveAlt : "case-sensitive-alt",
  Cash : "cash",
  Cash100 : "cash-100",
  CashMultiple : "cash-multiple",
  CashUsd : "cash-usd",
  Cast : "cast",
  CastConnected : "cast-connected",
  Castle : "castle",
  Cat : "cat",
  Cellphone : "cellphone",
  CellphoneAndroid : "cellphone-android",
  CellphoneBasic : "cellphone-basic",
  CellphoneDock : "cellphone-dock",
  CellphoneIphone : "cellphone-iphone",
  CellphoneLink : "cellphone-link",
  CellphoneLinkOff : "cellphone-link-off",
  CellphoneSettings : "cellphone-settings",
  Certificate : "certificate",
  ChairSchool : "chair-school",
  ChartArc : "chart-arc",
  ChartAreaspline : "chart-areaspline",
  ChartBar : "chart-bar",
  ChartBubble : "chart-bubble",
  ChartGantt : "chart-gantt",
  ChartHistogram : "chart-histogram",
  ChartLine : "chart-line",
  ChartPie : "chart-pie",
  ChartScatterplotHexbin : "chart-scatterplot-hexbin",
  ChartTimeline : "chart-timeline",
  Check : "check",
  CheckAll : "check-all",
  CheckCircle : "check-circle",
  CheckCircleOutline : "check-circle-outline",
  CheckboxBlank : "checkbox-blank",
  CheckboxBlankCircle : "checkbox-blank-circle",
  CheckboxBlankCircleOutline : "checkbox-blank-circle-outline",
  CheckboxBlankOutline : "checkbox-blank-outline",
  CheckboxMarked : "checkbox-marked",
  CheckboxMarkedCircle : "checkbox-marked-circle",
  CheckboxMarkedCircleOutline : "checkbox-marked-circle-outline",
  CheckboxMarkedOutline : "checkbox-marked-outline",
  CheckboxMultipleBlank : "checkbox-multiple-blank",
  CheckboxMultipleBlankCircle : "checkbox-multiple-blank-circle",
  CheckboxMultipleBlankCircleOutline : "checkbox-multiple-blank-circle-outline",
  CheckboxMultipleBlankOutline : "checkbox-multiple-blank-outline",
  CheckboxMultipleMarked : "checkbox-multiple-marked",
  CheckboxMultipleMarkedCircle : "checkbox-multiple-marked-circle",
  CheckboxMultipleMarkedCircleOutline : "checkbox-multiple-marked-circle-outline",
  CheckboxMultipleMarkedOutline : "checkbox-multiple-marked-outline",
  Checkerboard : "checkerboard",
  ChemicalWeapon : "chemical-weapon",
  ChevronDoubleDown : "chevron-double-down",
  ChevronDoubleLeft : "chevron-double-left",
  ChevronDoubleRight : "chevron-double-right",
  ChevronDoubleUp : "chevron-double-up",
  ChevronDown : "chevron-down",
  ChevronLeft : "chevron-left",
  ChevronRight : "chevron-right",
  ChevronUp : "chevron-up",
  Chip : "chip",
  Church : "church",
  CiscoWebex : "cisco-webex",
  City : "city",
  Clipboard : "clipboard",
  ClipboardAccount : "clipboard-account",
  ClipboardAlert : "clipboard-alert",
  ClipboardArrowDown : "clipboard-arrow-down",
  ClipboardArrowLeft : "clipboard-arrow-left",
  ClipboardCheck : "clipboard-check",
  ClipboardOutline : "clipboard-outline",
  ClipboardText : "clipboard-text",
  Clippy : "clippy",
  Clock : "clock",
  ClockAlert : "clock-alert",
  ClockEnd : "clock-end",
  ClockFast : "clock-fast",
  ClockIn : "clock-in",
  ClockOut : "clock-out",
  ClockStart : "clock-start",
  Close : "close",
  CloseBox : "close-box",
  CloseBoxOutline : "close-box-outline",
  CloseCircle : "close-circle",
  CloseCircleOutline : "close-circle-outline",
  CloseNetwork : "close-network",
  CloseOctagon : "close-octagon",
  CloseOctagonOutline : "close-octagon-outline",
  ClosedCaption : "closed-caption",
  Cloud : "cloud",
  CloudCheck : "cloud-check",
  CloudCircle : "cloud-circle",
  CloudDownload : "cloud-download",
  CloudOutline : "cloud-outline",
  CloudOutlineOff : "cloud-outline-off",
  CloudPrint : "cloud-print",
  CloudPrintOutline : "cloud-print-outline",
  CloudSync : "cloud-sync",
  CloudUpload : "cloud-upload",
  CodeArray : "code-array",
  CodeBraces : "code-braces",
  CodeBrackets : "code-brackets",
  CodeEqual : "code-equal",
  CodeGreaterThan : "code-greater-than",
  CodeGreaterThanOrEqual : "code-greater-than-or-equal",
  CodeLessThan : "code-less-than",
  CodeLessThanOrEqual : "code-less-than-or-equal",
  CodeNotEqual : "code-not-equal",
  CodeNotEqualVariant : "code-not-equal-variant",
  CodeParentheses : "code-parentheses",
  CodeString : "code-string",
  CodeTags : "code-tags",
  CodeTagsCheck : "code-tags-check",
  Codepen : "codepen",
  Coffee : "coffee",
  CoffeeToGo : "coffee-to-go",
  Coin : "coin",
  Coins : "coins",
  Collage : "collage",
  ColorHelper : "color-helper",
  Comment : "comment",
  CommentAccount : "comment-account",
  CommentAccountOutline : "comment-account-outline",
  CommentAlert : "comment-alert",
  CommentAlertOutline : "comment-alert-outline",
  CommentCheck : "comment-check",
  CommentCheckOutline : "comment-check-outline",
  CommentMultipleOutline : "comment-multiple-outline",
  CommentOutline : "comment-outline",
  CommentPlusOutline : "comment-plus-outline",
  CommentProcessing : "comment-processing",
  CommentProcessingOutline : "comment-processing-outline",
  CommentQuestionOutline : "comment-question-outline",
  CommentRemoveOutline : "comment-remove-outline",
  CommentText : "comment-text",
  CommentTextOutline : "comment-text-outline",
  Compare : "compare",
  Compass : "compass",
  CompassOutline : "compass-outline",
  Console : "console",
  ContactMail : "contact-mail",
  ContentCopy : "content-copy",
  ContentCut : "content-cut",
  ContentDuplicate : "content-duplicate",
  ContentPaste : "content-paste",
  ContentSave : "content-save",
  ContentSaveAll : "content-save-all",
  ContentSaveSettings : "content-save-settings",
  Contrast : "contrast",
  ContrastBox : "contrast-box",
  ContrastCircle : "contrast-circle",
  Cookie : "cookie",
  Copyright : "copyright",
  Counter : "counter",
  Cow : "cow",
  Creation : "creation",
  CreditCard : "credit-card",
  CreditCardMultiple : "credit-card-multiple",
  CreditCardOff : "credit-card-off",
  CreditCardPlus : "credit-card-plus",
  CreditCardScan : "credit-card-scan",
  Crop : "crop",
  CropFree : "crop-free",
  CropLandscape : "crop-landscape",
  CropPortrait : "crop-portrait",
  CropRotate : "crop-rotate",
  CropSquare : "crop-square",
  Crosshairs : "crosshairs",
  CrosshairsGps : "crosshairs-gps",
  Crown : "crown",
  Cube : "cube",
  CubeOutline : "cube-outline",
  CubeSend : "cube-send",
  CubeUnfolded : "cube-unfolded",
  Cup : "cup",
  CupOff : "cup-off",
  CupWater : "cup-water",
  CurrencyBtc : "currency-btc",
  CurrencyEur : "currency-eur",
  CurrencyGbp : "currency-gbp",
  CurrencyInr : "currency-inr",
  CurrencyNgn : "currency-ngn",
  CurrencyRub : "currency-rub",
  CurrencyTry : "currency-try",
  CurrencyUsd : "currency-usd",
  CurrencyUsdOff : "currency-usd-off",
  CursorDefault : "cursor-default",
  CursorDefaultOutline : "cursor-default-outline",
  CursorMove : "cursor-move",
  CursorPointer : "cursor-pointer",
  CursorText : "cursor-text",
  Database : "database",
  DatabaseMinus : "database-minus",
  DatabasePlus : "database-plus",
  DebugStepInto : "debug-step-into",
  DebugStepOut : "debug-step-out",
  DebugStepOver : "debug-step-over",
  DecimalDecrease : "decimal-decrease",
  DecimalIncrease : "decimal-increase",
  Delete : "delete",
  DeleteCircle : "delete-circle",
  DeleteForever : "delete-forever",
  DeleteSweep : "delete-sweep",
  DeleteVariant : "delete-variant",
  Delta : "delta",
  Deskphone : "deskphone",
  DesktopMac : "desktop-mac",
  DesktopTower : "desktop-tower",
  Details : "details",
  DeveloperBoard : "developer-board",
  Deviantart : "deviantart",
  Dialpad : "dialpad",
  Diamond : "diamond",
  Dice1 : "dice-1",
  Dice2 : "dice-2",
  Dice3 : "dice-3",
  Dice4 : "dice-4",
  Dice5 : "dice-5",
  Dice6 : "dice-6",
  DiceD20 : "dice-d20",
  DiceD4 : "dice-d4",
  DiceD6 : "dice-d6",
  DiceD8 : "dice-d8",
  Dictionary : "dictionary",
  Directions : "directions",
  DirectionsFork : "directions-fork",
  Discord : "discord",
  Disk : "disk",
  DiskAlert : "disk-alert",
  Disqus : "disqus",
  DisqusOutline : "disqus-outline",
  Division : "division",
  DivisionBox : "division-box",
  Dna : "dna",
  Dns : "dns",
  DoNotDisturb : "do-not-disturb",
  DoNotDisturbOff : "do-not-disturb-off",
  Dolby : "dolby",
  Domain : "domain",
  DotsHorizontal : "dots-horizontal",
  DotsVertical : "dots-vertical",
  Douban : "douban",
  Download : "download",
  Drag : "drag",
  DragHorizontal : "drag-horizontal",
  DragVertical : "drag-vertical",
  Drawing : "drawing",
  DrawingBox : "drawing-box",
  Dribbble : "dribbble",
  DribbbleBox : "dribbble-box",
  Drone : "drone",
  Dropbox : "dropbox",
  Drupal : "drupal",
  Duck : "duck",
  Dumbbell : "dumbbell",
  Earth : "earth",
  EarthOff : "earth-off",
  Edge : "edge",
  Eject : "eject",
  ElevationDecline : "elevation-decline",
  ElevationRise : "elevation-rise",
  Elevator : "elevator",
  Email : "email",
  EmailOpen : "email-open",
  EmailOpenOutline : "email-open-outline",
  EmailOutline : "email-outline",
  EmailSecure : "email-secure",
  EmailVariant : "email-variant",
  Emby : "emby",
  Emoticon : "emoticon",
  EmoticonCool : "emoticon-cool",
  EmoticonDead : "emoticon-dead",
  EmoticonDevil : "emoticon-devil",
  EmoticonExcited : "emoticon-excited",
  EmoticonHappy : "emoticon-happy",
  EmoticonNeutral : "emoticon-neutral",
  EmoticonPoop : "emoticon-poop",
  EmoticonSad : "emoticon-sad",
  EmoticonTongue : "emoticon-tongue",
  Engine : "engine",
  EngineOutline : "engine-outline",
  Equal : "equal",
  EqualBox : "equal-box",
  Eraser : "eraser",
  EraserVariant : "eraser-variant",
  Escalator : "escalator",
  Ethernet : "ethernet",
  EthernetCable : "ethernet-cable",
  EthernetCableOff : "ethernet-cable-off",
  Etsy : "etsy",
  EvStation : "ev-station",
  Evernote : "evernote",
  Exclamation : "exclamation",
  ExitToApp : "exit-to-app",
  Export : "export",
  Eye : "eye",
  EyeOff : "eye-off",
  Eyedropper : "eyedropper",
  EyedropperVariant : "eyedropper-variant",
  Face : "face",
  FaceProfile : "face-profile",
  Facebook : "facebook",
  FacebookBox : "facebook-box",
  FacebookMessenger : "facebook-messenger",
  Factory : "factory",
  Fan : "fan",
  FastForward : "fast-forward",
  Fax : "fax",
  Ferry : "ferry",
  File : "file",
  FileChart : "file-chart",
  FileCheck : "file-check",
  FileCloud : "file-cloud",
  FileDelimited : "file-delimited",
  FileDocument : "file-document",
  FileDocumentBox : "file-document-box",
  FileExcel : "file-excel",
  FileExcelBox : "file-excel-box",
  FileExport : "file-export",
  FileFind : "file-find",
  FileHidden : "file-hidden",
  FileImage : "file-image",
  FileImport : "file-import",
  FileLock : "file-lock",
  FileMultiple : "file-multiple",
  FileMusic : "file-music",
  FileOutline : "file-outline",
  FilePdf : "file-pdf",
  FilePdfBox : "file-pdf-box",
  FilePowerpoint : "file-powerpoint",
  FilePowerpointBox : "file-powerpoint-box",
  FilePresentationBox : "file-presentation-box",
  FileRestore : "file-restore",
  FileSend : "file-send",
  FileTree : "file-tree",
  FileVideo : "file-video",
  FileWord : "file-word",
  FileWordBox : "file-word-box",
  FileXml : "file-xml",
  Film : "film",
  Filmstrip : "filmstrip",
  FilmstripOff : "filmstrip-off",
  Filter : "filter",
  FilterOutline : "filter-outline",
  FilterRemove : "filter-remove",
  FilterRemoveOutline : "filter-remove-outline",
  FilterVariant : "filter-variant",
  Fingerprint : "fingerprint",
  Fire : "fire",
  Firefox : "firefox",
  Fish : "fish",
  Flag : "flag",
  FlagCheckered : "flag-checkered",
  FlagOutline : "flag-outline",
  FlagOutlineVariant : "flag-outline-variant",
  FlagTriangle : "flag-triangle",
  FlagVariant : "flag-variant",
  Flash : "flash",
  FlashAuto : "flash-auto",
  FlashOff : "flash-off",
  FlashRedEye : "flash-red-eye",
  Flashlight : "flashlight",
  FlashlightOff : "flashlight-off",
  Flask : "flask",
  FlaskEmpty : "flask-empty",
  FlaskEmptyOutline : "flask-empty-outline",
  FlaskOutline : "flask-outline",
  Flattr : "flattr",
  FlipToBack : "flip-to-back",
  FlipToFront : "flip-to-front",
  Floppy : "floppy",
  Flower : "flower",
  Folder : "folder",
  FolderAccount : "folder-account",
  FolderDownload : "folder-download",
  FolderGoogleDrive : "folder-google-drive",
  FolderImage : "folder-image",
  FolderLock : "folder-lock",
  FolderLockOpen : "folder-lock-open",
  FolderMove : "folder-move",
  FolderMultiple : "folder-multiple",
  FolderMultipleImage : "folder-multiple-image",
  FolderMultipleOutline : "folder-multiple-outline",
  FolderOutline : "folder-outline",
  FolderPlus : "folder-plus",
  FolderRemove : "folder-remove",
  FolderStar : "folder-star",
  FolderUpload : "folder-upload",
  Food : "food",
  FoodApple : "food-apple",
  FoodForkDrink : "food-fork-drink",
  FoodOff : "food-off",
  FoodVariant : "food-variant",
  Football : "football",
  FootballAustralian : "football-australian",
  FootballHelmet : "football-helmet",
  FormatAlignCenter : "format-align-center",
  FormatAlignJustify : "format-align-justify",
  FormatAlignLeft : "format-align-left",
  FormatAlignRight : "format-align-right",
  FormatAnnotationPlus : "format-annotation-plus",
  FormatBold : "format-bold",
  FormatClear : "format-clear",
  FormatColorFill : "format-color-fill",
  FormatColorText : "format-color-text",
  FormatFloatCenter : "format-float-center",
  FormatFloatLeft : "format-float-left",
  FormatFloatNone : "format-float-none",
  FormatFloatRight : "format-float-right",
  FormatHeader1 : "format-header-1",
  FormatHeader2 : "format-header-2",
  FormatHeader3 : "format-header-3",
  FormatHeader4 : "format-header-4",
  FormatHeader5 : "format-header-5",
  FormatHeader6 : "format-header-6",
  FormatHeaderDecrease : "format-header-decrease",
  FormatHeaderEqual : "format-header-equal",
  FormatHeaderIncrease : "format-header-increase",
  FormatHeaderPound : "format-header-pound",
  FormatHorizontalAlignCenter : "format-horizontal-align-center",
  FormatHorizontalAlignLeft : "format-horizontal-align-left",
  FormatHorizontalAlignRight : "format-horizontal-align-right",
  FormatIndentDecrease : "format-indent-decrease",
  FormatIndentIncrease : "format-indent-increase",
  FormatItalic : "format-italic",
  FormatLineSpacing : "format-line-spacing",
  FormatLineStyle : "format-line-style",
  FormatLineWeight : "format-line-weight",
  FormatListBulleted : "format-list-bulleted",
  FormatListBulletedType : "format-list-bulleted-type",
  FormatListNumbers : "format-list-numbers",
  FormatPaint : "format-paint",
  FormatParagraph : "format-paragraph",
  FormatQuote : "format-quote",
  FormatSection : "format-section",
  FormatSize : "format-size",
  FormatStrikethrough : "format-strikethrough",
  FormatStrikethroughVariant : "format-strikethrough-variant",
  FormatSubscript : "format-subscript",
  FormatSuperscript : "format-superscript",
  FormatText : "format-text",
  FormatTextdirectionLToR : "format-textdirection-l-to-r",
  FormatTextdirectionRToL : "format-textdirection-r-to-l",
  FormatTitle : "format-title",
  FormatUnderline : "format-underline",
  FormatVerticalAlignBottom : "format-vertical-align-bottom",
  FormatVerticalAlignCenter : "format-vertical-align-center",
  FormatVerticalAlignTop : "format-vertical-align-top",
  FormatWrapInline : "format-wrap-inline",
  FormatWrapSquare : "format-wrap-square",
  FormatWrapTight : "format-wrap-tight",
  FormatWrapTopBottom : "format-wrap-top-bottom",
  Forum : "forum",
  Forward : "forward",
  Foursquare : "foursquare",
  Fridge : "fridge",
  FridgeFilled : "fridge-filled",
  FridgeFilledBottom : "fridge-filled-bottom",
  FridgeFilledTop : "fridge-filled-top",
  Fullscreen : "fullscreen",
  FullscreenExit : "fullscreen-exit",
  Function : "function",
  Gamepad : "gamepad",
  GamepadVariant : "gamepad-variant",
  GasCylinder : "gas-cylinder",
  GasStation : "gas-station",
  Gate : "gate",
  Gauge : "gauge",
  Gavel : "gavel",
  GenderFemale : "gender-female",
  GenderMale : "gender-male",
  GenderMaleFemale : "gender-male-female",
  GenderTransgender : "gender-transgender",
  Ghost : "ghost",
  Gift : "gift",
  Git : "git",
  GithubBox : "github-box",
  GithubCircle : "github-circle",
  GlassFlute : "glass-flute",
  GlassMug : "glass-mug",
  GlassStange : "glass-stange",
  GlassTulip : "glass-tulip",
  Glassdoor : "glassdoor",
  Glasses : "glasses",
  Gmail : "gmail",
  Gnome : "gnome",
  Gondola : "gondola",
  Google : "google",
  GoogleCardboard : "google-cardboard",
  GoogleChrome : "google-chrome",
  GoogleCircles : "google-circles",
  GoogleCirclesCommunities : "google-circles-communities",
  GoogleCirclesExtended : "google-circles-extended",
  GoogleCirclesGroup : "google-circles-group",
  GoogleController : "google-controller",
  GoogleControllerOff : "google-controller-off",
  GoogleDrive : "google-drive",
  GoogleEarth : "google-earth",
  GoogleGlass : "google-glass",
  GoogleMaps : "google-maps",
  GoogleNearby : "google-nearby",
  GooglePages : "google-pages",
  GooglePhysicalWeb : "google-physical-web",
  GooglePlay : "google-play",
  GooglePlus : "google-plus",
  GooglePlusBox : "google-plus-box",
  GoogleTranslate : "google-translate",
  GoogleWallet : "google-wallet",
  Gradient : "gradient",
  GreasePencil : "grease-pencil",
  Grid : "grid",
  GridOff : "grid-off",
  Group : "group",
  GuitarElectric : "guitar-electric",
  GuitarPick : "guitar-pick",
  GuitarPickOutline : "guitar-pick-outline",
  Hackernews : "hackernews",
  Hamburger : "hamburger",
  HandPointingRight : "hand-pointing-right",
  Hanger : "hanger",
  Hangouts : "hangouts",
  Harddisk : "harddisk",
  Headphones : "headphones",
  HeadphonesBox : "headphones-box",
  HeadphonesSettings : "headphones-settings",
  Headset : "headset",
  HeadsetDock : "headset-dock",
  HeadsetOff : "headset-off",
  Heart : "heart",
  HeartBox : "heart-box",
  HeartBoxOutline : "heart-box-outline",
  HeartBroken : "heart-broken",
  HeartOutline : "heart-outline",
  HeartPulse : "heart-pulse",
  Help : "help",
  HelpCircle : "help-circle",
  HelpCircleOutline : "help-circle-outline",
  Hexagon : "hexagon",
  HexagonOutline : "hexagon-outline",
  Highway : "highway",
  History : "history",
  Hololens : "hololens",
  Home : "home",
  HomeMapMarker : "home-map-marker",
  HomeModern : "home-modern",
  HomeOutline : "home-outline",
  HomeVariant : "home-variant",
  Hops : "hops",
  Hospital : "hospital",
  HospitalBuilding : "hospital-building",
  HospitalMarker : "hospital-marker",
  Hotel : "hotel",
  Houzz : "houzz",
  HouzzBox : "houzz-box",
  Human : "human",
  HumanChild : "human-child",
  HumanFemale : "human-female",
  HumanGreeting : "human-greeting",
  HumanHandsdown : "human-handsdown",
  HumanHandsup : "human-handsup",
  HumanMale : "human-male",
  HumanMaleFemale : "human-male-female",
  HumanPregnant : "human-pregnant",
  Image : "image",
  ImageAlbum : "image-album",
  ImageArea : "image-area",
  ImageAreaClose : "image-area-close",
  ImageBroken : "image-broken",
  ImageBrokenVariant : "image-broken-variant",
  ImageFilter : "image-filter",
  ImageFilterBlackWhite : "image-filter-black-white",
  ImageFilterCenterFocus : "image-filter-center-focus",
  ImageFilterCenterFocusWeak : "image-filter-center-focus-weak",
  ImageFilterDrama : "image-filter-drama",
  ImageFilterFrames : "image-filter-frames",
  ImageFilterHdr : "image-filter-hdr",
  ImageFilterNone : "image-filter-none",
  ImageFilterTiltShift : "image-filter-tilt-shift",
  ImageFilterVintage : "image-filter-vintage",
  ImageMultiple : "image-multiple",
  Import : "import",
  Inbox : "inbox",
  InboxArrowDown : "inbox-arrow-down",
  InboxArrowUp : "inbox-arrow-up",
  Incognito : "incognito",
  Information : "information",
  InformationOutline : "information-outline",
  InformationVariant : "information-variant",
  Instagram : "instagram",
  Instapaper : "instapaper",
  InternetExplorer : "internet-explorer",
  InvertColors : "invert-colors",
  Itunes : "itunes",
  Jeepney : "jeepney",
  Jira : "jira",
  Jsfiddle : "jsfiddle",
  Json : "json",
  Keg : "keg",
  Kettle : "kettle",
  Key : "key",
  KeyChange : "key-change",
  KeyMinus : "key-minus",
  KeyPlus : "key-plus",
  KeyRemove : "key-remove",
  KeyVariant : "key-variant",
  Keyboard : "keyboard",
  KeyboardBackspace : "keyboard-backspace",
  KeyboardCaps : "keyboard-caps",
  KeyboardClose : "keyboard-close",
  KeyboardOff : "keyboard-off",
  KeyboardReturn : "keyboard-return",
  KeyboardTab : "keyboard-tab",
  KeyboardVariant : "keyboard-variant",
  Kodi : "kodi",
  Label : "label",
  LabelOutline : "label-outline",
  Lambda : "lambda",
  Lamp : "lamp",
  Lan : "lan",
  LanConnect : "lan-connect",
  LanDisconnect : "lan-disconnect",
  LanPending : "lan-pending",
  LanguageC : "language-c",
  LanguageCpp : "language-cpp",
  LanguageCsharp : "language-csharp",
  LanguageCss3 : "language-css3",
  LanguageHtml5 : "language-html5",
  LanguageJavascript : "language-javascript",
  LanguagePhp : "language-php",
  LanguagePython : "language-python",
  LanguagePythonText : "language-python-text",
  Laptop : "laptop",
  LaptopChromebook : "laptop-chromebook",
  LaptopMac : "laptop-mac",
  LaptopWindows : "laptop-windows",
  Lastfm : "lastfm",
  Launch : "launch",
  Layers : "layers",
  LayersOff : "layers-off",
  LeadPencil : "lead-pencil",
  Leaf : "leaf",
  LedOff : "led-off",
  LedOn : "led-on",
  LedOutline : "led-outline",
  LedVariantOff : "led-variant-off",
  LedVariantOn : "led-variant-on",
  LedVariantOutline : "led-variant-outline",
  Library : "library",
  LibraryBooks : "library-books",
  LibraryMusic : "library-music",
  LibraryPlus : "library-plus",
  Lightbulb : "lightbulb",
  LightbulbOutline : "lightbulb-outline",
  Link : "link",
  LinkOff : "link-off",
  LinkVariant : "link-variant",
  LinkVariantOff : "link-variant-off",
  Linkedin : "linkedin",
  LinkedinBox : "linkedin-box",
  Linux : "linux",
  Lock : "lock",
  LockOpen : "lock-open",
  LockOpenOutline : "lock-open-outline",
  LockOutline : "lock-outline",
  LockPlus : "lock-plus",
  Login : "login",
  LoginVariant : "login-variant",
  Logout : "logout",
  LogoutVariant : "logout-variant",
  Looks : "looks",
  Loupe : "loupe",
  Lumx : "lumx",
  Magnet : "magnet",
  MagnetOn : "magnet-on",
  Magnify : "magnify",
  MagnifyMinus : "magnify-minus",
  MagnifyPlus : "magnify-plus",
  MailRu : "mail-ru",
  Map : "map",
  MapMarker : "map-marker",
  MapMarkerCircle : "map-marker-circle",
  MapMarkerMinus : "map-marker-minus",
  MapMarkerMultiple : "map-marker-multiple",
  MapMarkerOff : "map-marker-off",
  MapMarkerPlus : "map-marker-plus",
  MapMarkerRadius : "map-marker-radius",
  Margin : "margin",
  Markdown : "markdown",
  Marker : "marker",
  MarkerCheck : "marker-check",
  Martini : "martini",
  MaterialUi : "material-ui",
  MathCompass : "math-compass",
  Matrix : "matrix",
  Maxcdn : "maxcdn",
  Medium : "medium",
  Memory : "memory",
  Menu : "menu",
  MenuDown : "menu-down",
  MenuDownOutline : "menu-down-outline",
  MenuLeft : "menu-left",
  MenuRight : "menu-right",
  MenuUp : "menu-up",
  MenuUpOutline : "menu-up-outline",
  Message : "message",
  MessageAlert : "message-alert",
  MessageBulleted : "message-bulleted",
  MessageBulletedOff : "message-bulleted-off",
  MessageDraw : "message-draw",
  MessageImage : "message-image",
  MessageOutline : "message-outline",
  MessagePlus : "message-plus",
  MessageProcessing : "message-processing",
  MessageReply : "message-reply",
  MessageReplyText : "message-reply-text",
  MessageText : "message-text",
  MessageTextOutline : "message-text-outline",
  MessageVideo : "message-video",
  Meteor : "meteor",
  Microphone : "microphone",
  MicrophoneOff : "microphone-off",
  MicrophoneOutline : "microphone-outline",
  MicrophoneSettings : "microphone-settings",
  MicrophoneVariant : "microphone-variant",
  MicrophoneVariantOff : "microphone-variant-off",
  Microscope : "microscope",
  Microsoft : "microsoft",
  Minecraft : "minecraft",
  Minus : "minus",
  MinusBox : "minus-box",
  MinusCircle : "minus-circle",
  MinusCircleOutline : "minus-circle-outline",
  MinusNetwork : "minus-network",
  Mixcloud : "mixcloud",
  Monitor : "monitor",
  MonitorMultiple : "monitor-multiple",
  More : "more",
  Motorbike : "motorbike",
  Mouse : "mouse",
  MouseOff : "mouse-off",
  MouseVariant : "mouse-variant",
  MouseVariantOff : "mouse-variant-off",
  MoveResize : "move-resize",
  MoveResizeVariant : "move-resize-variant",
  Movie : "movie",
  Multiplication : "multiplication",
  MultiplicationBox : "multiplication-box",
  MusicBox : "music-box",
  MusicBoxOutline : "music-box-outline",
  MusicCircle : "music-circle",
  MusicNote : "music-note",
  MusicNoteBluetooth : "music-note-bluetooth",
  MusicNoteBluetoothOff : "music-note-bluetooth-off",
  MusicNoteEighth : "music-note-eighth",
  MusicNoteHalf : "music-note-half",
  MusicNoteOff : "music-note-off",
  MusicNoteQuarter : "music-note-quarter",
  MusicNoteSixteenth : "music-note-sixteenth",
  MusicNoteWhole : "music-note-whole",
  Nature : "nature",
  NaturePeople : "nature-people",
  Navigation : "navigation",
  NearMe : "near-me",
  Needle : "needle",
  NestProtect : "nest-protect",
  NestThermostat : "nest-thermostat",
  NewBox : "new-box",
  Newspaper : "newspaper",
  Nfc : "nfc",
  NfcTap : "nfc-tap",
  NfcVariant : "nfc-variant",
  Nodejs : "nodejs",
  Note : "note",
  NoteMultiple : "note-multiple",
  NoteMultipleOutline : "note-multiple-outline",
  NoteOutline : "note-outline",
  NotePlus : "note-plus",
  NotePlusOutline : "note-plus-outline",
  NoteText : "note-text",
  NotificationClearAll : "notification-clear-all",
  Nuke : "nuke",
  Numeric : "numeric",
  Numeric0Box : "numeric-0-box",
  Numeric0BoxMultipleOutline : "numeric-0-box-multiple-outline",
  Numeric0BoxOutline : "numeric-0-box-outline",
  Numeric1Box : "numeric-1-box",
  Numeric1BoxMultipleOutline : "numeric-1-box-multiple-outline",
  Numeric1BoxOutline : "numeric-1-box-outline",
  Numeric2Box : "numeric-2-box",
  Numeric2BoxMultipleOutline : "numeric-2-box-multiple-outline",
  Numeric2BoxOutline : "numeric-2-box-outline",
  Numeric3Box : "numeric-3-box",
  Numeric3BoxMultipleOutline : "numeric-3-box-multiple-outline",
  Numeric3BoxOutline : "numeric-3-box-outline",
  Numeric4Box : "numeric-4-box",
  Numeric4BoxMultipleOutline : "numeric-4-box-multiple-outline",
  Numeric4BoxOutline : "numeric-4-box-outline",
  Numeric5Box : "numeric-5-box",
  Numeric5BoxMultipleOutline : "numeric-5-box-multiple-outline",
  Numeric5BoxOutline : "numeric-5-box-outline",
  Numeric6Box : "numeric-6-box",
  Numeric6BoxMultipleOutline : "numeric-6-box-multiple-outline",
  Numeric6BoxOutline : "numeric-6-box-outline",
  Numeric7Box : "numeric-7-box",
  Numeric7BoxMultipleOutline : "numeric-7-box-multiple-outline",
  Numeric7BoxOutline : "numeric-7-box-outline",
  Numeric8Box : "numeric-8-box",
  Numeric8BoxMultipleOutline : "numeric-8-box-multiple-outline",
  Numeric8BoxOutline : "numeric-8-box-outline",
  Numeric9Box : "numeric-9-box",
  Numeric9BoxMultipleOutline : "numeric-9-box-multiple-outline",
  Numeric9BoxOutline : "numeric-9-box-outline",
  Numeric9PlusBox : "numeric-9-plus-box",
  Numeric9PlusBoxMultipleOutline : "numeric-9-plus-box-multiple-outline",
  Numeric9PlusBoxOutline : "numeric-9-plus-box-outline",
  Nutrition : "nutrition",
  Oar : "oar",
  Octagon : "octagon",
  OctagonOutline : "octagon-outline",
  Odnoklassniki : "odnoklassniki",
  Office : "office",
  Oil : "oil",
  OilTemperature : "oil-temperature",
  Omega : "omega",
  Onedrive : "onedrive",
  Opacity : "opacity",
  OpenInApp : "open-in-app",
  OpenInNew : "open-in-new",
  Openid : "openid",
  Opera : "opera",
  Ornament : "ornament",
  OrnamentVariant : "ornament-variant",
  Owl : "owl",
  Package : "package",
  PackageDown : "package-down",
  PackageUp : "package-up",
  PackageVariant : "package-variant",
  PackageVariantClosed : "package-variant-closed",
  PageFirst : "page-first",
  PageLast : "page-last",
  Palette : "palette",
  PaletteAdvanced : "palette-advanced",
  Panda : "panda",
  Pandora : "pandora",
  Panorama : "panorama",
  PanoramaFisheye : "panorama-fisheye",
  PanoramaHorizontal : "panorama-horizontal",
  PanoramaVertical : "panorama-vertical",
  PanoramaWideAngle : "panorama-wide-angle",
  PaperCutVertical : "paper-cut-vertical",
  Paperclip : "paperclip",
  Parking : "parking",
  Pause : "pause",
  PauseCircle : "pause-circle",
  PauseCircleOutline : "pause-circle-outline",
  PauseOctagon : "pause-octagon",
  PauseOctagonOutline : "pause-octagon-outline",
  Paw : "paw",
  PawOff : "paw-off",
  Pen : "pen",
  Pencil : "pencil",
  PencilBox : "pencil-box",
  PencilBoxOutline : "pencil-box-outline",
  PencilLock : "pencil-lock",
  PencilOff : "pencil-off",
  Percent : "percent",
  Pharmacy : "pharmacy",
  Phone : "phone",
  PhoneBluetooth : "phone-bluetooth",
  PhoneClassic : "phone-classic",
  PhoneForward : "phone-forward",
  PhoneHangup : "phone-hangup",
  PhoneInTalk : "phone-in-talk",
  PhoneIncoming : "phone-incoming",
  PhoneLocked : "phone-locked",
  PhoneLog : "phone-log",
  PhoneMinus : "phone-minus",
  PhoneMissed : "phone-missed",
  PhoneOutgoing : "phone-outgoing",
  PhonePaused : "phone-paused",
  PhonePlus : "phone-plus",
  PhoneSettings : "phone-settings",
  PhoneVoip : "phone-voip",
  Pi : "pi",
  PiBox : "pi-box",
  Piano : "piano",
  Pig : "pig",
  Pill : "pill",
  Pin : "pin",
  PinOff : "pin-off",
  PineTree : "pine-tree",
  PineTreeBox : "pine-tree-box",
  Pinterest : "pinterest",
  PinterestBox : "pinterest-box",
  Pizza : "pizza",
  PlaneShield : "plane-shield",
  Play : "play",
  PlayBoxOutline : "play-box-outline",
  PlayCircle : "play-circle",
  PlayCircleOutline : "play-circle-outline",
  PlayPause : "play-pause",
  PlayProtectedContent : "play-protected-content",
  PlaylistCheck : "playlist-check",
  PlaylistMinus : "playlist-minus",
  PlaylistPlay : "playlist-play",
  PlaylistPlus : "playlist-plus",
  PlaylistRemove : "playlist-remove",
  Playstation : "playstation",
  Plex : "plex",
  Plus : "plus",
  PlusBox : "plus-box",
  PlusCircle : "plus-circle",
  PlusCircleMultipleOutline : "plus-circle-multiple-outline",
  PlusCircleOutline : "plus-circle-outline",
  PlusNetwork : "plus-network",
  PlusOne : "plus-one",
  Pocket : "pocket",
  Pokeball : "pokeball",
  Polaroid : "polaroid",
  Poll : "poll",
  PollBox : "poll-box",
  Polymer : "polymer",
  Pool : "pool",
  Popcorn : "popcorn",
  Pot : "pot",
  PotMix : "pot-mix",
  Pound : "pound",
  PoundBox : "pound-box",
  Power : "power",
  PowerPlug : "power-plug",
  PowerPlugOff : "power-plug-off",
  PowerSettings : "power-settings",
  PowerSocket : "power-socket",
  Presentation : "presentation",
  PresentationPlay : "presentation-play",
  Printer : "printer",
  Printer3d : "printer-3d",
  PrinterAlert : "printer-alert",
  PriorityHigh : "priority-high",
  PriorityLow : "priority-low",
  ProfessionalHexagon : "professional-hexagon",
  Projector : "projector",
  ProjectorScreen : "projector-screen",
  Publish : "publish",
  Pulse : "pulse",
  Puzzle : "puzzle",
  Qqchat : "qqchat",
  Qrcode : "qrcode",
  QrcodeScan : "qrcode-scan",
  Quadcopter : "quadcopter",
  QualityHigh : "quality-high",
  Quicktime : "quicktime",
  Radar : "radar",
  Radiator : "radiator",
  Radio : "radio",
  RadioHandheld : "radio-handheld",
  RadioTower : "radio-tower",
  Radioactive : "radioactive",
  RadioboxBlank : "radiobox-blank",
  RadioboxMarked : "radiobox-marked",
  Raspberrypi : "raspberrypi",
  RayEnd : "ray-end",
  RayEndArrow : "ray-end-arrow",
  RayStart : "ray-start",
  RayStartArrow : "ray-start-arrow",
  RayStartEnd : "ray-start-end",
  RayVertex : "ray-vertex",
  Rdio : "rdio",
  Read : "read",
  Readability : "readability",
  Receipt : "receipt",
  Record : "record",
  RecordRec : "record-rec",
  Recycle : "recycle",
  Reddit : "reddit",
  Redo : "redo",
  RedoVariant : "redo-variant",
  Refresh : "refresh",
  Regex : "regex",
  RelativeScale : "relative-scale",
  Reload : "reload",
  Remote : "remote",
  RenameBox : "rename-box",
  ReorderHorizontal : "reorder-horizontal",
  ReorderVertical : "reorder-vertical",
  Repeat : "repeat",
  RepeatOff : "repeat-off",
  RepeatOnce : "repeat-once",
  Replay : "replay",
  Reply : "reply",
  ReplyAll : "reply-all",
  Reproduction : "reproduction",
  ResizeBottomRight : "resize-bottom-right",
  Responsive : "responsive",
  Restore : "restore",
  Rewind : "rewind",
  Ribbon : "ribbon",
  Road : "road",
  RoadVariant : "road-variant",
  Robot : "robot",
  Rocket : "rocket",
  Rotate3d : "rotate-3d",
  Rotate90 : "rotate-90",
  RotateLeft : "rotate-left",
  RotateLeftVariant : "rotate-left-variant",
  RotateRight : "rotate-right",
  RotateRightVariant : "rotate-right-variant",
  RoundedCorner : "rounded-corner",
  RouterWireless : "router-wireless",
  Routes : "routes",
  Rowing : "rowing",
  Rss : "rss",
  RssBox : "rss-box",
  Ruler : "ruler",
  Run : "run",
  Sale : "sale",
  Satellite : "satellite",
  SatelliteVariant : "satellite-variant",
  Saxophone : "saxophone",
  Scale : "scale",
  ScaleBalance : "scale-balance",
  ScaleBathroom : "scale-bathroom",
  Scanner : "scanner",
  School : "school",
  ScreenRotation : "screen-rotation",
  ScreenRotationLock : "screen-rotation-lock",
  Screwdriver : "screwdriver",
  Script : "script",
  Sd : "sd",
  Seal : "seal",
  SeatFlat : "seat-flat",
  SeatFlatAngled : "seat-flat-angled",
  SeatIndividualSuite : "seat-individual-suite",
  SeatLegroomExtra : "seat-legroom-extra",
  SeatLegroomNormal : "seat-legroom-normal",
  SeatLegroomReduced : "seat-legroom-reduced",
  SeatReclineExtra : "seat-recline-extra",
  SeatReclineNormal : "seat-recline-normal",
  Security : "security",
  SecurityHome : "security-home",
  SecurityNetwork : "security-network",
  Select : "select",
  SelectAll : "select-all",
  SelectInverse : "select-inverse",
  SelectOff : "select-off",
  Selection : "selection",
  Send : "send",
  SerialPort : "serial-port",
  Server : "server",
  ServerMinus : "server-minus",
  ServerNetwork : "server-network",
  ServerNetworkOff : "server-network-off",
  ServerOff : "server-off",
  ServerPlus : "server-plus",
  ServerRemove : "server-remove",
  ServerSecurity : "server-security",
  Settings : "settings",
  SettingsBox : "settings-box",
  ShapeCirclePlus : "shape-circle-plus",
  ShapePlus : "shape-plus",
  ShapePolygonPlus : "shape-polygon-plus",
  ShapeRectanglePlus : "shape-rectangle-plus",
  ShapeSquarePlus : "shape-square-plus",
  Share : "share",
  ShareVariant : "share-variant",
  Shield : "shield",
  ShieldOutline : "shield-outline",
  Shopping : "shopping",
  ShoppingMusic : "shopping-music",
  Shredder : "shredder",
  Shuffle : "shuffle",
  ShuffleDisabled : "shuffle-disabled",
  ShuffleVariant : "shuffle-variant",
  Sigma : "sigma",
  SigmaLower : "sigma-lower",
  SignCaution : "sign-caution",
  Signal : "signal",
  SignalVariant : "signal-variant",
  Silverware : "silverware",
  SilverwareFork : "silverware-fork",
  SilverwareSpoon : "silverware-spoon",
  SilverwareVariant : "silverware-variant",
  Sim : "sim",
  SimAlert : "sim-alert",
  SimOff : "sim-off",
  Sitemap : "sitemap",
  SkipBackward : "skip-backward",
  SkipForward : "skip-forward",
  SkipNext : "skip-next",
  SkipNextCircle : "skip-next-circle",
  SkipNextCircleOutline : "skip-next-circle-outline",
  SkipPrevious : "skip-previous",
  SkipPreviousCircle : "skip-previous-circle",
  SkipPreviousCircleOutline : "skip-previous-circle-outline",
  Skull : "skull",
  Skype : "skype",
  SkypeBusiness : "skype-business",
  Slack : "slack",
  Sleep : "sleep",
  SleepOff : "sleep-off",
  Smoking : "smoking",
  SmokingOff : "smoking-off",
  Snapchat : "snapchat",
  Snowman : "snowman",
  Soccer : "soccer",
  Sofa : "sofa",
  Solid : "solid",
  Sort : "sort",
  SortAlphabetical : "sort-alphabetical",
  SortAscending : "sort-ascending",
  SortDescending : "sort-descending",
  SortNumeric : "sort-numeric",
  SortVariant : "sort-variant",
  Soundcloud : "soundcloud",
  SourceBranch : "source-branch",
  SourceFork : "source-fork",
  SourceMerge : "source-merge",
  SourcePull : "source-pull",
  Speaker : "speaker",
  SpeakerOff : "speaker-off",
  Speedometer : "speedometer",
  Spellcheck : "spellcheck",
  Spotify : "spotify",
  Spotlight : "spotlight",
  SpotlightBeam : "spotlight-beam",
  Spray : "spray",
  SquareInc : "square-inc",
  SquareIncCash : "square-inc-cash",
  Stackexchange : "stackexchange",
  Stackoverflow : "stackoverflow",
  Stairs : "stairs",
  Star : "star",
  StarCircle : "star-circle",
  StarHalf : "star-half",
  StarOff : "star-off",
  StarOutline : "star-outline",
  Steam : "steam",
  Steering : "steering",
  StepBackward : "step-backward",
  StepBackward2 : "step-backward-2",
  StepForward : "step-forward",
  StepForward2 : "step-forward-2",
  Stethoscope : "stethoscope",
  Sticker : "sticker",
  Stocking : "stocking",
  Stop : "stop",
  StopCircle : "stop-circle",
  StopCircleOutline : "stop-circle-outline",
  Store : "store",
  Store24Hour : "store-24-hour",
  Stove : "stove",
  SubdirectoryArrowLeft : "subdirectory-arrow-left",
  SubdirectoryArrowRight : "subdirectory-arrow-right",
  Subway : "subway",
  SubwayVariant : "subway-variant",
  Sunglasses : "sunglasses",
  SurroundSound : "surround-sound",
  SwapHorizontal : "swap-horizontal",
  SwapVertical : "swap-vertical",
  Swim : "swim",
  Switch : "switch",
  Sword : "sword",
  Sync : "sync",
  SyncAlert : "sync-alert",
  SyncOff : "sync-off",
  Tab : "tab",
  TabUnselected : "tab-unselected",
  Table : "table",
  TableColumnPlusAfter : "table-column-plus-after",
  TableColumnPlusBefore : "table-column-plus-before",
  TableColumnRemove : "table-column-remove",
  TableColumnWidth : "table-column-width",
  TableEdit : "table-edit",
  TableLarge : "table-large",
  TableRowHeight : "table-row-height",
  TableRowPlusAfter : "table-row-plus-after",
  TableRowPlusBefore : "table-row-plus-before",
  TableRowRemove : "table-row-remove",
  Tablet : "tablet",
  TabletAndroid : "tablet-android",
  TabletIpad : "tablet-ipad",
  Tag : "tag",
  TagFaces : "tag-faces",
  TagHeart : "tag-heart",
  TagMultiple : "tag-multiple",
  TagOutline : "tag-outline",
  TagTextOutline : "tag-text-outline",
  Target : "target",
  Taxi : "taxi",
  Teamviewer : "teamviewer",
  Telegram : "telegram",
  Television : "television",
  TelevisionGuide : "television-guide",
  TemperatureCelsius : "temperature-celsius",
  TemperatureFahrenheit : "temperature-fahrenheit",
  TemperatureKelvin : "temperature-kelvin",
  Tennis : "tennis",
  Tent : "tent",
  Terrain : "terrain",
  TestTube : "test-tube",
  TextShadow : "text-shadow",
  TextToSpeech : "text-to-speech",
  TextToSpeechOff : "text-to-speech-off",
  Textbox : "textbox",
  Texture : "texture",
  Theater : "theater",
  ThemeLightDark : "theme-light-dark",
  Thermometer : "thermometer",
  ThermometerLines : "thermometer-lines",
  ThumbDown : "thumb-down",
  ThumbDownOutline : "thumb-down-outline",
  ThumbUp : "thumb-up",
  ThumbUpOutline : "thumb-up-outline",
  ThumbsUpDown : "thumbs-up-down",
  Ticket : "ticket",
  TicketAccount : "ticket-account",
  TicketConfirmation : "ticket-confirmation",
  Tie : "tie",
  Timelapse : "timelapse",
  Timer : "timer",
  Timer10 : "timer-10",
  Timer3 : "timer-3",
  TimerOff : "timer-off",
  TimerSand : "timer-sand",
  TimerSandEmpty : "timer-sand-empty",
  Timetable : "timetable",
  ToggleSwitch : "toggle-switch",
  ToggleSwitchOff : "toggle-switch-off",
  Tooltip : "tooltip",
  TooltipEdit : "tooltip-edit",
  TooltipImage : "tooltip-image",
  TooltipOutline : "tooltip-outline",
  TooltipOutlinePlus : "tooltip-outline-plus",
  TooltipText : "tooltip-text",
  Tooth : "tooth",
  Tor : "tor",
  TowerBeach : "tower-beach",
  TowerFire : "tower-fire",
  TrafficLight : "traffic-light",
  Train : "train",
  Tram : "tram",
  Transcribe : "transcribe",
  TranscribeClose : "transcribe-close",
  Transfer : "transfer",
  TransitTransfer : "transit-transfer",
  Translate : "translate",
  Tree : "tree",
  Trello : "trello",
  TrendingDown : "trending-down",
  TrendingNeutral : "trending-neutral",
  TrendingUp : "trending-up",
  Triangle : "triangle",
  TriangleOutline : "triangle-outline",
  Trophy : "trophy",
  TrophyAward : "trophy-award",
  TrophyOutline : "trophy-outline",
  TrophyVariant : "trophy-variant",
  TrophyVariantOutline : "trophy-variant-outline",
  Truck : "truck",
  TruckDelivery : "truck-delivery",
  TshirtCrew : "tshirt-crew",
  TshirtV : "tshirt-v",
  Tumblr : "tumblr",
  TumblrReblog : "tumblr-reblog",
  Tune : "tune",
  TuneVertical : "tune-vertical",
  Twitch : "twitch",
  Twitter : "twitter",
  TwitterBox : "twitter-box",
  TwitterCircle : "twitter-circle",
  TwitterRetweet : "twitter-retweet",
  Ubuntu : "ubuntu",
  Umbraco : "umbraco",
  Umbrella : "umbrella",
  UmbrellaOutline : "umbrella-outline",
  Undo : "undo",
  UndoVariant : "undo-variant",
  UnfoldLess : "unfold-less",
  UnfoldMore : "unfold-more",
  Ungroup : "ungroup",
  Unity : "unity",
  Untappd : "untappd",
  Update : "update",
  Upload : "upload",
  Usb : "usb",
  VectorArrangeAbove : "vector-arrange-above",
  VectorArrangeBelow : "vector-arrange-below",
  VectorCircle : "vector-circle",
  VectorCircleVariant : "vector-circle-variant",
  VectorCombine : "vector-combine",
  VectorCurve : "vector-curve",
  VectorDifference : "vector-difference",
  VectorDifferenceAb : "vector-difference-ab",
  VectorDifferenceBa : "vector-difference-ba",
  VectorIntersection : "vector-intersection",
  VectorLine : "vector-line",
  VectorPoint : "vector-point",
  VectorPolygon : "vector-polygon",
  VectorPolyline : "vector-polyline",
  VectorRectangle : "vector-rectangle",
  VectorSelection : "vector-selection",
  VectorSquare : "vector-square",
  VectorTriangle : "vector-triangle",
  VectorUnion : "vector-union",
  Verified : "verified",
  Vibrate : "vibrate",
  Video : "video",
  VideoOff : "video-off",
  VideoSwitch : "video-switch",
  ViewAgenda : "view-agenda",
  ViewArray : "view-array",
  ViewCarousel : "view-carousel",
  ViewColumn : "view-column",
  ViewDashboard : "view-dashboard",
  ViewDay : "view-day",
  ViewGrid : "view-grid",
  ViewHeadline : "view-headline",
  ViewList : "view-list",
  ViewModule : "view-module",
  ViewQuilt : "view-quilt",
  ViewStream : "view-stream",
  ViewWeek : "view-week",
  Vimeo : "vimeo",
  Vine : "vine",
  Violin : "violin",
  Visualstudio : "visualstudio",
  Vk : "vk",
  VkBox : "vk-box",
  VkCircle : "vk-circle",
  Vlc : "vlc",
  Voice : "voice",
  Voicemail : "voicemail",
  VolumeHigh : "volume-high",
  VolumeLow : "volume-low",
  VolumeMedium : "volume-medium",
  VolumeOff : "volume-off",
  Vpn : "vpn",
  Walk : "walk",
  Wallet : "wallet",
  WalletGiftcard : "wallet-giftcard",
  WalletMembership : "wallet-membership",
  WalletTravel : "wallet-travel",
  Wan : "wan",
  Watch : "watch",
  WatchExport : "watch-export",
  WatchImport : "watch-import",
  WatchVibrate : "watch-vibrate",
  Water : "water",
  WaterOff : "water-off",
  WaterPercent : "water-percent",
  WaterPump : "water-pump",
  Watermark : "watermark",
  WeatherCloudy : "weather-cloudy",
  WeatherFog : "weather-fog",
  WeatherHail : "weather-hail",
  WeatherLightning : "weather-lightning",
  WeatherLightningRainy : "weather-lightning-rainy",
  WeatherNight : "weather-night",
  WeatherPartlycloudy : "weather-partlycloudy",
  WeatherPouring : "weather-pouring",
  WeatherRainy : "weather-rainy",
  WeatherSnowy : "weather-snowy",
  WeatherSnowyRainy : "weather-snowy-rainy",
  WeatherSunny : "weather-sunny",
  WeatherSunset : "weather-sunset",
  WeatherSunsetDown : "weather-sunset-down",
  WeatherSunsetUp : "weather-sunset-up",
  WeatherWindy : "weather-windy",
  WeatherWindyVariant : "weather-windy-variant",
  Web : "web",
  Webcam : "webcam",
  Webhook : "webhook",
  Wechat : "wechat",
  Weight : "weight",
  WeightKilogram : "weight-kilogram",
  Whatsapp : "whatsapp",
  WheelchairAccessibility : "wheelchair-accessibility",
  WhiteBalanceAuto : "white-balance-auto",
  WhiteBalanceIncandescent : "white-balance-incandescent",
  WhiteBalanceIridescent : "white-balance-iridescent",
  WhiteBalanceSunny : "white-balance-sunny",
  Wifi : "wifi",
  WifiOff : "wifi-off",
  Wii : "wii",
  Wikipedia : "wikipedia",
  WindowClose : "window-close",
  WindowClosed : "window-closed",
  WindowMaximize : "window-maximize",
  WindowMinimize : "window-minimize",
  WindowOpen : "window-open",
  WindowRestore : "window-restore",
  Windows : "windows",
  Wordpress : "wordpress",
  Worker : "worker",
  Wrap : "wrap",
  Wrench : "wrench",
  Wunderlist : "wunderlist",
  Xaml : "xaml",
  Xbox : "xbox",
  XboxController : "xbox-controller",
  XboxControllerOff : "xbox-controller-off",
  Xda : "xda",
  Xing : "xing",
  XingBox : "xing-box",
  XingCircle : "xing-circle",
  Xml : "xml",
  Yeast : "yeast",
  Yelp : "yelp",
  YinYang : "yin-yang",
  YoutubePlay : "youtube-play",
  ZipBox : "zip-box",

  AmericanSignLanguageInterpreting : "american-sign-language-interpreting",
  AslInterpreting : "asl-interpreting",
  AssistiveListeningSystems : "assistive-listening-systems",
  AudioDescription : "audio-description",
  Blind : "blind",
  Braille : "braille",
  Deaf : "deaf",
  Deafness : "deafness",
  Envira : "envira",
  Fa : "fa",
  FirstOrder : "first-order",
  FontAwesome : "font-awesome",
  Gitlab : "gitlab",
  Glide : "glide",
  GlideG : "glide-g",
  GooglePlusCircle : "google-plus-circle",
  GooglePlusOfficial : "google-plus-official",
  HardOfHearing : "hard-of-hearing",
  LowVision : "low-vision",
  QuestionCircleO : "question-circle-o",
  SignLanguage : "sign-language",
  Signing : "signing",
  Snapchat : "snapchat",
  SnapchatGhost : "snapchat-ghost",
  SnapchatSquare : "snapchat-square",
  Themeisle : "themeisle",
  UniversalAccess : "universal-access",
  Viadeo : "viadeo",
  ViadeoSquare : "viadeo-square",
  VolumeControlPhone : "volume-control-phone",
  WheelchairAlt : "wheelchair-alt",
  Wpbeginner : "wpbeginner",
  Wpforms : "wpforms",
  Yoast : "yoast"

};

// ==================================================================================================================

tulip.EventType = {
  TLP_DELETE : 0,
  TLP_MODIFICATION : 1,
  TLP_INFORMATION : 2,
  TLP_INVALID : 3
};

tulip.Event = function tulip_Event(sender, type) {
  var newObject = createObject(tulip.Event, this);
  if (tulip_Event.caller == null || tulip_Event.caller.name != "createObject") {
    newObject.sender = sender;
    newObject.type = type;
  }
  return newObject;
};
tulip.Event.prototype.getSender = function() {
  return this.sender;
};
tulip.Event.prototype.getType = function() {
  return this.type;
};

tulip.GraphEventType = {
  TLP_ADD_NODE : 0,
  TLP_DEL_NODE : 1,
  TLP_ADD_EDGE : 2,
  TLP_DEL_EDGE : 3,
  TLP_REVERSE_EDGE : 4,
  TLP_BEFORE_SET_ENDS : 5,
  TLP_AFTER_SET_ENDS : 6,
  TLP_ADD_NODES : 7,
  TLP_ADD_EDGES : 8,
  TLP_BEFORE_ADD_DESCENDANTGRAPH : 9,
  TLP_AFTER_ADD_DESCENDANTGRAPH : 10,
  TLP_BEFORE_DEL_DESCENDANTGRAPH : 11,
  TLP_AFTER_DEL_DESCENDANTGRAPH : 12,
  TLP_BEFORE_ADD_SUBGRAPH : 13,
  TLP_AFTER_ADD_SUBGRAPH : 14,
  TLP_BEFORE_DEL_SUBGRAPH : 15,
  TLP_AFTER_DEL_SUBGRAPH : 16,
  TLP_ADD_LOCAL_PROPERTY : 17,
  TLP_BEFORE_DEL_LOCAL_PROPERTY : 18,
  TLP_AFTER_DEL_LOCAL_PROPERTY : 19,
  TLP_ADD_INHERITED_PROPERTY : 20,
  TLP_BEFORE_DEL_INHERITED_PROPERTY : 21,
  TLP_AFTER_DEL_INHERITED_PROPERTY : 22,
  TLP_BEFORE_RENAME_LOCAL_PROPERTY : 23,
  TLP_AFTER_RENAME_LOCAL_PROPERTY : 24,
  TLP_BEFORE_SET_ATTRIBUTE : 25,
  TLP_AFTER_SET_ATTRIBUTE : 26,
  TLP_REMOVE_ATTRIBUTE : 27,
  TLP_BEFORE_ADD_LOCAL_PROPERTY : 28,
  TLP_BEFORE_ADD_INHERITED_PROPERTY : 29
};

tulip.GraphEvent = function tulip_GraphEvent(graph, eventType) {
  var newObject = createObject(tulip.GraphEvent, this);
  if (tulip_GraphEvent.caller == null || tulip_GraphEvent.caller.name != "createObject") {
    newObject.graph = graph;
    newObject.eventType = eventType;
    newObject.node = null;
    newObject.edge = null;
    newObject.name = null;
    newObject.nodes = null;
    newObject.edges = null;
    newObject.subgraph = null;
    newObject.property = null;
    tulip.Event.call(newObject, graph, tulip.EventType.TLP_MODIFICATION);
  }
  return newObject;
};

tulip.GraphEvent.inheritsFrom(tulip.Event);

tulip.GraphEvent.prototype.getGraph = function() {
  return this.graph;
};

tulip.GraphEvent.prototype.getEventType = function() {
  return this.eventType;
};

tulip.GraphEvent.prototype.getNode = function() {
  return this.node;
};

tulip.GraphEvent.prototype.getEdge = function() {
  return this.edge;
};

tulip.GraphEvent.prototype.getNodes = function() {
  return this.nodes;
};

tulip.GraphEvent.prototype.getEdges = function() {
  return this.edges;
};

tulip.GraphEvent.prototype.getSubGraph = function() {
  return this.subgraph;
};

tulip.GraphEvent.prototype.getAttributeName = function() {
  return this.name;
};

tulip.GraphEvent.prototype.getPropertyName = function() {
  return this.name;
};

tulip.GraphEvent.prototype.getProperty = function() {
  return this.property;
};
// ==================================================================================================================

tulip.PropertyEventType = {
  TLP_BEFORE_SET_NODE_VALUE: 0,
  TLP_AFTER_SET_NODE_VALUE: 1,
  TLP_BEFORE_SET_ALL_NODE_VALUE : 2,
  TLP_AFTER_SET_ALL_NODE_VALUE : 3,
  TLP_BEFORE_SET_ALL_EDGE_VALUE : 4,
  TLP_AFTER_SET_ALL_EDGE_VALUE : 5,
  TLP_BEFORE_SET_EDGE_VALUE : 6,
  TLP_AFTER_SET_EDGE_VALUE : 7
};

tulip.PropertyEvent = function tulip_PropertyEvent(prop, eventType) {
  var newObject = createObject(tulip.PropertyEvent, this);
  if (tulip_PropertyEvent.caller == null || tulip_PropertyEvent.caller.name != "createObject") {
    newObject.property = prop;
    newObject.eventType = eventType;
    newObject.node = null;
    newObject.edge = null;
    tulip.Event.call(newObject, prop, tulip.EventType.TLP_MODIFICATION);
  }
  return newObject;
};

tulip.PropertyEvent.inheritsFrom(tulip.Event);

tulip.PropertyEvent.prototype.getProperty = function() {
  return this.property;
};

tulip.PropertyEvent.prototype.getEventType = function() {
  return this.eventType;
};

tulip.PropertyEvent.prototype.getNode = function() {
  return this.node;
};

tulip.PropertyEvent.prototype.getEdge = function() {
  return this.edge;
};

// ==================================================================================================================

var tulipListeners = {};
var tulipObservers = {};

tulip.addListener = function(sender, listener) {
  checkArgumentsTypes(arguments, [[tulip.Graph, tulip.PropertyInterface], ["function", "object"]], 2);
  if (!tulipListeners.hasOwnProperty(sender.cppPointer)) {
    tulipListeners[sender.cppPointer] = [];
  }
  tulipListeners[sender.cppPointer].push(listener);
};

tulip.removeListener = function(sender, listener) {
  checkArgumentsTypes(arguments, [[tulip.Graph, tulip.PropertyInterface], ["function", "object"]], 2);
  if (!tulipListeners.hasOwnProperty(sender.cppPointer)) {
    return;
  }
  for (var i = 0, len = tulipListeners[sender.cppPointer].length ; i < len ; i++) {
    if (tulipListeners[sender.cppPointer][i] === listener) {
      tulipListeners[sender.cppPointer].splice(i, 1);
      if (tulipListeners[sender.cppPointer].length == 0) {
        delete tulipListeners[sender.cppPointer];
      }
      break;
    }
  }
};

tulip.hasListener = function(senderId) {
  return tulipListeners.hasOwnProperty(senderId);
};

tulip.addObserver = function(sender, observer) {
  checkArgumentsTypes(arguments, [[tulip.Graph, tulip.PropertyInterface], ["function", "object"]], 2);
  if (!tulipObservers.hasOwnProperty(sender.cppPointer)) {
    tulipObservers[sender.cppPointer] = [];
  }
  tulipObservers[sender.cppPointer].push(observer);
};

tulip.removeObserver = function(sender, observer) {
  checkArgumentsTypes(arguments, [[tulip.Graph, tulip.PropertyInterface], ["function", "object"]], 2);
  if (!tulipObservers.hasOwnProperty(sender.cppPointer)) {
    return;
  }
  for (var i = 0, len = tulipObservers[sender.cppPointer].length ; i < len ; i++) {
    if (tulipObservers[sender.cppPointer][i] === observer) {
      tulipObservers[sender.cppPointer].splice(i, 1);
      if (tulipObservers[sender.cppPointer].length == 0) {
        delete tulipObservers[sender.cppPointer];
      }
      break;
    }
  }
};

tulip.hasObserver = function(senderId) {
  return tulipObservers.hasOwnProperty(senderId);
};

tulip.sendEventToListeners = function(senderId, event) {
  if (!tulipListeners.hasOwnProperty(senderId)) {
    return;
  }
  for (var i = 0, len = tulipListeners[senderId].length ; i < len ; i++) {
    if (typeOf(tulipListeners[senderId][i]) == "function") {
      tulipListeners[senderId][i](event);
    } else if (typeOf(tulipListeners[senderId][i]) == "object" && typeOf(tulipListeners[senderId][i].treatEvent) == "function") {
      tulipListeners[senderId][i].treatEvent(event);
    }
  }
};

tulip.sendEventsToObservers = function(events) {
  var observerEvents = {};
  var observers = [];

  for (var senderId in tulipObservers) {
    if (tulipObservers.hasOwnProperty(senderId)) {
      for (var j = 0 ; j < tulipObservers[senderId].length ; ++j) {
        var observer = tulipObservers[senderId][j];
        if (observers.indexOf(observer) == -1) {
          observers.push(observer);
        }
      }
    }
  }

  for (var i = 0 ; i < events.length ; ++i) {
    for (var senderId in tulipObservers) {
      if (tulipObservers.hasOwnProperty(senderId)) {
        for (var j = 0 ; j < tulipObservers[senderId].length ; ++j) {
          var key = observers.indexOf(tulipObservers[senderId][j]);
          if (events[i].getSender().cppPointer.toString() == senderId) {
            if (!observerEvents.hasOwnProperty(key)) {
              observerEvents[key] = [];
            }
            observerEvents[key].push(events[i]);
          }
        }
      }
    }
  }

  for (var observerIdx in observerEvents) {
    var observer = observers[parseInt(observerIdx)];
    if (typeOf(observer) == "function") {
      observer(observerEvents[observerIdx]);
    } else if (typeOf(observer) == "object" && typeOf(observer.treatEvents) == "function") {
      observer.treatEvents(observerEvents[observerIdx]);
    }
  }


};

// ==================================================================================================================

if (workerMode) {

  var graphObject = {};
  var propertiesNames = {};
  var curNodeId = {};
  var curEdgeId = {};
  var propertiesFilter = {};
  var updateMode = {};
  var subGraphsData = {};
  var curSubGraphIdx = {};
  var nodesData = {};
  var edgesData = {};
  var graphs = {}

  var maxBatchSize = 300;
  var graphElementsBatchSize = {};

  function clamp(val, min, max) {
    return Math.max(Math.min(val, max), min);
  }

  function getNodeObject(graphHierarchyId, nodeId) {
    var graph = graphObject[graphHierarchyId];
    var nodeDataObj = {};
    nodeDataObj.nodeId = nodeId;
    nodeDataObj.properties = {};
    for (var j = 0 ; j < propertiesNames[graphHierarchyId].length ; ++j) {
      var propertyName = propertiesNames[graphHierarchyId][j];
      if (!propertiesFilter[graphHierarchyId] || propertiesFilter[graphHierarchyId].indexOf(propertyName) != -1) {
        var propertyData = graph.properties[propertyName];
        if (propertyData.nodesValues != undefined) {
          if (propertyData.nodesValues[nodeId] != undefined) {
            nodeDataObj.properties[propertyName] = propertyData.nodesValues[nodeId];
          } else {
            nodeDataObj.properties[propertyName] = propertyData.nodeDefault;
          }
        } else {
          nodeDataObj.properties[propertyName] = propertyData.nodeDefault;
        }
      }
    }
    return nodeDataObj;
  }

  function getEdgeObject(graphHierarchyId, edgeId) {
    var graph = graphObject[graphHierarchyId];
    if (graph.edgesNumber == 0) {
      return null;
    }
    var edgeDataObj = {};
    edgeDataObj.edgeId = edgeId;
    edgeDataObj.srcNodeId = graph.edges[edgeId][0];
    edgeDataObj.tgtNodeId = graph.edges[edgeId][1];
    edgeDataObj.properties = {};
    for (var j = 0 ; j < propertiesNames[graphHierarchyId].length ; ++j) {
      var propertyName = propertiesNames[graphHierarchyId][j];
      if (!propertiesFilter[graphHierarchyId] || propertiesFilter[graphHierarchyId].indexOf(propertyName) != -1) {
        var propertyData = graph.properties[propertyName];
        if (propertyData.edgesValues != undefined) {
          if (propertyData.edgesValues[edgeId] != undefined) {
            edgeDataObj.properties[propertyName] = propertyData.edgesValues[edgeId];
          } else {
            edgeDataObj.properties[propertyName] = propertyData.edgeDefault;
          }
        } else {
          edgeDataObj.properties[propertyName] = propertyData.edgeDefault;
        }
      }
    }
    return edgeDataObj;
  }

  function fetchNextNodesData(graphHierarchyId) {
    curNodeId[graphHierarchyId] = curNodeId[graphHierarchyId] + 1;
    nodesData[graphHierarchyId] = [];
    if (curNodeId[graphHierarchyId] == graphObject[graphHierarchyId].nodesNumber) {
      return;
    }
    var boundNodeId = curNodeId[graphHierarchyId] + graphElementsBatchSize[graphHierarchyId];
    if (boundNodeId >= graphObject[graphHierarchyId].nodesNumber) {
      boundNodeId = graphObject[graphHierarchyId].nodesNumber;
    }
    for (var i = curNodeId[graphHierarchyId] ; i < boundNodeId ; ++i) {
      nodesData[graphHierarchyId].push(getNodeObject(graphHierarchyId, i));
    }
    curNodeId[graphHierarchyId] = boundNodeId - 1;
  }

  function fetchNextEdgesData(graphHierarchyId) {
    curEdgeId[graphHierarchyId] = curEdgeId[graphHierarchyId] + 1;
    edgesData[graphHierarchyId] = [];
    if (curEdgeId[graphHierarchyId] == graphObject[graphHierarchyId].edgesNumber) {
      return;
    }
    var boundEdgeId = curEdgeId[graphHierarchyId] + graphElementsBatchSize[graphHierarchyId];
    if (boundEdgeId >= graphObject[graphHierarchyId].edgesNumber) {
      boundEdgeId = graphObject[graphHierarchyId].edgesNumber;
    }
    for (var i = curEdgeId[graphHierarchyId] ; i < boundEdgeId ; ++i) {
      edgesData[graphHierarchyId].push(getEdgeObject(graphHierarchyId, i));
    }
    curEdgeId[graphHierarchyId] = boundEdgeId - 1;
  }

  var _setPluginProgressGraphId = Module.cwrap('setPluginProgressGraphId', null, ['number']);

  function sendGraphData(graphHierarchyId) {
    curNodeId[graphHierarchyId] = 0;
    curEdgeId[graphHierarchyId] = 0;
    self.postMessage({eventType : 'addNodes', graphHierarchyId : graphHierarchyId,
                       nodesJson : JSON.stringify([getNodeObject(graphHierarchyId, 0)]),
                       lastNodeId : 0});
  }

  function prepareSubGraphsData(graphHierarchyId, graphObj) {
    for (var i = 0 ; i < graphObj.subgraphs.length ; ++i) {
      var subGraphObj = graphObj.subgraphs[i];
      var subGraphDataObj = {};
      subGraphDataObj.parentGraphId = graphObj.graphID;
      subGraphDataObj.subGraphId = subGraphObj.graphID;
      subGraphDataObj.nodesIds = "(";
      for (var j = 0 ; j < subGraphObj.nodesIDs.length ; ++j) {
        if (typeOf(subGraphObj.nodesIDs[j]) == "number") {
          subGraphDataObj.nodesIds += subGraphObj.nodesIDs[j].toString();
        } else {
          for (var k = subGraphObj.nodesIDs[j][0] ; k <= subGraphObj.nodesIDs[j][1] ; ++k) {
            subGraphDataObj.nodesIds += k.toString();
            if (k != subGraphObj.nodesIDs[j][1]) {
              subGraphDataObj.nodesIds += ", ";
            }
          }
        }
        if (j != subGraphObj.nodesIDs.length - 1) {
          subGraphDataObj.nodesIds += ", ";
        }
      }
      subGraphDataObj.nodesIds += ")";
      subGraphDataObj.edgesIds = "(";
      for (var j = 0 ; j < subGraphObj.edgesIDs.length ; ++j) {
        if (typeOf(subGraphObj.edgesIDs[j]) == "number") {
          subGraphDataObj.edgesIds += subGraphObj.edgesIDs[j].toString();
        } else {
          for (var k = subGraphObj.edgesIDs[j][0] ; k <= subGraphObj.edgesIDs[j][1] ; ++k) {
            subGraphDataObj.edgesIds += k.toString();
            if (k != subGraphObj.edgesIDs[j][1]) {
              subGraphDataObj.edgesIds += ", ";
            }
          }
        }
        if (j != subGraphObj.edgesIDs.length - 1) {
          subGraphDataObj.edgesIds += ", ";
        }
      }
      subGraphDataObj.edgesIds += ")";
      subGraphDataObj.attributes = JSON.stringify(subGraphObj.attributes);
      subGraphDataObj.properties = JSON.stringify(subGraphObj.properties);
      subGraphsData[graphHierarchyId].push(subGraphDataObj);
      prepareSubGraphsData(graphHierarchyId, subGraphObj);
    }
  }

  function getGraphData(graphHierarchyId, graphDataLoadedCallback) {
    tulip.sendProgressComment(graphHierarchyId, "Exporting graph to JSON format ...");
    tulip.sendProgressValue(graphHierarchyId, -1);
    setTimeout(function() {
      var jsonGraphStr = graphs[graphHierarchyId].toJSON();
      var jsonGraph = JSON.parse(jsonGraphStr);
      graphObject[graphHierarchyId] = jsonGraph.graph;
      subGraphsData[graphHierarchyId] = [];
      prepareSubGraphsData(graphHierarchyId, graphObject[graphHierarchyId]);
      graphElementsBatchSize[graphHierarchyId] = clamp(graphObject[graphHierarchyId].nodesNumber, 1, maxBatchSize) | 0;
      var propertiesNamesTmp = Object.keys(graphObject[graphHierarchyId].properties);
      propertiesNames[graphHierarchyId] = [];
      for (var i = 0 ; i < propertiesNamesTmp.length ; ++i) {
        propertiesNames[graphHierarchyId].push(propertiesNamesTmp[i]);
      }
      graphDataLoadedCallback();
    }, 0);
  }

  function loadGraph(graphHierarchyId, graphFilePath, sendData) {
    _setPluginProgressGraphId(graphHierarchyId);
    graphs[graphHierarchyId] = tulip.loadGraph(graphFilePath, sendData);
    if (!sendData) return;
    getGraphData(graphHierarchyId, function() {
      updateMode[graphHierarchyId] = false;
      self.postMessage({eventType : 'startGraphData',
                         graphHierarchyId : graphHierarchyId,
                         numberOfNodes : graphObject[graphHierarchyId].nodesNumber,
                         numberOfEdges : graphObject[graphHierarchyId].edgesNumber,
                         numberOfSubgraphs : subGraphsData[graphHierarchyId].length,
                         graphAttributes : JSON.stringify(graphObject[graphHierarchyId].attributes)
                       });
      var propertiesData = {};
      for (var i = 0 ; i < propertiesNames[graphHierarchyId].length ; ++i) {
        propertiesData[propertiesNames[graphHierarchyId][i]] = {};
        propertiesData[propertiesNames[graphHierarchyId][i]].type = graphObject[graphHierarchyId].properties[propertiesNames[graphHierarchyId][i]].type;
        propertiesData[propertiesNames[graphHierarchyId][i]].nodeDefault = graphObject[graphHierarchyId].properties[propertiesNames[graphHierarchyId][i]].nodeDefault;
        propertiesData[propertiesNames[graphHierarchyId][i]].edgeDefault = graphObject[graphHierarchyId].properties[propertiesNames[graphHierarchyId][i]].edgeDefault;
      }
      self.postMessage({eventType : 'createGraphProperties', graphHierarchyId : graphHierarchyId, properties : propertiesData});
      sendGraphData(graphHierarchyId);
    });
  }

  self.addEventListener('message', function(e) {
    var data = e.data;
    if (!data) return;
    switch (data.eventType) {
    case 'loadGraph':
      if (!data.graphFileData) {
        var graphReq = new XMLHttpRequest();
        graphReq.open("GET", data.graphFile, true);
        graphReq.responseType = "arraybuffer";
        graphReq.onload = function (oEvent) {
          var arrayBuffer = graphReq.response;
          var file = FS.findObject(data.graphFile);
          if (!file) {
            var paths = data.graphFile.split('/');
            var filePath = "/";
            for (var i = 0; i < paths.length - 1; ++i) {
              filePath += paths[i];
              filePath += "/";
            }
            FS.createPath('/', filePath, true, true);
            FS.createFile('/', data.graphFile, {}, true, true);
          }
          FS.writeFile(data.graphFile, new Uint8Array(arrayBuffer), {'encoding' : 'binary'});
          var graphToDestroy = null;
          if (data.graphHierarchyId in graphs) {
            graphToDestroy = graphs[data.graphHierarchyId];
          }
          loadGraph(data.graphHierarchyId, data.graphFile, data.sendDataBack);
          if (graphToDestroy) graphToDestroy.destroy();
        };
        graphReq.send(null);
      } else {
        var file = FS.findObject('/' + data.graphFile);
        if (!file) {
          var paths = data.graphFile.split('/');
          var filePath = "/";
          for (var i = 0; i < paths.length - 1; ++i) {
            filePath += paths[i];
            filePath += "/";
          }
          FS.createPath('/', filePath, true, true);
          FS.createFile('/', data.graphFile, {}, true, true);
        }
        FS.writeFile('/' + data.graphFile, new Uint8Array(data.graphFileData), {'encoding' : 'binary'});
        var graphToDestroy = null;
        if (data.graphHierarchyId in graphs) {
          graphToDestroy = graphs[data.graphHierarchyId];
        }
        loadGraph(data.graphHierarchyId, data.graphFile, data.sendDataBack);
        FS.unlink(data.graphFile);
        if (graphToDestroy) graphToDestroy.destroy();
      }
      break;
    case 'sendNextNodes':
      fetchNextNodesData(data.graphHierarchyId);
      if (nodesData[data.graphHierarchyId].length > 0) {
        self.postMessage({eventType : 'addNodes', graphHierarchyId : data.graphHierarchyId,
                           nodesJson : JSON.stringify(nodesData[data.graphHierarchyId]),
                           lastNodeId : nodesData[data.graphHierarchyId][nodesData[data.graphHierarchyId].length - 1].nodeId});
      } else {
        graphElementsBatchSize[data.graphHierarchyId] = clamp(graphObject[data.graphHierarchyId].edgesNumber, 1, maxBatchSize) | 0;
        self.postMessage({eventType : 'addEdges', graphHierarchyId : data.graphHierarchyId,
                           edgesJson : JSON.stringify([getEdgeObject(data.graphHierarchyId, 0)]),
                           lastEdgeId : 0});
      }
      break;
    case 'sendNextEdges':
      fetchNextEdgesData(data.graphHierarchyId);
      if (edgesData[data.graphHierarchyId].length == 0) {
        if (!updateMode[data.graphHierarchyId]) {
          if (subGraphsData[data.graphHierarchyId].length > 0) {
            curSubGraphIdx[data.graphHierarchyId] = 0;
            self.postMessage({eventType : 'addSubGraph', graphHierarchyId : data.graphHierarchyId, subGraphData : subGraphsData[data.graphHierarchyId][0]});
          } else {
            self.postMessage({eventType : 'endGraphData', graphHierarchyId : data.graphHierarchyId});
          }
        } else {
          if (subGraphsData[data.graphHierarchyId].length > 0) {
            curSubGraphIdx[data.graphHierarchyId] = 0;
            self.postMessage({eventType : 'addSubGraph', graphHierarchyId : data.graphHierarchyId, subGraphData : subGraphsData[data.graphHierarchyId][0]});
          } else {
            self.postMessage({eventType : 'endGraphUpdate', graphHierarchyId : data.graphHierarchyId});
          }
        }
        propertiesFilter[data.graphHierarchyId] = null;
        return;
      }
      self.postMessage({eventType : 'addEdges', graphHierarchyId : data.graphHierarchyId,
                        edgesJson : JSON.stringify(edgesData[data.graphHierarchyId]),
                        lastEdgeId : edgesData[data.graphHierarchyId][edgesData[data.graphHierarchyId].length - 1].edgeId});
      break;
    case 'sendNextSubGraph':
      curSubGraphIdx[data.graphHierarchyId] = curSubGraphIdx[data.graphHierarchyId] + 1;
      if (curSubGraphIdx[data.graphHierarchyId] < subGraphsData[data.graphHierarchyId].length) {
        self.postMessage({eventType : 'addSubGraph', graphHierarchyId : data.graphHierarchyId, subGraphData : subGraphsData[data.graphHierarchyId][curSubGraphIdx[data.graphHierarchyId]]});
      } else {
        if (!updateMode[data.graphHierarchyId]) {
          self.postMessage({eventType : 'endGraphData', graphHierarchyId : data.graphHierarchyId});
        } else {
          self.postMessage({eventType : 'endGraphUpdate', graphHierarchyId : data.graphHierarchyId});
        }
      }
      break;
    case 'algorithm' :
      _setPluginProgressGraphId(data.graphHierarchyId);
      var graph = graphs[data.graphHierarchyId];
      if (data.graphId != 0) {
        graph =  graph.getDescendantGraph(data.graphId);
      }
      var algoSucceed = graph.applyAlgorithm(data.algorithmName, JSON.parse(data.parameters), true);
      propertiesFilter[data.graphHierarchyId] = null;
      updateMode[data.graphHierarchyId] = true;
      getGraphData(data.graphHierarchyId, function() {
        self.postMessage({eventType : 'startGraphUpdate',
                          graphHierarchyId : data.graphHierarchyId,
                          graphId : data.graphId,
                          clearGraph : true,
                          algoSucceed : algoSucceed,
                          numberOfNodes : graphObject[data.graphHierarchyId].nodesNumber,
                          numberOfEdges : graphObject[data.graphHierarchyId].edgesNumber,
                          numberOfSubgraphs : subGraphsData[data.graphHierarchyId].length,
                          graphAttributes : JSON.stringify(graphObject[data.graphHierarchyId].attributes)});
        sendGraphData(data.graphHierarchyId);
      });
      break;
    case 'propertyAlgorithm' :
      _setPluginProgressGraphId(data.graphHierarchyId);
      var graph = graphs[data.graphHierarchyId];
      if (data.graphId != 0) {
        graph =  graph.getDescendantGraph(data.graphId);
      }
      var resultProp = graph.getProperty(data.resultPropertyName);
      var algoSucceed = graph.applyPropertyAlgorithm(data.algorithmName, resultProp, JSON.parse(data.parameters), true);
      propertiesFilter[data.graphHierarchyId] = [data.resultPropertyName];
      updateMode[data.graphHierarchyId] = true;
      getGraphData(data.graphHierarchyId, function() {
        self.postMessage({eventType : 'startGraphUpdate',
                           graphHierarchyId : data.graphHierarchyId,
                           graphId : data.graphId,
                           clearGraph : false,
                           algoSucceed : algoSucceed,
                           numberOfNodes : graphObject[data.graphHierarchyId].nodesNumber,
                           numberOfEdges : graphObject[data.graphHierarchyId].edgesNumber,
                           numberOfSubgraphs : subGraphsData[data.graphHierarchyId].length,
                           graphAttributes : JSON.stringify(graphObject[data.graphHierarchyId].attributes)});
        sendGraphData(data.graphHierarchyId);
      });
      break;
    case 'executeGraphScript' :
      _setPluginProgressGraphId(data.graphHierarchyId);
      var graph = graphs[data.graphHierarchyId];
      if (data.graphId != 0) {
        graph =  graph.getDescendantGraph(data.graphId);
      }
      var scriptSucceed = true;
      try {
        eval("f = " + data.scriptCode + "; f(graph, data.graphHierarchyId, " + JSON.stringify(data.scriptParameters) + " );");
      } catch (e) {
        console.log("exception caught");
        console.log(e);
        scriptSucceed = false;
      }
      propertiesFilter[data.graphHierarchyId] = null;
      updateMode[data.graphHierarchyId] = true;
      getGraphData(data.graphHierarchyId, function() {
        self.postMessage({eventType : 'startGraphUpdate',
                           graphHierarchyId : data.graphHierarchyId,
                           graphId : data.graphId,
                           clearGraph : true,
                           algoSucceed : scriptSucceed,
                           numberOfNodes : graphObject[data.graphHierarchyId].nodesNumber,
                           numberOfEdges : graphObject[data.graphHierarchyId].edgesNumber,
                           numberOfSubgraphs : subGraphsData[data.graphHierarchyId].length,
                           graphAttributes : JSON.stringify(graphObject[data.graphHierarchyId].attributes)});
        sendGraphData(data.graphHierarchyId);
      });
      break;
    };
  }, false);

  tulip.sendProgressValue = function(graphHierarchyId, val) {
    self.postMessage({eventType: 'progressValue', graphHierarchyId : graphHierarchyId, value: val});
  };

  tulip.sendProgressComment = function(graphHierarchyId, text) {
    self.postMessage({eventType: 'progressComment', graphHierarchyId : graphHierarchyId, comment: text});
  }

} else {

  // ===================================================================================================================

  var graphLoadedCallbacks = {};
  var algorithmFinishedCallbacks = {};
  var algorithmSucceed = {};
  var algorithmGraphId = {};
  var graphHierarchyIdToWrapper = {};

  var _fillMetaGraphInfos = Module.cwrap('fillMetaGraphInfos', null, ['number']);
  var _parseGraphAttributesJSONData =  Module.cwrap('parseGraphAttributesJSONData', null, ['number', 'string']);
  var _createGraphProperty = Module.cwrap('createGraphProperty', null, ['number', 'string', 'string', 'string', 'string']);
  var _addSubGraph = Module.cwrap('addSubGraph', 'number', ['number', 'number', 'number', 'string', 'string', 'string', 'string']);
  var _parseNodesJSONData = Module.cwrap('parseNodesJSONData', null, ['number', 'string']);
  var _parseEdgesJSONData = Module.cwrap('parseEdgesJSONData', null, ['number', 'string']);

  tulip.Graph.prototype._createGraphProperties = function(properties) {
    var propertiesNames = Object.keys(properties);
    for (var i = 0 ; i < propertiesNames.length ; ++i) {
      var propertyName = propertiesNames[i];
      var propertyData = properties[propertyName];
      var propertyType = propertyData.type;
      var propertyNodeDefaultValue = propertyData.nodeDefault;
      var propertyEdgeDefaultValue = propertyData.edgeDefault;
      _createGraphProperty(this.cppPointer, propertyType, propertyName, propertyNodeDefaultValue, propertyEdgeDefaultValue);
    }
  };

  tulip.Graph.prototype._fillMetaGraphInfos = function() {
    _fillMetaGraphInfos(this.cppPointer);
  };

  tulip.Graph.prototype._parseGraphAttributesJSONData = function(graphAttributesJSONData) {
    if (graphAttributesJSONData) {
      _parseGraphAttributesJSONData(this.cppPointer, graphAttributesJSONData);
    }
  };

  tulip.Graph.prototype._parseNodesJSONData = function(nodesJSONData) {
    if (nodesJSONData) {
      _parseNodesJSONData(this.cppPointer, nodesJSONData);
    }
  };

  tulip.Graph.prototype._parseEdgesJSONData = function(edgesJSONData) {
    if (edgesJSONData) {
      _parseEdgesJSONData(this.cppPointer, edgesJSONData);
    }
  };

  tulip.Graph.prototype._addSubGraph = function(subGraphData) {
    if (subGraphData) {
      _addSubGraph(this.cppPointer, subGraphData.parentGraphId, subGraphData.subGraphId, subGraphData.nodesIds, subGraphData.edgesIds, subGraphData.attributes, subGraphData.properties);
    }
  };

  if (tulip.vizFeatures) {

    var _centerScene = Module.cwrap('centerScene', null, ['string']);
    var _startGraphViewData = Module.cwrap('startGraphViewData', null, ['string']);
    var _endGraphViewData = Module.cwrap('endGraphViewData', null, ['string']);
    var _startGraphViewUpdate = Module.cwrap('startGraphViewUpdate', null, ['string', 'number']);
    var _endGraphViewUpdate = Module.cwrap('endGraphViewUpdate', null, ['string']);
    var _setGraphRenderingDataReady = Module.cwrap('setGraphRenderingDataReady', null, ['string', 'number']);

    var _activateInteractor = Module.cwrap('activateInteractor', null, ['string', 'string']);
    var _desactivateInteractor = Module.cwrap('desactivateInteractor', null, ['string']);

    var _initCanvas = Module.cwrap('initCanvas', null, ['string', 'number', 'number', 'number']);
    var _setCurrentCanvas = Module.cwrap('setCurrentCanvas', null, ['string']);
    var _getCurrentCanvas = Module.cwrap('getCurrentCanvas', 'string', []);
    var _resizeCanvas = Module.cwrap('resizeCanvas', null, ['string', 'number', 'number', 'number']);
    var _fullScreen = Module.cwrap('fullScreen', null, ['string']);
    var _updateGlScene = Module.cwrap('updateGlScene', null, ['string']);
    var _graphHasHull = Module.cwrap('graphHasHull', 'number', ['string', 'number']);
    var _addGraphHull = Module.cwrap('addGraphHull', null, ['string', 'number', 'number']);
    var _setGraphsHullsVisible = Module.cwrap('setGraphsHullsVisible', null, ['string', 'number', 'number']);
    var _clearGraphsHulls = Module.cwrap('clearGraphsHulls', null, ['string']);

    var _selectNodes = Module.cwrap('selectNodes', 'number', ['string', 'number', 'number', 'number', 'number']);
    var _getSelectedNodes = Module.cwrap('getSelectedNodes', null, ['number']);
    var _selectEdges = Module.cwrap('selectEdges', 'number', ['string', 'number', 'number', 'number', 'number']);
    var _getSelectedEdges = Module.cwrap('getSelectedEdges', null, ['number']);

    var graphHierarchyIdToView = {};
    var canvasIdToView = {};
    var graphData = {};

    tulip.getViewForCanvasId = function(canvasId) {
      return canvasIdToView[canvasId];
    };

  }

  if (!nodejs) {

    function createTulipWebWorker() {

        var tulipWorkerInit = false;

        var tulipWorker = new Worker(tulip.modulePrefixURL + scriptName);

        tulipWorker.addEventListener('message', function (event) {
          var delay = 0;
          var view = null;
          var graphHierarchyId = null;
          var graph = null;
          if ('graphHierarchyId' in event.data) {
            graphHierarchyId = event.data.graphHierarchyId;
            graph = graphHierarchyIdToWrapper[graphHierarchyId];
          }
          if (tulip.vizFeatures && graphHierarchyId) {
            view = graphHierarchyIdToView[graphHierarchyId];
          }
          switch (event.data.eventType) {
            case 'tulipWorkerInit':
              tulipWorkerInit = true;
              break;
            case 'print':
              console.log(event.data.text);
              break;
            case 'progressValue':
              if (tulip.vizFeatures && view) {
                view.stopBusyAnimation();
                view.setProgressBarPercent(event.data.value);
                if (event.data.value >= 0) {
                  view.draw();
                } else {
                  view.startBusyAnimation();
                }
              }
              break;
            case 'progressComment':
              if (tulip.vizFeatures && view) {
                view.setProgressBarComment(event.data.comment);
                view.draw();
              }
              break;
            case 'startGraphData':
              setTimeout(function() {
                tulip.holdObservers();
                graph._parseGraphAttributesJSONData(event.data.graphAttributes);
                if (tulip.vizFeatures && view) {
                  view.stopBusyAnimation();
                  view.setProgressBarComment("Initializing graph visualization ...");
                  view.setProgressBarPercent(0);
                  view.startGraphViewData();
                  graphData[graphHierarchyId] = event.data;
                }
              }, delay);
              break;
            case 'endGraphData':
              graph._fillMetaGraphInfos();
              setTimeout(function() {
                if (tulip.vizFeatures && view) {
                  view.setProgressBarComment("Finalizing graph rendering data ...");
                  view.endGraphViewData();
                  view.setGraphRenderingDataReady(true);
                  view.centerScene();
                }
                if (graphHierarchyId in graphLoadedCallbacks) {
                  graphLoadedCallbacks[graphHierarchyId](graph);
                }
                tulipWorker.terminate();
                tulipWorker = null;
                tulip.unholdObservers();
              }, delay);
              break;
            case 'startGraphUpdate':
              tulip.holdObservers();
              algorithmSucceed[graphHierarchyId] = event.data.algoSucceed;
              algorithmGraphId[graphHierarchyId] = event.data.graphId;
              graph._parseGraphAttributesJSONData(event.data.graphAttributes);
              if (tulip.vizFeatures && view) {
                view.stopBusyAnimation();
                setTimeout(function() {
                  view.setProgressBarComment("Updating graph visualization ...");
                  view.startGraphViewUpdate(event.data.clearGraph);
                  graphData[graphHierarchyId] = event.data;
                }, delay);
              }
              break;
            case 'endGraphUpdate':
              graph._fillMetaGraphInfos();
              if (graphHierarchyId in algorithmFinishedCallbacks) {
                var g = graph;
                if (algorithmGraphId[graphHierarchyId] != 0) {
                  g = graph.getDescendantGraph(algorithmGraphId[graphHierarchyId]);
                }
                algorithmFinishedCallbacks[graphHierarchyId](g, algorithmSucceed[graphHierarchyId]);
              }
              tulipWorker.terminate();
              tulipWorker = null;
              tulip.unholdObservers();
              if (tulip.vizFeatures && view) {
                setTimeout(function() {
                  view.endGraphViewUpdate();
                  view.setGraphRenderingDataReady(true);
                  view.centerScene();
                }, delay);
              }
              break;
            case 'createGraphProperties':
              setTimeout(function() {
                graph._createGraphProperties(event.data.properties);
              }, delay);
              break;
            case 'addNodes':
              setTimeout(function() {
                graph._parseNodesJSONData(event.data.nodesJson);
                if (tulip.vizFeatures && view) {
                  var nodeId = event.data.lastNodeId;
                  var percent = (nodeId / (graphData[graphHierarchyId].numberOfNodes + graphData[graphHierarchyId].numberOfEdges + graphData[graphHierarchyId].numberOfSubgraphs - 1)) * 100;
                  view.setProgressBarComment("Importing graph nodes data (" + nodeId + " / " + graphData[graphHierarchyId].numberOfNodes + ") ...");
                  view.setProgressBarPercent(percent);
                  view.draw();
                }
                tulipWorker.postMessage({eventType : 'sendNextNodes', graphHierarchyId : graphHierarchyId});
              }, delay);
              break;
            case 'addEdges':
              setTimeout(function() {
                graph._parseEdgesJSONData(event.data.edgesJson);
                if (tulip.vizFeatures && view) {
                  var edgeId = event.data.lastEdgeId;
                  var percent = ((graphData[graphHierarchyId].numberOfNodes + edgeId) / (graphData[graphHierarchyId].numberOfNodes + graphData[graphHierarchyId].numberOfEdges + graphData[graphHierarchyId].numberOfSubgraphs - 1)) * 100;
                  view.setProgressBarComment("Importing graph edges data (" + edgeId + " / " + graphData[graphHierarchyId].numberOfEdges + ") ...");
                  view.setProgressBarPercent(percent);
                  view.draw();
                }
                tulipWorker.postMessage({eventType : 'sendNextEdges' , graphHierarchyId : graphHierarchyId});
              }, delay);
              break;
            case 'addSubGraph':
              setTimeout(function() {
                graph._addSubGraph(event.data.subGraphData);
                if (tulip.vizFeatures && view) {
                  var sgId = graph.numberOfDescendantGraphs();
                  var percent = ((graphData[graphHierarchyId].numberOfNodes + graphData[graphHierarchyId].numberOfEdges + sgId) / (graphData[graphHierarchyId].numberOfNodes + graphData[graphHierarchyId].numberOfEdges + graphData[graphHierarchyId].numberOfSubgraphs - 1)) * 100;
                  view.setProgressBarComment("Importing subgraphs data (" + sgId + " / " + graphData[graphHierarchyId].numberOfSubgraphs + ") ...");
                  view.setProgressBarPercent(percent);
                  view.draw();
                }
                tulipWorker.postMessage({eventType : 'sendNextSubGraph', graphHierarchyId : event.data.graphHierarchyId});
              }, delay);
              break;
          };
        }, false);

        return new Promise(function(resolve, reject) {
          function checkWorkerInit() {
            if (tulipWorkerInit) {
              resolve(tulipWorker);
            } else {
              setTimeout(checkWorkerInit);
            }
          }
          checkWorkerInit();
        });

    }

    function sendGraphToWorker(graph, graphFilePath, graphFileData, sendDataBack) {
      if (arguments.length == 1) {
        var file = FS.findObject("/graph.tlpb.gz");
        if (!file) {
          FS.createFile('/', "graph.tlpb.gz", {}, true, true);
        }
        var saved = tulip.saveGraph(graph, "/graph.tlpb.gz");
        var graphData = FS.readFile("/graph.tlpb.gz");
        FS.unlink("/graph.tlpb.gz");
        return sendGraphToWorker(graph, "graph.tlpb.gz", graphData.buffer, false);
      } else {
        var messageData = {
          eventType: 'loadGraph',
          graphHierarchyId: graph.getCppPointer(),
          graphFile: graphFilePath,
          graphFileData : graphFileData,
          sendDataBack : sendDataBack
        };
        return createTulipWebWorker().then(function(tulipWorker) {
          if (graphFileData) {
            tulipWorker.postMessage(messageData, [messageData.graphFileData]);
          } else {
            tulipWorker.postMessage(messageData);
          }
          return tulipWorker;
        });
      }
    }

  }

  // ==================================================================================================================

  if (tulip.vizFeatures) {

    function addStyleString(str) {
      var node = document.createElement('style');
      node.innerHTML = str;
      document.body.appendChild(node);
    }

    addStyleString(' \
    .centerFlex { \
       align-items: center; \
       display: flex; \
       justify-content: center; \
    }');

    function addHTMLProgressBarToView(view) {
      view.canvasOverlayDiv = document.createElement('div');
      view.canvasOverlayDiv.style.position = 'absolute';
      view.canvasOverlayDiv.style.top = '0';
      view.canvasOverlayDiv.style.left = '0';
      view.canvasOverlayDiv.style.backgroundColor = 'rgba(255, 255, 255, 0.8)';
      if (view.sizeRelativeToContainer) {
        view.canvasOverlayDiv.style.width = '100%';
        view.canvasOverlayDiv.style.height = '100%';
      } else {
        view.canvasOverlayDiv.style.width = view.canvas.width + 'px';
        view.canvasOverlayDiv.style.height = view.canvas.height + 'px';
      }

      view.canvasOverlayDiv.style.display = "none";
      view.container.appendChild(view.canvasOverlayDiv);

      view.progress = document.createElement('progress');
      view.progress.max = '100';
      view.progress.style.width = '100%';
      view.progress.style.height = '30px';

      view.progressComment = document.createElement('p');
      view.progressComment.style.color = '#000000';
      view.progressComment.style.textAlign = 'center';

      var progressDiv = document.createElement('div');
      progressDiv.style.width = '70%';
      progressDiv.appendChild(view.progressComment);
      progressDiv.appendChild(view.progress);

      view.canvasOverlayDiv.classList.add('centerFlex');
      view.canvasOverlayDiv.appendChild(progressDiv);
    }

    var _setCanvasGraph = Module.cwrap('setCanvasGraph', null, ['string', 'number', 'number']);
    var _getViewRenderingParameters = Module.cwrap('getViewRenderingParameters', 'number', ['string']);
    var _getViewInputData = Module.cwrap('getViewInputData', 'number', ['string']);
    var _getViewCamera = Module.cwrap('getViewCamera', 'number', ['string']);
    var _setViewBackgroundColor = Module.cwrap('setViewBackgroundColor', null, ['string', 'number', 'number', 'number', 'number']);
    var _setViewBackupBackBuffer = Module.cwrap('setViewBackupBackBuffer', null, ['string', 'number']);

    var nextCanvasId = 0;

    tulip.View = function(container, width, height) {
      var newObject = createObject(tulip.View, this);
      newObject.canvasOverlayDiv = null;
      if (arguments.length > 0) {
        if (typeof(container) == 'string') {
          newObject.container = document.getElementById(container);
        } else {
          newObject.container = container;
        }

        if (newObject.container.tagName == 'DIV') {
          newObject.container.style.position = 'relative';

          var currentId = nextCanvasId++;

          newObject.canvasId = 'tulip-canvas-' + currentId;
          newObject.canvas = document.createElement("canvas");
          newObject.canvas.style.outline = 'none';
          newObject.canvas.style.position = 'absolute';
          newObject.canvas.style.top = '0';
          newObject.canvas.style.left = '0';
          newObject.canvas.id = newObject.canvasId;
          newObject.canvas.tabIndex = -1;
          newObject.container.appendChild(newObject.canvas);
          if (typeOf(width) != 'undefined' && typeOf(height) != 'undefined') {
            newObject.sizeRelativeToContainer = false;
          } else {
            newObject.sizeRelativeToContainer = true;
            newObject.canvas.style.width = '100%';
            newObject.canvas.style.height = '100%';
            width = newObject.container.clientWidth;
            height = newObject.container.clientHeight;
          }
        } else if (newObject.container.tagName == 'CANVAS') {
          newObject.canvas = newObject.container;
          newObject.container = null;
          if (newObject.canvas.id != "") {
            newObject.canvasId = newObject.canvas.id;
          } else {
            newObject.canvas.id = 'tulip-canvas-' + nextCanvasId++;
            newObject.canvasId = newObject.canvas.id;
          }
          newObject.sizeRelativeToContainer = false;
          width = newObject.canvas.width;
          height = newObject.canvas.height;
        }

        _initCanvas(newObject.canvasId, width, height, newObject.sizeRelativeToContainer);

        if (newObject.container) {
          addHTMLProgressBarToView(newObject);
        }

        newObject.graph = null;
        newObject.graphDrawingChanged = false;
        canvasIdToView[newObject.canvasId] = newObject;
      }
      newObject.fullScreenActivated = false;
      newObject.busyAnimationStarted = false;
      return newObject;
    };

    tulip.View.prototype.makeCurrent = function() {
      _setCurrentCanvas(this.canvasId);
    }

    tulip.View.prototype.updateGlScene = function() {
      _updateGlScene(this.canvasId);
    }

    tulip.View.prototype.setBackgroundColor = function(color) {
      checkArgumentsTypes(arguments, [tulip.Color]);
      _setViewBackgroundColor(this.canvasId, color.r, color.g, color.b, color.a);
    }

    tulip.View.prototype.setBackupBackBuffer = function(backup) {
      checkArgumentsTypes(arguments, ["boolean"]);
      _setViewBackupBackBuffer(this.canvasId, backup);
    }

    tulip.View.prototype.draw = function() {
      if (this.sizeRelativeToContainer && !this.fullScreenActivated) {
        _resizeCanvas(this.canvasId, this.container.clientWidth, this.container.clientHeight, this.sizeRelativeToContainer);
      }
      var view = this;
      Browser.requestAnimationFrame(function() {
        view.makeCurrent();
        view.updateGlScene();
      });
    };

    tulip.View.prototype.setProgressBarComment = function(comment) {
      if (this.canvasOverlayDiv) {
        this.canvasOverlayDiv.style.display = "";
        this.progressComment.innerHTML = comment;
      }
    };

    tulip.View.prototype.setProgressBarPercent = function(percent) {
      if (this.canvasOverlayDiv) {
        this.canvasOverlayDiv.style.display = "";
        this.progress.value = percent;
      }
    };

    tulip.View.prototype.startBusyAnimation = function() {
      if (this.busyAnimationStarted) {
        return;
      }

      if (this.canvasOverlayDiv) {
        this.canvasOverlayDiv.style.display = "";
        this.progress.removeAttribute('value');
        this.busyAnimationStarted = true;
      }
    };

    tulip.View.prototype.stopBusyAnimation = function(canvasId) {
      this.busyAnimationStarted = false;
      this.canvasOverlayDiv.style.display = "none";
    };

    tulip.View.prototype.activateInteractor = function(interactorName) {
      _activateInteractor(this.canvasId, interactorName);
    };

    tulip.View.prototype.desactivateInteractor = function() {
      _desactivateInteractor(this.canvasId);
    };

    tulip.View.prototype.centerScene = function() {
      _centerScene(this.canvasId);
    };

    tulip.View.prototype.fullScreen = function() {
      _fullScreen(this.canvasId);
    };

    tulip.View.prototype.resize = function(width, height) {
      if (!width && this.sizeRelativeToContainer) {
        width = this.container.clientWidth;
      }
      if (!height && this.sizeRelativeToContainer) {
        height = this.container.clientHeight;
      }
      _resizeCanvas(this.canvasId, width, height, this.sizeRelativeToContainer);
      if (!this.sizeRelativeToConatiner) {
        this.canvasOverlayDiv.style.width = width + 'px';
        this.canvasOverlayDiv.style.height = height + 'px';
      }
    };

    tulip.View.prototype.getWidth = function() {
      return this.canvas.width;
    };

    tulip.View.prototype.getHeight = function() {
      return this.canvas.height;
    };

    tulip.View.prototype.setGraph = function(graph, viewTakesGraphOwnership) {
      if (!graph.cppPointerValid()) return;
      this.graph = graph;
      _setCanvasGraph(this.canvasId, graph.cppPointer, viewTakesGraphOwnership);
      graphHierarchyIdToView[graph.getRoot().getCppPointer()] = this;
      graphHierarchyIdToWrapper[graph.getRoot().getCppPointer()] = graph.getRoot();
    };

    tulip.View.prototype.getGraph = function() {
      return this.graph;
    };

    tulip.View.prototype.loadGraphFromFile = function(graphFilePath, loadGraphInWorker, graphLoadedCallback) {

      var view = this;
      var graphReq = new XMLHttpRequest();
      graphReq.open("GET", graphFilePath, true);
      graphReq.responseType = "arraybuffer";
      graphReq.onload = function (oEvent) {
        var arrayBuffer = graphReq.response;
        var paths = graphFilePath.split('/');
        if (loadGraphInWorker) {
          view.loadGraphFromData(paths[paths.length-1], arrayBuffer, loadGraphInWorker, graphLoadedCallback);
        } else {
          var file = FS.findObject(graphFilePath);
          if (!file) {
            var filePath = "/";
            for (var i = 0; i < paths.length - 1; ++i) {
              filePath += paths[i];
              filePath += "/";
            }
            FS.createPath('/', filePath, true, true);
            FS.createFile('/', graphFilePath, {}, true, true);
          }
          FS.writeFile(graphFilePath, new Uint8Array(arrayBuffer), {'encoding' : 'binary'});
          var graph = tulip.loadGraph(graphFilePath, false);
          FS.unlink(graphFilePath);
          view.setGraph(graph, true);
          if (graphLoadedCallback) {
            graphLoadedCallback(view.graph);
          }
        }
      };
      graphReq.send(null);
    };

    tulip.View.prototype.loadGraphFromData = function(graphFilePath, graphFileData, loadGraphInWorker, graphLoadedCallback) {
      var view = this;
      if (loadGraphInWorker) {
        var graph = tulip.Graph();
        view.setGraph(graph, true);
        if (graphLoadedCallback) {
          graphLoadedCallbacks[view.graph.getCppPointer()] = graphLoadedCallback;
        }
        this.setGraphRenderingDataReady(false);
        sendGraphToWorker(view.graph, graphFilePath, graphFileData, true);
      } else {
        var file = FS.findObject(graphFilePath);
        if (!file) {
          var paths = graphFilePath.split('/');
          var filePath = "/";
          for (var i = 0; i < paths.length - 1; ++i) {
            filePath += paths[i];
            filePath += "/";
          }
          FS.createPath('/', filePath, true, true);
          FS.createFile('/', graphFilePath, {}, true, true);
        }
        FS.writeFile(graphFilePath, new Uint8Array(graphFileData), {'encoding' : 'binary'});
        var graph = tulip.loadGraph(graphFilePath, false);
        FS.unlink(graphFilePath);
        view.setGraph(graph, true);
        if (graphLoadedCallback) {
          graphLoadedCallback(view.graph);
        }
      }
    };

    tulip.View.prototype.setGraphRenderingDataReady = function(ready) {
      _setGraphRenderingDataReady(this.canvasId, ready);
      this.canvasOverlayDiv.style.display = ready ? "none" : "";
    };

    tulip.View.prototype.selectNodesEdges = function(x, y, w, h) {
      if (w == undefined) {
        w = 0;
        h = 0;
      }
      var nbNodes = _selectNodes(this.canvasId, x, y, w, h);
      var nbEdges = _selectEdges(this.canvasId, x, y, w, h);
      var selectedNodes = [];
      var selectedEdges = [];

      if (nbNodes > 0) {
        selectedNodes = getArrayOfTulipType(nbNodes, function(byteOffset) {_getSelectedNodes(byteOffset)}, tulip.Node);
      }

      if (nbEdges > 0) {
        selectedEdges = getArrayOfTulipType(nbEdges, function(byteOffset) {_getSelectedEdges(byteOffset)}, tulip.Edge);
      }

      return {nodes: selectedNodes, edges: selectedEdges};

    };

    tulip.View.prototype.getRenderingParameters = function() {
      return tulip.GlGraphRenderingParameters(_getViewRenderingParameters(this.canvasId));
    };

    tulip.View.prototype.getInputData = function() {
      return tulip.GlGraphInputData(_getViewInputData(this.canvasId));
    };

    tulip.View.prototype.getCamera = function() {
      return tulip.Camera(_getViewCamera(this.canvasId));
    };

    tulip.View.prototype.graphHasHull = function(graph) {
      checkArgumentsTypes(arguments, [tulip.Graph], 1);
      return _graphHasHull(this.canvasId, graph.cppPointer) > 0;
    };

    tulip.View.prototype.addGraphHull = function(graph) {
      checkArgumentsTypes(arguments, [tulip.Graph], 1);
      _addGraphHull(this.canvasId, graph.cppPointer, true);
    };

    tulip.View.prototype.clearGraphsHulls = function() {
      _clearGraphsHulls(this.canvasId);
    };

    tulip.View.prototype.computeSubGraphsHulls = function(subgraphsList) {
      _clearGraphsHulls(this.canvasId);
      var view = this;
      var subgraphs = this.graph.getSubGraphs();
      if (typeOf(subgraphsList) == "array") {
        subgraphs = subgraphsList;
      }
      subgraphs.forEach(function(sg) {
        if (!updateMode || view.graphHasHull(sg)) {
          setTimeout(
            function(g) {
              return function() {
                view.addGraphHull(g);
                view.draw();
              }
            }(sg)
          );
        }
      });
    };

    tulip.View.prototype.setGraphsHullsVisible = function(visible, onTop) {
      checkArgumentsTypes(arguments, ['boolean', 'boolean']);
      if (typeOf(visible) == 'undefined') {
        visible = true;
      }
      if (typeOf(onTop) == 'undefined') {
        onTop = true;
      }
      _setGraphsHullsVisible(this.canvasId, visible, onTop);
      this.draw();
    };

    function dataURItoBlob(dataURI) {
      // convert base64/URLEncoded data component to raw binary data held in a string
      var byteString;
      if (dataURI.split(',')[0].indexOf('base64') >= 0)
        byteString = atob(dataURI.split(',')[1]);
      else
        byteString = unescape(dataURI.split(',')[1]);

      // separate out the mime component
      var mimeString = dataURI.split(',')[0].split(':')[1].split(';')[0];

      // write the bytes of the string to a typed array
      var ia = new Uint8Array(byteString.length);
      for (var i = 0; i < byteString.length; i++) {
        ia[i] = byteString.charCodeAt(i);
      }

      return new Blob([ia], {type:mimeString});
    }

    tulip.View.prototype.getViewSnapshotBlob = function() {
      return dataURItoBlob(this.canvas.toDataURL());
    };

    tulip.View.prototype.startGraphViewData = function() {
      _startGraphViewData(this.canvasId);
    };

    tulip.View.prototype.endGraphViewData = function() {
      _endGraphViewData(this.canvasId);
    };

    tulip.View.prototype.startGraphViewUpdate = function(clearGraph) {
      _startGraphViewUpdate(this.canvasId, clearGraph);
    };

    tulip.View.prototype.endGraphViewUpdate = function() {
      _endGraphViewUpdate(this.canvasId);
    };

    // ==================================================================================================

    var _GlGraphRenderingParameters_setDisplayNodes = Module.cwrap('GlGraphRenderingParameters_setDisplayNodes', null, ['number', 'number']);
    var _GlGraphRenderingParameters_displayNodes = Module.cwrap('GlGraphRenderingParameters_displayNodes', 'number', ['number']);
    var _GlGraphRenderingParameters_setBillboardedNodes = Module.cwrap('GlGraphRenderingParameters_setBillboardedNodes', null, ['number', 'number']);
    var _GlGraphRenderingParameters_billboardedNodes = Module.cwrap('GlGraphRenderingParameters_billboardedNodes', 'number', ['number']);
    var _GlGraphRenderingParameters_setDisplayNodesLabels = Module.cwrap('GlGraphRenderingParameters_setDisplayNodesLabels', null, ['number', 'number']);
    var _GlGraphRenderingParameters_displayNodesLabels = Module.cwrap('GlGraphRenderingParameters_displayNodesLabels', 'number', ['number']);
    var _GlGraphRenderingParameters_setLabelsScaled = Module.cwrap('GlGraphRenderingParameters_setLabelsScaled', null, ['number', 'number']);
    var _GlGraphRenderingParameters_labelsScaled = Module.cwrap('GlGraphRenderingParameters_labelsScaled', 'number', ['number']);
    var _GlGraphRenderingParameters_setDisplayEdges = Module.cwrap('GlGraphRenderingParameters_setDisplayEdges', null, ['number', 'number']);
    var _GlGraphRenderingParameters_displayEdges = Module.cwrap('GlGraphRenderingParameters_displayEdges', 'number', ['number']);
    var _GlGraphRenderingParameters_setInterpolateEdgesColors = Module.cwrap('GlGraphRenderingParameters_setInterpolateEdgesColors', null, ['number', 'number']);
    var _GlGraphRenderingParameters_interpolateEdgesColors = Module.cwrap('GlGraphRenderingParameters_interpolateEdgesColors', 'number', ['number']);
    var _GlGraphRenderingParameters_setInterpolateEdgesSizes = Module.cwrap('GlGraphRenderingParameters_setInterpolateEdgesSizes', null, ['number', 'number']);
    var _GlGraphRenderingParameters_interpolateEdgesSizes = Module.cwrap('GlGraphRenderingParameters_interpolateEdgesSizes', 'number', ['number']);
    var _GlGraphRenderingParameters_setDisplayEdgesExtremities = Module.cwrap('GlGraphRenderingParameters_setDisplayEdgesExtremities', null, ['number', 'number']);
    var _GlGraphRenderingParameters_displayEdgesExtremities = Module.cwrap('GlGraphRenderingParameters_displayEdgesExtremities', 'number', ['number']);
    var _GlGraphRenderingParameters_setEdges3D = Module.cwrap('GlGraphRenderingParameters_setEdges3D', null, ['number', 'number']);
    var _GlGraphRenderingParameters_edges3D = Module.cwrap('GlGraphRenderingParameters_edges3D', 'number', ['number']);
    var _GlGraphRenderingParameters_setMinSizeOfLabels = Module.cwrap('GlGraphRenderingParameters_setMinSizeOfLabels', null, ['number', 'number']);
    var _GlGraphRenderingParameters_minSizeOfLabels = Module.cwrap('GlGraphRenderingParameters_minSizeOfLabels', 'number', ['number']);
    var _GlGraphRenderingParameters_setMaxSizeOfLabels = Module.cwrap('GlGraphRenderingParameters_setMaxSizeOfLabels', null, ['number', 'number']);
    var _GlGraphRenderingParameters_maxSizeOfLabels = Module.cwrap('GlGraphRenderingParameters_maxSizeOfLabels', 'number', ['number']);
    var _GlGraphRenderingParameters_setElementsOrdered = Module.cwrap('GlGraphRenderingParameters_setElementsOrdered', null, ['number', 'number']);
    var _GlGraphRenderingParameters_elementsOrdered = Module.cwrap('GlGraphRenderingParameters_elementsOrdered', 'number', ['number']);
    var _GlGraphRenderingParameters_setElementOrderedDescending = Module.cwrap('GlGraphRenderingParameters_setElementOrderedDescending', null, ['number', 'number']);
    var _GlGraphRenderingParameters_elementsOrderedDescending = Module.cwrap('GlGraphRenderingParameters_elementsOrderedDescending', 'number', ['number']);
    var _GlGraphRenderingParameters_setElementsOrderingProperty = Module.cwrap('GlGraphRenderingParameters_setElementsOrderingProperty', null, ['number', 'number']);
    var _GlGraphRenderingParameters_elementsOrderingProperty = Module.cwrap('GlGraphRenderingParameters_elementsOrderingProperty', 'number', ['number']);

    tulip.GlGraphRenderingParameters = function tulip_GlGraphRenderingParameters(cppPointer) {
      var newObject = createObject(tulip.GlGraphRenderingParameters, this);
      tulip.CppObjectWrapper.call(newObject, cppPointer, "GlGraphRenderingParameters");
      return newObject;
    };
    tulip.GlGraphRenderingParameters.inheritsFrom(tulip.CppObjectWrapper);

    tulip.GlGraphRenderingParameters.prototype.setDisplayNodes = function tulip_GlGraphRenderingParameters_prototype_setDisplayNodes(state) {
      checkArgumentsTypes(arguments, ["boolean"], 1);
      _GlGraphRenderingParameters_setDisplayNodes(this.cppPointer, state);
    };

    tulip.GlGraphRenderingParameters.prototype.displayNodes = function tulip_GlGraphRenderingParameters_prototype_displayNodes() {
      return _GlGraphRenderingParameters_displayNodes(this.cppPointer) > 0;
    };

    tulip.GlGraphRenderingParameters.prototype.setBillboardedNodes = function tulip_GlGraphRenderingParameters_prototype_setBillboardedNodes(state) {
      checkArgumentsTypes(arguments, ["boolean"], 1);
      _GlGraphRenderingParameters_setBillboardedNodes(this.cppPointer, state);
    };

    tulip.GlGraphRenderingParameters.prototype.billboardedNodes = function tulip_GlGraphRenderingParameters_prototype_billboardedNodes() {
      return _GlGraphRenderingParameters_billboardedNodes(this.cppPointer) > 0;
    };

    tulip.GlGraphRenderingParameters.prototype.setDisplayNodesLabels = function tulip_GlGraphRenderingParameters_prototype_setDisplayNodesLabels(state) {
      checkArgumentsTypes(arguments, ["boolean"], 1);
      _GlGraphRenderingParameters_setDisplayNodesLabels(this.cppPointer, state);
    };

    tulip.GlGraphRenderingParameters.prototype.displayNodesLabels = function tulip_GlGraphRenderingParameters_prototype_displayNodesLabels() {
      return _GlGraphRenderingParameters_displayNodesLabels(this.cppPointer) > 0;
    };

    tulip.GlGraphRenderingParameters.prototype.setLabelsScaled = function tulip_GlGraphRenderingParameters_prototype_setLabelsScaled(state) {
      checkArgumentsTypes(arguments, ["boolean"], 1);
      _GlGraphRenderingParameters_setLabelsScaled(this.cppPointer, state);
    };

    tulip.GlGraphRenderingParameters.prototype.labelsScaled = function tulip_GlGraphRenderingParameters_prototype_labelsScaled() {
      return _GlGraphRenderingParameters_labelsScaled(this.cppPointer) > 0;
    };

    tulip.GlGraphRenderingParameters.prototype.setDisplayEdges = function tulip_GlGraphRenderingParameters_prototype_setDisplayEdges(state) {
      checkArgumentsTypes(arguments, ["boolean"], 1);
      _GlGraphRenderingParameters_setDisplayEdges(this.cppPointer, state);
    };

    tulip.GlGraphRenderingParameters.prototype.displayEdges = function tulip_GlGraphRenderingParameters_prototype_displayEdges() {
      return _GlGraphRenderingParameters_displayEdges(this.cppPointer) > 0;
    };

    tulip.GlGraphRenderingParameters.prototype.setInterpolateEdgesColors = function tulip_GlGraphRenderingParameters_prototype_setInterpolateEdgesColors(state) {
      checkArgumentsTypes(arguments, ["boolean"], 1);
      _GlGraphRenderingParameters_setInterpolateEdgesColors(this.cppPointer, state);
    };

    tulip.GlGraphRenderingParameters.prototype.interpolateEdgesColors = function tulip_GlGraphRenderingParameters_prototype_interpolateEdgesColors() {
      return _GlGraphRenderingParameters_interpolateEdgesColors(this.cppPointer) > 0;
    };

    tulip.GlGraphRenderingParameters.prototype.setInterpolateEdgesSizes = function tulip_GlGraphRenderingParameters_prototype_setInterpolateEdgesSizes(state) {
      checkArgumentsTypes(arguments, ["boolean"], 1);
      _GlGraphRenderingParameters_setInterpolateEdgesSizes(this.cppPointer, state);
    };

    tulip.GlGraphRenderingParameters.prototype.interpolateEdgesSizes = function tulip_GlGraphRenderingParameters_prototype_interpolateEdgesSizes() {
      return _GlGraphRenderingParameters_interpolateEdgesSizes(this.cppPointer) > 0;
    };

    tulip.GlGraphRenderingParameters.prototype.setDisplayEdgesExtremities = function tulip_GlGraphRenderingParameters_prototype_setDisplayEdgesExtremities(state) {
      checkArgumentsTypes(arguments, ["boolean"], 1);
      _GlGraphRenderingParameters_setDisplayEdgesExtremities(this.cppPointer, state);
    };

    tulip.GlGraphRenderingParameters.prototype.displayEdgesExtremities = function tulip_GlGraphRenderingParameters_prototype_displayEdgesExtremities() {
      return _GlGraphRenderingParameters_displayEdgesExtremities(this.cppPointer) > 0;
    };

    tulip.GlGraphRenderingParameters.prototype.setEdges3D = function tulip_GlGraphRenderingParameters_prototype_setEdges3D(state) {
      checkArgumentsTypes(arguments, ["boolean"], 1);
      _GlGraphRenderingParameters_setEdges3D(this.cppPointer, state);
    };

    tulip.GlGraphRenderingParameters.prototype.edges3D = function tulip_GlGraphRenderingParameters_prototype_edges3D() {
      return _GlGraphRenderingParameters_edges3D(this.cppPointer) > 0;
    };

    tulip.GlGraphRenderingParameters.prototype.setMinSizeOfLabels = function tulip_GlGraphRenderingParameters_prototype_setMinSizeOfLabels(minSize) {
      checkArgumentsTypes(arguments, ["number"], 1);
      _GlGraphRenderingParameters_setMinSizeOfLabels(this.cppPointer, minSize);
    };

    tulip.GlGraphRenderingParameters.prototype.minSizeOfLabels = function tulip_GlGraphRenderingParameters_prototype_minSizeOfLabels() {
      return _GlGraphRenderingParameters_minSizeOfLabels(this.cppPointer);
    };

    tulip.GlGraphRenderingParameters.prototype.setMaxSizeOfLabels = function tulip_GlGraphRenderingParameters_prototype_setMaxSizeOfLabels(maxSize) {
      checkArgumentsTypes(arguments, ["number"], 1);
      _GlGraphRenderingParameters_setMaxSizeOfLabels(this.cppPointer, maxSize);
    };

    tulip.GlGraphRenderingParameters.prototype.maxSizeOfLabels = function tulip_GlGraphRenderingParameters_prototype_maxSizeOfLabels() {
      return _GlGraphRenderingParameters_maxSizeOfLabels(this.cppPointer);
    };

    tulip.GlGraphRenderingParameters.prototype.setElementsOrdered = function tulip_GlGraphRenderingParameters_prototype_setElementsOrdered(state) {
      checkArgumentsTypes(arguments, ["boolean"], 1);
      _GlGraphRenderingParameters_setElementsOrdered(this.cppPointer, state);
    };

    tulip.GlGraphRenderingParameters.prototype.elementsOrdered = function tulip_GlGraphRenderingParameters_prototype_elementsOrdered() {
      return _GlGraphRenderingParameters_elementsOrdered(this.cppPointer) > 0;
    }

    tulip.GlGraphRenderingParameters.prototype.setElementsOrderedDescending = function tulip_GlGraphRenderingParameters_prototype_setElementsOrderedDescending(state) {
      checkArgumentsTypes(arguments, ["boolean"], 1);
      _GlGraphRenderingParameters_setElementOrderedDescending(this.cppPointer, state);
    };

    tulip.GlGraphRenderingParameters.prototype.elementsOrderedDescending = function tulip_GlGraphRenderingParameters_prototype_elementsOrderedDescending() {
      return _GlGraphRenderingParameters_elementsOrderedDescending(this.cppPointer) > 0;
    }

    tulip.GlGraphRenderingParameters.prototype.setElementsOrderingProperty = function tulip_GlGraphRenderingParameters_prototype_setElementsOrderingProperty(prop) {
      if (!prop) {
        _GlGraphRenderingParameters_setElementsOrderingProperty(this.cppPointer, 0);
      } else {
        checkArgumentsTypes(arguments, [[tulip.DoubleProperty, tulip.IntegerProperty]], 1);
        _GlGraphRenderingParameters_setElementsOrderingProperty(this.cppPointer, prop.cppPointer);
      }
    };

    // ==================================================================================================

    var _GlGraphInputData_getElementColor = Module.cwrap('GlGraphInputData_getElementColor', 'number', ['number']);
    var _GlGraphInputData_setElementColor = Module.cwrap('GlGraphInputData_setElementColor', null, ['number', 'number']);
    var _GlGraphInputData_getElementLabelColor = Module.cwrap('GlGraphInputData_getElementLabelColor', 'number', ['number']);
    var _GlGraphInputData_setElementLabelColor = Module.cwrap('GlGraphInputData_setElementLabelColor', null, ['number', 'number']);
    var _GlGraphInputData_getElementSize = Module.cwrap('GlGraphInputData_getElementSize', 'number', ['number']);
    var _GlGraphInputData_setElementSize = Module.cwrap('GlGraphInputData_setElementSize', null, ['number', 'number']);
    var _GlGraphInputData_getElementLabelPosition = Module.cwrap('GlGraphInputData_getElementLabelPosition', 'number', ['number']);
    var _GlGraphInputData_setElementLabelPosition = Module.cwrap('GlGraphInputData_setElementLabelPosition', null, ['number', 'number']);
    var _GlGraphInputData_getElementShape = Module.cwrap('GlGraphInputData_getElementShape', 'number', ['number']);
    var _GlGraphInputData_setElementShape = Module.cwrap('GlGraphInputData_setElementShape', null, ['number', 'number']);
    var _GlGraphInputData_getElementRotation = Module.cwrap('GlGraphInputData_getElementRotation', 'number', ['number']);
    var _GlGraphInputData_setElementRotation = Module.cwrap('GlGraphInputData_setElementRotation', null, ['number', 'number']);
    var _GlGraphInputData_getElementSelection = Module.cwrap('GlGraphInputData_getElementSelection', 'number', ['number']);
    var _GlGraphInputData_setElementSelection = Module.cwrap('GlGraphInputData_setElementSelection', null, ['number', 'number']);
    var _GlGraphInputData_getElementFont = Module.cwrap('GlGraphInputData_getElementFont', 'number', ['number']);
    var _GlGraphInputData_setElementFont = Module.cwrap('GlGraphInputData_setElementFont', null, ['number', 'number']);
    var _GlGraphInputData_getElementFontSize = Module.cwrap('GlGraphInputData_getElementFontSize', 'number', ['number']);
    var _GlGraphInputData_setElementFontSize = Module.cwrap('GlGraphInputData_setElementFontSize', null, ['number', 'number']);
    var _GlGraphInputData_getElementLabel = Module.cwrap('GlGraphInputData_getElementLabel', 'number', ['number']);
    var _GlGraphInputData_setElementLabel = Module.cwrap('GlGraphInputData_setElementLabel', null, ['number', 'number']);
    var _GlGraphInputData_getElementLayout = Module.cwrap('GlGraphInputData_getElementLayout', 'number', ['number']);
    var _GlGraphInputData_setElementLayout = Module.cwrap('GlGraphInputData_setElementLayout', null, ['number', 'number']);
    var _GlGraphInputData_getElementTexture = Module.cwrap('GlGraphInputData_getElementTexture', 'number', ['number']);
    var _GlGraphInputData_setElementTexture = Module.cwrap('GlGraphInputData_setElementTexture', null, ['number', 'number']);
    var _GlGraphInputData_getElementBorderColor = Module.cwrap('GlGraphInputData_getElementBorderColor', 'number', ['number']);
    var _GlGraphInputData_setElementBorderColor = Module.cwrap('GlGraphInputData_setElementBorderColor', null, ['number', 'number']);
    var _GlGraphInputData_getElementBorderWidth = Module.cwrap('GlGraphInputData_getElementBorderWidth', 'number', ['number']);
    var _GlGraphInputData_setElementBorderWidth = Module.cwrap('GlGraphInputData_setElementBorderWidth', null, ['number', 'number']);
    var _GlGraphInputData_getElementSrcAnchorShape = Module.cwrap('GlGraphInputData_getElementSrcAnchorShape', 'number', ['number']);
    var _GlGraphInputData_setElementSrcAnchorShape = Module.cwrap('GlGraphInputData_setElementSrcAnchorShape', null, ['number', 'number']);
    var _GlGraphInputData_getElementSrcAnchorSize = Module.cwrap('GlGraphInputData_getElementSrcAnchorSize', 'number', ['number']);
    var _GlGraphInputData_setElementSrcAnchorSize = Module.cwrap('GlGraphInputData_setElementSrcAnchorSize', null, ['number', 'number']);
    var _GlGraphInputData_getElementTgtAnchorShape = Module.cwrap('GlGraphInputData_getElementTgtAnchorShape', 'number', ['number']);
    var _GlGraphInputData_setElementTgtAnchorShape = Module.cwrap('GlGraphInputData_setElementTgtAnchorShape', null, ['number', 'number']);
    var _GlGraphInputData_getElementTgtAnchorSize = Module.cwrap('GlGraphInputData_getElementTgtAnchorSize', 'number', ['number']);
    var _GlGraphInputData_setElementTgtAnchorSize = Module.cwrap('GlGraphInputData_setElementTgtAnchorSize', null, ['number', 'number']);
    var _GlGraphInputData_getElementFontAwesomeIcon = Module.cwrap('GlGraphInputData_getElementFontAwesomeIcon', 'number', ['number']);
    var _GlGraphInputData_setElementFontAwesomeIcon = Module.cwrap('GlGraphInputData_setElementFontAwesomeIcon', null, ['number', 'number']);
    var _GlGraphInputData_getElementGlow = Module.cwrap('GlGraphInputData_getElementGlow', 'number', ['number']);
    var _GlGraphInputData_setElementGlow = Module.cwrap('GlGraphInputData_setElementGlow', null, ['number', 'number']);
    var _GlGraphInputData_getElementGlowColor = Module.cwrap('GlGraphInputData_getElementGlowColor', 'number', ['number']);
    var _GlGraphInputData_setElementGlowColor = Module.cwrap('GlGraphInputData_setElementGlowColor', null, ['number', 'number']);
    var _GlGraphInputData_reloadGraphProperties = Module.cwrap('GlGraphInputData_reloadGraphProperties', null, ['number', 'number']);

    tulip.GlGraphInputData = function tulip_GlGraphInputData(cppPointer) {
      var newObject = createObject(tulip.GlGraphInputData, this);
      tulip.CppObjectWrapper.call(newObject, cppPointer, "GlGraphInputData");
      return newObject;
    };
    tulip.GlGraphInputData.inheritsFrom(tulip.CppObjectWrapper);

    tulip.GlGraphInputData.prototype.getElementColor = function tulip_GlGraphInputData_prototype_getElementColor() {
      return tulip.ColorProperty(_GlGraphInputData_getElementColor(this.cppPointer));
    };

    tulip.GlGraphInputData.prototype.setElementColor = function tulip_GlGraphInputData_prototype_setElementColor(prop) {
      checkArgumentsTypes(arguments, [tulip.ColorProperty], 1);
      _GlGraphInputData_setElementColor(this.cppPointer, prop.cppPointer);
    };

    tulip.GlGraphInputData.prototype.getElementLabelColor = function tulip_GlGraphInputData_prototype_getElementLabelColor() {
      return tulip.ColorProperty(_GlGraphInputData_getElementLabelColor(this.cppPointer));
    };

    tulip.GlGraphInputData.prototype.setElementLabelColor = function tulip_GlGraphInputData_prototype_setElementLabelColor(prop) {
      checkArgumentsTypes(arguments, [tulip.ColorProperty], 1);
      _GlGraphInputData_setElementLabelColor(this.cppPointer, prop.cppPointer);
    };

    tulip.GlGraphInputData.prototype.getElementSize = function tulip_GlGraphInputData_prototype_getElementSize() {
      return tulip.SizeProperty(_GlGraphInputData_getElementSize(this.cppPointer));
    };

    tulip.GlGraphInputData.prototype.setElementSize = function tulip_GlGraphInputData_prototype_setElementSize(prop) {
      checkArgumentsTypes(arguments, [tulip.SizeProperty], 1);
      _GlGraphInputData_setElementSize(this.cppPointer, prop.cppPointer);
    };

    tulip.GlGraphInputData.prototype.getElementLabelPosition = function tulip_GlGraphInputData_prototype_getElementLabelPosition() {
      return tulip.IntegerProperty(_GlGraphInputData_getElementLabelPosition(this.cppPointer));
    };

    tulip.GlGraphInputData.prototype.setElementLabelPosition = function tulip_GlGraphInputData_prototype_setElementLabelPosition(prop) {
      checkArgumentsTypes(arguments, [tulip.IntegerProperty], 1);
      _GlGraphInputData_setElementLabelPosition(this.cppPointer, prop.cppPointer);
    };

    tulip.GlGraphInputData.prototype.getElementShape = function tulip_GlGraphInputData_prototype_getElementShape() {
      return tulip.IntegerProperty(_GlGraphInputData_getElementShape(this.cppPointer));
    };

    tulip.GlGraphInputData.prototype.setElementShape = function tulip_GlGraphInputData_prototype_setElementShape(prop) {
      checkArgumentsTypes(arguments, [tulip.IntegerProperty], 1);
      _GlGraphInputData_setElementShape(this.cppPointer, prop.cppPointer);
    };

    tulip.GlGraphInputData.prototype.getElementRotation = function tulip_GlGraphInputData_prototype_getElementRotation() {
      return tulip.DoubleProperty(_GlGraphInputData_getElementRotation(this.cppPointer));
    };

    tulip.GlGraphInputData.prototype.setElementRotation = function tulip_GlGraphInputData_prototype_setElementRotation(prop) {
      checkArgumentsTypes(arguments, [tulip.DoubleProperty], 1);
      _GlGraphInputData_setElementRotation(this.cppPointer, prop.cppPointer);
    };

    tulip.GlGraphInputData.prototype.getElementSelection = function tulip_GlGraphInputData_prototype_getElementSelection() {
      return tulip.BooleanProperty(_GlGraphInputData_getElementSelection(this.cppPointer));
    };

    tulip.GlGraphInputData.prototype.setElementSelection = function tulip_GlGraphInputData_prototype_setElementSelection(prop) {
      checkArgumentsTypes(arguments, [tulip.BooleanProperty], 1);
      _GlGraphInputData_setElementSelection(this.cppPointer, prop.cppPointer);
    };

    tulip.GlGraphInputData.prototype.getElementFont = function tulip_GlGraphInputData_prototype_getElementFont() {
      return tulip.StringProperty(_GlGraphInputData_getElementFont(this.cppPointer));
    };

    tulip.GlGraphInputData.prototype.setElementFont = function tulip_GlGraphInputData_prototype_setElementFont(prop) {
      checkArgumentsTypes(arguments, [tulip.StringProperty], 1);
      _GlGraphInputData_setElementFont(this.cppPointer, prop.cppPointer);
    };

    tulip.GlGraphInputData.prototype.getElementFontSize = function tulip_GlGraphInputData_prototype_getElementFontSize() {
      return tulip.IntegerProperty(_GlGraphInputData_getElementFontSize(this.cppPointer));
    };

    tulip.GlGraphInputData.prototype.setElementFontSize = function tulip_GlGraphInputData_prototype_setElementFontSize(prop) {
      checkArgumentsTypes(arguments, [tulip.IntegerProperty], 1);
      _GlGraphInputData_setElementFontSize(this.cppPointer, prop.cppPointer);
    };

    tulip.GlGraphInputData.prototype.getElementLabel = function tulip_GlGraphInputData_prototype_getElementLabel() {
      return tulip.StringProperty(_GlGraphInputData_getElementLabel(this.cppPointer));
    };

    tulip.GlGraphInputData.prototype.setElementLabel = function tulip_GlGraphInputData_prototype_setElementLabel(prop) {
      checkArgumentsTypes(arguments, [tulip.StringProperty], 1);
      _GlGraphInputData_setElementLabel(this.cppPointer, prop.cppPointer);
    };

    tulip.GlGraphInputData.prototype.getElementLayout = function tulip_GlGraphInputData_prototype_getElementLayout() {
      return tulip.LayoutProperty(_GlGraphInputData_getElementLayout(this.cppPointer));
    };

    tulip.GlGraphInputData.prototype.setElementLayout = function tulip_GlGraphInputData_prototype_setElementLayout(prop) {
      checkArgumentsTypes(arguments, [tulip.LayoutProperty], 1);
      _GlGraphInputData_setElementLayout(this.cppPointer, prop.cppPointer);
    };

    tulip.GlGraphInputData.prototype.getElementTexture = function tulip_GlGraphInputData_prototype_getElementTexture() {
      return tulip.StringProperty(_GlGraphInputData_getElementTexture(this.cppPointer));
    };

    tulip.GlGraphInputData.prototype.setElementTexture = function tulip_GlGraphInputData_prototype_setElementTexture(prop) {
      checkArgumentsTypes(arguments, [tulip.StringProperty], 1);
      _GlGraphInputData_setElementTexture(this.cppPointer, prop.cppPointer);
    };

    tulip.GlGraphInputData.prototype.getElementBorderColor = function tulip_GlGraphInputData_prototype_getElementBorderColor() {
      return tulip.ColorProperty(_GlGraphInputData_getElementBorderColor(this.cppPointer));
    };

    tulip.GlGraphInputData.prototype.setElementBorderColor = function tulip_GlGraphInputData_prototype_setElementBorderColor(prop) {
      checkArgumentsTypes(arguments, [tulip.ColorProperty], 1);
      _GlGraphInputData_setElementBorderColor(this.cppPointer, prop.cppPointer);
    };

    tulip.GlGraphInputData.prototype.getElementBorderWidth = function tulip_GlGraphInputData_prototype_getElementBorderWidth() {
      return tulip.DoubleProperty(_GlGraphInputData_getElementBorderWidth(this.cppPointer));
    };

    tulip.GlGraphInputData.prototype.setElementBorderWidth = function tulip_GlGraphInputData_prototype_setElementBorderWidth(prop) {
      checkArgumentsTypes(arguments, [tulip.DoubleProperty], 1);
      _GlGraphInputData_setElementBorderWidth(this.cppPointer, prop.cppPointer);
    };

    tulip.GlGraphInputData.prototype.getElementSrcAnchorShape = function tulip_GlGraphInputData_prototype_getElementSrcAnchorShape() {
      return tulip.IntegerProperty(_GlGraphInputData_getElementSrcAnchorShape(this.cppPointer));
    };

    tulip.GlGraphInputData.prototype.setElementSrcAnchorShape = function tulip_GlGraphInputData_prototype_setElementSrcAnchorShape(prop) {
      checkArgumentsTypes(arguments, [tulip.IntegerProperty], 1);
      _GlGraphInputData_setElementSrcAnchorShape(this.cppPointer, prop.cppPointer);
    };

    tulip.GlGraphInputData.prototype.getElementSrcAnchorSize = function tulip_GlGraphInputData_prototype_getElementSrcAnchorSize() {
      return tulip.SizeProperty(_GlGraphInputData_getElementSrcAnchorSize(this.cppPointer));
    };

    tulip.GlGraphInputData.prototype.setElementSrcAnchorSize = function tulip_GlGraphInputData_prototype_setElementSrcAnchorSize(prop) {
      checkArgumentsTypes(arguments, [tulip.SizeProperty], 1);
      _GlGraphInputData_setElementSrcAnchorSize(this.cppPointer, prop.cppPointer);
    };

    tulip.GlGraphInputData.prototype.getElementTgtAnchorShape = function tulip_GlGraphInputData_prototype_getElementTgtAnchorShape() {
      return tulip.IntegerProperty(_GlGraphInputData_getElementTgtAnchorShape(this.cppPointer));
    };

    tulip.GlGraphInputData.prototype.setElementTgtAnchorShape = function tulip_GlGraphInputData_prototype_setElementTgtAnchorShape(prop) {
      checkArgumentsTypes(arguments, [tulip.IntegerProperty], 1);
      _GlGraphInputData_setElementTgtAnchorShape(this.cppPointer, prop.cppPointer);
    };

    tulip.GlGraphInputData.prototype.getElementTgtAnchorSize = function tulip_GlGraphInputData_prototype_getElementTgtAnchorSize() {
      return tulip.SizeProperty(_GlGraphInputData_getElementTgtAnchorSize(this.cppPointer));
    };

    tulip.GlGraphInputData.prototype.setElementTgtAnchorSize = function tulip_GlGraphInputData_prototype_setElementTgtAnchorSize(prop) {
      checkArgumentsTypes(arguments, [tulip.SizeProperty], 1);
      _GlGraphInputData_setElementTgtAnchorSize(this.cppPointer, prop.cppPointer);
    };

    tulip.GlGraphInputData.prototype.getElementFontAwesomeIcon = function tulip_GlGraphInputData_prototype_getElementFontAwesomeIcon() {
      return tulip.StringProperty(_GlGraphInputData_getElementFontAwesomeIcon(this.cppPointer));
    };

    tulip.GlGraphInputData.prototype.setElementFontAwesomeIcon = function tulip_GlGraphInputData_prototype_setElementFontAwesomeIcon(prop) {
      checkArgumentsTypes(arguments, [tulip.StringProperty], 1);
      _GlGraphInputData_setElementFontAwesomeIcon(this.cppPointer, prop.cppPointer);
    };

    tulip.GlGraphInputData.prototype.getElementGlow = function tulip_GlGraphInputData_prototype_getElementGlow() {
      return tulip.BooleanProperty(_GlGraphInputData_getElementGlow(this.cppPointer));
    };

    tulip.GlGraphInputData.prototype.setElementGlow = function tulip_GlGraphInputData_prototype_setElementGlow(prop) {
      checkArgumentsTypes(arguments, [tulip.BooleanProperty], 1);
      _GlGraphInputData_setElementGlow(this.cppPointer, prop.cppPointer);
    };

    tulip.GlGraphInputData.prototype.getElementGlowColor = function tulip_GlGraphInputData_prototype_getElementGlowColor() {
      return tulip.ColorProperty(_GlGraphInputData_getElementGlowColor(this.cppPointer));
    };

    tulip.GlGraphInputData.prototype.setElementGlowColor = function tulip_GlGraphInputData_prototype_setElementGlowColor(prop) {
      checkArgumentsTypes(arguments, [tulip.ColorProperty], 1);
      _GlGraphInputData_setElementGlowColor(this.cppPointer, prop.cppPointer);
    };

    tulip.GlGraphInputData.prototype.reloadGraphProperties = function tulip_GlGraphInputData_prototype_reloadGraphProperties(reset) {
      checkArgumentsTypes(arguments, ["boolean"], 0);
      _GlGraphInputData_reloadGraphProperties(this.cppPointer, reset);
    };

    // ==================================================================================================

    var _Camera_getViewport = Module.cwrap('Camera_getViewport', null, ['number', 'number']);
    var _Camera_modelViewMatrix = Module.cwrap('Camera_modelViewMatrix', null, ['number', 'number']);
    var _Camera_projectionMatrix = Module.cwrap('Camera_projectionMatrix', null, ['number', 'number']);
    var _Camera_setModelViewMatrix = Module.cwrap('Camera_setModelViewMatrix', null, ['number', 'number']);
    var _Camera_setProjectionMatrix = Module.cwrap('Camera_setProjectionMatrix', null, ['number', 'number']);

    tulip.Camera = function tulip_Camera(cppPointer) {
      var newObject = createObject(tulip.Camera, this);
      tulip.CppObjectWrapper.call(newObject, cppPointer, "Camera");
      return newObject;
    };
    tulip.Camera.inheritsFrom(tulip.CppObjectWrapper);

    tulip.Camera.prototype.getViewport = function tulip_Camera_prototype_getViewport() {
      var intBuffer = allocArrayInEmHeap(Int32Array, 4);
      _Camera_getViewport(this.cppPointer, intBuffer.byteOffset);
      var ret = Array.prototype.slice.call(intBuffer).map(Number);
      freeArrayInEmHeap(intBuffer);
      return ret;
    };

    tulip.Camera.prototype.modelViewMatrix = function tulip_Camera_prototype_modelViewMatrix() {
      var floatBuffer = allocArrayInEmHeap(Float32Array, 16);
      _Camera_modelViewMatrix(this.cppPointer, floatBuffer.byteOffset);
      var ret = Array.prototype.slice.call(floatBuffer).map(Number);
      freeArrayInEmHeap(floatBuffer);
      return ret;
    };

    tulip.Camera.prototype.setModelViewMatrix = function tulip_Camera_prototype_setModelViewMatrix(mdvMat) {
      checkArgumentsTypes(arguments, ["array"]);
      checkArrayOfType(mdvMat, "number", 0);
      if (mdvMat.length == 16) {
        var floatBuffer = allocArrayInEmHeap(Float32Array, 16);
        floatBuffer.set(mdvMat);
        _Camera_setModelViewMatrix(this.cppPointer, floatBuffer.byteOffset);
        freeArrayInEmHeap(floatBuffer);
      }
    };

    tulip.Camera.prototype.projectionMatrix = function tulip_Camera_prototype_projectionMatrix() {
      var floatBuffer = allocArrayInEmHeap(Float32Array, 16);
      _Camera_projectionMatrix(this.cppPointer, floatBuffer.byteOffset);
      var ret = Array.prototype.slice.call(floatBuffer).map(Number);
      freeArrayInEmHeap(floatBuffer);
      return ret;
    };

    tulip.Camera.prototype.setProjectionMatrix = function tulip_Camera_prototype_setProjectionMatrix(projMat) {
      checkArgumentsTypes(arguments, ["array"]);
      checkArrayOfType(projMat, "number", 0);
      if (projMat.length == 16) {
        var floatBuffer = allocArrayInEmHeap(Float32Array, 16);
        floatBuffer.set(projMat);
        _Camera_setProjectionMatrix(this.cppPointer, floatBuffer.byteOffset);
        freeArrayInEmHeap(floatBuffer);
      }
    };

    // ==================================================================================================

    tulip.Graph.prototype.getTlpFileBlob = function(gzip) {
      var filename = "/graph.tlp";
      if (gzip) {
        filename += ".gz";
      }
      tulip.saveGraph(this, filename);
      var graphData = FS.readFile(filename);
      FS.unlink(filename);
      return new Blob([graphData.buffer]);
    };

  }

  // ==================================================================================================================


  tulip.isLoaded = function() {
    return tulip.mainCalled;
  }

  if (!nodejs) {

    tulip.Graph.prototype.applyAlgorithmInWorker = function(algorithmName, algoParameters, algoFinishedCallback) {
      if (!tulip.algorithmExists(algorithmName)) {
        console.log("Error : no Tulip algorithm named '" + algorithmName + "'");
        return;
      }
      if (algoParameters === undefined) {
        algoParameters = {};
      }
      var graphHierarchyId = this.getRoot().getCppPointer();
      if (tulip.vizFeatures && graphHierarchyId in graphHierarchyIdToView) {
        var view = graphHierarchyIdToView[graphHierarchyId];
        view.setGraphRenderingDataReady(false);
        view.startBusyAnimation();
        view.setProgressBarComment("Applying " + algorithmName + " algorithm ...");
      }
      var graph = this;
      sendGraphToWorker(this.getRoot()).then(function(tulipWorker) {
        var messageData = {
          graphHierarchyId : graphHierarchyId,
          graphId : graph.getId(),
          eventType: 'algorithm',
          algorithmName : algorithmName,
          parameters : JSON.stringify(algoParameters)
        };
        if (algoFinishedCallback) {
          algorithmFinishedCallbacks[graphHierarchyId] = algoFinishedCallback;
        }
        tulipWorker.postMessage(messageData);
      });
    };

    function applyPropertyAlgorithmInWorker(graph, algorithmName, resultProperty, algoParameters, algoFinishedCallback) {
      if (algoParameters === undefined) {
        algoParameters = {};
      }
      var graphHierarchyId = graph.getRoot().getCppPointer();
      sendGraphToWorker(graph.getRoot()).then(function(tulipWorker) {
        var messageData = {
          graphHierarchyId : graphHierarchyId,
          graphId : graph.getId(),
          eventType: 'propertyAlgorithm',
          algorithmName : algorithmName,
          resultPropertyName : resultProperty.getName(),
          parameters : JSON.stringify(algoParameters)
        };
        if (algoFinishedCallback) {
          algorithmFinishedCallbacks[graphHierarchyId] = algoFinishedCallback;
        }
        tulipWorker.postMessage(messageData);
        if (tulip.vizFeatures && graphHierarchyId in graphHierarchyIdToView) {
          var view = graphHierarchyIdToView[graphHierarchyId];
          view.setGraphRenderingDataReady(false);
          view.startBusyAnimation();
          view.setProgressBarComment("Applying " + algorithmName + " " + resultProperty.getTypename() + " algorithm ...");
        }
      });
    }

    tulip.Graph.prototype.applyDoubleAlgorithmInWorker = function(algorithmName, resultProperty, algoParameters, algoFinishedCallback) {
      if (!tulip.doubleAlgorithmExists(algorithmName)) {
        console.log("Error : no Tulip double algorithm named '" + algorithmName + "'");
        return;
      }
      if (resultProperty instanceof tulip.DoubleProperty) {
        applyPropertyAlgorithmInWorker(this, algorithmName, resultProperty, algoParameters, algoFinishedCallback);
      } else {
        console.log("Error : Second parameter of tulip.Graph.applyDoubleAlgorithm method must be an instance of tulip.DoubleProperty type.");
      }
    };

    tulip.Graph.prototype.applyLayoutAlgorithmInWorker = function(algorithmName, resultProperty, algoParameters, algoFinishedCallback) {
      if (!tulip.layoutAlgorithmExists(algorithmName)) {
        console.log("Error : no Tulip layout algorithm named '" + algorithmName + "'");
        return;
      }
      if (resultProperty instanceof tulip.LayoutProperty) {
        applyPropertyAlgorithmInWorker(this, algorithmName, resultProperty, algoParameters, algoFinishedCallback);
      } else {
        console.log("Error : Second parameter of tulip.Graph.applyLayoutAlgorithm method must be an instance of tulip.LayoutProperty type.");
      }
    };

    tulip.Graph.prototype.applySizeAlgorithmInWorker = function(algorithmName, resultProperty, algoParameters, algoFinishedCallback) {
      if (!tulip.sizeAlgorithmExists(algorithmName)) {
        console.log("Error : no Tulip size algorithm named '" + algorithmName + "'");
        return;
      }
      if (resultProperty instanceof tulip.SizeProperty) {
        applyPropertyAlgorithmInWorker(this, algorithmName, resultProperty, algoParameters, algoFinishedCallback);
      } else {
        console.log("Error : Second parameter of tulip.Graph.applySizeAlgorithm method must be an instance of tulip.SizeProperty type.");
      }
    };

    tulip.Graph.prototype.executeScriptInWorker = function(graphFunction, scriptExecutedCallback, scriptParameters) {
      var graph = this;
      var graphHierarchyId = this.getRoot().getCppPointer();
      sendGraphToWorker(this.getRoot()).then(function(tulipWorker) {
        if (scriptExecutedCallback) {
          algorithmFinishedCallbacks[graphHierarchyId] = scriptExecutedCallback;
        }
        if (tulip.vizFeatures && graphHierarchyId in graphHierarchyIdToView) {
          var view = graphHierarchyIdToView[graphHierarchyId];
          view.setGraphRenderingDataReady(false);
          view.startBusyAnimation();
          view.setProgressBarComment("Executing script on graph ...");
        }
        tulipWorker.postMessage({
                                   graphHierarchyId: graphHierarchyId,
                                   graphId : graph.getId(),
                                   eventType: 'executeGraphScript',
                                   scriptCode: graphFunction.toString(),
                                   scriptParameters: scriptParameters
                                 });
      });
    };

  }

  // ==================================================================================================================
}
