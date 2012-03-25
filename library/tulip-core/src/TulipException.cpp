#include <tulip/TulipException.h>

using namespace tlp;

TulipException::TulipException(const std::string &desc):desc(desc) {
}
TulipException::~TulipException() throw () {
}
const char* TulipException::what() const throw() {
  return desc.c_str();
}
