#include <tulip/StaticProperty.h>

namespace tlp {

// vector<bool> does not support // update
// so do an iterative specialization

template <>
void NodeStaticProperty<bool>::setAll(const bool &val) {
  for (const node &n : graph->nodes())
    (*this)[n] = val;
}

template <>
void EdgeStaticProperty<bool>::setAll(const bool &val) {
  for (const edge &e : graph->edges())
    (*this)[e] = val;
}
}
