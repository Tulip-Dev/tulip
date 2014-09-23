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

#ifndef Tulip_GLMETANODERENDERER_H
#define Tulip_GLMETANODERENDERER_H

#include <tulip/tulipconf.h>
#include <tulip/Observable.h>
#include <map>

namespace tlp {

class GlGraphInputData;
class Camera;
class Graph;
class GlScene;

/**
 * Class used to render a meta node
 */
class TLP_GL_SCOPE GlMetaNodeRenderer : public Observable {

public:

  GlMetaNodeRenderer(GlGraphInputData *inputData);

  virtual ~GlMetaNodeRenderer();

  virtual void render(node,float,Camera*);

  virtual void setInputData(GlGraphInputData *inputData);

  virtual GlGraphInputData *getInputData() const;

  GlScene* getSceneForMetaGraph(Graph *g) const;

protected:

  void clearScenes();

  void treatEvent(const Event&);

  virtual GlScene* createScene(Graph*) const;

private:

  GlGraphInputData *_inputData;
  std::map<Graph *,GlScene *> _metaGraphToSceneMap;

};

}

#endif // Tulip_GLMETANODERENDERER_H
///@endcond
