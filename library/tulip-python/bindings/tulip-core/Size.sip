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

  PySize(const float a0=1, const float a1=1, const float a2=1) : tlp::Vec3f(a0, a1, a2) {}

  PySize(const tlp::Vec3f &v) : tlp::Vec3f(v) {}

};

}
%End

namespace tlp {

class PySize : tlp::Vec3f /PyName=Size/ {

 public:

  PySize(const float a0=1, const float a1=1, const float a2=1);

  PySize(const tlp::Vec3f &v);

};

typedef tlp::PySize Size;

};
