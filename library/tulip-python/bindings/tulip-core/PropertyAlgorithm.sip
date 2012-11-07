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
// | inspired from bindings by the Booggie project development team          |
// | (http://booggie.org/)                                                   |
// +-------------------------------------------------------------------------+

namespace tlp {

class PropertyAlgorithm : tlp::Algorithm {
%TypeHeaderCode
#include <tulip/PropertyAlgorithm.h>
%End
public :

  PropertyAlgorithm(const tlp::PluginContext* context);

  virtual std::string category() const;

};

class BooleanAlgorithm : tlp::PropertyAlgorithm {
%TypeHeaderCode
#include <tulip/PropertyAlgorithm.h>
%End
public:
    tlp::BooleanProperty *result;
    virtual std::string category() const;
protected:
    BooleanAlgorithm (const tlp::PluginContext*);
};


class ColorAlgorithm : tlp::PropertyAlgorithm {
%TypeHeaderCode
#include <tulip/PropertyAlgorithm.h>
%End
public:
    tlp::ColorProperty *result;
    virtual std::string category() const;
protected:
    ColorAlgorithm (const tlp::PluginContext*);
};

class DoubleAlgorithm : tlp::PropertyAlgorithm {
%TypeHeaderCode
#include <tulip/PropertyAlgorithm.h>
%End
public:
    tlp::DoubleProperty *result;
    virtual std::string category() const;
protected:
    DoubleAlgorithm (const tlp::PluginContext*);
};

class IntegerAlgorithm : tlp::PropertyAlgorithm {
%TypeHeaderCode
#include <tulip/PropertyAlgorithm.h>
%End
public:
    tlp::IntegerProperty *result;
    virtual std::string category() const;
protected:
    IntegerAlgorithm (const tlp::PluginContext*);
};

class LayoutAlgorithm : tlp::PropertyAlgorithm {
%TypeHeaderCode
#include <tulip/PropertyAlgorithm.h>
%End
public:
    tlp::LayoutProperty *result;
    virtual std::string category() const;
protected:
    LayoutAlgorithm (const tlp::PluginContext*);
};

class SizeAlgorithm : tlp::PropertyAlgorithm {
%TypeHeaderCode
#include <tulip/PropertyAlgorithm.h>
%End
public:
    tlp::SizeProperty *result;
    virtual std::string category() const;
protected:
    SizeAlgorithm (const tlp::PluginContext*);
};

class StringAlgorithm : tlp::PropertyAlgorithm {
%TypeHeaderCode
#include <tulip/PropertyAlgorithm.h>
%End
public:
    tlp::StringProperty *result;
    virtual std::string category() const;
protected:
    StringAlgorithm (const tlp::PluginContext*);
};

};
