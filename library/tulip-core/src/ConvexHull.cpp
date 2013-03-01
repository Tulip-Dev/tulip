#include <tulip/ConvexHull.h>

#include <stack>
#include <list>

using namespace std;
using namespace tlp;

//================================================================================
bool isLeft(const tlp::Coord& p, const std::pair<tlp::Coord, tlp::Coord>& seg) {
  double cp1 =
    (seg.second.getX()-seg.first.getX())*(p.getY()-seg.first.getY())
    - (seg.second.getY()-seg.first.getY())*(p.getX()-seg.first.getX());
  return (cp1<=0);
}
//================================================================================
double distanceToSeg(const tlp::Coord& p,
                     const std::pair<tlp::Coord, tlp::Coord>& seg,
                     double segx, double segy) {
  double dist = segx*(seg.first.getY()-p.getY())-segy*(seg.first.getX()-p.getX());

  if (dist < 0.)
    dist = -dist;

  return dist;
}

// used to store call infos of the original
// recursive algorithm
struct hullLoopIteration {
  unsigned int insertPos;
  std::pair<Coord, Coord> seg;
  list<unsigned int> subset;
};
//================================================================================
void hullSet(stack<hullLoopIteration>& loopIterStack,
             const vector<Coord>& points,
             vector<unsigned int> &hull) {
  while(loopIterStack.empty() != false) {
    hullLoopIteration current(loopIterStack.top());
    loopIterStack.pop();

    //test end of loop
    if (current.subset.empty())
      continue;

    //Get furthest point from seg
    list<unsigned int>::iterator itS = current.subset.begin();
    unsigned int iFurther = *itS;

    if (current.subset.size() > 1) {
      double segx = current.seg.second.getX()-current.seg.first.getX();
      double segy = current.seg.second.getY()-current.seg.first.getY();
      double pmaxDist=distanceToSeg(points[iFurther], current.seg, segx, segy);
      list<unsigned int>::iterator itMax = itS;

      for (++itS; itS != current.subset.end(); ++itS) {
        double dist = distanceToSeg(points[*itS], current.seg, segx, segy);

        if (dist > pmaxDist) {
          pmaxDist = dist;
          itMax = itS;
        }
      }

      iFurther = *itMax;
      current.subset.erase(itMax);
    }
    else current.subset.clear();

    // insert new hull point
    hull.insert(hull.begin() + current.insertPos, iFurther);

    if(current.subset.empty())
      continue;

    // divide points
    // so push 2 new loop iterations
    // on the stack
    hullLoopIteration leftPBloop;
    leftPBloop.insertPos=current.insertPos;
    pair<Coord,Coord>& segPB = leftPBloop.seg;
    segPB.first = points[iFurther];
    segPB.second = current.seg.second;
    list<unsigned int>& leftPB = leftPBloop.subset;

    hullLoopIteration leftABloop;
    leftABloop.insertPos=current.insertPos;
    pair<Coord,Coord>& segAP = leftABloop.seg;
    segAP.first = points[iFurther];
    segAP.second = current.seg.second;
    list<unsigned int>& leftAP = leftABloop.subset;


    for(itS=current.subset.begin(); itS!=current.subset.end(); ++itS) {
      if(isLeft(points[*itS],segPB))
        leftPB.push_front(*itS);
      else if(isLeft(points[*itS],segAP))
        leftAP.push_front(*itS);
    }

    loopIterStack.push(leftPBloop);
    loopIterStack.push(leftABloop);


//    loopIterStack.push(hullLoopIteration());
//    loopIterStack.top().insertPos = current.insertPos;
//    pair<Coord,Coord>& segPB = loopIterStack.top().seg;
//    segPB.first = points[iFurther];
//    segPB.second = current.seg.second;
//    list<unsigned int>& leftPB = loopIterStack.top().subset;
//    for(itS=current.subset.begin();itS!=current.subset.end();++itS){
//      if(isLeft(points[*itS],segPB))
//  leftPB.push_front(*itS);
//    }

//    loopIterStack.push(hullLoopIteration());
//    loopIterStack.top().insertPos = current.insertPos + 1;
//    pair<Coord,Coord>& segAP = loopIterStack.top().seg;
//    segAP.first = current.seg.first;
//    segAP.second = points[iFurther];
//    list<unsigned int>& leftAP = loopIterStack.top().subset;
//    for(itS=current.subset.begin();itS!=current.subset.end();++itS){
//      if(isLeft(points[*itS], segAP))
//  leftAP.push_front(*itS);
//    }
  }
}
//================================================================================
void tlp::convexHull(const std::vector<Coord> &points,
                     std::vector<unsigned int> &hull) {
  //clear hull
  hull.clear();

  //dummy test
  if(points.size()<=3) {
    for(unsigned int i=0; i<points.size(); ++i)
      hull.push_back(i);

    return;
  }

  //get first segment
  Coord pMinX = points[0];
  Coord pMaxX = pMinX;
  unsigned int iMinX = 0;
  unsigned int iMaxX = 0;

  for(unsigned int i=0; i<points.size(); ++i) {
    if(points[i].getX()<pMinX.getX())
      pMinX=points[iMinX = i];
    else if(points[i].getX()>pMaxX.getX())
      pMaxX=points[iMaxX = i];
  }

  hull.push_back(iMinX);
  hull.push_back(iMaxX);
  pair<Coord,Coord> seg = make_pair(pMinX,pMaxX);
  // divide points
  // so push 2 loop iterations
  // on the stack
  stack<hullLoopIteration> loopIterStack;
  loopIterStack.push(hullLoopIteration());
  loopIterStack.top().insertPos = 0;
  loopIterStack.top().seg.first = pMaxX;
  loopIterStack.top().seg.second = pMinX;
  list<unsigned int>& rightPoints = loopIterStack.top().subset;
  loopIterStack.push(hullLoopIteration());
  loopIterStack.top().insertPos = 1;
  loopIterStack.top().seg.first = pMinX;
  loopIterStack.top().seg.second = pMaxX;
  list<unsigned int>& leftPoints = loopIterStack.top().subset;

  for(unsigned int i=0; i<points.size(); ++i) {
    if((i!=iMinX) && (i!=iMaxX)) {
      if(isLeft(points[i], seg))
        leftPoints.push_front(i);
      else
        rightPoints.push_front(i);
    }
  }

  //loop calls
  hullSet(loopIterStack, points, hull);
}
//================================================================================
