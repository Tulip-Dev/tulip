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
#include <ogdf/energybased/multilevelmixer/MMMExampleNoTwistLayout.h>

#include "tulip2ogdf/OGDFLayoutPluginBase.h"

// comments below have been extracted from OGDF/src/energybased/multilevelmixer/MMEExampleNoTwistLayout.cpp
/** \addtogroup layout */

/// useable example of the Modular Multilevel Mixer
/**
 *
 * This example is tuned to reduce twists in the final drawing.
 * Use this layout or a variation of it if many twists occur.
 * LocalBiconnectedMerger and BarycenterPlacer are used as merging and placement strategies.
 * The FastMultipoleEmbedder is for force calculation.
 *
 * \author Gereon Bartel
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

class OGDFMMMExampleNoTwistLayout : public OGDFLayoutPluginBase {

public:
  PLUGININFORMATION("MMM Example No Twist Layout (OGDF)","Gereon Bartel","01/09/2010","Ok","1.0","Multilevel")
  OGDFMMMExampleNoTwistLayout(const tlp::PluginContext* context) :OGDFLayoutPluginBase(context, new ogdf::MMMExampleNoTwistLayout()) {}
  ~OGDFMMMExampleNoTwistLayout() {}

};


PLUGIN(OGDFMMMExampleNoTwistLayout)
