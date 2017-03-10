#include <QString>

namespace tlp {

#ifdef TULIP_BUILD_PYTHON_COMPONENTS
QString getSipVersion() {
  return SIP_VERSION;
}
#endif

QString getTulipSvnRevision() {
  return TULIP_SVN_REVISION;
}

}
