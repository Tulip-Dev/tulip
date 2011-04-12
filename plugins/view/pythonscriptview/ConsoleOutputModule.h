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

// +-------------------------------------------------------------------------+
// | Tulip Python View                                                       |
// | Author:  Antoine Lambert                                                |
// | Contact:  antoine.lambert@labri.fr                                      |
// +-------------------------------------------------------------------------+
// | License:                                                                |
// |                                                                         |
// | Tulip Python View is free software; you can redistribute it             |
// | and/or modify  it under the terms of the GNU General Public License     |
// | as published by the Free Software Foundation; either version 2 of the   |
// | License, or (at your option) any later version.                         |
// |                                                                         |
// | Tulip Python View is distributed in the hope that it will be            |
// | useful, but WITHOUT ANY WARRANTY; without even the implied warranty of  |
// | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            |
// | GNU General Public License for more details.                            |
// |                                                                         |
// | You should have received a copy of the GNU General Public License       |
// | along with this program.  If not, see <http://www.gnu.org/licenses/>.   |
// |                                                                         |
// +-------------------------------------------------------------------------+

#if defined(__GNUC__) && __GNUC__ >= 4 && ((__GNUC_MINOR__ == 2 && __GNUC_PATCHLEVEL__ >= 1) || (__GNUC_MINOR__ >= 3))
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif

#ifndef CONSOLEOUTPUTMODULE_H_
#define CONSOLEOUTPUTMODULE_H_

#include <QtGui/QPlainTextEdit>

#include "PythonInterpreter.h"

#include <iostream>

using namespace std;

QPlainTextEdit *consoleWidget = NULL;

typedef struct {
	PyObject_HEAD
	bool stderrflag;
	bool writeToConsole;
} scriptengine_ConsoleOutput;

static void
scriptengine_ConsoleOutput_dealloc(scriptengine_ConsoleOutput* self) {
	self->ob_type->tp_free((PyObject*)self);
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

	if (((scriptengine_ConsoleOutput *)self)->stderrflag) {
		cerr << buf << endl;
	}

	if (consoleWidget) {
		if (buf != NULL && ((scriptengine_ConsoleOutput *)self)->writeToConsole) {
			QBrush brush(Qt::SolidPattern);
			if (((scriptengine_ConsoleOutput *)self)->stderrflag) {
				brush.setColor(Qt::red);

			} else {
				brush.setColor(Qt::black);
			}

			QTextCharFormat formt;
			formt.setForeground(brush);
			consoleWidget->moveCursor(QTextCursor::End);
			QTextCursor cursor = consoleWidget->textCursor();
			cursor.insertText(buf, formt);
		}
	}
	Py_RETURN_NONE;
}

/* This redirects stdout from the calling script. */
static PyObject *
scriptengine_ConsoleOutput_enableConsoleOutput(PyObject *self, PyObject *o) {
	int i;

	if(!PyArg_ParseTuple(o, "i", &i))
		return NULL;

	((scriptengine_ConsoleOutput *)self)->writeToConsole = i > 0;

	Py_RETURN_NONE;
}

// T_BOOL is not defined for older versions of Python (2.5 for instance)
// define it as T_INT in that case
#ifndef T_BOOL
#define T_BOOL T_INT
#endif

static PyMemberDef scriptengine_ConsoleOutput_members[] = {
		{const_cast<char*>("stderrflag"), T_BOOL, offsetof(scriptengine_ConsoleOutput, stderrflag), 0, const_cast<char *>("flag for stderrflag")},
		{const_cast<char*>("writeToConsole"), T_BOOL, offsetof(scriptengine_ConsoleOutput, writeToConsole), 0, const_cast<char *>("flag for enabling/disabling console output")},
		{NULL}  /* Sentinel */
};


static PyMethodDef scriptengine_ConsoleOutput_methods[] = {
		{"write", (PyCFunction) scriptengine_ConsoleOutput_write, METH_VARARGS,
				"Post output to the scripting engine"},
				{"enableConsoleOutput", (PyCFunction) scriptengine_ConsoleOutput_enableConsoleOutput, METH_VARARGS,
						"enable / disable console output"},
						{0, 0, 0, 0}  /* Sentinel */
};

static PyTypeObject scriptengine_ConsoleOutputType = {
		PyObject_HEAD_INIT(NULL)
		0,                         /*ob_size*/
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
		0,		               /* tp_traverse */
		0,		               /* tp_clear */
		0,		               /* tp_richcompare */
		0,		               /* tp_weaklistoffset */
		0,		               /* tp_iter */
		0,		               /* tp_iternext */
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


void
initscriptengine(void) {
	PyObject* m;

	scriptengine_ConsoleOutputType.tp_new = PyType_GenericNew;
	if (PyType_Ready(&scriptengine_ConsoleOutputType) < 0)
		return;

	m = Py_InitModule3("scriptengine", NULL,"");

	Py_INCREF(&scriptengine_ConsoleOutputType);
	PyModule_AddObject(m, "ConsoleOutput", (PyObject *)&scriptengine_ConsoleOutputType);
}

#endif /* CONSOLEOUTPUTMODULE_H_ */
