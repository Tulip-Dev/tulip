
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
#if PY_MAJOR_VERSION >= 3
        sipRes = &(*a0 << convertPythonUnicodeObjectToStdString(objStr));
#else
        sipRes = &(*a0 << PyString_AS_STRING(objStr));
#endif
	} else if (objRepr) {
#if PY_MAJOR_VERSION >= 3
        sipRes = &(*a0 << convertPythonUnicodeObjectToStdString(objRepr));
#else
        sipRes = &(*a0 << PyString_AS_STRING(objRepr));
#endif
	} else {
		sipRes = a0;
	}
	
	Py_XDECREF(objStr);
	Py_XDECREF(objRepr);
%End	

};

};
