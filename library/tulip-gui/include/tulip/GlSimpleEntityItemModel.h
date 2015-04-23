/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
///@cond DOXYGEN_HIDDEN


#ifndef GLSIMPLEENTITYITEMMODEL_H
#define GLSIMPLEENTITYITEMMODEL_H

#include <QAbstractItemModel>

#include <tulip/tulipconf.h>

namespace tlp {

class GlSimpleEntity;

class TLP_QT_SCOPE GlSimpleEntityItemEditor {
public:

  GlSimpleEntityItemEditor(GlSimpleEntity* ent):entity(ent) {}
  virtual ~GlSimpleEntityItemEditor() {}

  /**
   * @brief Return properties names for this entity
   * These properties names are used to dynamically configure the embedded entity
   * for example these function can be used by Mouse information interactor
   * If you create a class that inherits of GlSimpleEntityItemEditor : you can reimplement this function to return your properties names
   * for example : return QStringList() << "fillColor" << "outlineColor";
   * @return QList of properties names
   */
  virtual QStringList propertiesNames() const;

  /**
   * @brief Return properties (in  QVariant format) for this entity
   * These properties QVariant are used to dynamically configure the entity
   * for example these function can be used by Mouse information interactor
   * If you create a class that inherits of GlSimpleEntity : you can reimplement this function to return your properties
   * for example : return QVariantList() << QVariant::fromValue<Color>(getFillColor()) << QVariant::fromValue<Color>(getOutlineColor());
   * @return QList of properties (in QVariant format)
   */
  virtual QVariantList propertiesQVariant() const;

  /**
   * @brief Set value for a property previously returned by propertiesNames() and properties() functions
   * This function is call when we want to set value of a property
   * this parameter is returned in list by propertiesNames() and properties funtions
   * If you create a class that inherits of GlSimpleEntityItemEditor : you can reimplement this function to set your properties
   * For example :
   * if(name=="fillColor")
   *   setFillColor(value.value<Color>());
   * else if(name=="outlineColor")
   *   setOutlineColor(value.value<Color>());
   */
  virtual void setProperty(const QString &name, const QVariant &value);

protected :

  GlSimpleEntity *entity;
};


class TLP_QT_SCOPE GlSimpleEntityItemModel : public QAbstractItemModel {
  Q_OBJECT
  Q_ENUMS(SimpleEntityRole)

public:
  enum SimpleEntityRole {
    SimpleEntityRole = Qt::UserRole+1
  };

  GlSimpleEntityItemModel(GlSimpleEntityItemEditor *itemEditor, QObject *parent=NULL);
  virtual ~GlSimpleEntityItemModel();

  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &child) const;

  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

  QString headerText() const {
    return QString("toto");
  }

  bool setData(const QModelIndex &index, const QVariant &value, int role);

  Qt::ItemFlags flags(const QModelIndex &index) const {
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
  }

protected :

  GlSimpleEntityItemEditor *editor;
};

}

#endif // GLSIMPLEENTITYITEMMODEL_H
///@endcond
