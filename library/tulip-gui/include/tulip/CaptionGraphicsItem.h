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

#ifndef CAPTIONGRAPHICSITEM_H
#define CAPTIONGRAPHICSITEM_H

#include <QObject>

#include <vector>
#include <string>

class QGradient;
class QGraphicsProxyWidget;
class QGraphicsSimpleTextItem;
class QPushButton;

namespace tlp {

class GlMainView;
class View;
class CaptionGraphicsBackgroundItem;

class CaptionGraphicsItem : public QObject {

  Q_OBJECT

public:

  CaptionGraphicsItem(View *view);
  ~CaptionGraphicsItem();

  void setType(unsigned int type);

  void loadConfiguration();

  void generateColorCaption(const QGradient &activeGradient, const QGradient &hideGradient, const std::string &propertyName, double minValue, double maxValue);

  void generateSizeCaption(const std::vector< std::pair <double,float> > &metricToSizeFilteredList,const std::string &propertyName, double minValue, double maxValue);

  CaptionGraphicsBackgroundItem *getCaptionItem() const {
    return _rondedRectItem;
  }

  std::string usedProperty();

signals :

  void filterChanged(float begin, float end);
  void selectedPropertyChanged(std::string propertyName);

protected slots :

  void filterChangedSlot(float begin, float end);
  void selectPropertyButtonClicked();
  void propertySelectedSlot();

private :
  QString wrappedPropName(const QString& originalName) const;

  void constructConfigWidget();

  View *_view;

  CaptionGraphicsBackgroundItem *_rondedRectItem;

  QGraphicsProxyWidget *_confPropertySelectionItem;
  QGraphicsSimpleTextItem *_nodesEdgesTextItem;
  QPushButton* _confPropertySelectionWidget;
};

}

#endif // CAPTIONGRAPHICSITEM_H
///@endcond
