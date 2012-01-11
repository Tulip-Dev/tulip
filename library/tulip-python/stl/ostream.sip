
namespace std {

class ostream /NoDefaultCtors/ {
%TypeHeaderCode
#include <iostream>
%End

public:

	std::ostream& operator<< (SIP_PYOBJECT val) /NoCopy/;
%MethodCode
	PyObject *objStr = PyObject_Str(a1);
	PyObject *objRepr = PyObject_Repr(a1);
	if (objStr) {
		sipRes = &(*a0 << PyString_AS_STRING(objStr));
	} else if (objRepr) {
		sipRes = &(*a0 << PyString_AS_STRING(objRepr));
	} else {
		sipRes = a0;
	}
	
	Py_XDECREF(objStr);
	Py_XDECREF(objRepr);
%End	

};

};