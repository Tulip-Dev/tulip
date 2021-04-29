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

#include <ogdf/energybased/SpringEmbedderKK.h>

#include <tulip2ogdf/OGDFLayoutPluginBase.h>

using namespace tlp;
using namespace ogdf;

static const char *paramHelp[] = {
    // stop tolerance
    "The value for the stop tolerance, below which the system is regarded stable (balanced) and "
    "the optimization stopped.",

    // used layout
    "If set to true, the given layout is used for the initial positions.",

    // zero length
    "If set != 0, value zerolength is used to determine the desirable edge length by L = "
    "zerolength / max distance_ij. Otherwise, zerolength is determined using the node number and "
    "sizes.",

    // edge length
    "The desirable edge length.",

    // compute max iterations
    "If set to true, the number of iterations is computed depending on G.",

    // global iterations
    "The number of global iterations.",

    // local iterations
    "The number of local iterations."};

class OGDFKamadaKawai : public OGDFLayoutPluginBase {

public:
  PLUGININFORMATION("Kamada Kawai (OGDF)", "Karsten Klein", "12/11/2007",
                    "Implements the Kamada-Kawai layout algorithm.<br/>It is a force-directed "
                    "layout algorithm that tries to place vertices with a distance corresponding "
                    "to their graph theoretic distance. ",
                    "1.0", "Force Directed")
  OGDFKamadaKawai(const tlp::PluginContext *context)
      : OGDFLayoutPluginBase(context, context ? new ogdf::SpringEmbedderKK() : nullptr) {
    addInParameter<double>("stop tolerance", paramHelp[0], "0.001");
    addInParameter<bool>("used layout", paramHelp[1], "true");
    addInParameter<double>("zero length", paramHelp[2], "0");
    addInParameter<double>("edge length", paramHelp[3], "0");
    addInParameter<bool>("compute max iterations", paramHelp[4], "true");
    addInParameter<int>("global iterations", paramHelp[5], "50");
    addInParameter<int>("local iterations", paramHelp[6], "50");
  }
  ~OGDFKamadaKawai() override {}

  void beforeCall() override {
    ogdf::SpringEmbedderKK *kamada = static_cast<ogdf::SpringEmbedderKK *>(ogdfLayoutAlgo);

    if (dataSet != nullptr) {
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
