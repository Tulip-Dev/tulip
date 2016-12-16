/*
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

// +-------------------------------------------------------------------------+
// | Tulip Python Bindings                                                   |
// | inspired from bindings by the Booggie project development team          |
// | (https://sourceforge.net/projects/booggie/)                             |
// +-------------------------------------------------------------------------+

// Small hack used internally to differentiate tlp.Coord and tlp.Size types in Python
// (as tlp::Coord and tlp::Size are just typedef to tlp::Vec3f in C++)

%ModuleHeaderCode
namespace tlp {

class PySize : public tlp::Vec3f {

 public:

  PySize(const float v=1) : tlp::Vec3f(v) {}

  PySize(const float a0, const float a1, const float a2) : tlp::Vec3f(a0, a1, a2) {}

  PySize(const tlp::PySize &s) : tlp::Vec3f(s) {}

  PySize(const tlp::Vec3f &s) : tlp::Vec3f(s) {}

  tlp::PySize operator*(const tlp::Vec3f &v) const {
    return static_cast<tlp::Vec3f>(*this) * v;
  }

  tlp::PySize operator*(const float &f) const {
    return static_cast<tlp::Vec3f>(*this) * f;
  }

  tlp::PySize operator+(const tlp::Vec3f &v) const {
    return static_cast<tlp::Vec3f>(*this) * v;
  }

  tlp::PySize operator+(const float &f) const {
    return static_cast<tlp::Vec3f>(*this) + f;
  }

  tlp::PySize operator-(const tlp::Vec3f &v) const {
    return static_cast<tlp::Vec3f>(*this) - v;
  }

  tlp::PySize operator-(const float &f) const {
    return static_cast<tlp::Vec3f>(*this) - f;
  }

};

}
%End

namespace tlp {

class PySize : tlp::Vec3f /PyName=Size/ {

%ConvertToTypeCode

  if (sipIsErr == NULL) {
    if (sipCanConvertToType(sipPy, sipFindType("tlp::Vec3f"), SIP_NOT_NONE|SIP_NO_CONVERTORS)) {
      return 1;
    }
    return 0;
  }

  int state=0, err=0;

  *sipCppPtr = new tlp::PySize(*reinterpret_cast<tlp::Vec3f*>(sipConvertToType(sipPy, sipFindType("tlp::Vec3f"), Py_None, SIP_NOT_NONE|SIP_NO_CONVERTORS, &state, &err)));

  if (!*sipCppPtr) {
    *sipIsErr = 1;
  }

  return sipGetState(sipTransferObj);

%End

 public:

  PySize(const float v=1);

  PySize(const float a0, const float a1, const float a2);

  PySize(const tlp::PySize &s);

  PySize(const tlp::Vec3f &s);

  tlp::PySize operator*(const tlp::Vec3f &) const;

  tlp::PySize operator*(const float &) const;

  tlp::PySize operator+(const tlp::Vec3f &) const;

  tlp::PySize operator+(const float &) const;

  tlp::PySize operator-(const tlp::Vec3f &) const;

  tlp::PySize operator-(const float &) const;

  tlp::PySize operator/(const tlp::Vec3f &) const;
%MethodCode
  if ((*a1)[0] && (*a1)[1] && (*a1)[2]) {
    sipRes = new tlp::PySize((*a0 / *a1));
  } else {
    sipIsErr = -1;
    PyErr_SetString(PyExc_ZeroDivisionError, "division of a float vector[3] by a float vector[3] with a zero element");
  }
%End

  tlp::PySize operator/(const float &) const;
%MethodCode
  if (a1 != 0) {
    sipRes = new tlp::PySize((*a0 / a1));
  } else {
    sipIsErr = -1;
    PyErr_SetString(PyExc_ZeroDivisionError, "division of a float vector[3] by zero");
  }
%End

};

typedef tlp::PySize Size;

};
