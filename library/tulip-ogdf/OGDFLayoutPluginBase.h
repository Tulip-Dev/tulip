#ifndef OGDF_LAYOUT_PLUGIN_BASE
#define OGDF_LAYOUT_PLUGIN_BASE

#include <tulip/TulipPlugin.h>
#include <tulip/LayoutAlgorithm.h>

#include <ogdf/basic/Graph_d.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/module/LayoutModule.h>

#include "TulipToOGDF.h"

class OGDFLayoutPluginBase: public tlp::LayoutAlgorithm {

public:

	OGDFLayoutPluginBase(const tlp::PropertyContext &context, ogdf::LayoutModule *ogdfLayoutAlgo);
	virtual ~OGDFLayoutPluginBase();

	virtual bool run();

protected:

	virtual void beforeCall(TulipToOGDF *, ogdf::LayoutModule *) {}

	virtual void afterCall(TulipToOGDF *, ogdf::LayoutModule *) {}

	void transposeLayoutVertically();

private:
	TulipToOGDF *tlpToOGDF;
	ogdf::LayoutModule *ogdfLayoutAlgo;

};

#endif // OGDF_LAYOUT_PLUGIN_BASE
