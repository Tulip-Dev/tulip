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

TLP_PYTHON_SCOPE void decrefPyObject(PyObject *obj);

template<typename T>
bool PythonInterpreter::evalSingleStatementAndGetValue(const QString &pythonStatement, T &value) {
  holdGIL();

  PyObject *ret = evalPythonStatement(pythonStatement);

  bool ok = false;

  if (ret) {
    PyObjectToCppObjectConvertor<T> convertor;
    ok = convertor.convert(ret, value);
    decrefPyObject(ret);
  }

  releaseGIL();

  return ok;
}

template<typename PARAM_TYPE>
bool PythonInterpreter::callFunctionOneParam(const QString &module, const QString &function, const PARAM_TYPE &parameter) {
  tlp::DataSet parameters;
  parameters.set("param1", parameter);
  return callFunction(module, function, parameters);
}

template<typename PARAM1_TYPE, typename PARAM2_TYPE>
bool PythonInterpreter::callFunctionTwoParams(const QString &module, const QString &function, const PARAM1_TYPE &parameter1, const PARAM2_TYPE &parameter2) {
  tlp::DataSet parameters;
  parameters.set("param1", parameter1);
  parameters.set("param2", parameter2);
  return callFunction(module, function, parameters);
}

template<typename PARAM1_TYPE, typename PARAM2_TYPE, typename PARAM3_TYPE>
bool PythonInterpreter::callFunctionThreeParams(const QString &module, const QString &function, const PARAM1_TYPE &parameter1, const PARAM2_TYPE &parameter2,
    const PARAM3_TYPE &parameter3) {
  tlp::DataSet parameters;
  parameters.set("param1", parameter1);
  parameters.set("param2", parameter2);
  parameters.set("param3", parameter3);
  return callFunction(module, function, parameters);
}

template<typename PARAM1_TYPE, typename PARAM2_TYPE, typename PARAM3_TYPE, typename PARAM4_TYPE>
bool PythonInterpreter::callFunctionFourParams(const QString &module, const QString &function, const PARAM1_TYPE &parameter1, const PARAM2_TYPE &parameter2,
    const PARAM3_TYPE &parameter3, const PARAM4_TYPE &parameter4) {
  tlp::DataSet parameters;
  parameters.set("param1", parameter1);
  parameters.set("param2", parameter2);
  parameters.set("param3", parameter3);
  parameters.set("param4", parameter4);
  return callFunction(module, function, parameters);
}

template<typename PARAM_TYPE, typename RETURN_TYPE>
bool PythonInterpreter::callFunctionOneParamAndGetReturnValue(const QString &module, const QString &function, const PARAM_TYPE &parameter, RETURN_TYPE &returnValue) {
  tlp::DataSet parameters;
  parameters.set("param1", parameter);
  return callFunctionAndGetReturnValue(module, function, parameters, returnValue);
}

template<typename PARAM1_TYPE, typename PARAM2_TYPE, typename RETURN_TYPE>
bool PythonInterpreter::callFunctionTwoParamsAndGetReturnValue(const QString &module, const QString &function, const PARAM1_TYPE &parameter1,
    const PARAM2_TYPE &parameter2, RETURN_TYPE &returnValue) {

  tlp::DataSet parameters;
  parameters.set("param1", parameter1);
  parameters.set("param2", parameter2);
  return callFunctionAndGetReturnValue(module, function, parameters, returnValue);
}

template<typename PARAM1_TYPE, typename PARAM2_TYPE, typename PARAM3_TYPE, typename RETURN_TYPE>
bool PythonInterpreter::callFunctionThreeParamsAndGetReturnValue(const QString &module, const QString &function, const PARAM1_TYPE &parameter1,
    const PARAM2_TYPE &parameter2, const PARAM3_TYPE &parameter3, RETURN_TYPE &returnValue) {

  tlp::DataSet parameters;
  parameters.set("param1", parameter1);
  parameters.set("param2", parameter2);
  parameters.set("param3", parameter3);
  return callFunctionAndGetReturnValue(module, function, parameters, returnValue);
}

template<typename PARAM1_TYPE, typename PARAM2_TYPE, typename PARAM3_TYPE, typename PARAM4_TYPE, typename RETURN_TYPE>
bool PythonInterpreter::callFunctionFourParamsAndGetReturnValue(const QString &module, const QString &function, const PARAM1_TYPE &parameter1,
    const PARAM2_TYPE &parameter2, const PARAM3_TYPE &parameter3, const PARAM4_TYPE &parameter4,
    RETURN_TYPE &returnValue) {

  tlp::DataSet parameters;
  parameters.set("param1", parameter1);
  parameters.set("param2", parameter2);
  parameters.set("param3", parameter3);
  parameters.set("param4", parameter4);
  return callFunctionAndGetReturnValue(module, function, parameters, returnValue);
}

template<typename RETURN_TYPE>
bool PythonInterpreter::callFunctionAndGetReturnValue(const QString &module, const QString &function, const tlp::DataSet &parameters, RETURN_TYPE &returnValue) {
  holdGIL();
  bool ok = false;
  PyObject *ret = callPythonFunction(module, function, parameters);
  PyObjectToCppObjectConvertor<RETURN_TYPE> retConvertor;

  if (ret && retConvertor.convert(ret, returnValue)) {
    ok = true;
  }

  decrefPyObject(ret);
  releaseGIL();
  return ok;
}

