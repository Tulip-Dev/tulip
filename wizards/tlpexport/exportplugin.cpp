#include "%ProjectName:l%.%CppHeaderSuffix%"

PLUGIN(%ProjectName:c%);

using namespace tlp;
using namespace std;

/*
 Plugin constructor: declare dependencies and input/output parameters here
 */
%ProjectName:c%::%ProjectName:c%(tlp::PluginContext* context): ExportModule(context) {
//  addInParameter<ParameterType>("Name","Help string","Default value");
//  addDependency("name", "version");
}

%ProjectName:c%::~%ProjectName:c%() {
}

/*
 Executes the plugin and returns true if sucessful
 */
bool %ProjectName:c%::exportGraph(std::ostream &os) {
  return true;
}

/*
 Returns the file extension associated to this kind of export method
 */
std::string %ProjectName:c%::fileExtension() const {
  return "";
}

