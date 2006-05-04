//-*-c++-*-
/*
 AUTHORS : 
 Sebastien Carceles,
 Pascal Niotout,
 Sophie Bardet,
 Julien Mercadal, 
 Bertrand Ng Sing Kwong

 Maintainer :
 David Auber 

 LICENCE :
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

*/

#include <tulip/TextRenderer.h>
#include <tulip/TlpTools.h>

#include <cmath>
#include <tulip/TulipPlugin.h>
#include <tulip/ForEach.h>

using namespace std;

/** \addtogroup size */
/*@{*/
/// FitToLabel.cpp - Compute size of elements according to the size of the displayed label.
/**
 *
 *  AUTHORS : 
 *  Sebastien Carceles,
 *  Pascal Niotout,
 *  Sophie Bardet,
 *  Julien Mercadal, 
 *  Bertrand Ng Sing Kwong
 *
 *  \author Maintainer : David Auber University Bordeaux I France: Email:auber@tulip-software.org
 */
class FitToLabel: public SizeAlgorithm { 
public:
  //==============================================Algorithm======
  FitToLabel(const PropertyContext &context):SizeAlgorithm(context){}
  //====================================================
  ~FitToLabel(){}
  //====================================================
  bool run() {
    TextRenderer textRender;
    textRender.setMode(TLP_POLYGON);
    textRender.setContext(tlp::TulipLibDir + "tlp/bitmaps/font.ttf", 12, 255, 255, 255); // valeur par défault pour l'affichage
    sizeResult->setAllNodeValue(Size(18,18,1));
    StringProperty *entryLabel = graph->getProperty<StringProperty>("viewLabel");
    node n;
    forEach(n, graph->getNodes()) {
      const string &str = entryLabel->getNodeValue(n);
      if (str != "") {
	float w_max, h, w;
	w_max = 256.0;
	textRender.setString(str, VERBATIM);
	textRender.getBoundingBox(w_max, h, w);
	sizeResult->setNodeValue(n, Size(int(w), int(h), 1));
      }
    }
    sizeResult->setAllEdgeValue(Size(1,1,8));
    return true;
  }
  //====================================================
};
/*@}*/
SIZEPLUGIN(FitToLabel,"Fit to label","Carceles,Niotout,Bardet,Mercadal,Ng Sing Kwong","25/01/2006","0","0","1");
