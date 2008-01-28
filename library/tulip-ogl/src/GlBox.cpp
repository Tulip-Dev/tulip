#include "tulip/GlBox.h"

using namespace std;

namespace tlp {

  GlBox::GlBox()
  {
    // no default constructor :)
  }

  GlBox::GlBox(const Coord& position, const Size &size, const Color& color)
  {
    this->position = new Coord(position);
    this->color    = new Color(color);
    this->size     = new Size(size);

    for(int i=0; i < N_BOX_POINTS; i++)
      points[i] = NULL;

    for(int i=0; i < N_BOX_FACES; i++)
      faces[i] = NULL;

    /*renderOptions.setRenderState(GlAD_Wireframe, true);
      renderOptions.setRenderState(GlAD_Solid, true);*/

    boundingBox.check(position-size/2);
    boundingBox.check(position+size/2);

    computePoints();
  }

  GlBox::GlBox(Coord points[N_BOX_POINTS], const Color& color)
  {
    Coord average = Coord(0, 0, 0);

    for(int i=0; i < N_BOX_POINTS; i++){
      average += points[i];
      this->points[i] = new Coord(points[i]);
      boundingBox.check(points[i]);
    }

    average /= N_BOX_POINTS;

    this->position = new Coord(average);
    this->color    = new Color(color);

    // We can't calculate a size since it's not a parallelepiped
    this->size = NULL;

    /*renderOptions.setRenderState(GlAD_Wireframe, true);
      renderOptions.setRenderState(GlAD_Solid, true);*/

    for(int i=0; i < N_BOX_FACES; i++)
      faces[i] = NULL;

    computeFaces();
  }

  GlBox::GlBox(const Coord& frontTopLeft, const Coord& backBottomRight, const Color& color)
  {
    Coord middle  = (frontTopLeft + backBottomRight) / 2;
    Coord tmpsize = (frontTopLeft - backBottomRight) / 2;

    for(int i=0; i < 3; i++)
      tmpsize[i] = fabs(tmpsize[i]);

    this->position               = new Coord(middle);
    this->size                   = new Size(tmpsize.getX(), tmpsize.getY(), tmpsize.getZ());
    this->color                  = new Color(color);

    /*renderOptions.setRenderState(GlAD_Wireframe, true);
      renderOptions.setRenderState(GlAD_Solid, true);*/

    for(int i=0; i < N_BOX_POINTS; i++)
      points[i] = NULL;

    for(int i=0; i < N_BOX_FACES; i++)
      faces[i] = NULL;

    boundingBox.check(frontTopLeft);
    boundingBox.check(backBottomRight);

    computePoints();
  }

  GlBox::~GlBox()
  {
    for(int i=0; i < N_BOX_POINTS; i++)
      delete points[i];

    if (size != NULL)
      delete size;
  }

  void GlBox::draw(float lod,Camera *camera)
  {
    for(int i=0; i < N_BOX_FACES; i++)
      faces[i]->draw(lod,camera);   
  }

  void GlBox::setSize(const Size& size)
  {
    delete this->size;

    this->size = new Size(size);

    computePoints();
  }

  void GlBox::setPosition(const Coord& position)
  {
    delete this->position;

    boundingBox.check(position);

    this->position = new Coord(position);

    computePoints();
  }

  Size* GlBox::getSize() const
  {
    return size;
  }

  void GlBox::computePoints()
  {
    if (size == NULL)
      return;

    for(int i=0; i < N_BOX_POINTS; i++)
      if (points[i] != NULL)
	delete (points[i]);

    //       p4 +--------------+ p5
    //         /|             /|
    //        / |            / |
    //       /  |           /  |
    //      /   |          /   |
    //  p0 +--------------+ p1 |
    //     |    |         |    |
    //     |    |         |    |
    //     | p7 +---------|----+ p6
    //     |   /          |   /
    //     |  /           |  /
    //     | /            | /
    //     |/             |/
    //  p3 +--------------+ p2 

    points[0] = new Coord(position->getX() - size->getW(), position->getY() - size->getH(), position->getZ() - size->getD());
    points[1] = new Coord(position->getX() + size->getW(), position->getY() - size->getH(), position->getZ() - size->getD());
    points[2] = new Coord(position->getX() + size->getW(), position->getY() + size->getH(), position->getZ() - size->getD()); 
    points[3] = new Coord(position->getX() - size->getW(), position->getY() + size->getH(), position->getZ() - size->getD()); 
    points[4] = new Coord(position->getX() - size->getW(), position->getY() - size->getH(), position->getZ() + size->getD()); 
    points[5] = new Coord(position->getX() + size->getW(), position->getY() - size->getH(), position->getZ() + size->getD()); 
    points[6] = new Coord(position->getX() + size->getW(), position->getY() + size->getH(), position->getZ() + size->getD()); 
    points[7] = new Coord(position->getX() - size->getW(), position->getY() + size->getH(), position->getZ() + size->getD()); 

    computeFaces();
 
  }

  void GlBox::computeFaces()
  {
    boundingBox=BoundingBox(*points[0],*points[6]);

    for(int i=0; i < N_BOX_FACES; i++)
      if (faces[i] != NULL)
	delete faces[i];

    int indices[N_QUAD_POINTS * N_BOX_FACES] = {0, 1, 2, 3,  // Front
						5, 4, 7, 6,  // Back
						0, 3, 7, 4,  // Left
						1, 5, 6, 2,  // Right
						4, 5, 1, 0,  // Top
						3, 2, 6, 7}; // Bottom
			
    Coord Quad[N_QUAD_POINTS];

    // For each face
    for(int i=0; i < N_BOX_FACES; i++)
      {
	// We compute the 4 points of the considered face (using the index array)

	for(int j=0; j < N_QUAD_POINTS; j++)
	  Quad[j] = *points[indices[(i * N_QUAD_POINTS) + j]];

	faces[i] = new GlPolygon(true,false);
	for(int j=0;j<N_QUAD_POINTS;++j)
	  faces[i]->addPoint(Quad[j], *color, *color);
	//faces[i]->setRenderOptions(renderOptions);
      }
  }
  //===========================================================
  void GlBox::getXML(xmlNodePtr rootNode) {
    xmlNodePtr dataNode=NULL;

    xmlNewProp(rootNode,BAD_CAST "type",BAD_CAST "GlBox");
    
    GlXMLTools::getDataNode(rootNode,dataNode);

    GlXMLTools::getXML(dataNode,"position",*position);
    GlXMLTools::getXML(dataNode,"color",*color);
    GlXMLTools::getXML(dataNode,"size",*size);
    
  }
  //============================================================
  void GlBox::setWithXML(xmlNodePtr rootNode) {
    xmlNodePtr dataNode=NULL;

    GlXMLTools::getDataNode(rootNode,dataNode);

    // Parse Data
    if(dataNode) {
      Coord position;
      Color color;
      Size size;

      GlXMLTools::setWithXML(dataNode, "position", position);
      GlXMLTools::setWithXML(dataNode, "color", color);
      GlXMLTools::setWithXML(dataNode, "size", size);

      boundingBox.check(position-size/2);
      boundingBox.check(position+size/2);
      
      computePoints();
    }
  }
}
