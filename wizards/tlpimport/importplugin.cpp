#include "%ProjectName:l%.%CppHeaderSuffix%"

PLUGIN(%ProjectName:c%);

using namespace tlp;
using namespace std;

/*
 Plugin constructor: declare dependencies and input/output parameters here
 */
%ProjectName:c%::%ProjectName:c%(tlp::PluginContext* context): ImportModule(context) {
//  addInParameter<ParameterType>("Name","Help string","Default value");
//  addDependency("name", "version");
}

%ProjectName:c%::~%ProjectName:c%() {
}

/*
 Executes the plugin and returns true if sucessful
 */
bool %ProjectName:c%::importGraph() {
  return true;
}

/*
 Returns the file extensions associated to this kind of import method
 */
std::list<std::string> %ProjectName:c%::fileExtensions() const {
  return std::list<std::string>();
}
