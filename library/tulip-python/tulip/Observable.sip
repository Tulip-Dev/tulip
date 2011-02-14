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
// | Tulip Python Bindings                                                   |
// | Original Bindings by the Booggie project developpment team              |
// | (http://booggie.org/)                                                   |
// | Improved and maintained by Antoine Lambert 			                 |
// | Contact:  antoine.lambert@labri.fr 								     |
// +-------------------------------------------------------------------------+
// | License:                                                                |
// |                                                                         |
// | Tulip Python Bindings is free software; you can redistribute it         |
// | and/or modify  it under the terms of the GNU General Public License     |
// | as published by the Free Software Foundation; either version 2 of the   |
// | License, or (at your option) any later version.                         |
// |                                                                         |
// | Tulip Python Bindings is distributed in the hope that it will be        |
// | useful, but WITHOUT ANY WARRANTY; without even the implied warranty of  |
// | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            |
// | GNU General Public License for more details.                            |
// |                                                                         |
// | You should have received a copy of the GNU General Public License       |
// | along with this program.  If not, see <http://www.gnu.org/licenses/>.   |
// |                                                                         |
// +-------------------------------------------------------------------------+

// Introduce the ObservableSetIterator mapped type - this type is currently not implemented. However it allows the declaration of the update function.

%ModuleHeaderCode
typedef std::set<tlp::Observable *>::iterator ObservableSetIterator;
%End

%MappedType ObservableSetIterator
{
%TypeHeaderCode
#include <set>
%End
%ConvertToTypeCode
	return 0;
%End
%ConvertFromTypeCode
	return 0;
%End
};

namespace tlp {
class Observer {
%TypeHeaderCode
#include <tulip/Observable.h>
%End
public:
	/*
	 * The actual method which has to be implemented by Python classes inheriting Observer is update(self) without
	 * any additional arguments.
	 * sp
	 * Calling this function from Python is currently unsupported (TODO). 
	 */
	virtual void update(ObservableSetIterator, ObservableSetIterator) = 0;
%MethodCode
	sipIsErr = 1;
%End
%VirtualCatcherCode
	PyObject *result = sipCallMethod(0, sipMethod, "");

	if (result != NULL) {
		Py_DECREF(result);
	}
%End

	virtual void observableDestroyed(tlp::Observable *) = 0;
};

class Observable {
%TypeHeaderCode
#include <tulip/Observable.h>
%End

public:
	void addObserver(tlp::Observer *);

	unsigned int countObservers();

	void removeObserver(tlp::Observer *);

	void removeObservers();

	void notifyObservers();

	void notifyDestroy();

	static void holdObservers();

	static void unholdObservers();
};
};

