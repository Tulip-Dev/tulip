#include <tulip/tulipconf.h>

QDebug operator<<(QDebug dbg,const std::string& s) {
  dbg.nospace() << s.c_str();
  return dbg.space();
}
