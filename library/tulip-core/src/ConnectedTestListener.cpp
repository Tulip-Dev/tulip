/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

#include <tulip/ConnectedTestListener.h>
#include <tulip/Graph.h>

using namespace std;
using namespace tlp;

void ConnectedTestListener::treatEvent(const Event &evt) {
  const GraphEvent *gEvt = dynamic_cast<const GraphEvent *>(&evt);

  if (gEvt) {
    Graph *graph = gEvt->getGraph();

    auto it = resultsBuffer.find(graph);
    switch (gEvt->getType()) {
    case GraphEvent::TLP_ADD_NODE:
      resultsBuffer[graph] = false;
      break;

    case GraphEvent::TLP_AFTER_DEL_NODE:
      if (it != resultsBuffer.end()) {
        graph->removeListener(this);
        resultsBuffer.erase(it);
      }
      break;

    case GraphEvent::TLP_ADD_EDGE:

      if (it != resultsBuffer.end()) {
        if (it->second)
          return;

        graph->removeListener(this);
        resultsBuffer.erase(it);
      }
      break;

    case GraphEvent::TLP_AFTER_DEL_EDGE:

      if (it != resultsBuffer.end()) {
        if (!it->second)
          return;

        graph->removeListener(this);
        resultsBuffer.erase(it);
      }
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
