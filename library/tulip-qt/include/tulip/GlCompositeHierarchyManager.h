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
#ifndef GL_HCVXHULL_H
#define GL_HCVXHULL_H

#include <string>
#include <vector>
#include <map>

#include <tulip/ObservableGraph.h>
#include <tulip/ObservableProperty.h>
#include <tulip/PropertyInterface.h>
#include <tulip/GlComposite.h>

namespace tlp {

class GlConvexGraphHull;

	struct node;
	class GlComposite;
	class Color;
	class LayoutProperty;
	class DoubleProperty;
	class SizeProperty;
	struct ConvexHullItem;
	class GlConvexHull;
	class Graph;
	class GlLayer;

  /**
   * Create a GlComposite item for each and every graph and subgraph.
	 * This class observes the graph to update said hierarchy when a subgraph is added or deleted.
   */
  class TLP_QT_SCOPE GlCompositeHierarchyManager : public GraphObserver, Observable {

  public:
		GlCompositeHierarchyManager(Graph* graph, GlLayer* layer, std::string layerName, LayoutProperty* layout, SizeProperty* size, 
																DoubleProperty* rotation, bool visible = false, std::string namingProperty = "name", std::string subCompositeSuffix = " sub-hulls");
		~GlCompositeHierarchyManager();
		
		void setGraph(tlp::Graph* graph);
		DataSet getData();
		void setData(DataSet dataSet);

		void createComposite();

		void setVisible(bool visible);
		bool isVisible() const;

	protected :
		virtual void addSubGraph(Graph *, Graph *);
		virtual void delSubGraph(Graph *, Graph *);
		virtual void afterSetAttribute(Graph*, const std::string&);
		virtual void beforeSetAttribute(Graph*, const std::string&);
		virtual void addNode(Graph* , const tlp::node );
    
		virtual void update(std::set< Observable* >::iterator begin, std::set< Observable* >::iterator end);
		virtual void observableDestroyed(Observable* );
		
	private:
		bool _shouldRecreate;
		const tlp::Color getColor();
		int _currentColor;
		
		/**
     * Create the hierarchy of ConvexHullItem
     */
    void buildComposite(tlp::Graph* current, tlp::GlComposite* composite);
		
		Graph* _graph;
		GlLayer* _layer;
		GlComposite* _composite;
		LayoutProperty* _layout;
		SizeProperty* _size;
		DoubleProperty* _rotation;
		std::vector<Color> _fillColors;
		std::string _layerName;
		bool _isVisible;
		const std::string _subCompositesSuffix;
		const std::string _property;
		
		const static std::string temporaryPropertyValue;
		/**
		 * This map contains the composite that contains the associated graph's hull.
		 */
		std::map<tlp::Graph*, std::pair<tlp::GlComposite*, GlConvexGraphHull*> > _graphsComposites;
  };
	
	class GlHierarchyMainComposite : public GlComposite {
		public:
			GlHierarchyMainComposite(GlCompositeHierarchyManager* manager);
			virtual void setVisible(bool visible);
		private:
			GlCompositeHierarchyManager* _manager;
	};
}

#endif
