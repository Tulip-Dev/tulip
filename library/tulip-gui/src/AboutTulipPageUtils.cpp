#include <QString>
#include <QFile>
#include <QTextStream>

#include <tulip/TlpTools.h>
#include <tulip/TlpQtTools.h>

namespace tlp {

#ifdef TULIP_BUILD_PYTHON_COMPONENTS
QString getSipVersion() {
  return SIP_VERSION;
}
#endif

QString getTulipGitRevision() {
  QFile gitCommitFile(tlpStringToQString(TulipShareDir + "GIT_COMMIT"));

  if (gitCommitFile.open(QFile::ReadOnly | QFile::Text)) {
    QTextStream in(&gitCommitFile);
    in.setCodec("UTF-8");
    return in.readAll().replace("\n", "");
  }
  return "";
}
}
