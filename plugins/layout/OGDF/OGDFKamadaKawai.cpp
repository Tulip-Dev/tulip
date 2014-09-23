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
#include <ogdf/energybased/SpringEmbedderKK.h>

#include "tulip2ogdf/OGDFLayoutPluginBase.h"

// comments below have been extracted from OGDF/src/energybased/SpringEmbedderKK.cpp
/** \addtogroup layout */

/**
 * Implementation of Kamada-Kaway layout algorithm.
 *
 * \author Karsten Klein
 *
 * \par License:
 * This is part of the Open Graph Drawing Framework (OGDF).
 * Copyright (C) 2005-2009
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
class OGDFKamadaKawai : public OGDFLayoutPluginBase {

public:
  PLUGININFORMATION("Kamada Kawai (OGDF)","Karsten Klein","12/11/2007","Ok","1.0","Force Directed")
  OGDFKamadaKawai(const tlp::PluginContext* context) :OGDFLayoutPluginBase(context, new ogdf::SpringEmbedderKK()) {
    addInParameter<double>("stop tolerance",
                           HTML_HELP_OPEN()
                           HTML_HELP_DEF( "type", "double" )
                           HTML_HELP_BODY()
                           "The value for the stop tolerance, below which the system is regarded stable (balanced) and the optimization stopped. "
                           HTML_HELP_CLOSE(),
                           "0.001");
    addInParameter<bool>("used layout",
                         HTML_HELP_OPEN()
                         HTML_HELP_DEF( "type", "bool" )
                         HTML_HELP_BODY()
                         "If set to true, the given layout is used for the initial positions."
                         HTML_HELP_CLOSE(),
                         "true");
    addInParameter<double>("zero length",
                           HTML_HELP_OPEN()
                           HTML_HELP_DEF( "type", "double" )
                           HTML_HELP_BODY()
                           "If set != 0, value zerolength is used to determine the desirable edge length by L = zerolength / max distance_ij. Otherwise, zerolength is determined using the node number and sizes."
                           HTML_HELP_CLOSE(),
                           "0");
    addInParameter<double>("edge length",
                           HTML_HELP_OPEN()
                           HTML_HELP_DEF( "type", "double" )
                           HTML_HELP_BODY()
                           "desirable edge length directly."
                           HTML_HELP_CLOSE(),
                           "0");
    addInParameter<bool>("compute max iterations",
                         HTML_HELP_OPEN()
                         HTML_HELP_DEF( "type", "bool" )
                         HTML_HELP_BODY()
                         "If set to true, the number of iterations is computed depending on G."
                         HTML_HELP_CLOSE(),
                         "true");
    addInParameter<int>("global iterations",
                        HTML_HELP_OPEN()
                        HTML_HELP_DEF( "type", "int" )
                        HTML_HELP_BODY()
                        "The number of global iterations."
                        HTML_HELP_CLOSE(),
                        "50");
    addInParameter<int>("local iterations",
                        HTML_HELP_OPEN()
                        HTML_HELP_DEF( "type", "int" )
                        HTML_HELP_BODY()
                        "The number of local iterations."
                        HTML_HELP_CLOSE(),
                        "50");
  }
  ~OGDFKamadaKawai() {}

  void beforeCall() {
    ogdf::SpringEmbedderKK *kamada = static_cast<ogdf::SpringEmbedderKK*>(ogdfLayoutAlgo);

    if (dataSet != NULL) {
      double dval = 0;
      int ival = 0;
      bool bval = false;

      if (dataSet->get("stop tolerance", dval))
        kamada->setStopTolerance(dval);

      if (dataSet->get("use layout", bval))
        kamada->setUseLayout(bval);

      if (dataSet->get("zero length", dval))
        kamada->setZeroLength(dval);

      if (dataSet->get("edge length", dval))
        kamada->setDesLength(dval);

      if (dataSet->get("compute max iterations", bval))
        kamada->computeMaxIterations(bval);

      if (dataSet->get("global iterations", ival))
        kamada->setMaxGlobalIterations(ival);

      if (dataSet->get("local iterations", ival))
        kamada->setMaxLocalIterations(ival);

    }
  }

};


PLUGIN(OGDFKamadaKawai)
