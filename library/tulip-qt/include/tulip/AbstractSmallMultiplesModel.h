#ifndef ABSTRACTSMALLMULTIPLESMODEL_H
#define ABSTRACTSMALLMULTIPLESMODEL_H

#include "tulip/tulipconf.h"
#include <limits.h>

#include <QtCore/QVariant>

namespace tlp {

/**
  * @brief The AbstractSmallMultiplesModel provides a way for a SmallMultiplesView to retrieve data displayed to preview each data set.
  * An AbstractSmallMultiplesModel implementation controls the position, the texture and the name of data sets previews. For each of those,
  * the data method is called and should return a QVariant containing the data.
  * @see data
  * @see SmallMultiplesView
  */
class TLP_QT_SCOPE AbstractSmallMultiplesModel: public QObject {
  Q_OBJECT
public:
  /**
    * @brief Data roles that can be asked when retrieving data. For each of them is associated a data type that should be included in the resulting QVariant:
    *  . Position: The position of the item in the overview graph. (tlp::Coord)
    *  . Label: The name of the item (displayed below). (QString)
    *  . Texture: The texture for the item's preview. Can be an internal or external path. (QString)
    * @see GlTextureManager
    */
  enum SmallMultiplesDataRole {
    Label     = 1, // 0b001 QString
    Texture   = 2, // 0b010 QImage
    Position  = 4, // 0b100 tlp::Coord
    AllRoles  = 7
  };
  Q_DECLARE_FLAGS(Roles, SmallMultiplesDataRole)

  AbstractSmallMultiplesModel();
  virtual ~AbstractSmallMultiplesModel();

  /**
    * @brief This method is called when the view wants to refresh visual data for an item.
    * @param id The id of the item.
    * @param role The display role
    */
  virtual QVariant data(int id, SmallMultiplesDataRole role);

  /**
    * @return The number of items that should be displayed in the overview.
    */
  virtual int countItems() = 0;

signals:
  /**
    * @brief Refresh data for specified roles on a given item.
    */
  void dataChanged(int fromId, int toId, AbstractSmallMultiplesModel::Roles dataRoles = AllRoles);

  /**
    * @brief Reverse two items.
    * This will reverse a and b id and call data method for a and b with the Position role.
    */
  void reverseItems(int a,int b);

};

Q_DECLARE_OPERATORS_FOR_FLAGS(AbstractSmallMultiplesModel::Roles)

}
#endif // ABSTRACTSMALLMULTIPLESMODEL_H
