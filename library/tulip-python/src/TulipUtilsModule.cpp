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

#include "tulip/PythonIncludes.h"
#include "tulip/PythonInterpreter.h"

#include <tulip/PluginLister.h>
#include <tulip/Perspective.h>

using namespace tlp;

static PyObject *
tuliputils_updateVisualization(PyObject *, PyObject *args) {
  tlp::Perspective* persp = tlp::Perspective::instance();
  int i;

  if (!PyArg_ParseTuple(args, "|i", &i))
    Py_RETURN_NONE;

  bool centerViews = i > 0;

  if (persp != NULL) {
    persp->redrawPanels(centerViews);
  }

  Py_RETURN_NONE;
}

static PyObject *
tuliputils_pauseRunningScript(PyObject *, PyObject *) {
  Py_RETURN_NONE;
}

static PyObject *
tuliputils_runGraphScript(PyObject *, PyObject *args) {
  char *s = NULL;
  PyObject *o = NULL;

  if (PyArg_ParseTuple(args, "sO", &s, &o)) {
    QString scriptName(s);
    scriptName.replace(".py", "");

    if (PythonInterpreter::getInstance()->runString(QString("import ") + scriptName)) {

      // Getting proper sipWrapperType
      const sipTypeDef* kpTypeDef = sipFindType("tlp::Graph");

      // Checking if the Python object wraps a tlp::Graph instance
      if (sipCanConvertToType(o, kpTypeDef, SIP_NOT_NONE)) {
        int state = 0;
        int err = 0;

        // Unwrapping C++ instance
        tlp::Graph *graph = reinterpret_cast<tlp::Graph *>(sipConvertToType(o, kpTypeDef, NULL, SIP_NOT_NONE, &state, &err));

        if (!PythonInterpreter::getInstance()->runGraphScript(scriptName, "main", graph)) {
          PyErr_SetString(PyExc_Exception, (std::string("An exception occurred when executing the ") + std::string(s) + " script").c_str());
          return NULL;
        }

      }
      else {
        PyErr_SetString(PyExc_TypeError, "Second parameter of the runGraphScript function must be of type tlp.Graph");
        return NULL;
      }
    }
    else {
      PyErr_SetString(PyExc_Exception, (std::string("The script ") + std::string(s) + " does not exist").c_str());
      return NULL;
    }
  }
  else {
    PyErr_SetString(PyExc_TypeError, "Parameters provided to the runGraphScript function have invalid types");
    return NULL;
  }

  Py_RETURN_NONE;
}

static PyObject *
tuliputils_removePlugin(PyObject *, PyObject *args) {

  char *buf;

  if (!PyArg_ParseTuple(args, "s", &buf))
    Py_RETURN_NONE;

  if (tlp::PluginLister::pluginExists(buf)) {
    tlp::PluginLister::removePlugin(buf);
  }

  Py_RETURN_NONE;
}

static PyObject *
tuliputils_setProcessQtEvents(PyObject *, PyObject *o) {
  int i;

  if(!PyArg_ParseTuple(o, "i", &i))
    return NULL;

  PythonInterpreter::getInstance()->setProcessQtEventsDuringScriptExecution(i > 0);

  Py_RETURN_NONE;
}

static PyMethodDef tulipUtilsMethods[] = {
  {"updateVisualization",  tuliputils_updateVisualization, METH_VARARGS, "Update views on current graph."},
  {"pauseRunningScript",  tuliputils_pauseRunningScript, METH_VARARGS, "Pause the execution of the current running script."},
  {"removePlugin",  tuliputils_removePlugin, METH_VARARGS, ""},
  {"runGraphScript",  tuliputils_runGraphScript, METH_VARARGS, "Allow to execute a script from a script."},
  {"setProcessQtEvents",  tuliputils_setProcessQtEvents, METH_VARARGS, ""},
  {NULL, NULL, 0, NULL}        /* Sentinel */
};

#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef tulipUtilsModuleDef = {
  PyModuleDef_HEAD_INIT,
  "tuliputils",     /* m_name */
  "",  /* m_doc */
  -1,                  /* m_size */
  tulipUtilsMethods,    /* m_methods */
  NULL,                /* m_reload */
  NULL,                /* m_traverse */
  NULL,                /* m_clear */
  NULL,                /* m_free */
};
#endif

void
inittuliputils(void) {
#if PY_MAJOR_VERSION >= 3
  PyObject *m = PyModule_Create(&tulipUtilsModuleDef);
  _PyImport_FixupBuiltin(m, const_cast<char *>("tuliputils"));
#else
  Py_InitModule("tuliputils", tulipUtilsMethods);
  _PyImport_FixupExtension(const_cast<char *>("tuliputils"), const_cast<char *>("tuliputils"));
#endif
}

