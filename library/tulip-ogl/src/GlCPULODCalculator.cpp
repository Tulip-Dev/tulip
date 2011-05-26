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
#include <tulip/GlCPULODCalculator.h>

#include <tulip/Matrix.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#include <tulip/Camera.h>
#include <tulip/GlEntity.h>
#include <tulip/GlTools.h>
#include <tulip/GlScene.h>

#include <iostream>

using namespace std;

namespace tlp {

  GlCPULODCalculator::GlCPULODCalculator():computeEdgesLOD(false){

  }

  GlCPULODCalculator::~GlCPULODCalculator() {
  }

  void GlCPULODCalculator::beginNewCamera(Camera* camera) {
    // add a new layerLODUnit add set camera pointer
    layersLODVector.push_back(LayerLODUnit());
    currentLayerLODUnit = &layersLODVector.back();
    currentLayerLODUnit->camera = camera;
  }

  void GlCPULODCalculator::addSimpleEntityBoundingBox(GlSimpleEntity * entity,const BoundingBox& bb) {
    assert(bb.isValid());

    // This code is here to prevent adding entities in percent
    //  If you look the Gl2DRect, you can see an option inPercent,
    //   if this entitie is inPercent we can't compute bounding box , so we create the bigest possible bounding box
    //   and here we don't add this false bounding box to the scene bounding box
    //   TODO : See if we can change the bounding box compute in Gl2DRect
    if(bb[0][0]!=numeric_limits<float>::min()){
      sceneBoundingBox.expand(bb[0]);
      sceneBoundingBox.expand(bb[1]);
    }

    // check if we have to render simple entities
    if((renderingEntitiesFlag & RenderingSimpleEntities) !=0)
      currentLayerLODUnit->simpleEntitiesLODVector.push_back(SimpleEntityLODUnit(entity,bb));
  }
  void GlCPULODCalculator::addNodeBoundingBox(unsigned int id,const BoundingBox& bb) {
    sceneBoundingBox.expand(bb[0]);
    sceneBoundingBox.expand(bb[1]);

    // check if we have to render nodes
    if((renderingEntitiesFlag & RenderingNodes) !=0)
      currentLayerLODUnit->nodesLODVector.push_back(ComplexEntityLODUnit(id,bb));
  }
  void GlCPULODCalculator::addEdgeBoundingBox(unsigned int id,const BoundingBox& bb) {
    sceneBoundingBox.expand(bb[0]);
    sceneBoundingBox.expand(bb[1]);

    // check if we have to render edges
    if((renderingEntitiesFlag & RenderingEdges) !=0)
      currentLayerLODUnit->edgesLODVector.push_back(ComplexEntityLODUnit(id,bb));
  }

  void GlCPULODCalculator::reserveMemoryForNodes(unsigned int numberOfNodes) {
    if((renderingEntitiesFlag & RenderingNodes) !=0)
      currentLayerLODUnit->nodesLODVector.reserve(numberOfNodes);
  }

  void GlCPULODCalculator::reserveMemoryForEdges(unsigned int numberOfEdges) {
    if((renderingEntitiesFlag & RenderingEdges) !=0)
      currentLayerLODUnit->edgesLODVector.reserve(numberOfEdges);
  }

  void GlCPULODCalculator::compute(const Vector<int,4>& globalViewport,const Vector<int,4>& currentViewport) {

    for(vector<LayerLODUnit>::iterator it=layersLODVector.begin();it!=layersLODVector.end();++it){
      Camera *camera=(Camera*)((*it).camera);

      Matrix<float,4> transformMatrix;
      camera->getTransformMatrix(globalViewport,transformMatrix);

      Coord eye;
      if(camera->is3D()) {
        eye=camera->getEyes() + ( camera->getEyes() - camera->getCenter() ) / (float)camera->getZoomFactor();
        computeFor3DCamera(&(*it),eye,transformMatrix,globalViewport,currentViewport);
      }else{
        computeFor2DCamera(&(*it),globalViewport,currentViewport);
      }

      glMatrixMode(GL_MODELVIEW);
    }
  }

  void GlCPULODCalculator::computeFor3DCamera(LayerLODUnit *layerLODUnit,
                                              const Coord &eye,
                                              const Matrix<float, 4> transformMatrix,
                                              const Vector<int,4>& globalViewport,
                                              const Vector<int,4>& currentViewport) {
    size_t nb=layerLODUnit->simpleEntitiesLODVector.size();

#ifdef _OPENMP
    omp_set_num_threads(std::min(omp_get_num_procs(), 4));
    omp_set_nested(true);
    omp_set_dynamic(false);
#endif

#ifdef _OPENMP
#pragma omp parallel for
#endif
    for(int i=0;i<static_cast<int>(nb);++i){
      layerLODUnit->simpleEntitiesLODVector[i].lod=calculateAABBSize(layerLODUnit->simpleEntitiesLODVector[i].boundingBox,eye,transformMatrix,globalViewport,currentViewport);
    }
    nb=layerLODUnit->nodesLODVector.size();
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for(int i=0;i<static_cast<int>(nb);++i){
			layerLODUnit->nodesLODVector[i].lod=calculateAABBSize(layerLODUnit->nodesLODVector[i].boundingBox,eye,transformMatrix,globalViewport,currentViewport);
    }
    nb=layerLODUnit->edgesLODVector.size();
    if(computeEdgesLOD){
#ifdef _OPENMP
#pragma omp parallel for
#endif
			for(int i=0;i<static_cast<int>(nb);++i){
				layerLODUnit->edgesLODVector[i].lod=calculateAABBSize(layerLODUnit->edgesLODVector[i].boundingBox,eye,transformMatrix,globalViewport,currentViewport);
			}
		}else{
#ifdef _OPENMP
#pragma omp parallel for
#endif
			for(int i=0;i<static_cast<int>(nb);++i){
				layerLODUnit->edgesLODVector[i].lod=10;
			}
		}
  }

  void GlCPULODCalculator::computeFor2DCamera(LayerLODUnit *layerLODUnit,
      const Vector<int,4>& globalViewport,
      const Vector<int,4>& currentViewport) {

    for(vector<SimpleEntityLODUnit>::iterator it=layerLODUnit->simpleEntitiesLODVector.begin();it!=layerLODUnit->simpleEntitiesLODVector.end();++it){
      (*it).lod=calculate2DLod((*it).boundingBox,globalViewport,currentViewport);
    }
    for(vector<ComplexEntityLODUnit>::iterator it=layerLODUnit->nodesLODVector.begin();it!=layerLODUnit->nodesLODVector.end();++it){
      (*it).lod=calculate2DLod((*it).boundingBox,globalViewport,currentViewport);
    }
    for(vector<ComplexEntityLODUnit>::iterator it=layerLODUnit->edgesLODVector.begin();it!=layerLODUnit->edgesLODVector.end();++it){
      (*it).lod=calculate2DLod((*it).boundingBox,globalViewport,currentViewport);
    }
  }

}

