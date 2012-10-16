///@cond DOXYGEN_HIDDEN
#ifndef PYTHONVERSIONCHECKER_H
#define PYTHONVERSIONCHECKER_H

#include <tulip/tulipconf.h>
#include <QtCore/QString>

namespace tlp {
class TLP_QT_SCOPE PythonVersionChecker {
public:
  static QString installedVersion();
  static QString compiledVersion();
  static bool isPythonVersionMatching();
};
}

#endif // PYTHONVERSIONCHECKER_H
///@endcond
