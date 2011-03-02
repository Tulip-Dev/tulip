#include <ogdf/energybased/multilevelmixer/MMMExampleFastLayout.h>

#include "tulip2ogdf/OGDFLayoutPluginBase.h"

class OGDFMMMExampleFastLayout : public OGDFLayoutPluginBase {
  
 public:

	OGDFMMMExampleFastLayout(const tlp::PropertyContext &context) :OGDFLayoutPluginBase(context, new ogdf::MMMExampleFastLayout()) {}
   ~OGDFMMMExampleFastLayout() {}
  
};


LAYOUTPLUGINOFGROUP(OGDFMMMExampleFastLayout,"MMM Example Fast Layout (OGDF)","Gereon Bartel","01/09/2010","Ok","1.0","Multilevel");
