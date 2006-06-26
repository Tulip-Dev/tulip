#include <algorithm>
#include <cfloat>
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
//the following inline function returns true if the point
//is in the left halfplane defined by the vector
inline bool inLeftHalfplane (const Coord halfplaneVecHead,
			     const Coord halfplaneVecTail,
			     const Coord point) {
  return cross (halfplaneVecHead - halfplaneVecTail,
		point - halfplaneVecTail) >= 0;
}

//==============================================================
//the following inline function returns the intersection hit
//of vector v1 on vector v2.  The vectors are anchored at the
//points p1 and p2 respectively in the plane.
inline float hitTime (const Coord &p1, const Coord &v1,
		      const Coord &p2, const Coord &v2) {
  cout << v1.getY() << " " << v2.getY() << endl;
  //paralell lines don't intersect
  if ((fabs (v1.getY()) < 1e-6) && (fabs (v2.getY()) < 1e-6))
    return FLT_MAX;

  //if v2's denominator is zero, we have a degenerate form
  if (fabs (v2.getY()) < 1e-6)
    return (p2.getY() - p1.getY())/v1.getY();

  //compute the denominator and make sure it is not zero
  float invMv2 = v2.getX()/v2.getY();
  if (fabs (v1.getX() - invMv2*v1.getY()) < 1e-6)
    return FLT_MAX;

  //otherwise we have the standard case.
  return (p2.getX() - p1.getX() + invMv2*(p1.getY() - p2.getY()))/
    (v1.getX() - invMv2*v1.getY());
}

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
  //of the calipers.  start1 and start2 are the end positions of the calipers
  unsigned int caliper1, caliper2;
  unsigned int start1, start2;
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
      if (onHull1) { 
	if (mergedConvexHull[0] != hull2[caliper2])
	  mergedConvexHull.push_back (hull2[caliper2]);
      }
      else {
	if (mergedConvexHull[0] != hull1[caliper1])
	  mergedConvexHull.push_back (hull1[caliper1]);
      }//end else
      onHull1 = !onHull1;
    }//end if

    //rotate the calipers
    if ((cross (points[hull1[next1]] - points[hull1[caliper1]],
		points[hull2[next2]] - points[hull2[caliper2]]) > 0) ||
	(hull2.size() == 1)) {
      ++caliper1;
      if (caliper1 == hull1.size()) caliper1 = 0;
      if (onHull1) {
	if (mergedConvexHull[0] != hull1[caliper1])
	  mergedConvexHull.push_back (hull1[caliper1]);
      }
    }//end if
    else {
      ++caliper2;
      if (caliper2 == hull2.size()) caliper2 = 0;
      if (!onHull1) {
	if (mergedConvexHull[0] != hull2[caliper2])
	  mergedConvexHull.push_back (hull2[caliper2]);
      }//end if
    }//end else
  } while ((caliper1 != start1) || (caliper2 != start2));
}//end convex hull

//==============================================================
inline unsigned int advance (unsigned int caliper, unsigned int &adv,
			     bool inside,
			     const vector<unsigned int> &hull,
			     vector<unsigned int> &intersection) {
  if (inside) 
    intersection.push_back (hull[caliper]);
  ++adv;
  return (caliper + 1) % hull.size();
}//end advance

void tlp::intersectHulls (vector<Coord> &points,
			  const vector<unsigned int> &hull1,
			  const vector<unsigned int> &hull2,
			  vector<unsigned int> &intersection) {
  intersection.clear();

  //no second hull to compute intersection empty intersection
  if (hull1.empty() || hull2.empty()) return;

  //These two variables are used to indicate the current positions
  //of the calipers.  start1 and start2 are the end positions of the calipers
  unsigned int caliper1, caliper2;
  unsigned int start1, start2;
  start1 = caliper1 = 1;
  start2 = caliper2 = 1;
  unsigned int adv1 = 0;
  unsigned int adv2 = 0;

  cout << "hull 1 " << endl;
  for (unsigned int i = 0; i < hull1.size(); ++i)
    cout << points[hull1[i]] << endl;
  cout << "hull 2" << endl;
  for (unsigned int i = 0; i < hull2.size(); ++i)
    cout << points[hull2[i]] << endl;
  
  cout << "start1 start2 " << start1 << " " << start2 << endl; 
  bool hullsIntersect = false;
  bool hull1Inside = false;
  do {
    unsigned int prev1 = (caliper1 + hull1.size() - 1) % hull1.size();
    unsigned int prev2 = (caliper2 + hull2.size() - 1) % hull2.size();
    cout << "prev1 caliper1 " << prev1 << " " << caliper1 << endl;
    cout << "prev2 caliper2 " << prev2 << " " << caliper2 << endl << endl;
    Coord vec1 = points[hull1[caliper1]] - points[hull1[prev1]];
    Coord vec2 = points[hull2[caliper2]] - points[hull2[prev2]];

    //check for intersection
    float t = hitTime (points[hull1[prev1]], vec1,
		       points[hull2[prev2]], vec2);
    cout << "hit time " << t << endl;
    if ((t >= 0) && (t <= 1)) {
      cout << "add intersection point " << endl;
      intersection.push_back (points.size());
      points.push_back (points[hull1[caliper1]] + vec1*t);
      hullsIntersect = true;
      hull1Inside = inLeftHalfplane (points[hull2[caliper2]],
				     points[hull2[prev2]],
				     points[hull1[caliper1]]);
      start1 = caliper1;
      start2 = caliper2;
      adv1 = adv2 = 0;
    }//end if
   
    //advance calipers
    cout << "cross " << cross (vec1, vec2) << endl;
    cout << "2 in left of 1 " 
	 << inLeftHalfplane (points[hull1[caliper1]],
			     points[hull1[prev1]],
			     points[hull2[caliper2]]) << endl;
    cout << "1 in left of 2 "
	 << inLeftHalfplane (points[hull2[caliper2]], 
			     points[hull2[prev2]],
			     points[hull1[caliper1]]) << endl;
    if (cross (vec1, vec2) >= 0) {
      if (inLeftHalfplane (points[hull1[caliper1]],
			   points[hull1[prev1]],
			   points[hull2[caliper2]])) 
	caliper1 = advance (caliper1, adv1,
			    (hullsIntersect && hull1Inside),
			    hull1, intersection);
      else
	caliper2 = advance (caliper2, adv2, 
			    (hullsIntersect && !hull1Inside),
			     hull2, intersection);
    }//end if
    else {
      if (inLeftHalfplane (points[hull2[caliper2]], 
			   points[hull2[prev2]],
			   points[hull1[caliper1]])) 
	caliper2 = advance (caliper2, adv2, 
			    (hullsIntersect && !hull1Inside),
			    hull2, intersection);
       else
	 caliper1 = advance (caliper1, adv1,
			     (hullsIntersect && hull1Inside),
			     hull1, intersection);
    }//end else

    cout << "adv " << adv1 << " " << adv2 << endl;
  } while ((adv1 < hull1.size()) || (adv2 < hull2.size()));
}

double tlp::areaOfHull (const vector<Coord> &points,
			const vector<unsigned int> &hull) {
  if (hull.size() < 3) return 0.0;
  double area = 0;
  unsigned int prevElement = hull[0];
  vector<unsigned int>::const_iterator hullIt = hull.begin();
  ++hullIt;
  for (; hullIt != hull.end(); ++hullIt) {
    area += cross (points[prevElement], points[*hullIt]);
    prevElement = *hullIt;
  }//end for
  area += cross (points[prevElement], points[hull[0]]);
  return area/2.0;
}

bool tlp::insideHull (const vector<Coord> &points,
		      const vector<unsigned int> &hull,
		      const Coord &point) {
  if (hull.size() < 3) return false;
  unsigned int prevElement = hull[0];
  vector<unsigned int>::const_iterator hullIt = hull.begin();
  ++hullIt;
  for (; hullIt != hull.end(); ++hullIt) {
    if (!inLeftHalfplane (points[*hullIt],
			  points[prevElement], point)) return false;
    prevElement = *hullIt;
  }
  return inLeftHalfplane (points[hull[0]],
			  points[prevElement], point);
}
