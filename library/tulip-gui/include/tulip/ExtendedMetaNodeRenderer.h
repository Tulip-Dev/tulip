/*
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
///@cond DOXYGEN_HIDDEN

#ifndef Tulip_ExtendedMetaNodeRenderer_H
#define Tulip_ExtendedMetaNodeRenderer_H

#include <tulip/tulipconf.h>

#include <tulip/GlMetaNodeRenderer.h>
#include <tulip/GlMainView.h>

namespace tlp {

class GlGraphInputData;

/**
 * Class use to render a meta node, this version render meta node with old OpenGl tulip system
 */
class TLP_QT_SCOPE ExtendedMetaNodeRenderer : public GlMetaNodeRenderer, public Observable {

public:

  ExtendedMetaNodeRenderer(GlGraphInputData *inputData);
  virtual ~ExtendedMetaNodeRenderer();

  virtual void render(node n,float lod,Camera* camera);

  virtual void setInputData(GlGraphInputData *data) {
    clearViews();//Need to regenerate views
    inputData=data;
  }
  virtual GlGraphInputData *getInputData() {
    return inputData;
  }

  /**
   * @brief clearViews delete all the view created to render metanodes.
   */
  void clearViews();

protected :

  void treatEvent(const Event&);

  /**
   * @brief createView creates and initialize the GlMainView object used to render the metanode.
   *
   * Override this method if you want to use a different view to render metanodes in a subclasses.
   * @param metaGraph the graph to display in the view.
   * @return the initialized view.
   */
  virtual GlMainView* createView(Graph* metaGraph)const;

  /**
   * @brief viewForGraph search and returns the GlMainView used to render the given metagraph.
   * @param metaGraph the metagraph.
   * @return the GlMainView or NULL if there is no GlMainView associated to it.
   */
  GlMainView* viewForGraph(Graph* metaGraph)const;


private :

  GlGraphInputData *inputData;
  std::map<Graph *,GlMainView *> metaGraphToViewMap;

};

}

#endif // Tulip_ExtendedMetaNodeRenderer_H
///@endcond
