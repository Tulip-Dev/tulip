#ifndef TULIPMODEL_H
#define TULIPMODEL_H

#include <QtCore/QAbstractItemModel>
#include <tulip/tulipconf.h>

namespace tlp {

class TLP_QT_SCOPE TulipModel: public QAbstractItemModel {
  Q_OBJECT
  Q_ENUMS(TulipRole)

public:
  enum TulipRole {
    GraphRole = Qt::UserRole+1
  };

  explicit TulipModel(QObject *parent=0);
};

}

#endif // TULIPMODEL_H
