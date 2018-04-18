/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

namespace tlp {

class TulipMaterialDesignIcons {

%TypeHeaderCode
#include <tulip/TulipMaterialDesignIcons.h>
%End

%Docstring

.. versionadded:: 5.0

Helper class for the configuration of a Material Design Icon glyph.

`Material Design Icons <https://materialdesignicons.com/>`_ : a growing icon collection allows designers and developers
targeting various platforms to download icons in the format, color and size they need for any project.

A glyph has been added to Tulip enabling to use these great icons
as nodes and edges extremities shapes.

That class offers utility functions and supported icons names constants.

To set a node glyph as a Material Design icon, the Icon glyph must be associated
to the node through the modification of the "viewShape" integer property attached to the graph.
The name of the icon to use must then be set in the "viewIcon" string property.
As an example, the following code snippet activates the Icon glyph for all nodes
and sets the "account" icon (:mdicon:`account`)::

  # graph is an instance of a tlp.Graph object
  viewShape = graph.getIntegerProperty("viewShape")
  viewIcon = graph.getStringProperty("viewIcon")

  # sets the Icon glyph on all nodes
  viewShape.setAllNodeValue(tlp.NodeShape.Icon)
  # sets the "account" glyph for all nodes
  viewIcon.setAllNodeValue(tlp.TulipMaterialDesignIcons.Account);

%End

public:

  static std::string getVersion();
%Docstring
tlp.TulipMaterialDesignIcons.getVersion()

Returns the version of the Material Design Icons bundled with Tulip.

:rtype: string
%End

  static std::string getTrueTypeFileLocation();
%Docstring
tlp.TulipMaterialDesignIcons.getTrueTypeFileLocation()

Returns the location of the Material Design Icons TrueType font file bundled with Tulip.

:rtype: string
%End

  static const std::vector<std::string> &getSupportedIcons();
%Docstring
tlp.TulipMaterialDesignIcons.getSupportedIcons()

Returns the list of supported Material Design icons names.

:rtype: list of string
%End

  static bool isIconSupported(const std::string &iconName);
%Docstring
tlp.TulipMaterialDesignIcons.isIconSupported(iconName)

Checks if the provided Material Design icon name is supported.

:param iconName: the name of the icon to check support
:type iconName: string
:rtype: boolean
%End

  static unsigned int getIconCodePoint(const std::string &iconName);
%Docstring
tlp.TulipMaterialDesignIcons.getIconCodePoint(iconName)

Returns the Unicode code point associated to an icon name.

:param iconName: the name of the icon to get the codepoint
:type iconName: string
:rtype: integer
%End


  static std::string getIconUtf8String(const std::string &iconName);
%Docstring
tlp.TulipMaterialDesignIcons.getIconUtf8String(iconName)

Returns an UTF-8 encoded string of a Material Design icon.

:param iconName: a Material Design icon name
:type iconName: string
:rtype: string
%End

};

};
