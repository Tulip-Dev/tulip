//-*-c++-*-
/**
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 06/09/2005
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef Tulip_GLGRAPHRENDERINGPARAMETERS_H
#define Tulip_GLGRAPHRENDERINGPARAMETERS_H

#ifndef NDEBUG
#define TRACE_EXEC()	
//	cerr << __PRETTY_FUNCTION__ << endl;
#else
#define TRACE_EXEC()
#endif


#include <string>
#include <tulip/Color.h>
#include <tulip/Camera.h>
#include <tulip/Reflect.h>

namespace tlp {
  /**
   * That class defines all the paremters used by the GlGraph class
   * during the rendering of a graph.
   */
  class TLP_GL_SCOPE GlGraphRenderingParameters {
  
  public:
    GlGraphRenderingParameters();
    //============================================
    DataSet getParameters() const;
    void    setParameters(const DataSet &);
    /**
     * set the graph used for rendering
     */
    //void setGraph(tlp::Graph *graph);
    /**
     * get the graph used for rendering
     */
    //Graph* getGraph() const;
    /**
     * set viewport of the drawing (screen size)
     */
    //void setViewport(const unsigned x, const unsigned y, const unsigned int width, const unsigned int height);
    /**
     * set viewport of the drawing (screen size)
     */
    //void setViewport(const Vector<int, 4>&viewport);
    /**
     * copy current values into given parameters (names are corresponding)
     */
    //tlp::Vector<int, 4> getViewport() const;
    /** 
     * return a Camera object which desribes the 
     * camera used for he rendering
     */
    //Camera getCamera() const;
    /** 
     * set the camera parameter for the rendering
     */
    //void setCamera(const Camera &cam);
    /**
     * return the background color
     */
    //Color getBackgroundColor() const;
    /** 
     * set the background color, RGB colorspace.
     */
    //void setBackgroundColor(const Color &);
    /** 
     * activate or deactivate displaying of arrows along the graph's edges.
     */
    void setViewArrow(const bool state);
    /** return true if arrows drawing is activated else false.
     * \sa setViewArrow
     */
    bool isViewArrow()const;
    /** 
     * activate or deactivate displaying of labels on nodes.
     */
    void setViewNodeLabel(const bool state);
    /** 
     * return true if drawing of nodes' labels is activated.
     */
    bool isViewNodeLabel() const;
    /** 
     * activate or deactivate displaying of labels on nodes.
     */
    void setViewEdgeLabel(const bool state);
    /** 
     * return true if drawing of nodes' labels is activated.
     */
    bool isViewEdgeLabel() const;
    /** 
     *activate or deactivate interpolation of nodes colors along edge
     */
    void setEdgeColorInterpolate(const bool state);
    /** 
     * return true if color interpolation is activated
     */
    bool isEdgeColorInterpolate() const;
    /** 
     * activate or deactivate interpolation of nodes colors along edge
     */
    void setEdgeSizeInterpolate(const bool state);
    /** 
     * return true if color interpolation is activated
     */
    bool isEdgeSizeInterpolate() const;
    /** 
     * return the type of fonts used in the renderer(0=3D,1=bitmap,2=texture)
     */
    unsigned int getFontsType() const;
    /** 
     * set fonts type used in the renderer (0=3D,1=bitmap,2=texture)
     */
    void setFontsType(unsigned int type);
    /** 
     * set the path for the texture loading
     */
    void setTexturePath(const std::string& path);
    /** 
     * return the absolute path used for texture loading
     */
    std::string getTexturePath() const;
    /**
     * set the absolute path used for the fonts loading
     */
    void setFontsPath(const std::string& path);
    /**
     * return the absolute path used for the fonts loading
     */
    std::string getFontsPath();
    /** 
     * if true glgraph use the incremental rendering else display all elements in one pass
     */
    void setIncrementalRendering(const bool state);
    /** 
     * return the incremental rendering state
     */
    bool isIncrementalRendering() const;
    /**
     * If true and if labels are activated label inside meta nodes will be displayed 
     */
    void setViewMetaLabel(const bool state);
    /**
     * return the true if viewMetaLabels is enable else false
     */
    bool isViewMetaLabel() const;
    /**
     * If true edges are displayed.
     */
    void setDisplayEdges(const bool state);
    /**
     * return true if edges are displayed.
     */
    bool isDisplayEdges() const;
    /** 
     * If true node and edges are ordered according to the viewMetric property
     */
    void setElementOrdered(const bool state);
    /** 
     * return true if element ordering is activated
     */
    bool isElementOrdered() const;
    /** 
     * if true use 3D displaying of edges instead of 2D lines on ALL edges
     */
    void setEdge3D(const bool state);
    /** 
     * return true if 3D drawing of edges is activated
     */
    bool isEdge3D() const;
    /** 
     * Get the size of the border that is added to any label when
     * using bitmap fonts.
     */
    unsigned int getLabelsBorder() const;
    /** 
     * Get the size of the border that is added to any label when
     * using bitmap fonts. Setting this parameter to a high value
     * enables de reduce the label density in the final drawing.
     * Default value is set to 2 which enables readable labels.
     */
    void setLabelsBorder(const unsigned int);
    /**
     * Define the name of the layout that is used
     * for rendering the graph (default is "viewLayout")
     */
    void setInputLayout(const std::string &layoutName);
    /**
     * Return the name of the layout that is used
     * for rendering the graph (default is "viewLayout")
     */
    std::string getInputLayout() const;
  private:
    //tlp::Graph  *_graph;
    bool _viewArrow;
    bool _viewNodeLabel;
    bool _viewEdgeLabel;
    bool _viewMetaLabel;
    bool _elementOrdered;
    bool _viewAutoScale;
    bool _incrementalRendering;
    bool _edgeColorInterpolate;
    bool _edge3D;
    bool _edgeSizeInterpolate;
    bool _displayEdges;
    unsigned int _FontsType;
    unsigned int _labelsBorder;
    std::string _layoutName;
    std::string _fontsPath;
    std::string _texturePath;
  }; 
}

#endif // Tulip_GLGRAPH_H
