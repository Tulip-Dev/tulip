#include "tulip/GlScene.h"

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <iostream>
#include <cstdlib>
#include <climits>

#include "tulip/GlLODSceneVisitor.h"
#include "tulip/TextRenderer.h"
#include "tulip/OcclusionTest.h"
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

  struct entityWithDistanceCompare {
    static GlGraphInputData *inputData;
    static bool compare(const EntityWithDistance &e1, const EntityWithDistance &e2 ){
      if(e1.distance>e2.distance)
        return true;
      if(e1.distance<e2.distance)
        return false;
      BoundingBox bb1,bb2;
      if(e1.complexEntity)
        bb1=((GlComplexeEntity*)(e1.entity->first))->getBoundingBox(inputData);
      else
        bb1=((GlSimpleEntity*)(e1.entity->first))->getBoundingBox();

      if(e2.complexEntity)
        bb2=((GlComplexeEntity*)(e2.entity->first))->getBoundingBox(inputData);
      else
        bb2=((GlSimpleEntity*)(e2.entity->first))->getBoundingBox();

      if(bb1.second[0]-bb1.first[0] > bb2.second[0]-bb2.first[0])
        return false;
      else
        return true;

    }
  };
  //====================================================
  #ifdef _WIN32
  #ifdef DLL_EXPORT
  GlGraphInputData *entityWithDistanceCompare::inputData=NULL;
  #endif
  #else
  GlGraphInputData *entityWithDistanceCompare::inputData=NULL;
  #endif

  GlScene::GlScene(GlLODCalculator *calculator):backgroundColor(255, 255, 255, 255),viewLabel(true),viewOrtho(true),glGraphComposite(NULL) {
    Camera camera(this,false);
    selectionLayer= new GlLayer("Selection");
    selectionLayer->setCamera(camera);
    selectionLayer->setScene(this);
    if(calculator!=NULL)
      lodCalculator=calculator;
    else
      lodCalculator=new GlCPULODCalculator();
  }

  void GlScene::initGlParameters() {
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
    bool antialiased = true;
    if(glGraphComposite) {
      antialiased=glGraphComposite->getInputData()->parameters->isAntialiased();
    }

    if(antialiased) {
      glEnable(GL_LINE_SMOOTH);
      glEnable(GL_POLYGON_SMOOTH);
      glHint(GL_LINE_SMOOTH_HINT,GL_DONT_CARE);
      glHint(GL_POLYGON_SMOOTH_HINT,GL_DONT_CARE);
      glShadeModel(GL_SMOOTH);
    }else{
      glDisable(GL_LINE_SMOOTH);
      glDisable(GL_POLYGON_SMOOTH);
    }

    glDisable(GL_POINT_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(1.0);
    glPointSize(1.0);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_NORMALIZE);
    glDepthFunc(GL_LEQUAL );
    glPolygonMode(GL_FRONT, GL_FILL);
    glColorMask(1, 1, 1, 1);
    glEnable(GL_BLEND);
    glIndexMask(UINT_MAX);
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
    initGlParameters();

    GlLODSceneVisitor lodVisitor(lodCalculator,glGraphComposite->getInputData());

    //cout << ">>> Check bounding box" << endl ;


    for(vector<pair<string,GlLayer *> >::iterator it=layersList.begin();it!=layersList.end();++it) {
      (*it).second->acceptVisitor(&lodVisitor);
      if((*it).first=="Main")
	selectionLayer->acceptVisitor(&lodVisitor);
    }

    //cout << "<<< End Check bounding box" << endl;
    //cout << ">>> Begin LOD compute" << endl;
    lodCalculator->compute(viewport,viewport);
    //cout << "<<< End LOD compute" << endl;
    //lodCalculator->compute();

    TextRenderer fontRenderer;
    OcclusionTest occlusionTest;

    LODResultVector* ceVector=lodCalculator->getResultForComplexeEntities();
    LODResultVector* seVector=lodCalculator->getResultForSimpleEntities();
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
      camera=(*it).second->getCamera();
      camera->initGl();

      bool zOrdering=false;
      if(glGraphComposite)
        zOrdering=glGraphComposite->getRenderingParameters().isElementZOrdered();

      if(!zOrdering){
        if((Camera*)((*itSE).first)==camera) {
          for(vector<LODResultEntity>::iterator itE=(*itSE).second.begin();itE!=(*itSE).second.end();++itE) {
	  if((*itE).second>=0) {
              glStencilFunc(GL_LEQUAL,((GlSimpleEntity*)((*itE).first))->getStencil(),0xFFFF);
              ((GlSimpleEntity*)((*itE).first))->draw((*itE).second,camera);
            }
          }
          ++itSE;
        }

        if((Camera*)((*itCE).first)==camera) {
          for(vector<LODResultEntity>::iterator itE=(*itCE).second.begin();itE!=(*itCE).second.end();++itE) {
	  if((*itE).second>=0) {
              ((GlComplexeEntity*)((*itE).first))->draw((*itE).second,glGraphComposite->getInputData(),camera);
            }
          }
        }
      }else{
        entityWithDistanceCompare::inputData=glGraphComposite->getInputData();
        vector<EntityWithDistance> entitiesVector;
        Coord camPos=camera->getEyes();
        BoundingBox bb;
        Coord middle;
        double dist;

        if((Camera*)((*itSE).first)==camera) {
          for(vector<LODResultEntity>::iterator itE=(*itSE).second.begin();itE!=(*itSE).second.end();++itE) {
            if((*itE).second>=0) {
              bb=((GlSimpleEntity*)((*itE).first))->getBoundingBox();
              middle=bb.first+(bb.second-bb.first)/2;
              dist=(((double)middle[0])-((double)camPos[0]))*(((double)middle[0])-((double)camPos[0]));
              dist+=(((double)middle[1])-((double)camPos[1]))*(((double)middle[1])-((double)camPos[1]));
              dist+=(((double)middle[2])-((double)camPos[2]))*(((double)middle[2])-((double)camPos[2]));
              entitiesVector.push_back(EntityWithDistance(dist,&(*itE),false));
            }
          }
        }
        if((Camera*)((*itCE).first)==camera) {
          for(vector<LODResultEntity>::iterator itE=(*itCE).second.begin();itE!=(*itCE).second.end();++itE) {
            if((*itE).second>=0) {
              bb=((GlComplexeEntity*)((*itE).first))->getBoundingBox(glGraphComposite->getInputData());
              middle=bb.first+(bb.second-bb.first)/2;
              dist=(((double)middle[0])-((double)camPos[0]))*(((double)middle[0])-((double)camPos[0]));
              dist+=(((double)middle[1])-((double)camPos[1]))*(((double)middle[1])-((double)camPos[1]));
              dist+=(((double)middle[2])-((double)camPos[2]))*(((double)middle[2])-((double)camPos[2]));
              entitiesVector.push_back(EntityWithDistance(dist,&(*itE),true));
            }
          }
        }

        sort(entitiesVector.begin(),entitiesVector.end(),entityWithDistanceCompare::compare);

        for(vector<EntityWithDistance>::iterator it=entitiesVector.begin();it!=entitiesVector.end();++it){
          if((*it).complexEntity){
            ((GlComplexeEntity*)((*it).entity->first))->draw((*it).entity->second,glGraphComposite->getInputData(),camera);
          }else{
            glStencilFunc(GL_LEQUAL,((GlSimpleEntity*)((*it).entity->first))->getStencil(),0xFFFF);
            ((GlSimpleEntity*)((*it).entity->first))->draw((*it).entity->second,camera);
          }
        }
      }

      if((Camera*)((*itCE).first)==camera) {
        // Draw Label
        if(viewLabel) {
          glPushAttrib(GL_ALL_ATTRIB_BITS);
          glDisable(GL_LIGHTING);
          glDepthFunc(GL_ALWAYS );
          glDisable(GL_CULL_FACE);
          glDisable(GL_COLOR_MATERIAL);

          // Draw Nodes Label
          if(glGraphComposite->getInputData()->parameters->isViewNodeLabel()) {
            // Draw Label for selected Nodes
            for(vector<LODResultEntity>::iterator itE=(*itCE).second.begin();itE!=(*itCE).second.end();++itE) {
	      if((*itE).second>=0) {
                ((GlComplexeEntity*)((*itE).first))->drawLabel(true,
                    true,
                    false,
                    &occlusionTest,
                    &fontRenderer,glGraphComposite->getInputData());
              }
            }
            // Draw Label for others Nodes
            for(vector<LODResultEntity>::iterator itE=(*itCE).second.begin();itE!=(*itCE).second.end();++itE) {
	      if((*itE).second>=0) {
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
	      if((*itE).second>=0) {
                ((GlComplexeEntity*)((*itE).first))->drawLabel(true,
                    false,
                    true,
                    &occlusionTest,
                    &fontRenderer,glGraphComposite->getInputData());
              }
            }
            // Draw Label for others Edges
            for(vector<LODResultEntity>::iterator itE=(*itCE).second.begin();itE!=(*itCE).second.end();++itE) {
	      if((*itE).second>=0) {
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
	      if((*itE).second>=0) {
                ((GlSimpleEntity*)((*itE).first))->draw((*itE).second,camera);
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
    lodCalculator->clear();
  }

  void GlScene::addLayer(GlLayer *layer) {
    layersList.push_back(std::pair<std::string,GlLayer*>(layer->getName(),layer));
    layer->setScene(this);
    notifyAddLayer(this,layer->getName(),layer);
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
    GlSelectSceneVisitor visitor(type,glGraphComposite->getInputData(),lodCalculator);

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
    selectionViewport[2]=w;
    selectionViewport[3]=h;

    glViewport(selectionViewport[0],selectionViewport[1],selectionViewport[2],selectionViewport[3]);

    lodCalculator->compute(viewport,selectionViewport);
    //lodCalculator->compute();

    LODResultVector* lodResultVector;

    if(type==SelectSimpleEntities) {
      lodResultVector=lodCalculator->getResultForSimpleEntities();
    }else{
      lodResultVector=lodCalculator->getResultForComplexeEntities();
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

      map<unsigned int, unsigned long> idToEntity;
      unsigned int id=1;

      for(vector<LODResultEntity>::iterator itE=(*it).second.begin();itE!=(*it).second.end();++itE) {
        idToEntity[id]=(*itE).first;
        glLoadName(id);
        id++;
	if((*itE).second>=0) {
          if(type==SelectSimpleEntities) {
            ((GlSimpleEntity*)((*itE).first))->draw(20.,camera);
          }else{
            ((GlComplexeEntity*)((*itE).first))->draw(20.,glGraphComposite->getInputData(),camera);
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
        selectedEntities.push_back((GlEntity*)(idToEntity[selectBuf[hits-1][3]]));
        hits--;
      }

      glPopClientAttrib();
      glPopAttrib();

      delete[] selectBuf;
    }

    lodCalculator->clear();
    glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);
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
    glGraphComposite->getInputData()->parameters->setFeedbackRender(true);
    draw();
    glGraphComposite->getInputData()->parameters->setFeedbackRender(false);
    glGetFloatv(GL_COLOR_CLEAR_VALUE, clearColor);
    glGetFloatv(GL_LINE_WIDTH, &lineWidth);
    glGetFloatv(GL_POINT_SIZE, &pointSize);

    glFlush();
    glFinish();
    returned = glRenderMode(GL_RENDER);
    GlSVGFeedBackBuilder builder;
    GlFeedBackRecorder recorder(&builder);
    builder.begin(viewport,clearColor,pointSize,lineWidth);
    recorder.record(true,returned,buffer,layersList[0].second->getCamera()->getViewport());
    string str;
    builder.getResult(&str);
    if(!filename.empty()) {
      /* subgraphs drawing disabled
	 initMapsSVG(_renderingParameters.getGraph(), &ge); */
      FILE* file = fopen(filename.c_str(), "w");
      if (file) {
	fprintf(file, str.c_str());
	fclose(file);
      } else {
	perror(filename.c_str());
      }
    }
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
    glGraphComposite->getInputData()->parameters->setFeedbackRender(true);
    draw();
    glGraphComposite->getInputData()->parameters->setFeedbackRender(false);

    glGetFloatv(GL_COLOR_CLEAR_VALUE, clearColor);
    glGetFloatv(GL_LINE_WIDTH, &lineWidth);
    glGetFloatv(GL_POINT_SIZE, &pointSize);

    glFlush();
    glFinish();
    returned = glRenderMode(GL_RENDER);
    //cout << returned << endl;
    GlEPSFeedBackBuilder builder;
    GlFeedBackRecorder recorder(&builder);
    builder.begin(viewport,clearColor,pointSize,lineWidth);
    recorder.record(true,returned,buffer,layersList[0].second->getCamera()->getViewport());
    string str;
    builder.getResult(&str);
    if(!filename.empty()) {
      FILE* file = fopen(filename.c_str(), "w");
      if (file) {
	fprintf(file, str.c_str());
	fclose(file);
      } else {
	perror(filename.c_str());
      }
    }
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
  //====================================================
  void GlScene::getXML(string &out) {
    xmlDocPtr doc = NULL;       /* document pointer */
    xmlNodePtr node = NULL;
    xmlNodePtr rootNode = NULL;
    xmlNodePtr dataNode= NULL;
    xmlNodePtr childrenNode = NULL;/* node pointers */

    doc = xmlNewDoc(BAD_CAST "1.0");
    rootNode = xmlNewNode(NULL, BAD_CAST "scene");
    xmlDocSetRootElement(doc, rootNode);

    GlXMLTools::createDataAndChildrenNodes(rootNode, dataNode, childrenNode);

    GlXMLTools::getXML(dataNode,"viewport",viewport);
    GlXMLTools::getXML(dataNode,"background",backgroundColor);

    for(vector<pair<string, GlLayer *> >::iterator it=layersList.begin();it!=layersList.end();++it) {
      GlXMLTools::createChild(childrenNode, "GlLayer", node);
      GlXMLTools::createProperty(node, "name", (*it).first);
      (*it).second->getXML(node);
    }

    /*
     * Dumping document to stdio or file
     */
    xmlChar *xmlbuff;
    int buffersize;

    xmlDocDumpFormatMemory(doc, &xmlbuff, &buffersize, 1);
    out.append((char *)xmlbuff);

    int lastPos=0;
    int current=out.find("\"",lastPos);
    while(current!=-1){
      out.replace(current,1,"\\\"");
      lastPos=current+2;
      current=out.find("\"",lastPos);
    }

    xmlFree(xmlbuff);

    /*free the document */
    xmlFreeDoc(doc);

    /*
     *Free the global variables that may
     *have been allocated by the parser.
     */
    xmlCleanupParser();

    /*
     * this is to debug memory for regression tests
     */
    xmlMemoryDump();
  }
  //====================================================
  void GlScene::setWithXML(string &in, Graph *graph) {
    xmlDocPtr doc;
    xmlNodePtr rootNode = NULL;
    xmlNodePtr dataNode= NULL;
    xmlNodePtr childrenNode = NULL;
    xmlNodePtr node = NULL;/* node pointers */

    glGraphComposite=new GlGraphComposite(graph);

    doc = xmlReadMemory(&in[0], in.length(), "noname.xml", NULL, 0);

    rootNode = xmlDocGetRootElement(doc);

    string name;

    name=((char*)rootNode->name);
    if (rootNode->type == XML_ELEMENT_NODE && name=="scene") {
      GlXMLTools::getDataAndChildrenNodes(rootNode,dataNode,childrenNode);
    }else{
      assert(false);
    }

    // Parse data
    if(dataNode) {
      GlXMLTools::setWithXML(dataNode,"viewport",viewport);
      GlXMLTools::setWithXML(dataNode,"background",backgroundColor);
    }

    // Parse children
    for (node = childrenNode->children; node; node = node->next) {
      if(node->type == XML_ELEMENT_NODE) {
	if(string((char*)node->name)=="GlLayer") {
	  //New version
	  string propName=(char*)node->properties->name;
	  string propValue=(char*)node->properties->children->content;
	  if(propName=="name") {
	    GlLayer *newLayer=new GlLayer(propValue);
	    addLayer(newLayer);
	    newLayer->setWithXML(node);
	  }else{
	    assert(false);
	  }
	}else{
	  //Previous version compatibility
	  string propName=(char*)node->properties->name;
	  string propValue=(char*)node->properties->children->content;
	  if(propName=="type" && propValue=="GlLayer") {
	    GlLayer *newLayer=new GlLayer((char*)node->name);
	    addLayer(newLayer);
	    newLayer->setWithXML(node);
	  }else{
	    assert(false);
	  }
	}
      }
    }

    getLayer("Main")->addGlEntity(glGraphComposite,"graph");
    addGlGraphCompositeInfo(getLayer("Main"),glGraphComposite);
  }

}
