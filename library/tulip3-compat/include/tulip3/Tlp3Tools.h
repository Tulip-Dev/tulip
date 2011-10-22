/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#ifndef _TLP3TOOLS_H
#define _TLP3TOOLS_H

#include <tulip/tulipconf.h>

#include <tulip/DataSet.h>
#include <tulip/WithParameter.h>
#include <tulip/Graph.h>

class QWidget;

namespace tlp3 {
/**
 * Open a QT Dialog Box to fill-up an user DataSet according to a required
 * StructDef
 * inDef    : Input-variable informations
 * outSet   : Output (filled) set
 * [inSet   : Input (optional) set, containing some default values]
 */

TLP3_COMPAT_SCOPE bool openDataSetDialog (tlp::DataSet & outSet,
    const tlp::ParameterList *sysDef,
    tlp::ParameterList *inDef,
    const tlp::DataSet *inSet = NULL,
    const char *inName = NULL,
    tlp::Graph *inG = NULL,
    QWidget *parent = NULL);

}

#endif /* _TLP3TOOLS_H */
