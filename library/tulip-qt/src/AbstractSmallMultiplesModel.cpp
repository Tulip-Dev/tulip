#include "tulip/AbstractSmallMultiplesModel.h"
#include "tulip/Coord.h"
#include "tulip/TlpQtTools.h"
#include <math.h>

namespace tlp {

AbstractSmallMultiplesModel::AbstractSmallMultiplesModel() {
}

AbstractSmallMultiplesModel::~AbstractSmallMultiplesModel() {
}

QVariant AbstractSmallMultiplesModel::data(int id, SmallMultiplesDataRole role) {
  if (role == Position) {
    QVariant v;
    int w = round(sqrt(countItems()));
    int row = abs(id / w);
    int col = id % w;
    double spacing = 1.3;
    Coord c(col * spacing, -1. * spacing * row, 0);
    v.setValue<Coord>(c);
    return v;
  }
  return QVariant();
}

}
