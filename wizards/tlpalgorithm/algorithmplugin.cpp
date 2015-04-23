#include "%ProjectName:l%.%CppHeaderSuffix%"

PLUGIN(%ProjectName:c%)

using namespace tlp;

/*
 Plugin constructor: declare dependencies and input/output parameters here
 */
%ProjectName:c%::%ProjectName:c%(tlp::PluginContext* context): Algorithm(context) {
//  addInParameter<ParameterType>("Name","Help string","Default value");
//  addDependency("name", "version");
}

%ProjectName:c%::~%ProjectName:c%() {
}

/*
 Executes the plugin and returns true if sucessful
 */
bool %ProjectName:c%::run() {
  return true;
}

/*
 Checks if the input data is valid
 */
bool %ProjectName:c%::check(std::string &msg) {
  msg="";
  return true;
}
