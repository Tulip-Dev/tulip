/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
	struct node;
	class GlComposite;
	class Color;
	class LayoutProperty;
	class DoubleProperty;
	class SizeProperty;
	class ConvexHullItem;
	class GlConvexHull;
	class Graph;
	class GlLayer;

  /**
   * Create a GlComposite item for each and every graph and subgraph.
	 * This class observes the graph to update said hierarchy when a subgraph is added or deleted.
   */
  class TLP_QT_SCOPE GlCompositeHierarchyManager : public GraphObserver, PropertyObserver {

  public:
		GlCompositeHierarchyManager(Graph* graph, GlLayer* layer, std::string layerName, LayoutProperty* layout, SizeProperty* size, 
																DoubleProperty* rotation, std::string namingProperty = "name", std::string subCompositeSuffix = " sub-hulls");
		~GlCompositeHierarchyManager();
		
		virtual void addSubGraph(Graph *, Graph *);
		virtual void delSubGraph(Graph *, Graph *);
		virtual void afterSetAttribute(Graph*, const std::string&);
		virtual void beforeSetAttribute(Graph*, const std::string&);
		virtual void addNode(tlp::Graph* graph, tlp::node n);
		
		virtual void afterSetNodeValue(PropertyInterface* property, const node n);
		
		void setGraph(tlp::Graph* graph);
		
		void createComposite();
		void deleteComposite();
		
		void setVisible(bool visible);
		bool isVisible() const;
		
		private:
		
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
		std::map<tlp::Graph*, tlp::GlComposite*> _graphsComposites;
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
