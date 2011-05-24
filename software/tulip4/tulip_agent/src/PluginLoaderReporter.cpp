#include "PluginLoaderReporter.h"

PluginLoaderReporter::PluginLoaderReporter(): tlp::PluginLoader() {
}

void PluginLoaderReporter::aborted(const std::string &filename, const std::string &errormsg) {
  _errors[filename.c_str()] = errormsg.c_str();
}
