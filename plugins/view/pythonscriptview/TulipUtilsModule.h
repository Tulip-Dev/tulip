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

#ifndef TULIPUTILSMODULE_H_
#define TULIPUTILSMODULE_H_

#include "PythonInterpreter.h"

#include <tulip/MainController.h>
#include <tulip/GlMainView.h>
#include <tulip/GlMainWidget.h>


static PyObject *
tuliputils_updateVisualization(PyObject *self, PyObject *args)
{
	int i;
	if (!PyArg_ParseTuple(args, "|i", &i))
		Py_RETURN_NONE;
	bool centerViews = i > 0;

	tlp::MainController *mainController = dynamic_cast<tlp::MainController *>(tlp::Controller::getCurrentController());
	if (mainController) {
		std::vector<tlp::View*> tulipViews = mainController->getViewsOfGraph(mainController->getGraph());
		for (size_t i = 0 ; i < tulipViews.size() ; ++i) {
			tlp::GlMainView *glView = dynamic_cast<tlp::GlMainView *>(tulipViews[i]);
			if (centerViews && glView) {
				glView->getGlMainWidget()->getScene()->centerScene();
			}
			tulipViews[i]->draw();
		}
	}

	Py_RETURN_NONE;
}

static PyMethodDef TulipUtilsMethods[] = {
    {"updateVisualization",  tuliputils_updateVisualization, METH_VARARGS, "Update views on current graph."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

void
inittuliputils(void) {
	PyObject *m = Py_InitModule("tuliputils", TulipUtilsMethods);
	if (m == NULL)
		return;
}

#endif /* TULIPUTILSMODULE_H_ */
