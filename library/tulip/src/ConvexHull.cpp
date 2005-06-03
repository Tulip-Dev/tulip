#include <algorithm>
#include <tulip/ConvexHull.h>

using namespace std;

//==============================================================
//A private struct for vectors about a point used in convex hull computation
struct p0Vectors {
  Coord pos;
  unsigned int index;
};

bool operator<(const p0Vectors &p1, const p0Vectors &p2) {
  return (p1.pos^p2.pos)[2] > 0; 
}
//==============================================================

//A function that returns a point on the convex hull
//(the point of maximum x-coordinate).
inline unsigned int findP0(const vector<Coord> &points) {
  Coord p0 = points[0];
  unsigned int p0Index = 0;
  for (unsigned int i = 1; i < points.size(); i++) {
    if ( p0.getX() < points[i].getX()) continue;
    if ( p0.getX() > points[i].getX()) {
      p0 = points[i];
      p0Index = i;
    }//end if
    else if (p0.getY() > points[i].getY()) {
      p0 = points[i];
      p0Index = i;
    }//end else if
  }//end for i
  return p0Index;
}//end findP0

//==============================================================
void tlp::convexHull (const vector<Coord> &points, 
		      vector <unsigned int> &convexHull) {

  //if we have less than three points, the convex hull consists of
  //all zero, one, or two points.
  if (points.size() < 3) {
    for (unsigned int i = 0; i < points.size(); i++) 
      convexHull.push_back (i);
    if (points.size() == 2) {
      if ((points[1].getX() > points[0].getX()) ||
	  (!(points[1].getX() < points[0].getX()) &&
	   (points[1].getY() > points[0].getY()))) {
	convexHull[0] = 1;
	convexHull[1] = 0;
      }//end if
    }//end if
    return;
  }//end if

  //get the starting point of the convex hull computation
  unsigned int p0Index = findP0(points);
  
  //compute vectors from all the points to p0
  vector<p0Vectors> vectors;
  for (unsigned int i = 0; i < points.size(); i++) {
    if (p0Index == i) continue;
    p0Vectors curAngle;
    curAngle.pos = points[i] - points[p0Index];
    curAngle.index = i;
    vectors.push_back (curAngle);
  }//end for
  
  //sort vectors counterclockwise and compute the convex hull
  stable_sort (vectors.begin(), vectors.end());
  vector<p0Vectors>::const_iterator it = vectors.begin();
  convexHull.push_back(p0Index);
  convexHull.push_back((*it++).index);
  convexHull.push_back((*it++).index);
  for (;it != vectors.end(); ++it) {
    while (!((((*it).pos - 
	       (points[convexHull[convexHull.size() - 1]] -
		points[p0Index])) ^
	      ((points[convexHull[convexHull.size() - 2]] - 
	       points[p0Index]) -
	       (points[convexHull[convexHull.size() - 1]] -
		points[p0Index])))[2] > 0))
      convexHull.pop_back();
    convexHull.push_back((*it).index);
  }//end for
}//end ConvexHull
