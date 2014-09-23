/**
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
#include <ogdf/misclayout/CircularLayout.h>

#include "tulip2ogdf/OGDFLayoutPluginBase.h"

// comments below have been extracted from OGDF/src/misclayout/CircularLayout.cpp

/// An implementation of the circular layout
/**
 *
 * \author Carsten Gutwenger
 *
 * \par License:
 * This is part of the Open Graph Drawing Framework (OGDF).
 *
 * Copyright (C). All rights reserved.
 * See README.txt in the root directory of the OGDF installation for details.
 *
 * \par
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * Version 2 or 3 as published by the Free Software Foundation
 * and appearing in the files LICENSE_GPL_v2.txt and
 * LICENSE_GPL_v3.txt included in the packaging of this file.
 *
 * \par
 * In addition, as a special exception, you have permission to link
 * this software with the libraries of the COIN-OR Osi project
 * (http://www.coin-or.org/projects/Osi.xml), all libraries required
 * by Osi, and all LP-solver libraries directly supported by the
 * COIN-OR Osi project, and distribute executables, as long as
 * you follow the requirements of the GNU General Public License
 * in regard to all of the software in the executable aside from these
 * third-party libraries.
 *
 * \par
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * \par
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * \see  http://www.gnu.org/copyleft/gpl.html
 ***************************************************************/
class OGDFCircular : public OGDFLayoutPluginBase {

public:
  PLUGININFORMATION("Circular (OGDF)","Carsten Gutwenger","13/11/2007","Ok","1.4","Basic")
  OGDFCircular(const tlp::PluginContext* context) :OGDFLayoutPluginBase(context, new ogdf::CircularLayout()) {
    addInParameter<double> ("minDistCircle",
                            HTML_HELP_OPEN()
                            HTML_HELP_DEF( "type", "double" )
                            HTML_HELP_BODY()
                            "The minimal distance between nodes on a circle."
                            HTML_HELP_CLOSE(),
                            "20.0", false);
    addInParameter<double> ("minDistLevel",
                            HTML_HELP_OPEN()
                            HTML_HELP_DEF( "type", "double" )
                            HTML_HELP_BODY()
                            "The minimal distance between father and child circle. "
                            HTML_HELP_CLOSE(),
                            "20.0", false);
    addInParameter<double> ("minDistSibling",
                            HTML_HELP_OPEN()
                            HTML_HELP_DEF( "type", "double" )
                            HTML_HELP_BODY()
                            "The minimal distance between circles on same level. "
                            HTML_HELP_CLOSE(),
                            "10.0", false);
    addInParameter<double> ("minDistCC",
                            HTML_HELP_OPEN()
                            HTML_HELP_DEF( "type", "double" )
                            HTML_HELP_BODY()
                            "The minimal distance between connected components."
                            HTML_HELP_CLOSE(),
                            "20.0", false);
    addInParameter<double> ("pageRatio",
                            HTML_HELP_OPEN()
                            HTML_HELP_DEF( "type", "double" )
                            HTML_HELP_BODY()
                            "The page ratio used for packing connected components."
                            HTML_HELP_CLOSE(),
                            "1.0", false);
  }
  ~OGDFCircular() {}

  void beforeCall() {
    ogdf::CircularLayout *circular = static_cast<ogdf::CircularLayout*>(ogdfLayoutAlgo);

    if (dataSet != NULL) {
      double val = 0;

      if (dataSet->get("minDistCircle", val))
        circular->minDistCircle(val);

      if (dataSet->get("minDistLevel", val))
        circular->minDistLevel(val);

      if (dataSet->get("minDistSibling", val))
        circular->minDistSibling(val);

      if (dataSet->get("minDistCC", val))
        circular->minDistCC(val);

      if (dataSet->get("pageRatio", val))
        circular->pageRatio(val);
    }
  }

};


PLUGIN(OGDFCircular)
