/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

#include <tulip/Color.h>

#include "tulip/ColorButton.h"
#include "tulip/TlpQtTools.h"
#include "tulip/SizeEditor.h"
#include "tulip/ColorButton.h"
#include <tulip/CoordEditor.h>
#include <tulip/TulipFileDescriptorWidget.h>
#include <tulip/TulipMetaTypes.h>


#include <tulip/VectorEditionWidget.h>
#include <tulip/GraphTableModel.h>


#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#include <QtGui/QPainter>
#include <QtGui/QApplication>
#include <QtGui/QTextLayout>

using namespace tlp;
using namespace std;





GraphTableItemDelegate::GraphTableItemDelegate(QObject* parent) :
  TulipItemDelegate(parent) {

}

QWidget* GraphTableItemDelegate::createEditor(QWidget* p, const QStyleOptionViewItem& option,
    const QModelIndex& index) const {
  return TulipItemDelegate::createEditor(p, option, index);
}

void GraphTableItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
    const QModelIndex& index) const {
  TulipItemDelegate::setModelData(editor, model, index);
}


void GraphTableItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {


  //Get the normalized value of the element.
  QVariant normalizedValue = index.data(GraphTableModel::NormalizedValueRole);

  //If we have a normalized value represent it as an histogram.
  if(normalizedValue.isValid() && normalizedValue.type()==QVariant::Double) {
    //Change the color of the background if the element is selected
    if (option.state & QStyle::State_Selected)
      painter->fillRect(option.rect, option.palette.highlight());

    //Draw histogram
    double normalization = normalizedValue.toDouble();
    QRect histogramFrame = option.rect;
    QRect histogramRect(histogramFrame.topLeft(),QSize(histogramFrame.width()*normalization,histogramFrame.height()));
    painter->save();
    painter->fillRect(histogramRect,QBrush(QColor(230,230,230)));
    painter->restore();

    //get data
    QVariant data = index.data();
    TulipItemEditorCreator *c = creator(data.userType());

    //If the TulipItemEditorCreator paint function don't work use default paint system
    if (!c || !c->paint(painter,option,data)) {

      //Get the locale
      QLocale locale=QLocale::system();

      if(const QStyleOptionViewItemV4 *vopt = qstyleoption_cast<const QStyleOptionViewItemV4 *>(&option)) {
        locale=vopt->locale;
      }

      //get display value
      QString text = displayText(data,locale);
      const int textMargin = QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1;
      QRect boundingRect = option.rect;
      boundingRect.adjust(textMargin, 0, -textMargin, 0);
      //Change the color of the text if the element is selected
      QPen pen = painter->pen();

      if (option.state & QStyle::State_Selected)
        pen.setColor(option.palette.color(QPalette::HighlightedText));
      else
        pen.setColor(option.palette.color(QPalette::Text));

      painter->save();
      painter->setPen(pen);
      QTextOption textOption;

      if(const QStyleOptionViewItemV4 *vopt = qstyleoption_cast<const QStyleOptionViewItemV4 *>(&option)) {
        const bool wrapText = vopt->features & QStyleOptionViewItemV2::WrapText;
        textOption.setWrapMode(wrapText ? QTextOption::WordWrap : QTextOption::ManualWrap);
      }

      textOption.setTextDirection(option.direction);
      textOption.setAlignment(QStyle::visualAlignment(option.direction, option.displayAlignment));
      painter->drawText(boundingRect,text,textOption);
      painter->restore();
    }
  }
  else {
    TulipItemDelegate::paint(painter,option,index);
  }
}
