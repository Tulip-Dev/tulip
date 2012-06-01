/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

#if defined(__GNUC__) && __GNUC__ >= 4 && ((__GNUC_MINOR__ == 2 && __GNUC_PATCHLEVEL__ >= 1) || (__GNUC_MINOR__ >= 3))
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif

#ifndef CONSOLEOUTPUTMODULE_H_
#define CONSOLEOUTPUTMODULE_H_

#include "PythonShellWidget.h"
#include "ConsoleOutputHandler.h"
#include "PythonIncludes.h"

#include <iostream>

using namespace std;

static ConsoleOutputHandler *consoleOuputHandler = NULL;
static ConsoleOutputEmitter *consoleOuputEmitter = NULL;
static std::string consoleOuputString = "";
static std::string consoleErrorOuputString = "";
static QString mainScriptFileName = "";
static bool outputActivated = true;

typedef struct {
  PyObject_HEAD
  bool stderrflag;
  bool writeToConsole;
} scriptengine_ConsoleOutput;

static void
scriptengine_ConsoleOutput_dealloc(scriptengine_ConsoleOutput* self) {
#if PY_MAJOR_VERSION >= 3
  Py_TYPE(self)->tp_free((PyObject*)self);
#else
  self->ob_type->tp_free((PyObject*)self);
#endif
}

static PyObject *
scriptengine_ConsoleOutput_new(PyTypeObject *type, PyObject *, PyObject *) {
  scriptengine_ConsoleOutput *self;
  self = reinterpret_cast<scriptengine_ConsoleOutput *>(type->tp_alloc(type, 0));
  self->stderrflag = false;
  self->writeToConsole = true;
  return reinterpret_cast<PyObject *>(self);
}

static int
scriptengine_ConsoleOutput_init(scriptengine_ConsoleOutput *self, PyObject *args, PyObject *) {
  int i;

  if (!PyArg_ParseTuple(args, "|i", &i))
    return -1;

  self->stderrflag = i > 0;
  self->writeToConsole = true;
  return 0;
}


/* This redirects stdout from the calling script. */
static PyObject *
scriptengine_ConsoleOutput_write(PyObject *self, PyObject *o) {

  char *buf;

  if(!PyArg_ParseTuple(o, "s", &buf))
    return NULL;

  QString output(buf);

  if (reinterpret_cast<scriptengine_ConsoleOutput *>(self)->stderrflag) {
    if (mainScriptFileName != "") {
      output.replace("<string>", mainScriptFileName);
    }

    consoleErrorOuputString += output.toUtf8().data();
  }
  else {
    consoleOuputString += output.toUtf8().data();
  }

  if (outputActivated) {

    if (reinterpret_cast<scriptengine_ConsoleOutput *>(self)->stderrflag) {
      cerr << output.toUtf8().data() << endl;
    }

    if (consoleOuputEmitter) {
      if (buf != NULL && reinterpret_cast<scriptengine_ConsoleOutput *>(self)->writeToConsole) {
        consoleOuputEmitter->sendOutputToConsole(output, reinterpret_cast<scriptengine_ConsoleOutput *>(self)->stderrflag);
      }
    }
  }

  Py_RETURN_NONE;
}

static PyObject *
scriptengine_ConsoleOutput_enableConsoleOutput(PyObject *self, PyObject *o) {
  int i;

  if(!PyArg_ParseTuple(o, "i", &i))
    return NULL;

  ((scriptengine_ConsoleOutput *)self)->writeToConsole = i > 0;

  Py_RETURN_NONE;
}

static PyObject *
scriptengine_ConsoleOutput_flush(PyObject *, PyObject *) {
  Py_RETURN_NONE;
}

// T_BOOL is not defined for older versions of Python (2.5 for instance)
// define it as T_INT in that case
#ifndef T_BOOL
#define T_BOOL T_INT
#endif

static PyMemberDef scriptengine_ConsoleOutput_members[] = {
  {const_cast<char*>("stderrflag"), T_BOOL, offsetof(scriptengine_ConsoleOutput, stderrflag), 0, const_cast<char *>("flag for stderr")},
  {const_cast<char*>("writeToConsole"), T_BOOL, offsetof(scriptengine_ConsoleOutput, writeToConsole), 0, const_cast<char *>("flag for enabling/disabling console output")},
  {NULL}  /* Sentinel */
};


static PyMethodDef scriptengine_ConsoleOutput_methods[] = {
  {
    "write", (PyCFunction) scriptengine_ConsoleOutput_write, METH_VARARGS,
    "Post output to the scripting engine"
  },
  {
    "enableConsoleOutput", (PyCFunction) scriptengine_ConsoleOutput_enableConsoleOutput, METH_VARARGS,
    "enable / disable console output"
  },
  {
    "flush", (PyCFunction) scriptengine_ConsoleOutput_flush, METH_VARARGS,
    ""
  },
  {0, 0, 0, 0}  /* Sentinel */
};

static PyTypeObject scriptengine_ConsoleOutputType = {
#if PY_MAJOR_VERSION >= 3
  PyVarObject_HEAD_INIT(NULL, 0)
#else
  PyObject_HEAD_INIT(NULL)
  0,                         /*ob_size*/
#endif
  "scriptengine.ConsoleOutput",             /*tp_name*/
  sizeof(scriptengine_ConsoleOutput),             /*tp_basicsize*/
  0,                         /*tp_itemsize*/
  (destructor)scriptengine_ConsoleOutput_dealloc, /*tp_dealloc*/
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
  scriptengine_ConsoleOutput_methods,             /* tp_methods */
  scriptengine_ConsoleOutput_members,             /* tp_members */
  0,                         /* tp_getset */
  0,                         /* tp_base */
  0,                         /* tp_dict */
  0,                         /* tp_descr_get */
  0,                         /* tp_descr_set */
  0,                         /* tp_dictoffset */
  (initproc)scriptengine_ConsoleOutput_init,      /* tp_init */
  0,                         /* tp_alloc */
  scriptengine_ConsoleOutput_new,                 /* tp_new */
};


#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef scriptEngineModuleDef = {
  PyModuleDef_HEAD_INIT,
  "scriptengine",     /* m_name */
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
initscriptengine(void) {
  PyObject* m;

  scriptengine_ConsoleOutputType.tp_new = PyType_GenericNew;

  if (PyType_Ready(&scriptengine_ConsoleOutputType) < 0)
    return;

#if PY_MAJOR_VERSION >= 3
  m = PyModule_Create(&scriptEngineModuleDef);
  _PyImport_FixupBuiltin(m, const_cast<char *>("scriptengine"));
#else
  m = Py_InitModule3("scriptengine", NULL,"");
  _PyImport_FixupExtension(const_cast<char *>("scriptengine"), const_cast<char *>("scriptengine"));
#endif
  Py_INCREF(&scriptengine_ConsoleOutputType);
  PyModule_AddObject(m, "ConsoleOutput", (PyObject *)&scriptengine_ConsoleOutputType);
}

#endif /* CONSOLEOUTPUTMODULE_H_ */
