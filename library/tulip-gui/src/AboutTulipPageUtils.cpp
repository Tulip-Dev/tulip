#include <QString>

namespace tlp {

#ifdef TULIP_BUILD_PYTHON_COMPONENTS
QString getSipVersion() {
  return SIP_VERSION;
}
#endif

QString getTulipGitRevision() {
  return TULIP_GIT_REVISION;
}
}
