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

%Docstring
Bases: :class:`tlp.Algorithm`

This class is the base interface for selection plugins.

.. rubric:: class attributes

.. py:attribute:: result

    The result of the algorithm must be stored in this :class:`tlp.BooleanProperty`.
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

%Docstring
Bases: :class:`tlp.Algorithm`

This class is the base interface for color plugins.

.. rubric:: class attributes

.. py:attribute:: result

    The result of the algorithm must be stored in this :class:`tlp.ColorProperty`.
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

%Docstring
Bases: :class:`tlp.Algorithm`

This class is the base interface for metric plugins.

.. rubric:: class attributes

.. py:attribute:: result

    The result of the algorithm must be stored in this :class:`tlp.DoubleProperty`.
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

%Docstring
Bases: :class:`tlp.IntegerAlgorithm`

This class is the base interface for integer plugins.

.. rubric:: class attributes

.. py:attribute:: result

    The result of the algorithm must be stored in this :class:`tlp.IntegerProperty`.
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

%Docstring
Bases: :class:`tlp.Algorithm`

This class is the base interface for layout plugins.

.. rubric:: class attributes

.. py:attribute:: result

    The result of the algorithm must be stored in this :class:`tlp.LayoutProperty`.
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

%Docstring
Bases: :class:`tlp.Algorithm`

This class is the base interface for size plugins.

.. rubric:: class attributes

.. py:attribute:: result

The result of the algorithm must be stored in this :class:`tlp.SizeProperty`.
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

%Docstring
Bases: :class:`tlp.Algorithm`

This class is the base interface for string plugins.

.. rubric:: class attributes

.. py:attribute:: result

    The result of the algorithm must be stored in this :class:`tlp.StringProperty`.
%End

public:
    tlp::StringProperty *result;
    virtual std::string category() const;
protected:
    StringAlgorithm (const tlp::PluginContext*);
};

};
