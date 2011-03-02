#include <ogdf/energybased/multilevelmixer/MMMExampleNiceLayout.h>

#include "tulip2ogdf/OGDFLayoutPluginBase.h"

class OGDFMMMExampleNiceLayout : public OGDFLayoutPluginBase {
  
 public:

	OGDFMMMExampleNiceLayout(const tlp::PropertyContext &context) :OGDFLayoutPluginBase(context, new ogdf::MMMExampleNiceLayout()) {}
   ~OGDFMMMExampleNiceLayout() {}
  
};


LAYOUTPLUGINOFGROUP(OGDFMMMExampleNiceLayout,"MMM Example Nice Layout (OGDF)","Gereon Bartel","01/09/2010","Ok","1.0","Multilevel");
