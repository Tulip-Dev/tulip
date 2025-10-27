/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

template <typename T>
bool PythonInterpreter::evalSingleStatementAndGetValue(const QString &pythonStatement, T &value) {
  holdGIL();

  PyObject *ret = evalPythonStatement(pythonStatement);

  bool ok = false;

  if (ret) {
    PyObjectToCppObjectConverter<T> converter;
    ok = converter.convert(ret, value);
    decrefPyObject(ret);
  }

  releaseGIL();

  return ok;
}

// use c++11 variadic template for more convenience
template <typename RETURN_TYPE, typename... Param>
bool PythonInterpreter::callFunctionWithParamsAndGetReturnValue(const QString &module,
                                                                const QString &function,
                                                                RETURN_TYPE &returnValue,
                                                                Param... param) {
  tlp::DataSet ds;
  buildParamDataSet(&ds, param...);
  return callFunctionAndGetReturnValue(module, function, ds, returnValue);
}

template <typename... Param>
bool PythonInterpreter::callFunctionWithParams(const QString &module, const QString &function,
                                               Param... param) {
  tlp::DataSet ds;
  buildParamDataSet(&ds, param...);
  return callFunction(module, function, ds);
}

template <typename T, typename... Param>
void PythonInterpreter::buildParamDataSet(DataSet *ds, T a, Param... param) {
  addParameter(ds, a);
  buildParamDataSet(ds, param...);
}

template <typename T>
void PythonInterpreter::buildParamDataSet(DataSet *ds, T a) {
  addParameter(ds, a);
}

template <typename T>
void PythonInterpreter::addParameter(DataSet *ds, T a) {
  std::string st("param_");
  st += std::to_string(ds->size() + 1);
  ds->set(st, a);
}

template <typename RETURN_TYPE>
bool PythonInterpreter::callFunctionAndGetReturnValue(const QString &module,
                                                      const QString &function,
                                                      const tlp::DataSet &parameters,
                                                      RETURN_TYPE &returnValue) {
  holdGIL();
  bool ok = false;
  PyObject *ret = callPythonFunction(module, function, parameters);
  PyObjectToCppObjectConverter<RETURN_TYPE> retConverter;

  if (ret && retConverter.convert(ret, returnValue)) {
    ok = true;
  }

  decrefPyObject(ret);
  releaseGIL();
  return ok;
}
