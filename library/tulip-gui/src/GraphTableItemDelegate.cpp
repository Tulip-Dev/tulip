/**
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
#include <tulip/GraphTableItemDelegate.h>

#include <QDebug>

#include <tulip/GraphModel.h>
#include <tulip/DoubleProperty.h>
#include <tulip/TulipMetaTypes.h>

using namespace tlp;
using namespace std;

GraphTableItemDelegate::GraphTableItemDelegate(QObject* parent):TulipItemDelegate(parent) {
}

void GraphTableItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
  PropertyInterface* pi = index.data(TulipModel::PropertyRole).value<PropertyInterface*>();

  if (index.data().type() == QVariant::Double && dynamic_cast<DoubleProperty*>(pi) != NULL) {
    DoubleProperty* prop = dynamic_cast<DoubleProperty*>(pi);
    double value = index.data().value<double>();
    double min=0,max=0;

    if (index.data(TulipModel::IsNodeRole).value<bool>()) {
      min = prop->getNodeMin(index.data(TulipModel::GraphRole).value<tlp::Graph*>());
      max = prop->getNodeMax(index.data(TulipModel::GraphRole).value<tlp::Graph*>());
    }

    if (max != min) {
      painter->setBrush(QColor(200,200,200));
      painter->setPen(QColor(200,200,200));
      painter->drawRect(option.rect.x(),option.rect.y()+1,((value - min) / (max - min)) * option.rect.width(), option.rect.height()-2);
    }
  }

  TulipItemDelegate::paint(painter,option,index);
}
