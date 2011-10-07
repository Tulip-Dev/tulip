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
