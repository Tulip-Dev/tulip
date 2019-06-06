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
#include <unordered_map>

#include <tulip/ConnectedTestListener.h>
#include <tulip/Graph.h>

using namespace std;
using namespace tlp;

void ConnectedTestListener::treatEvent(const Event &evt) {
  const GraphEvent *gEvt = dynamic_cast<const GraphEvent *>(&evt);

  if (gEvt) {
    Graph *graph = gEvt->getGraph();

    switch (gEvt->getType()) {
    case GraphEvent::TLP_ADD_NODE:
      resultsBuffer[graph] = false;
      break;

    case GraphEvent::TLP_DEL_NODE:
      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    case GraphEvent::TLP_ADD_EDGE:

      if (resultsBuffer.find(graph) != resultsBuffer.end())
        if (resultsBuffer[graph])
          return;

      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

   case GraphEvent::TLP_DEL_EDGE:

      if (resultsBuffer.find(graph) != resultsBuffer.end())
        if (!resultsBuffer[graph])
          return;

      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    default:
      // we don't care about other events
      break;
    }
  } else {

    Graph *graph = static_cast<Graph *>(evt.sender());

    if (evt.type() == Event::TLP_DELETE) {
      resultsBuffer.erase(graph);
    }
  }
}
