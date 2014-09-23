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

#ifndef CAPTIONITEM_H
#define CAPTIONITEM_H

#include <QObject>

#include <tulip/Color.h>
#include <tulip/Observable.h>

class QGradient;

namespace tlp {

class View;
class CaptionGraphicsBackgroundItem;
class CaptionGraphicsItem;
class Graph;
class ColorProperty;
class DoubleProperty;
class SizeProperty;

class CaptionItem : public QObject, public Observable {

  Q_OBJECT

public:

  enum CaptionType {
    NodesColorCaption=1,
    NodesSizeCaption=2,
    EdgesColorCaption=3,
    EdgesSizeCaption=4
  };

  CaptionItem(View *view);
  ~CaptionItem();

  void create(CaptionType captionType);

  void initCaption();

  void generateColorCaption(CaptionType captionType);

  void generateSizeCaption(CaptionType captionType);

  CaptionGraphicsBackgroundItem *captionGraphicsItem();

  void treatEvents(const std::vector<Event> &ev);

signals :

  void filtering(bool);

public slots :

  void removeObservation(bool);

  void applyNewFilter(float begin,float end);
  void selectedPropertyChanged(std::string propertyName);

private :

  void clearObservers();

  void generateGradients(const std::vector<std::pair <double,Color> > &metricToColorFiltered, QGradient &activeGradient, QGradient &hideGradient);

  View *view;

  CaptionType _captionType;
  CaptionGraphicsItem *_captionGraphicsItem;

  Graph *_graph;
  DoubleProperty *_metricProperty;
  ColorProperty *_colorProperty;
  SizeProperty *_sizeProperty;
  ColorProperty *_backupColorProperty;
  ColorProperty *_backupBorderColorProperty;
};

}

#endif // CAPTIONITEM_H
///@endcond
