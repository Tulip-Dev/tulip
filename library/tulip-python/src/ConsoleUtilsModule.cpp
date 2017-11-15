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
  PyObject_HEAD
  bool stderrflag;
  bool writeToConsole;
} consoleutils_ConsoleOutput;

static void
consoleutils_ConsoleOutput_dealloc(consoleutils_ConsoleOutput* self) {
#if PY_MAJOR_VERSION >= 3
  Py_TYPE(self)->tp_free(reinterpret_cast<PyObject*>(self));
#else
  self->ob_type->tp_free(reinterpret_cast<PyObject*>(self));
#endif
}

static PyObject *
consoleutils_ConsoleOutput_new(PyTypeObject *type, PyObject *, PyObject *) {
  consoleutils_ConsoleOutput *self;
  self = reinterpret_cast<consoleutils_ConsoleOutput *>(type->tp_alloc(type, 0));
  self->stderrflag = false;
  self->writeToConsole = true;
  return reinterpret_cast<PyObject *>(self);
}

static int
consoleutils_ConsoleOutput_init(consoleutils_ConsoleOutput *self, PyObject *args, PyObject *) {
  int i;

  if (!PyArg_ParseTuple(args, "|i", &i))
    return -1;

  self->stderrflag = i > 0;
  self->writeToConsole = true;
  return 0;
}


/* This redirects stdout from the calling script. */
static PyObject *
consoleutils_ConsoleOutput_write(PyObject *self, PyObject *o) {

  char *buf;

  if(!PyArg_ParseTuple(o, "s", &buf))
    return NULL;

  QString output(QString::fromUtf8(buf));

  bool stdErr = reinterpret_cast<consoleutils_ConsoleOutput *>(self)->stderrflag;

  if (stdErr) {
    if (mainScriptFileName != "") {
      output.replace("<string>", mainScriptFileName);
    }

    consoleErrorOuputString += output;
  }
  else {
    consoleOuputString += output;
  }

  if ((PythonInterpreter::getInstance()->outputEnabled() && !stdErr) ||
      (PythonInterpreter::getInstance()->errorOutputEnabled() && stdErr)) {

    if (buf != NULL && reinterpret_cast<consoleutils_ConsoleOutput *>(self)->writeToConsole) {

      currentConcatOutput += output;

      QStringList lines = currentConcatOutput.split('\n');

      for (int i = 0 ; i < lines.count() - 1 ; ++i) {
        PythonInterpreter::getInstance()->sendOutputToConsole(lines[i], stdErr);
      }

      currentConcatOutput = lines[lines.size() - 1];
    }

  }

  Py_RETURN_NONE;
}

static PyObject *
consoleutils_ConsoleOutput_enableConsoleOutput(PyObject *self, PyObject *o) {
  int i;

  if(!PyArg_ParseTuple(o, "i", &i))
    return NULL;

  reinterpret_cast<consoleutils_ConsoleOutput *>(self)->writeToConsole = i > 0;

  Py_RETURN_NONE;
}

static PyObject *
consoleutils_ConsoleOutput_flush(PyObject *self, PyObject *) {
  if (!currentConcatOutput.isEmpty()) {
    PythonInterpreter::getInstance()->sendOutputToConsole(currentConcatOutput, reinterpret_cast<consoleutils_ConsoleOutput *>(self)->stderrflag);
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
  {const_cast<char*>("stderrflag"), T_BOOL, offsetof(consoleutils_ConsoleOutput, stderrflag), 0, const_cast<char *>("flag for stderr")},
  {const_cast<char*>("writeToConsole"), T_BOOL, offsetof(consoleutils_ConsoleOutput, writeToConsole), 0, const_cast<char *>("flag for enabling/disabling console output")},
  {0, 0, 0, 0, 0}  /* Sentinel */
};


static PyMethodDef consoleutils_ConsoleOutput_methods[] = {
  {
    "write", static_cast<PyCFunction>(consoleutils_ConsoleOutput_write), METH_VARARGS,
    "Post output to the scripting engine"
  },
  {
    "enableConsoleOutput", static_cast<PyCFunction>(consoleutils_ConsoleOutput_enableConsoleOutput), METH_VARARGS,
    "enable / disable console output"
  },
  {
    "flush", static_cast<PyCFunction>(consoleutils_ConsoleOutput_flush), METH_VARARGS,
    ""
  },
  {0, 0, 0, 0}  /* Sentinel */
};

static PyTypeObject consoleutils_ConsoleOutputType = {
#if PY_MAJOR_VERSION >= 3
  PyVarObject_HEAD_INIT(NULL, 0)
#else
  PyObject_HEAD_INIT(NULL)
  0,                         /*ob_size*/
#endif
  "consoleutils.ConsoleOutput",             /*tp_name*/
  sizeof(consoleutils_ConsoleOutput),             /*tp_basicsize*/
  0,                         /*tp_itemsize*/
  reinterpret_cast<destructor>(consoleutils_ConsoleOutput_dealloc), /*tp_dealloc*/
  0,                         /*tp_print*/
  0,                         /*tp_getattr*/
  0,                         /*tp_setattr*/
  0,                         /*tp_compare*/
  0,                         /*tp_repr*/
  0,                         /*tp_as_number*/
  0,                         /*tp_as_sequence*/
  0,                         /*tp_as_mapping*/
  0,                         /*tp_hash */
  0,                         /*tp_call*/
  0,                         /*tp_str*/
  0,                         /*tp_getattro*/
  0,                         /*tp_setattro*/
  0,                         /*tp_as_buffer*/
  Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
  "",           /* tp_doc */
  0,                   /* tp_traverse */
  0,                   /* tp_clear */
  0,                   /* tp_richcompare */
  0,                   /* tp_weaklistoffset */
  0,                   /* tp_iter */
  0,                   /* tp_iternext */
  consoleutils_ConsoleOutput_methods,             /* tp_methods */
  consoleutils_ConsoleOutput_members,             /* tp_members */
  0,                         /* tp_getset */
  0,                         /* tp_base */
  0,                         /* tp_dict */
  0,                         /* tp_descr_get */
  0,                         /* tp_descr_set */
  0,                         /* tp_dictoffset */
  reinterpret_cast<initproc>(consoleutils_ConsoleOutput_init),      /* tp_init */
  0,                         /* tp_alloc */
  consoleutils_ConsoleOutput_new,                 /* tp_new */
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0
#if PY_MAJOR_VERSION >= 3 && PY_MINOR_VERSION >= 4
  ,0
#endif
};


typedef struct {
  PyObject_HEAD
} consoleutils_ConsoleInput;

static void
consoleutils_ConsoleInput_dealloc(consoleutils_ConsoleInput* self) {
#if PY_MAJOR_VERSION >= 3
  Py_TYPE(self)->tp_free(reinterpret_cast<PyObject*>(self));
#else
  self->ob_type->tp_free(reinterpret_cast<PyObject*>(self));
#endif
}

static PyObject *
consoleutils_ConsoleInput_new(PyTypeObject *type, PyObject *, PyObject *) {
  return type->tp_alloc(type, 0);
}

static int
consoleutils_ConsoleInput_init(consoleutils_ConsoleInput *, PyObject *, PyObject *) {
  return 0;
}

/* This redirects stdin from the calling script. */
static PyObject *
consoleutils_ConsoleInput_readline(PyObject *, PyObject *) {
  QString line = PythonInterpreter::getInstance()->readLineFromConsole();
#if PY_MAJOR_VERSION >= 3
  PyObject *obj = PyUnicode_FromString(line.toLatin1().data());
#else
  PyObject *obj = PyString_FromString(line.toLatin1().data());
#endif
  return obj;
}

static PyMemberDef consoleutils_ConsoleInput_members[] = {
  {0, 0, 0, 0, 0}  /* Sentinel */
};


static PyMethodDef consoleutils_ConsoleInput_methods[] = {
  {
    "readline", static_cast<PyCFunction>(consoleutils_ConsoleInput_readline), METH_VARARGS,
    "read an input line from the console"
  },
  {0, 0, 0, 0}  /* Sentinel */
};

static PyTypeObject consoleutils_ConsoleInputType = {
#if PY_MAJOR_VERSION >= 3
  PyVarObject_HEAD_INIT(NULL, 0)
#else
  PyObject_HEAD_INIT(NULL)
  0,                         /*ob_size*/
#endif
  "consoleutils.ConsoleInput",             /*tp_name*/
  sizeof(consoleutils_ConsoleInput),             /*tp_basicsize*/
  0,                         /*tp_itemsize*/
  reinterpret_cast<destructor>(consoleutils_ConsoleInput_dealloc), /*tp_dealloc*/
  0,                         /*tp_print*/
  0,                         /*tp_getattr*/
  0,                         /*tp_setattr*/
  0,                         /*tp_compare*/
  0,                         /*tp_repr*/
  0,                         /*tp_as_number*/
  0,                         /*tp_as_sequence*/
  0,                         /*tp_as_mapping*/
  0,                         /*tp_hash */
  0,                         /*tp_call*/
  0,                         /*tp_str*/
  0,                         /*tp_getattro*/
  0,                         /*tp_setattro*/
  0,                         /*tp_as_buffer*/
  Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
  "",           /* tp_doc */
  0,                   /* tp_traverse */
  0,                   /* tp_clear */
  0,                   /* tp_richcompare */
  0,                   /* tp_weaklistoffset */
  0,                   /* tp_iter */
  0,                   /* tp_iternext */
  consoleutils_ConsoleInput_methods,             /* tp_methods */
  consoleutils_ConsoleInput_members,             /* tp_members */
  0,                         /* tp_getset */
  0,                         /* tp_base */
  0,                         /* tp_dict */
  0,                         /* tp_descr_get */
  0,                         /* tp_descr_set */
  0,                         /* tp_dictoffset */
  reinterpret_cast<initproc>(consoleutils_ConsoleInput_init),      /* tp_init */
  0,                         /* tp_alloc */
  consoleutils_ConsoleInput_new,                 /* tp_new */
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0
#if PY_MAJOR_VERSION >= 3 && PY_MINOR_VERSION >= 4
  ,0
#endif
};

#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef consoleutilsModuleDef = {
  PyModuleDef_HEAD_INIT,
  "consoleutils",     /* m_name */
  "",  /* m_doc */
  -1,                  /* m_size */
  NULL,    /* m_methods */
  NULL,                /* m_reload */
  NULL,                /* m_traverse */
  NULL,                /* m_clear */
  NULL,                /* m_free */
};
#endif

void
initconsoleutils(void) {
  PyObject* m;

  consoleutils_ConsoleOutputType.tp_new = PyType_GenericNew;
  consoleutils_ConsoleInputType.tp_new = PyType_GenericNew;

  if (PyType_Ready(&consoleutils_ConsoleOutputType) < 0)
    return;

  if (PyType_Ready(&consoleutils_ConsoleInputType) < 0)
    return;

#if PY_MAJOR_VERSION >= 3
  m = PyModule_Create(&consoleutilsModuleDef);
  _PyImport_FixupBuiltin(m, const_cast<char *>("consoleutils"));
#else
  m = Py_InitModule3("consoleutils", NULL,"");
  _PyImport_FixupExtension(const_cast<char *>("consoleutils"), const_cast<char *>("consoleutils"));
#endif
  PyObject *cot = reinterpret_cast<PyObject*>(&consoleutils_ConsoleOutputType);
  Py_INCREF(cot);
  PyModule_AddObject(m, "ConsoleOutput", cot);
  PyObject *cit = reinterpret_cast<PyObject*>(&consoleutils_ConsoleInputType);
  Py_INCREF(cit);
  PyModule_AddObject(m, "ConsoleInput", cit);
}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
