#include "tulip/GlScene.h"

#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>
#include <stdio.h>

#include "tulip/GlLODSceneVisitor.h"
#include "tulip/TextRenderer.h"
#include "tulip/OcclusionTest.h"
#include "tulip/GlDrawSceneVisitor.h"
#include "tulip/GlCPULODCalculator.h"
#include "tulip/GlBoundingBoxSceneVisitor.h"
#include "tulip/GlSelectSceneVisitor.h"
#include "tulip/Camera.h"
#include "tulip/GlSimpleEntity.h"
#include "tulip/GlComplexeEntity.h"
#include "tulip/GlNode.h"
#include "tulip/GlEdge.h"
#include "tulip/GlFeedBackRecorder.h"
#include "tulip/GlSVGFeedBackBuilder.h"
#include "tulip/GlEPSFeedBackBuilder.h"

using namespace std;

namespace tlp {
  
  GlScene::GlScene():backgroundColor(255, 255, 255, 255),viewLabel(true),viewOrtho(true),glGraphComposite(NULL) {
    Camera camera(this,false);
    selectionLayer= new GlLayer();
    selectionLayer->setCamera(camera);
    selectionLayer->setScene(this);
  }

  void GlScene::initGlParametters() {
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
    glEnable(GL_LINE_SMOOTH);
    glDisable(GL_POINT_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT,GL_DONT_CARE);
    glLineWidth(1.0);
    glPointSize(1.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_NORMALIZE);
    glDepthFunc(GL_LEQUAL );
    glPolygonMode(GL_FRONT, GL_FILL);
    glColorMask(1, 1, 1, 1);
    glEnable(GL_BLEND);
    glIndexMask(~0);
    glClearColor(backgroundColor.getRGL(), backgroundColor.getGGL(), backgroundColor.getBGL(), 1.0);
    glClearStencil(0xFFFF);
    glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glDisable(GL_TEXTURE_2D);
    
    GLenum error = glGetError();
    if ( error != GL_NO_ERROR)
      cerr << "[OpenGL Error] => " << gluErrorString(error) << endl << "\tin : " << __PRETTY_FUNCTION__ << endl;
  }

  void GlScene::draw() {
    initGlParametters();
    
    GlCPULODCalculator calculator;
    GlLODSceneVisitor lodVisitor(&calculator,glGraphComposite->getInputData());

    //cout << ">>> Check bounding box" << endl ;
    

    for(vector<pair<string,GlLayer *> >::iterator it=layersList.begin();it!=layersList.end();++it) {
      (*it).second->acceptVisitor(&lodVisitor);
      if((*it).first=="Main")
	selectionLayer->acceptVisitor(&lodVisitor);
    }
    
    //cout << "<<< End Check bounding box" << endl;
    //cout << ">>> Begin LOD compute" << endl;
    calculator.compute(viewport);
    //cout << "<<< End LOD compute" << endl;
    //calculator.compute();

    TextRenderer fontRenderer;
    OcclusionTest occlusionTest;

    LODResultVector* ceVector=calculator.getResultForComplexeEntities();
    LODResultVector* seVector=calculator.getResultForSimpleEntities();
    LODResultVector::iterator itCE=ceVector->begin();
    LODResultVector::iterator itSE=seVector->begin();
    

    //cout << ">>> Begin draw" << endl;
    /*Camera *camera=selectionLayer->getCamera();
    if((Camera*)((*itSE).first)==camera) {
      camera->initGl();
      for(vector<LODResultEntity>::iterator itE=(*itSE).second.begin();itE!=(*itSE).second.end();++itE) {
	if((*itE).second>0) {
	  ((GlSimpleEntity*)((*itE).first))->draw((*itE).second);
	}
      }
      ++itSE;
      ++itCE;
      }*/
    Camera *camera;

    for(vector<pair<string,GlLayer *> >::iterator it=layersList.begin();it!=layersList.end();++it) {
   
      //cout << ">>> Draw SE" << endl;
      //selection layout Draw

      camera=(*it).second->getCamera();
      camera->initGl();
      
      if((Camera*)((*itSE).first)==camera) {
	for(vector<LODResultEntity>::iterator itE=(*itSE).second.begin();itE!=(*itSE).second.end();++itE) {
	  if((*itE).second>0) {
	    glStencilFunc(GL_LEQUAL,((GlSimpleEntity*)((*itE).first))->getStencil(),0xFFFF);
	    ((GlSimpleEntity*)((*itE).first))->draw((*itE).second);
	  }
	}
	++itSE;
      }
      //cout << "<<< End draw SE" << endl;
      //cout << ">>> Draw CE" << endl;
      if((Camera*)((*itCE).first)==camera) {
	// Draw Nodes and Edges
	for(vector<LODResultEntity>::iterator itE=(*itCE).second.begin();itE!=(*itCE).second.end();++itE) {
	  if((*itE).second>0) {
	    ((GlComplexeEntity*)((*itE).first))->draw((*itE).second,glGraphComposite->getInputData(),camera);
	  }
	}

	// Draw Label
	if(viewLabel) {
	  glPushAttrib(GL_ALL_ATTRIB_BITS);
	  glDisable(GL_LIGHTING);
	  glDepthFunc(GL_ALWAYS );
	  glDisable(GL_CULL_FACE);
	  glDisable(GL_COLOR_MATERIAL);

	  // Draw Nodes Label
	  if(glGraphComposite->getInputData()->parameters->isViewNodeLabel()) {
	    glStencilFunc(GL_LEQUAL,glGraphComposite->getNodesLabelStencil(),0xFFFF);
	    // Draw Label for selected Nodes
	    for(vector<LODResultEntity>::iterator itE=(*itCE).second.begin();itE!=(*itCE).second.end();++itE) {
	      if((*itE).second>0) {
		((GlComplexeEntity*)((*itE).first))->drawLabel(true,
							       true,
							       false,
							       &occlusionTest,
							       &fontRenderer,glGraphComposite->getInputData());
	      }
	    }
	    // Draw Label for others Nodes
	    for(vector<LODResultEntity>::iterator itE=(*itCE).second.begin();itE!=(*itCE).second.end();++itE) {
	      if((*itE).second>0) {
		((GlComplexeEntity*)((*itE).first))->drawLabel(false,
							       true,
							       false,
							       &occlusionTest,
							       &fontRenderer,glGraphComposite->getInputData());
	      }
	    }
	  }

	  // Draw Edges Label
	  if(glGraphComposite->getInputData()->parameters->isViewEdgeLabel()) {
	    glStencilFunc(GL_LEQUAL,glGraphComposite->getEdgesLabelStencil(),0xFFFF);
	    // Draw Label for selected Edges
	    for(vector<LODResultEntity>::iterator itE=(*itCE).second.begin();itE!=(*itCE).second.end();++itE) {
	      if((*itE).second>0) {
		((GlComplexeEntity*)((*itE).first))->drawLabel(true,
							       false,
							       true,
							       &occlusionTest,
							       &fontRenderer,glGraphComposite->getInputData());
	      }
	    }
	    // Draw Label for others Edges
	    for(vector<LODResultEntity>::iterator itE=(*itCE).second.begin();itE!=(*itCE).second.end();++itE) {
	      if((*itE).second>0) {
		((GlComplexeEntity*)((*itE).first))->drawLabel(false,
							       false,
							       true,
							       &occlusionTest,
							       &fontRenderer,glGraphComposite->getInputData());
	      }
	    }
	  }

	  glPopAttrib();
	}

	++itCE;
      }

      if((*it).first=="Main") {
	if((*it).second->isVisible()) {
	  camera=selectionLayer->getCamera();
	  if((Camera*)((*itSE).first)==camera) {
	    camera->initGl();
	    for(vector<LODResultEntity>::iterator itE=(*itSE).second.begin();itE!=(*itSE).second.end();++itE) {
	      if((*itE).second>0) {
		((GlSimpleEntity*)((*itE).first))->draw((*itE).second);
	      }
	    }
	  }
	}
	++itSE;
	++itCE;
      }
      //cout << "<<< End draw CE" << endl;
    }
    //cout << "<<< End draw" << endl;
  }

  void GlScene::addLayer(const std::string &name,GlLayer *layer) {
    layersList.push_back(std::pair<std::string,GlLayer*>(name,layer));
    layer->setScene(this);
    //notifyAddLayer(this,name,layer);
  }

  void GlScene::centerScene() {
    GlBoundingBoxSceneVisitor visitor(glGraphComposite->getInputData());

    for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin();it!=layersList.end();++it) {
      if((*it).second->getCamera()->is3D())
	(*it).second->acceptVisitor(&visitor);
    }

    Coord maxC = visitor.getBoundingBox().second;
    Coord minC = visitor.getBoundingBox().first;
    
    double dx = maxC[0] - minC[0];
    double dy = maxC[1] - minC[1];
    double dz = maxC[2] - minC[2];

    for(vector<pair<string,GlLayer *> >::iterator it=layersList.begin();it!=layersList.end();++it) {
      Camera* camera=(*it).second->getCamera();
      camera->setCenter((maxC + minC) / 2.0);

      if ((dx==0) && (dy==0) && (dz==0))
	dx = dy = dz = 10.0;
  
      camera->setSceneRadius(sqrt(dx*dx+dy*dy+dz*dz)/2.0); //radius of the sphere hull of the layer bounding box

      camera->setEyes(Coord(0, 0, camera->getSceneRadius()));
      camera->setEyes(camera->getEyes() + camera->getCenter());
      camera->setUp(Coord(0, 1., 0));
      camera->setZoomFactor(0.5);
    }
  } 

  void GlScene::zoomXY(int step, const int x, const int y) {
    
    for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin();it!=layersList.end();++it) {
      if((*it).second->getCamera()->is3D())
	(*it).second->getCamera()->setZoomFactor((*it).second->getCamera()->getZoomFactor() * pow(1.1,step));
    }
    if (step < 0) step *= -1;
    int factX = (int)(step*(double(viewport[2])/2.0-x)/ 7.0);
    int factY = (int)(step*(double(viewport[3])/2.0-y)/ 7.0);
    translateCamera(factX,-factY,0);
  }

  void GlScene::zoom(float factor,const Coord& dest) {
    for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin();it!=layersList.end();++it) {
      if((*it).second->getCamera()->is3D()) {
	(*it).second->getCamera()->setEyes(dest + ((*it).second->getCamera()->getEyes() - (*it).second->getCamera()->getCenter()));
	(*it).second->getCamera()->setCenter(dest);
      }
    }
  }

  void GlScene::translateCamera(const int x, const int y, const int z) {
    for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin();it!=layersList.end();++it) {
      if((*it).second->getCamera()->is3D()) {
	Coord v1(0, 0, 0);
	Coord v2(x, y, z);
	v1 = (*it).second->getCamera()->screenTo3DWorld(v1);
	v2 = (*it).second->getCamera()->screenTo3DWorld(v2);
	Coord move = v2 - v1;
	(*it).second->getCamera()->setEyes((*it).second->getCamera()->getEyes() + move);
	(*it).second->getCamera()->setCenter((*it).second->getCamera()->getCenter() + move);
      }
    }
  }

  void GlScene::zoom(int step) {

    for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin();it!=layersList.end();++it) {
      if((*it).second->getCamera()->is3D()) {
	(*it).second->getCamera()->setZoomFactor((*it).second->getCamera()->getZoomFactor() * pow(1.1, step));
      }
    }
  }
  
  void GlScene::rotateScene(const int x, const int y, const int z) {
    for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin();it!=layersList.end();++it) {
      if((*it).second->getCamera()->is3D()) {
	(*it).second->getCamera()->rotate(float(x)/360.0 * M_PI, 1.0, 0, 0);
	(*it).second->getCamera()->rotate(float(y)/360.0 * M_PI, 0, 1.0, 0);
	(*it).second->getCamera()->rotate(float(z)/360.0 * M_PI, 0, 0, 1.0);
      }
    }
  }

  bool GlScene::selectEntities(SelectionFlag type,int x, int y, int w, int h, GlLayer* layer, vector<GlEntity*>& selectedEntities) {
    if(w==0)
      w=1;
    if(h==0)
      h=1;
    GlCPULODCalculator calculator;
    GlSelectSceneVisitor visitor(type,glGraphComposite->getInputData(),&calculator);

    if(layer) {
      layer->acceptVisitor(&visitor);
    }else {
      if(type==SelectSimpleEntities)
	selectionLayer->acceptVisitor(&visitor);
      
      for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin();it!=layersList.end();++it) {
	(*it).second->acceptVisitor(&visitor);
      }
    }

    Vector<int,4> selectionViewport;
    selectionViewport[0]=x;
    selectionViewport[1]=y;
    selectionViewport[2]=w+50;
    selectionViewport[3]=h+50;

    glViewport(selectionViewport[0],selectionViewport[1],selectionViewport[2],selectionViewport[3]);

    calculator.compute(selectionViewport);
    //calculator.compute();

    LODResultVector* lodResultVector;

    if(type==SelectSimpleEntities) {
      lodResultVector=calculator.getResultForSimpleEntities();
    }else{
      lodResultVector=calculator.getResultForComplexeEntities();
    }


    for(LODResultVector::iterator it=lodResultVector->begin();it!=lodResultVector->end();++it) {
      if((*it).second.size()==0)
	continue;

      Camera *camera=(Camera*)((*it).first);

      Vector<int, 4> viewport = camera->getViewport();

      glPushAttrib(GL_ALL_ATTRIB_BITS); //save previous attributes
      glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS); //save previous attributes

      //Allocate memory to store the result oh the selection
      GLuint (*selectBuf)[4] = new GLuint[(*it).second.size()][4];
      glSelectBuffer((*it).second.size()*4 , (GLuint *)selectBuf);
      //Activate Open Gl Selection mode
      glRenderMode(GL_SELECT);
      glInitNames();
      glPushName(0);
      
      glMatrixMode(GL_PROJECTION);
      glPushMatrix(); //save previous projection matrix
      
      //initialize picking matrix
      glLoadIdentity();
      x += w/2;
      y =  viewport[3] - (y + h/2);
      gluPickMatrix(x, y, w, h, (GLint *)&viewport);
 

      camera->initProjection(false);

      glMatrixMode(GL_MODELVIEW);
      glPushMatrix(); //save previous model view matrix
      
      camera->initModelView();
      
      glPolygonMode(GL_FRONT, GL_FILL);
      glDisable(GL_LIGHTING);
      glDisable(GL_BLEND);
      glDisable(GL_STENCIL_TEST);

      for(vector<LODResultEntity>::iterator itE=(*it).second.begin();itE!=(*it).second.end();++itE) {
	if((*itE).second>0) {
	  glLoadName((unsigned int)((*itE).first));
	  if(type==SelectSimpleEntities) {
	    ((GlSimpleEntity*)((*itE).first))->draw((*itE).second);
	  }else{
	    ((GlComplexeEntity*)((*itE).first))->draw((*itE).second,glGraphComposite->getInputData(),camera);
	  }
	}
      }
      
      glPopMatrix();
      
      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      //glLoadIdentity();

      glFlush();
      GLint hits = glRenderMode(GL_RENDER);

      while(hits>0) {
	selectedEntities.push_back((GlEntity*)(selectBuf[hits-1][3]));
	hits--;
      }

      glPopClientAttrib();
      glPopAttrib();

      delete[] selectBuf;
    }
    
    
    return (selectedEntities.size()!=0);
  }
  //====================================================
  void GlScene::outputSVG(unsigned int size,const string& filename) {
    GLint returned;
    GLfloat clearColor[4];
    GLfloat lineWidth;
    GLfloat pointSize;
    GLfloat* buffer = (GLfloat *)calloc(size, sizeof(GLfloat));
    glFeedbackBuffer(size, GL_3D_COLOR, buffer);
    glRenderMode(GL_FEEDBACK);
    draw();

    glGetFloatv(GL_COLOR_CLEAR_VALUE, clearColor);
    glGetFloatv(GL_LINE_WIDTH, &lineWidth);
    glGetFloatv(GL_POINT_SIZE, &pointSize);

    glFlush();
    glFinish();
    returned = glRenderMode(GL_RENDER);
    cout << returned << endl;
    GlSVGFeedBackBuilder builder;
    GlFeedBackRecorder recorder(&builder);
    builder.begin(viewport,clearColor,pointSize,lineWidth);
    recorder.record(false,returned,buffer,layersList[0].second->getCamera()->getViewport());
    string str;
    builder.getResult(&str);
    if(!filename.empty()) {
      /* subgraphs drawing disabled
	 initMapsSVG(_renderingParameters.getGraph(), &ge); */
      //File* file = fopen(filename, "w");
      /*if (file) {
	fprintf(file, str.c_str());
	fclose(file);
      } else
	perror(filename);
	} */
    }
    cout << str << endl;
  }
  //====================================================
  void GlScene::outputEPS(unsigned int size,const string& filename) {
    GLint returned;
    GLfloat clearColor[4];
    GLfloat lineWidth;
    GLfloat pointSize;
    GLfloat* buffer = (GLfloat *)calloc(size, sizeof(GLfloat));
    glFeedbackBuffer(size, GL_3D_COLOR, buffer);
    glRenderMode(GL_FEEDBACK);
    draw();

    glGetFloatv(GL_COLOR_CLEAR_VALUE, clearColor);
    glGetFloatv(GL_LINE_WIDTH, &lineWidth);
    glGetFloatv(GL_POINT_SIZE, &pointSize);

    glFlush();
    glFinish();
    returned = glRenderMode(GL_RENDER);
    cout << returned << endl;
    GlEPSFeedBackBuilder builder;
    GlFeedBackRecorder recorder(&builder);
    builder.begin(viewport,clearColor,pointSize,lineWidth);
    recorder.record(false,returned,buffer,layersList[0].second->getCamera()->getViewport());
    string str;
    builder.getResult(&str);
    if(!filename.empty()) {
      /* subgraphs drawing disabled
	 initMapsSVG(_renderingParameters.getGraph(), &ge); */
      //File* file = fopen(filename, "w");
      /*if (file) {
	fprintf(file, str.c_str());
	fclose(file);
      } else
	perror(filename);
	} */
    }
    cout << str << endl;
  }
  //====================================================
  unsigned char * GlScene::getImage() {
    unsigned char *image = (unsigned char *)malloc(viewport[2]*viewport[3]*3*sizeof(unsigned char));
    draw();
    glFlush();
    glFinish();
    glPixelStorei(GL_PACK_ALIGNMENT,1);
    glReadPixels(viewport[0],viewport[1],viewport[2],viewport[3],GL_RGB,GL_UNSIGNED_BYTE,image);
    return image;
  }
  //====================================================
  GlLayer* GlScene::getLayer(const std::string& name) {
    for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin();it!=layersList.end();++it) {
      if((*it).first==name)
	return (*it).second;
    }
    return NULL;
  }

}
