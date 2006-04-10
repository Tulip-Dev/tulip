//-*-c++-*-
#include <tulip/TulipPlugin.h>
#include <cmath>

/** \addtogroup size */
/*@{*/
/// AutoSizes.cpp - Compute size in order to prevent node-node overlapping
/**
 * This plug-ins compute size of nodes and edges such that, node-node overlapping do not exists (if it is possible).
 * and sizes of edges are proportional to size of nodes.
 *
 *  \author David Auber University Bordeaux I France: Email:auber@tulip-software.org
 */
class AutoSizes:public Sizes
{ 
public:
  AutoSizes(const PropertyContext &context):Sizes(context){}
  ~AutoSizes(){}
  Size getNodeValue(const node n) {
    LayoutProxy *entryLayout=superGraph->getProperty<LayoutProxy>("viewLayout");

    //Compute the minimal distance to one neighbour.
    Iterator<node> *itN=superGraph->getNodes();
    Coord tmp1=entryLayout->getNodeValue(n);
    double dist=1000;
    if (itN->hasNext()) {
      node itn=itN->next();
      while ((itn==n) && itN->hasNext()) itn=itN->next();
      if (itn!=n) {
	Coord tmp2=entryLayout->getNodeValue(itn);
	dist=sqrt((tmp1.getX()-tmp2.getX())*(tmp1.getX()-tmp2.getX())
		  +(tmp1.getY()-tmp2.getY())*(tmp1.getY()-tmp2.getY())
		  +(tmp1.getZ()-tmp2.getZ())*(tmp1.getZ()-tmp2.getZ())
		  );
      }
      else {
	dist=10;
      }
    }
    for (;itN->hasNext();) {
      node itn=itN->next();
      if (itn!=n) {
	Coord tmp2=entryLayout->getNodeValue(itn);
	double tmpDist=sqrt( (tmp1.getX()-tmp2.getX())*(tmp1.getX()-tmp2.getX())
			     +(tmp1.getY()-tmp2.getY())*(tmp1.getY()-tmp2.getY())
			     +(tmp1.getZ()-tmp2.getZ())*(tmp1.getZ()-tmp2.getZ())
			     );
	if (tmpDist<dist) dist=tmpDist;
      }
    }delete itN;
    return Size(dist/2,dist/2,dist/2);
  }

  Size getEdgeValue(const edge e) {
    Size s=sizesProxy->getNodeValue(superGraph->source(e));
    Size t=sizesProxy->getNodeValue(superGraph->target(e));
    Coord tmp(s.getW(),s.getH(),s.getD());
    Coord tmp2(t.getW(),t.getH(),t.getD());
    float sizes=tmp.norm();
    float sizet=tmp2.norm();
    return (Size(sizes/16,sizet/16,sizet/4));
  }

};
/*@}*/
SIZESPLUGIN(AutoSizes,"Auto_sizing","Auber","04/05/2001","0","0","1")
