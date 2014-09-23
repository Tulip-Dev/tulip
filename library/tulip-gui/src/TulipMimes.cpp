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

#include "tulip/TulipMimes.h"

#include <tulip/TulipMetaTypes.h>
#include <tulip/ForEach.h>
#include <tulip/Perspective.h>
#include <tulip/TlpQtTools.h>

using namespace tlp;




AlgorithmMimeType::AlgorithmMimeType(QString algorithmName, const DataSet &data): _algorithm(algorithmName), _params(data) {
}

void AlgorithmMimeType::run(Graph* g) const {
  emit mimeRun(g);
}

QStringList GraphMimeType::formats()const {
  return QMimeData::formats()<<GRAPH_MIME_TYPE;
}

QStringList AlgorithmMimeType::formats()const {
  return QMimeData::formats()<<ALGORITHM_NAME_MIME_TYPE<<DATASET_MIME_TYPE;
}

QStringList PanelMimeType::formats()const {
  return QMimeData::formats()<<WORKSPACE_PANEL_MIME_TYPE;
}
