/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#endif

#include "tulip/PythonIncludes.h"
#include "tulip/PythonInterpreter.h"

#include <QStringList>

using namespace tlp;

QString consoleOuputString = "";
QString consoleErrorOuputString = "";
QString mainScriptFileName = "";

QString currentConcatOutput = "";

typedef struct {
  PyObject_HEAD bool stderrflag;
  bool writeToConsole;
} consoleutils_ConsoleOutput;

static void consoleutils_ConsoleOutput_dealloc(consoleutils_ConsoleOutput *self) {
#if PY_MAJOR_VERSION >= 3
  Py_TYPE(self)->tp_free(reinterpret_cast<PyObject *>(self));
#else
  self->ob_type->tp_free(reinterpret_cast<PyObject *>(self));
#endif
}

static PyObject *consoleutils_ConsoleOutput_new(PyTypeObject *type, PyObject *, PyObject *) {
  consoleutils_ConsoleOutput *self;
  self = reinterpret_cast<consoleutils_ConsoleOutput *>(type->tp_alloc(type, 0));
  self->stderrflag = false;
  self->writeToConsole = true;
  return reinterpret_cast<PyObject *>(self);
}

static int consoleutils_ConsoleOutput_init(consoleutils_ConsoleOutput *self, PyObject *args,
                                           PyObject *) {
  int i;

  if (!PyArg_ParseTuple(args, "|i", &i))
    return -1;

  self->stderrflag = i > 0;
  self->writeToConsole = true;
  return 0;
}

/* This redirects stdout from the calling script. */
static PyObject *consoleutils_ConsoleOutput_write(PyObject *self, PyObject *o) {

  char *buf;

  if (!PyArg_ParseTuple(o, "s", &buf))
    return nullptr;

  QString output(QString::fromUtf8(buf));

  bool stdErr = reinterpret_cast<consoleutils_ConsoleOutput *>(self)->stderrflag;

  if (stdErr) {
    if (!mainScriptFileName.isEmpty()) {
      output.replace("<string>", mainScriptFileName);
    }

    consoleErrorOuputString += output;
  } else {
    consoleOuputString += output;
  }

  if ((PythonInterpreter::getInstance()->outputEnabled() && !stdErr) ||
      (PythonInterpreter::getInstance()->errorOutputEnabled() && stdErr)) {

    if (buf != nullptr && reinterpret_cast<consoleutils_ConsoleOutput *>(self)->writeToConsole) {

      currentConcatOutput += output;

      QStringList lines = currentConcatOutput.split('\n');

      for (int i = 0; i < lines.count() - 1; ++i) {
        PythonInterpreter::getInstance()->sendOutputToConsole(lines[i], stdErr);
      }

      currentConcatOutput = lines[lines.size() - 1];
    }
  }

  Py_RETURN_NONE;
}

static PyObject *consoleutils_ConsoleOutput_enableConsoleOutput(PyObject *self, PyObject *o) {
  int i;

  if (!PyArg_ParseTuple(o, "i", &i))
    return nullptr;

  reinterpret_cast<consoleutils_ConsoleOutput *>(self)->writeToConsole = i > 0;

  Py_RETURN_NONE;
}

static PyObject *consoleutils_ConsoleOutput_flush(PyObject *self, PyObject *) {
  if (!currentConcatOutput.isEmpty()) {
    PythonInterpreter::getInstance()->sendOutputToConsole(
        currentConcatOutput, reinterpret_cast<consoleutils_ConsoleOutput *>(self)->stderrflag);
    currentConcatOutput = "";
  }

  Py_RETURN_NONE;
}

// T_BOOL is not defined for older versions of Python (2.5 for instance)
// define it as T_INT in that case
#ifndef T_BOOL
#define T_BOOL T_INT
#endif

static PyMemberDef consoleutils_ConsoleOutput_members[] = {
    {const_cast<char *>("stderrflag"), T_BOOL, offsetof(consoleutils_ConsoleOutput, stderrflag), 0,
     const_cast<char *>("flag for stderr")},
    {const_cast<char *>("writeToConsole"), T_BOOL,
     offsetof(consoleutils_ConsoleOutput, writeToConsole), 0,
     const_cast<char *>("flag for enabling/disabling console output")},
    {nullptr, 0, 0, 0, nullptr} /* Sentinel */
};

static PyMethodDef consoleutils_ConsoleOutput_methods[] = {
    {"write", static_cast<PyCFunction>(consoleutils_ConsoleOutput_write), METH_VARARGS,
     "Post output to the scripting engine"},
    {"enableConsoleOutput",
     static_cast<PyCFunction>(consoleutils_ConsoleOutput_enableConsoleOutput), METH_VARARGS,
     "enable / disable console output"},
    {"flush", static_cast<PyCFunction>(consoleutils_ConsoleOutput_flush), METH_VARARGS, ""},
    {nullptr, nullptr, 0, nullptr} /* Sentinel */
};

static PyTypeObject consoleutils_ConsoleOutputType = {
#if PY_MAJOR_VERSION >= 3
    PyVarObject_HEAD_INIT(nullptr, 0)
#else
    PyObject_HEAD_INIT(nullptr) 0, /*ob_size*/
#endif
        "consoleutils.ConsoleOutput",                                 /*tp_name*/
    sizeof(consoleutils_ConsoleOutput),                               /*tp_basicsize*/
    0,                                                                /*tp_itemsize*/
    reinterpret_cast<destructor>(consoleutils_ConsoleOutput_dealloc), /*tp_dealloc*/
    nullptr,                                                          /*tp_print*/
    nullptr,                                                          /*tp_getattr*/
    nullptr,                                                          /*tp_setattr*/
    nullptr,                                                          /*tp_compare*/
    nullptr,                                                          /*tp_repr*/
    nullptr,                                                          /*tp_as_number*/
    nullptr,                                                          /*tp_as_sequence*/
    nullptr,                                                          /*tp_as_mapping*/
    nullptr,                                                          /*tp_hash */
    nullptr,                                                          /*tp_call*/
    nullptr,                                                          /*tp_str*/
    nullptr,                                                          /*tp_getattro*/
    nullptr,                                                          /*tp_setattro*/
    nullptr,                                                          /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,                         /*tp_flags*/
    "",                                                               /* tp_doc */
    nullptr,                                                          /* tp_traverse */
    nullptr,                                                          /* tp_clear */
    nullptr,                                                          /* tp_richcompare */
    0,                                                                /* tp_weaklistoffset */
    nullptr,                                                          /* tp_iter */
    nullptr,                                                          /* tp_iternext */
    consoleutils_ConsoleOutput_methods,                               /* tp_methods */
    consoleutils_ConsoleOutput_members,                               /* tp_members */
    nullptr,                                                          /* tp_getset */
    nullptr,                                                          /* tp_base */
    nullptr,                                                          /* tp_dict */
    nullptr,                                                          /* tp_descr_get */
    nullptr,                                                          /* tp_descr_set */
    0,                                                                /* tp_dictoffset */
    reinterpret_cast<initproc>(consoleutils_ConsoleOutput_init),      /* tp_init */
    nullptr,                                                          /* tp_alloc */
    consoleutils_ConsoleOutput_new,                                   /* tp_new */
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    0
#if PY_MAJOR_VERSION >= 3 && PY_MINOR_VERSION >= 4
    ,
    0
#endif
};

typedef struct {
  PyObject_HEAD
} consoleutils_ConsoleInput;

static void consoleutils_ConsoleInput_dealloc(consoleutils_ConsoleInput *self) {
#if PY_MAJOR_VERSION >= 3
  Py_TYPE(self)->tp_free(reinterpret_cast<PyObject *>(self));
#else
  self->ob_type->tp_free(reinterpret_cast<PyObject *>(self));
#endif
}

static PyObject *consoleutils_ConsoleInput_new(PyTypeObject *type, PyObject *, PyObject *) {
  return type->tp_alloc(type, 0);
}

static int consoleutils_ConsoleInput_init(consoleutils_ConsoleInput *, PyObject *, PyObject *) {
  return 0;
}

/* This redirects stdin from the calling script. */
static PyObject *consoleutils_ConsoleInput_readline(PyObject *, PyObject *) {
  QString line = PythonInterpreter::getInstance()->readLineFromConsole();
#if PY_MAJOR_VERSION >= 3
  PyObject *obj = PyUnicode_FromString(line.toLatin1().data());
#else
  PyObject *obj = PyString_FromString(line.toLatin1().data());
#endif
  return obj;
}

static PyMemberDef consoleutils_ConsoleInput_members[] = {
    {nullptr, 0, 0, 0, nullptr} /* Sentinel */
};

static PyMethodDef consoleutils_ConsoleInput_methods[] = {
    {"readline", static_cast<PyCFunction>(consoleutils_ConsoleInput_readline), METH_VARARGS,
     "read an input line from the console"},
    {nullptr, nullptr, 0, nullptr} /* Sentinel */
};

static PyTypeObject consoleutils_ConsoleInputType = {
#if PY_MAJOR_VERSION >= 3
    PyVarObject_HEAD_INIT(nullptr, 0)
#else
    PyObject_HEAD_INIT(nullptr) 0, /*ob_size*/
#endif
        "consoleutils.ConsoleInput",                                 /*tp_name*/
    sizeof(consoleutils_ConsoleInput),                               /*tp_basicsize*/
    0,                                                               /*tp_itemsize*/
    reinterpret_cast<destructor>(consoleutils_ConsoleInput_dealloc), /*tp_dealloc*/
    nullptr,                                                         /*tp_print*/
    nullptr,                                                         /*tp_getattr*/
    nullptr,                                                         /*tp_setattr*/
    nullptr,                                                         /*tp_compare*/
    nullptr,                                                         /*tp_repr*/
    nullptr,                                                         /*tp_as_number*/
    nullptr,                                                         /*tp_as_sequence*/
    nullptr,                                                         /*tp_as_mapping*/
    nullptr,                                                         /*tp_hash */
    nullptr,                                                         /*tp_call*/
    nullptr,                                                         /*tp_str*/
    nullptr,                                                         /*tp_getattro*/
    nullptr,                                                         /*tp_setattro*/
    nullptr,                                                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,                        /*tp_flags*/
    "",                                                              /* tp_doc */
    nullptr,                                                         /* tp_traverse */
    nullptr,                                                         /* tp_clear */
    nullptr,                                                         /* tp_richcompare */
    0,                                                               /* tp_weaklistoffset */
    nullptr,                                                         /* tp_iter */
    nullptr,                                                         /* tp_iternext */
    consoleutils_ConsoleInput_methods,                               /* tp_methods */
    consoleutils_ConsoleInput_members,                               /* tp_members */
    nullptr,                                                         /* tp_getset */
    nullptr,                                                         /* tp_base */
    nullptr,                                                         /* tp_dict */
    nullptr,                                                         /* tp_descr_get */
    nullptr,                                                         /* tp_descr_set */
    0,                                                               /* tp_dictoffset */
    reinterpret_cast<initproc>(consoleutils_ConsoleInput_init),      /* tp_init */
    nullptr,                                                         /* tp_alloc */
    consoleutils_ConsoleInput_new,                                   /* tp_new */
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    0
#if PY_MAJOR_VERSION >= 3 && PY_MINOR_VERSION >= 4
    ,
    0
#endif
};

#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef consoleutilsModuleDef = {
    PyModuleDef_HEAD_INIT,
    "consoleutils", /* m_name */
    "",             /* m_doc */
    -1,             /* m_size */
    nullptr,        /* m_methods */
    nullptr,        /* m_reload */
    nullptr,        /* m_traverse */
    nullptr,        /* m_clear */
    nullptr,        /* m_free */
};
#endif

// This is called via the PyImport_AppendInittab mechanism called
// during interpreter initialization, to make the built-in consoleutils
// module known to Python
PyMODINIT_FUNC initconsoleutils(void) {
  PyObject *m;

  consoleutils_ConsoleOutputType.tp_new = PyType_GenericNew;
  consoleutils_ConsoleInputType.tp_new = PyType_GenericNew;

  PyType_Ready(&consoleutils_ConsoleOutputType);
  PyType_Ready(&consoleutils_ConsoleInputType);

#if PY_MAJOR_VERSION >= 3
  m = PyModule_Create(&consoleutilsModuleDef);
#else
  m = Py_InitModule("consoleutils", nullptr);
#endif
  PyObject *cot = reinterpret_cast<PyObject *>(&consoleutils_ConsoleOutputType);
  Py_INCREF(cot);
  PyModule_AddObject(m, "ConsoleOutput", cot);
  PyObject *cit = reinterpret_cast<PyObject *>(&consoleutils_ConsoleInputType);
  Py_INCREF(cit);
  PyModule_AddObject(m, "ConsoleInput", cit);
#if PY_MAJOR_VERSION >= 3
  return m;
#endif
}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
