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
///@cond DOXYGEN_HIDDEN

#ifndef OGDF_LAYOUT_PLUGIN_BASE
#define OGDF_LAYOUT_PLUGIN_BASE

#include <tulip/TulipPluginHeaders.h>
#include <tulip/PropertyAlgorithm.h>

#include <ogdf/basic/Graph_d.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/LayoutModule.h>

#include "TulipToOGDF.h"

namespace tlp {

class OGDFLayoutPluginBase : public tlp::LayoutAlgorithm {
public:
  OGDFLayoutPluginBase(const tlp::PluginContext *context, ogdf::LayoutModule *ogdfLayoutAlgo);
  ~OGDFLayoutPluginBase() override;

  bool run() override;

protected:
  virtual void beforeCall() {}
  virtual void callOGDFLayoutAlgorithm(ogdf::GraphAttributes &gAttributes);
  virtual void afterCall() {}

  void transposeLayoutVertically();

  TulipToOGDF *tlpToOGDF;
  ogdf::LayoutModule *ogdfLayoutAlgo;
};
} // namespace tlp
#endif // OGDF_LAYOUT_PLUGIN_BASE
       ///@endcond
