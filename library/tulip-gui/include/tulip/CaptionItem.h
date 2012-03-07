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
#ifndef CAPTION_H
#define CAPTION_H

#include <QtCore/QObject>

#include "tulip/View.h"
#include "tulip/CaptionGraphicsItem.h"

namespace tlp {

class CaptionItem : public QObject, public Observer {

  Q_OBJECT

public:

  CaptionItem(View *view);
  ~CaptionItem();

  void create();

  void initCaption();

  void generateCaption();

  QGraphicsItem *captionGraphicsItem();

  void treatEvent(const Event &ev);

public slots :

  void applyNewFilter(float begin,float end);
  void selectedPropertyChanged(std::string propertyName);

private :

  void generateGradients(const std::vector<std::pair <double,Color> > &metricToColorFiltered, QGradient &activeGradient, QGradient &hideGradient);

  View *view;

  CaptionGraphicsItem *_captionGraphicsItem;

  Graph *_graph;
  DoubleProperty *_metricProperty;
  ColorProperty *_colorProperty;
};

}

#endif // CAPTION_H
