/*
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

// +-------------------------------------------------------------------------+
// | Tulip Python Bindings                                                   |
// | inspired from bindings by the Booggie project development team          |
// | (https://sourceforge.net/projects/booggie/)                             |
// +-------------------------------------------------------------------------+

namespace tlp {

class TulipFontAwesome {

%TypeHeaderCode
#include <tulip/TulipFontAwesome.h>
%End

%Docstring

.. versionadded:: 4.8

Helper class for the configuration of a Font Awesome glyph.

`Font Awesome <http://fontawesome.io>`_ is a free iconic font by Dave Gandy
offering more than 500 customizable scalable vector icons.

A glyph has been added to Tulip enabling to use these great icons
as nodes and edges extremities shapes.

That class offers utility functions and supported icons names constants.

To set a node glyph as a Font Awesome icon, the Icon glyph must be associated
to the node through the modification of the "viewShape" integer property attached to the graph.
The name of the icon to use must then be set in the "viewIcon" string property.
As an example, the following code snippet activates the Icon glyph for all nodes
and sets the "user" icon (:faicon:`user`)::

  # graph is a pointer to a tlp.Graph object
  viewShape = graph.getIntegerProperty("viewShape")
  viewIcon = graph.getStringProperty("viewIcon")

  # sets the Icon glyph on all nodes
  viewShape.setAllNodeValue(tlp.NodeShape.Icon)
  # sets the "user" glyph for all nodes
  viewIcon.setAllNodeValue("fa-user")

%End

public:

  static std::string getVersion();
%Docstring
tlp.TulipFontAwesome.getVersion()

Returns the version of the Font Awesome icons bundled with Tulip.

:rtype: string
%End

  static std::string getTrueTypeFileLocation();
%Docstring
tlp.TulipFontAwesome.getTrueTypeFileLocation()

Returns the location of the Font Awesome TrueType font file bundled with Tulip.

:rtype: string
%End

  static const std::vector<std::string> &getSupportedIcons() /TypeHint="List[str]"/;
%Docstring
tlp.TulipFontAwesome.getSupportedIcons()

Returns the list of supported Font Awesome icons names.

:rtype: list of string
%End

  static bool isIconSupported(const std::string &iconName);
%Docstring
tlp.TulipFontAwesome.isIconSupported(iconName)

Checks if the provided Font Awesome icon name is supported.

:param iconName: the name of the icon to check support
:type iconName: string
:rtype: boolean
%End

  static unsigned int getIconCodePoint(const std::string &iconName);
%Docstring
tlp.TulipFontAwesome.getIconCodePoint(iconName)

Returns the Unicode code point associated to an icon name.

:param iconName: the name of the icon to get the codepoint
:type iconName: string
:rtype: integer
%End


  static std::string getIconUtf8String(const std::string &iconName);
%Docstring
tlp.TulipFontAwesome.getIconUtf8String(iconName)

Returns an UTF-8 encoded string of a Font Awesome icon.

:param iconName: a Font Awesome icon name
:type iconName: string
:rtype: string
%End
};

};
