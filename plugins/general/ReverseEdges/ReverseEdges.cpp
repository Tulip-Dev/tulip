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

#include "ReverseEdges.h"

#include <tulip/BooleanProperty.h>

using namespace tlp;

static const char *paramHelp[] = {
    // selection
    "Only edges selected in this property (or all edges if no property is given) will be "
    "reversed."};

ReverseEdges::ReverseEdges(tlp::PluginContext *context) : Algorithm(context) {
  addInParameter<BooleanProperty>("selection", paramHelp[0], "viewSelection", false);
}

bool ReverseEdges::run() {
  BooleanProperty *selection = NULL;

  if (dataSet)
    dataSet->get<BooleanProperty *>("selection", selection);

  Iterator<edge> *ite = selection ? selection->getEdgesEqualTo(true) : graph->getEdges();

  pluginProgress->showPreview(false);

  int step = 0, max_step = graph->numberOfEdges();
  edge e;
  forEach(e, ite) {
    if ((++step % 100) == 0) {
      ProgressState state = pluginProgress->progress(step, max_step);

      if (state != TLP_CONTINUE)
        return state != TLP_CANCEL;
    }

    graph->reverse(e);
  }

  return true;
}

PLUGIN(ReverseEdges)
