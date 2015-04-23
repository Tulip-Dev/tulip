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
class Plugin : tlp::WithParameter, tlp::WithDependency {
%TypeHeaderCode
#include <tulip/Plugin.h>
%End

%Docstring
Bases: :class:`tlp.WithParameter`

This is the base class for all Tulip plugins.
%End

public:
        virtual ~Plugin();

        virtual std::string category() const=0;
%Docstring
tlp.Plugin.category()

Returns the category of the plugin.

:rtype: string
%End

        virtual std::string name() const=0;
%Docstring
tlp.Plugin.name()

Returns the name of the plugin.

:rtype: string
%End

        virtual std::string group() const=0;
%Docstring
tlp.Plugin.group()

Returns the group of the plugin.

:rtype: string
%End
        virtual std::string author() const=0;
%Docstring
tlp.Plugin.author()

Returns the author of the plugin.

:rtype: string
%End

        virtual std::string date() const=0;
%Docstring
tlp.Plugin.category()

Returns the creation date of the plugin.

:rtype: string
%End

        virtual std::string info() const=0;
%Docstring
tlp.Plugin.category()

Returns some information about the plugin.

:rtype: string
%End

        virtual std::string release() const=0;
%Docstring
tlp.Plugin.category()

Returns the release number of the plugin.

:rtype: string
%End

        virtual std::string tulipRelease() const=0;
%Docstring
tlp.Plugin.category()

Returns the Tulip release number.

:rtype: string
%End

        virtual std::string major() const;

        virtual std::string minor() const;

        virtual std::string tulipMajor() const;

        virtual std::string tulipMinor() const;

        virtual int id() const;

        virtual std::string programmingLanguage() const;


};

class FactoryInterface {
%TypeHeaderCode
#include <tulip/Plugin.h>
%End
public:

  virtual tlp::Plugin* createPluginObject(tlp::PluginContext* context) = 0 /Factory/;

  void registerPlugin();
%MethodCode
    tlp::PluginLister::registerPlugin(sipCpp);
%End

};

};
