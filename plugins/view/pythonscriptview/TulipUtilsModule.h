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

#ifndef TULIPUTILSMODULE_H_
#define TULIPUTILSMODULE_H_

#include "PythonInterpreter.h"

#include <tulip/GlMainView.h>
#include <tulip/GlMainWidget.h>
#include <tulip/ImportModule.h>
#include <tulip/ExportModule.h>

#include <QtGui/QMenu>

static PyObject *
tuliputils_updateVisualization(PyObject *, PyObject *args) {
}

static PyObject *
tuliputils_updatePluginsMenus(PyObject *, PyObject *) {
}

static PyMethodDef TulipUtilsMethods[] = {
  {"updateVisualization",  tuliputils_updateVisualization, METH_VARARGS, "Update views on current graph."},
  {"updatePluginsMenus",  tuliputils_updatePluginsMenus, METH_VARARGS, "Update the plugins menus entries in the Tulip GUI."},
  {NULL, NULL, 0, NULL}        /* Sentinel */
};

void
inittuliputils(void) {
}

#endif /* TULIPUTILSMODULE_H_ */
