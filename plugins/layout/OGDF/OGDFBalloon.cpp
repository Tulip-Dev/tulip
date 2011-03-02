#include <ogdf/misclayout/BalloonLayout.h>

#include "tulip2ogdf/OGDFLayoutPluginBase.h"

// comments below have been extracted from OGDF/src/misclayout/BallonLayout.cpp
/*@{*/
/** \file
 * \brief BalloonLayout for trees that can also be applied to
 * general graphs.
 *
 * Partially based on the papers by Lin/Yen and Carriere/Kazman
 *
 * \author Karsten Klein
 *
 * \par License:
 * This file is part of the Open Graph Drawing Framework (OGDF).
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

namespace {

const char * paramHelp[] = { HTML_HELP_OPEN()
		HTML_HELP_DEF( "type", "bool" )
		HTML_HELP_BODY()
		"Subtrees may be assigned even angles or angles depending on their size."
		HTML_HELP_CLOSE()};
}

class OGDFBalloon : public OGDFLayoutPluginBase {
  
 public:

  OGDFBalloon(const tlp::PropertyContext &context) :OGDFLayoutPluginBase(context, new ogdf::BalloonLayout()) {
	  addParameter<bool> ("Even angles", paramHelp[0], "false", false);
  }
  ~OGDFBalloon() {}
  
  void beforeCall(TulipToOGDF*, ogdf::LayoutModule *ogdfLayoutAlgo) {
  		ogdf::BalloonLayout *balloon = static_cast<ogdf::BalloonLayout*>(ogdfLayoutAlgo);
  		if (dataSet != 0) {
  			bool val;
  			if (dataSet->get("Even angles", val))
  				balloon->setEvenAngles(val);
  		}
  	}

};


LAYOUTPLUGINOFGROUP(OGDFBalloon,"Balloon (OGDF)","Karsten Klein","13/11/2007","Ok","1.3","Hierarchical");
