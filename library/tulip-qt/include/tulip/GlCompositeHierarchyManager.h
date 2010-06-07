#ifndef GL_HCVXHULL_H
#define GL_HCVXHULL_H

#include <string>

#include <tulip/ObservableGraph.h>
#include <vector>
#include <map>


namespace tlp {
	class GlComposite;
	class Color;
	class LayoutProperty;
	class DoubleProperty;
	class SizeProperty;
	class GlComposite;
	class ConvexHullItem;
	class GlConvexHull;
	class Graph;
	class GlLayer;

  /**
   * Create a GlComposite item for each and every graph and subgraph.
	 * This class observes the graph to update said hierarchy when a subgraph is added or deleted.
   */
  class TLP_GL_SCOPE GlCompositeHierarchyManager : public GraphObserver {

  public:
		GlCompositeHierarchyManager(Graph* graph, GlLayer* layer, std::string layerName, LayoutProperty* layout, SizeProperty* size, 
																DoubleProperty* rotation, std::string namingProperty = "name", std::string subCompositeSuffix = " sub-hulls");
		~GlCompositeHierarchyManager();
		
		virtual void addSubGraph(Graph *, Graph *);
		virtual void delSubGraph(Graph *, Graph *);
		virtual void afterSetAttribute(Graph*, const std::string&);
		virtual void beforeSetAttribute(Graph*, const std::string&);
		
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
		const std::string _subCompositesSuffix;
		const std::string _property;
		
		const static std::string temporaryPropertyValue;
		/**
		 * This map contains the composite that contains the associated graph's hull.
		 */
		std::map<tlp::Graph*, tlp::GlComposite*> _hulls;
  };
}

#endif
