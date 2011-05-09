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
#ifndef TULIPELEMENTPROPERTIES_H
#define TULIPELEMENTPROPERTIES_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <QtCore/qstringlist.h>
#include <QtCore/qsize.h>

#include <tulip/Graph.h>
#include <tulip/ObservableGraph.h>
#include "tulip/ElementPropertiesWidgetUI.h"

namespace tlp {

  class TLP_QT_SIMPLE_SCOPE ElementPropertiesWidget : public ElementPropertiesWidgetUI, public GraphObserver
    {
      Q_OBJECT;
      // moc as of qt 3.0.5 doesn't understand '::'... silly
      //  Q_ENUMS(Tulip::AtomType);
      //  Q_PROPERTY(Tulip::AtomType displayMode READ getDisplayMode WRITE setDisplayMode);
      Q_PROPERTY(QStringList nodeListedProperties READ getNodeListedProperties WRITE setNodeListedProperties);
      Q_PROPERTY(QStringList edgeListedProperties READ getEdgeListedProperties WRITE setEdgeListedProperties);
      Q_PROPERTY(bool elementDisplayed READ isElementDisplayed);
    protected:
      ElementType displayMode;
      node currentNode;
      edge currentEdge;
      bool nodeSet;
      bool edgeSet;
      bool displayAllProperties;
      Graph *graph;
      QStringList nodeListedProperties;
      QStringList edgeListedProperties;

      //inherited slots

  
    public:
      ElementPropertiesWidget(Graph *, const QStringList &nodeListedProperties,
			      const QStringList &edgeListedProperties,
			      QWidget *parent = 0);
      ElementPropertiesWidget(QWidget *parent = 0);
      ~ElementPropertiesWidget();
    
      QStringList getCurrentListedProperties() const;
      QStringList getNodeListedProperties() const;
      QStringList getEdgeListedProperties() const;
      Graph *getGraph() const;
      node getCurrentNode() const;
      edge getCurrentEdge() const;
      ElementType getDisplayMode() const;
      bool isElementDisplayed() const;
  
      public slots:
      void updateTable();
      void propertyTableValueChanged(int, int);
      void setDisplayMode(const ElementType);
      void setCurrentNode(Graph *, const node &);
      void setCurrentEdge(Graph *, const edge &);
      void setGraph(Graph *, bool distroy = true);
      void setCurrentListedProperties(const QStringList &);
      void setNodeListedProperties(const QStringList &);
      void setEdgeListedProperties(const QStringList &);
      void nodePropertyChanged(Graph *, const node &, const QString &, const QString &);
      void edgePropertyChanged(Graph *, const edge &, const QString &, const QString &);
  
    signals:
      void tulipNodePropertyChanged(Graph *, const node &, const QString &property, const QString &value);
      void tulipEdgePropertyChanged(Graph *, const edge &, const QString &property, const QString &value);


    private:
      void delNode(Graph *, const node);
      void delEdge(Graph *, const edge);
      void destroy(Graph *);
    };

}

#endif //TULIPELEMENTPROPERTIES_H
