#include "tulip/AbstractSmallMultiplesModel.h"
#include "tulip/Coord.h"
#include "tulip/TlpQtTools.h"
#include "tulip/SmallMultiplesView.h"

#include <tulip/tulipconf.h>
#include <math.h>
#include <iostream>

namespace tlp {

AbstractSmallMultiplesModel::AbstractSmallMultiplesModel(): _overview(0), _spacing(1.7) {
}

AbstractSmallMultiplesModel::~AbstractSmallMultiplesModel() {
}

QVariant AbstractSmallMultiplesModel::data(int id, SmallMultiplesDataRole role) {
  if (role == Position) {
    QVariant v;
    int w = round(sqrt(countItems()));
    int row = abs(id / w);
    int col = id % w;

    // Compute spacing
    Coord c(col * _spacing, -1. * _spacing * row, 0);
    v.setValue<Coord>(c);
    return v;
  }
  return QVariant();
}

}
