static PyObject* callPythonFunction(const QString &module, const QString &function, const tlp::DataSet &parameters) {
  PyObject *ret = NULL;
#if PY_MAJOR_VERSION >= 3
  PyObject *pName = PyUnicode_FromString(module.toStdString().c_str());
#else
  PyObject *pName = PyString_FromString(module.toStdString().c_str());
#endif

  PyObject *pModule = PyImport_Import(pName);
  Py_DECREF(pName);

  PyObject *pDict = PyModule_GetDict(pModule);
  Py_DECREF(pModule);

  PyObject *pFunc = PyDict_GetItemString(pDict, function.toStdString().c_str());

  if (PyCallable_Check(pFunc)) {
    PyObject *argTup = PyTuple_New(parameters.size());
    int idx = 0;
    bool paramError = false;
    std::pair<std::string, DataType*> param;
    forEach(param, parameters.getValues()) {
      PyObject *pyParam = getPyObjectFromDataType(param.second);

      if (!pyParam) {
        paramError = true;
        break;
      }

      PyTuple_SetItem(argTup, idx++, pyParam);
    }

    if (!paramError) {
      ret = PyObject_CallObject(pFunc, argTup);

      if (PyErr_Occurred()) {
        PyErr_Print();
        PyErr_Clear();
      }

    }

    Py_DECREF(argTup);
  }

  return ret;
}

template<typename T>
bool PythonInterpreter::evalSingleStatementAndGetValue(const QString &pythonStatement, T &value) {
  holdGIL();
#if PY_MAJOR_VERSION >= 3
  PyObject *pName = PyUnicode_FromString("__main__");
#else
  PyObject *pName = PyString_FromString("__main__");
#endif
  PyObject *pMainModule = PyImport_Import(pName);
  Py_DECREF(pName);
  PyObject *pMainDict = PyModule_GetDict(pMainModule);

  PyObject *ret = PyRun_String(pythonStatement.toUtf8().data(), Py_eval_input, pMainDict, pMainDict);

  bool ok = false;

  initSipAPI();

  if (ret) {
    PyObjectToCppObjectConvertor<T> convertor;
    ok = convertor.convert(ret, value);
    Py_DECREF(ret);
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
  initSipAPI();
  PyObject *ret = callPythonFunction(module, function, parameters);
  PyObjectToCppObjectConvertor<RETURN_TYPE> retConvertor;

  if (ret && retConvertor.convert(ret, returnValue)) {
    ok = true;
  }

  Py_XDECREF(ret);
  releaseGIL();
  return ok;
}

