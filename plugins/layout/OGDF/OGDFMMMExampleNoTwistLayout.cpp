#include <ogdf/energybased/multilevelmixer/MMMExampleNoTwistLayout.h>

#include "tulip2ogdf/OGDFLayoutPluginBase.h"

class OGDFMMMExampleNoTwistLayout : public OGDFLayoutPluginBase {
  
 public:

	OGDFMMMExampleNoTwistLayout(const tlp::PropertyContext &context) :OGDFLayoutPluginBase(context, new ogdf::MMMExampleNoTwistLayout()) {}
   ~OGDFMMMExampleNoTwistLayout() {}
  
};


LAYOUTPLUGINOFGROUP(OGDFMMMExampleNoTwistLayout,"MMM Example No Twist Layout (OGDF)","Gereon Bartel","01/09/2010","Ok","1.0","Multilevel");
