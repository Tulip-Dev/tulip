#include <algorithm>
#include <tulip/ConvexHull.h>

using namespace std;
using namespace tlp;

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
//(the point of minimum x-coordinate).  The index returned is
//an index of the vector points.
inline unsigned int findP0(const vector<Coord> &points) {
  unsigned int p0Index = 0;
  for (unsigned int i = 1; i < points.size(); i++) {
    if (points[p0Index].getX() < points[i].getX()) continue;
    if (points[p0Index].getX() > points[i].getX())
      p0Index = i;
    else if (points[p0Index].getY() > points[i].getY())
      p0Index = i;
  }//end for i
  return p0Index;
}//end findP0

//==============================================================
typedef vector<unsigned int>::const_iterator constUintIt;
//A function that returns the index of a point on the convex hull
//(the point of minimum x-coordinate) given a list of indexes
//in the integer array index.  The index returned is a index of the
//vector index.
inline unsigned int findP0(const vector<Coord> &points,
			   const vector<unsigned int> &index) {
  vector<Coord> pointsSubset;
  for (constUintIt it = index.begin(); it != index.end(); ++it)
    pointsSubset.push_back (points[*it]);
  return findP0 (pointsSubset);
}//end findP0

//==============================================================
void tlp::convexHull (const vector<Coord> &points, 
		      std::vector<unsigned int> &convexHull) {

  //clear the input vector
  convexHull.clear();

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
//==============================================================
//the following inline function of cross product ensures we
//ignore the z-coordinate.  Only 2d-hull merging allowed.
inline double cross (Coord v1, Coord v2) {
  v1.setZ (0.0);
  v2.setZ (0.0);
  return ((v1^v2).getZ());
}//end cross
//==============================================================
void tlp::mergeHulls (const vector<Coord> &points,
		      const vector<unsigned int> &hull1,
		      const vector<unsigned int> &hull2,
		      vector<unsigned int> &mergedConvexHull) {
  //clear the input vector
  mergedConvexHull.clear();

  if (hull1.empty() || hull2.empty()) {
    mergedConvexHull = hull1.empty() ? hull2 : hull1;
    return;
  }//end if

  //These two variables are used to indicate the current positions
  //of the calipers.  end1 and end2 are the end positions of the calipers
  unsigned int caliper1, caliper2;
  unsigned int start1, start2;

  //find the two initial postions of the calipers:  max x coordinates
  //on the two convex hulls
  start1 = caliper1 = findP0 (points, hull1);
  start2 = caliper2 = findP0 (points, hull2);

  //determine which points of the hull is currently on the hull,
  //(max of the two caliper positions in x coord)
  bool onHull1 = true;
  if (points[hull1[caliper1]].getX() > points[hull2[caliper2]].getX()) 
    onHull1 = false;
  else if (!(points[hull1[caliper1]].getX() < 
	     points[hull2[caliper2]].getX()) &&
	   (points[hull1[caliper1]].getY() > 
	    points[hull2[caliper2]].getY())) onHull1 = false;

  //place the first vertex on the convex hull 
  if (onHull1) mergedConvexHull.push_back (hull1[caliper1]);
  else mergedConvexHull.push_back (hull2[caliper2]);

  //merge the two convex hulls
  do {
    //check for a convex hull bridge
    unsigned int next1 = (caliper1 == hull1.size() - 1) ? 0 : caliper1 + 1;
    unsigned int prev1 = (caliper1 == 0) ? hull1.size() - 1 : caliper1 - 1;
    unsigned int next2 = (caliper2 == hull2.size() - 1) ? 0 : caliper2 + 1;
    unsigned int prev2 = (caliper2 == 0) ? hull2.size() - 1 : caliper2 - 1;
    Coord copodal = points[hull2[caliper2]] - points[hull1[caliper1]];
    if (!onHull1) copodal *= -1;
    if ((cross (copodal, 
		points[hull1[next1]] - points[hull1[caliper1]]) >= 0) &&
	(cross (copodal, 
		points[hull1[prev1]] - points[hull1[caliper1]]) >= 0) &&
	(cross (copodal, 
		points[hull2[next2]] - points[hull2[caliper2]]) >= 0) &&
	(cross (copodal, 
		points[hull2[prev2]] - points[hull2[caliper2]]) >= 0)) {
      if (onHull1) mergedConvexHull.push_back (hull2[caliper2]);
      else mergedConvexHull.push_back (hull1[caliper1]);
      onHull1 = !onHull1;
    }//end if

    //rotate the calipers
    if ((cross (points[hull1[next1]] - points[hull1[caliper1]],
		points[hull2[next2]] - points[hull2[caliper2]]) > 0) ||
	(hull2.size() == 1)) {
      caliper1++;
      if (caliper1 == hull1.size()) caliper1 = 0;
      if (onHull1) mergedConvexHull.push_back (hull1[caliper1]);
    }//end if
    else {
      caliper2++;
      if (caliper2 == hull2.size()) caliper2 = 0;
      if (!onHull1) mergedConvexHull.push_back (hull2[caliper2]);
    }//end else
  } while ((caliper1 != start1) || (caliper2 != start2));
}//end convex hull
//==============================================================
