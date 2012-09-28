#include "%ProjectName:l%.%CppHeaderSuffix%"

PLUGIN(%ProjectName:c%)

using namespace tlp;
using namespace std;

/*
const char * paramHelp[] = {
    // number of clusters
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "unsigned int" ) \
    HTML_HELP_BODY() \
    "Determine the number of desired clusters, if 0 the algorithm will find determine the optimal number of clusters" \
     HTML_HELP_CLOSE(),
};
*/
//================================================================================
%ProjectName:
c%::%ProjectName:
c%(tlp::PluginContext* context):Algorithm(context) {
//    addParameter<unsigned int>("shifting", paramHelp[0], 0, false);
//    addParameter<DoubleProperty>("Edge weights", paramHelp[0], 0, false);
}
//================================================================================
%ProjectName:
c%::~%ProjectName:
c%() {
}
//================================================================================
bool %ProjectName:
c% ::run() {
  return true;
}
//================================================================================
bool %ProjectName:
c%::check(string &erreurMsg) {
  erreurMsg="";
  return true;
}
//================================================================================
void %ProjectName:
c%::reset() {
}
//================================================================================
