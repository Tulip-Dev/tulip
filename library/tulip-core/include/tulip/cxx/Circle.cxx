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
#include <cstdlib>

template<typename Obj, typename OTYPE>
tlp::Circle<Obj, OTYPE> & tlp::Circle<Obj, OTYPE>::merge(const tlp::Circle<Obj, OTYPE> &c) {
  Vector<Obj,2, OTYPE> p1(*this);
  Vector<Obj,2, OTYPE> p2(c);
  Vector<Obj,2, OTYPE> c12=p2-p1;
  double norm=c12.norm();

  if (norm < 0.0000001) {
    if (radius<c.radius)
      return (*this)=c;
    else
      return *this;
  }

  double dx=c12[0]/norm;
  double dy=c12[1]/norm;
  p1[0]-=dx*radius;
  p1[1]-=dy*radius;
  p2[0]+=dx*c.radius;
  p2[1]+=dy*c.radius;
  Obj tmpDist=p1.dist(p2)/2.;

  if ( (tmpDist<radius) || (tmpDist<c.radius)) {
    if (radius>c.radius)
      return *this;
    else
      return (*this)=c;
  }
  else {
    p1+=p2;
    p1/=2.;
    (*this)[0]=p1[0];
    (*this)[1]=p1[1];
    radius=tmpDist;
    return *this;
  }
}

template<typename Obj, typename OTYPE>
bool tlp::Circle<Obj, OTYPE>::isIncludeIn(const tlp::Circle<Obj, OTYPE> &c) const {
  Vector<Obj,2, OTYPE> dir=c-*this;
  return (dir.norm()+radius)<=c.radius;
}

template<typename Obj, typename OTYPE>
tlp::Circle<Obj, OTYPE> tlp::enclosingCircle(const tlp::Circle<Obj, OTYPE> &c1,const tlp::Circle<Obj, OTYPE> &c2) {
  Vector<Obj,2, OTYPE> dir=c2-c1;
  Obj n=dir.norm();

  if (n==0)
    return Circle<Obj, OTYPE>(c1, std::max(c1.radius,  c2.radius));

  dir/=n;
  Vector<Obj, 2, OTYPE> ext1=c1-dir*c1.radius;
  Vector<Obj, 2, OTYPE> ext2=c2+dir*c2.radius;
  return Circle<Obj, OTYPE>((ext1+ext2)/Obj(2),(ext2-ext1).norm()/Obj(2));
}

template<typename Obj, typename OTYPE>
tlp::Circle<Obj, OTYPE> tlp::lazyEnclosingCircle(const std::vector<tlp::Circle<Obj, OTYPE> > & circles) {
  //compute bounding box of a
  tlp::Vector<Obj,4, OTYPE> boundingBox;
  //  for (int i=0;i<4;++i) boundingBox[i]=0;
  typename std::vector< tlp::Circle<Obj, OTYPE> >::const_iterator it=circles.begin();
  boundingBox[0]=(*it)[0]-(*it).radius;
  boundingBox[1]=(*it)[1]-(*it).radius;
  boundingBox[2]=(*it)[0]+(*it).radius;
  boundingBox[3]=(*it)[1]+(*it).radius;
  ++it;

  for (; it!=circles.end(); ++it) {
    boundingBox[0] = std::min(boundingBox[0] , ((*it)[0]-(*it).radius));
    boundingBox[1] = std::min(boundingBox[1] , ((*it)[1]-(*it).radius));
    boundingBox[2] = std::max(boundingBox[2] , ((*it)[0]+(*it).radius));
    boundingBox[3] = std::max(boundingBox[3] , ((*it)[1]+(*it).radius));
  }

  tlp::Vector<Obj,2, OTYPE> center;
  center[0]=(boundingBox[0]+boundingBox[2])/2.;
  center[1]=(boundingBox[1]+boundingBox[3])/2.;
  Obj radius = std::max( (boundingBox[2]-boundingBox[0])/2., (boundingBox[3]-boundingBox[1])/2. );
  tlp::Circle<Obj, OTYPE> result(center,radius);

  //compute circle hull
  for (typename std::vector< tlp::Circle<Obj, OTYPE> >::const_iterator it=circles.begin(); it!=circles.end(); ++it)
    result.merge(*it);

  return result;
}

template<typename Obj, typename OTYPE>
tlp::Circle<Obj, OTYPE> tlp::enclosingCircle(const std::vector<tlp::Circle<Obj, OTYPE> > & circles) {
  class OptimumCircleHull {
    const std::vector<tlp::Circle<Obj, OTYPE> > *circles;
    std::vector<unsigned> enclosedCircles;
    unsigned first,last;
    unsigned b1,b2;
    tlp::Circle<Obj, OTYPE> result;

    static tlp::Circle<Obj, OTYPE> enclosingCircle(const tlp::Circle<Obj, OTYPE> &c1,const tlp::Circle<Obj, OTYPE> &c2,const tlp::Circle<Obj, OTYPE> &c3) {
      Obj a1 = c1[0];
      Obj b1 = c1[1];
      Obj r1 = c1.radius;
      Obj a2 = c2[0];
      Obj b2 = c2[1];
      Obj r2 = c2.radius;
      Obj a3 = c3[0];
      Obj b3 = c3[1];
      Obj r3 = c3.radius;
      Obj b = -b1*r2*a1*a1*b3+a3*a3*r2*r2*r2+b2*b2*r3*r3*r3+b1*b1*r3*r3*r3+b1*b1*r2*r2*r2+r1*r1*r1*b3*b3+r1*r1*r1*b2*b2+r2*r2*r2*b3*b3
              +a1*a1*r3*r3*r3+a2*a2*r1*r1*r1+a2*a2*r3*r3*r3+a1*a1*r2*r2*r2+a3*a3*r1*r1*r1-b2*b2*r3*a3*a3-b2*b2*r3*b3*b3-2*b2*r3*r3*r3*b1
              -b1*b1*r3*a3*a3-b1*b1*r3*b3*b3-b1*b1*r2*a2*a2+2*b1*b1*r2*b3*b3-b1*b1*r2*r3*r3-r1*b3*b3*b3*b2+r1*b3*b3*b3*b1
              +2*r1*b2*b2*b3*b3-r1*b2*b2*r3*r3+r2*b3*b3*b3*b2-r2*b3*b3*b3*b1-b2*b2*b2*r3*b1+2*b2*b2*r3*b1*b1+b2*b2*b2*r3*b3
              -b2*b2*r3*r1*r1+b1*b1*b1*r3*b3-b1*b1*b1*r3*b2-b1*b1*r3*r2*r2-b1*b1*r2*b2*b2-b1*b1*b1*r2*b3+b1*b1*b1*r2*b2
              -2*b1*r2*r2*r2*b3-r1*b3*b3*b1*b1-2*r1*r1*r1*b3*b2-r1*b3*b3*r2*r2-r1*b3*b3*a1*a1+r1*b2*b2*b2*b1
              -r1*b2*b2*b1*b1-r1*b2*b2*b2*b3-r1*b2*b2*a1*a1-r2*b3*b3*a2*a2-r2*b3*b3*b2*b2-r2*b3*b3*r1*r1
              -b2*r3*b1*b1*b3+b2*r3*a2*a2*b3+b2*r3*r1*r1*b3-b2*r3*r2*r2*b3+b2*r3*a1*a1*b3-2*b2*r3*a1*a2*b3+a1*a3*b2*b2*r3
              -2*a1*a3*b2*b1*r3-2*a1*a3*b2*b1*r2-2*a1*a3*b2*r1*b3+a1*a3*b2*b2*r1-2*a1*a3*b2*r2*b3-b2*r3*b1*a2*a2
              +2*b2*r3*b1*a3*a3+2*b2*r3*b1*b3*b3+b1*r2*b2*a3*a3-b1*r2*b2*b3*b3+b1*r2*b2*r3*r3+r1*b3*b1*a2*a2
              -r1*b3*b2*a3*a3+r1*b3*b2*r3*r3+r1*b3*b1*a3*a3-r1*b3*b1*r3*r3+r1*b2*b1*a2*a2+r1*b2*b1*a3*a3
              -r1*b2*b1*b3*b3+r1*b2*b1*r3*r3+2*r2*b3*b1*a2*a2+r2*b3*b2*a3*a3-r2*b3*b2*r3*r3-r2*b3*b1*a3*a3
              +r2*b3*b1*r3*r3+b2*r3*b1*r2*r2+4*b2*r3*a1*a2*b1+b1*r3*a2*a2*b3-b1*r3*b2*b2*b3
              -b1*r3*r1*r1*b3+b1*r3*r1*r1*b2+b1*r3*r2*r2*b3+b1*r3*a1*a1*b3-b1*r3*a1*a1*b2-2*b1*r3*a1*a2*b3-b1*b1*r3*a1*a2
              +b1*b1*r3*a1*a3+2*b1*r2*b2*b2*b3+b1*r2*r1*r1*b3-b1*r2*r1*r1*b2+b1*r2*a1*a1*b2
              -2*b1*r2*a1*a2*b3+b1*b1*r2*a1*a2-b1*b1*r2*a1*a3-r1*b3*b1*b2*b2+2*r1*b3*b1*b1*b2+2*r1*b3*a1*a1*b2+r1*b3*b1*r2*r2
              +r1*b3*b3*a1*a2-r1*b2*a2*a2*b3+r1*b2*r2*r2*b3-r1*b2*b1*r2*r2-2*r1*b2*a1*a2*b3-r2*b3*b1*b1*b2
              +r2*b3*r1*r1*b2+r2*b3*a1*a1*b2+r2*b3*b3*a1*a2+4*r2*b3*a1*a3*b1-a1*a1*a3*a3*r3+2*a1*a1*a3*a3*r2+a1*a3*a3*a3*r1
              -a1*a1*b3*b3*r3-a1*b3*b3*a3*r2+a1*r3*r3*a3*r2-a2*a1*a1*a3*r2+a2*b1*b1*a3*r2+a2*b3*b3*a3*r2
              -a1*a3*a3*a2*r1+2*a1*a3*a3*a2*r3+2*a1*b3*b3*a2*r3+a1*b3*b3*a3*r1-2*a1*r3*r3*r3*a2-a1*a1*r3*r3*r2
              -a2*a1*a1*a1*r3-a2*a2*a1*a1*r1+2*a2*a2*a1*a1*r3+a2*a1*a1*a1*r2-a2*a2*b1*b1*r1+2*a2*a2*a3*a3*r1-a2*a2*a3*a3*r3
              -a2*a3*a3*a3*r1-a2*a2*b3*b3*r3-a2*a2*r1*r1*r3-2*a2*r1*r1*r1*a3+a1*r3*r3*a2*r1-a1*r3*r3*a3*r1
              +2*a2*a1*a1*a3*r1+2*a2*b1*b1*a3*r1-a2*a3*a3*a1*r2-a2*b3*b3*a3*r1+a2*r1*r1*a1*r3-a2*r1*r1*a1*r2-a2*a2*r3*r3*r1
              -a1*a3*a3*a3*r2+a2*a3*a3*a3*r2-a1*a1*r3*r2*r2+a1*a1*a1*r3*a3+a2*a2*a2*r1*a1-a2*a2*a2*r1*a3-a2*a2*a2*r3*a1+a2*a2*a2*r3*a3
              -a1*a1*r2*a2*a2-a1*a1*r2*b2*b2-a1*a1*a1*r2*a3-2*a1*r2*r2*r2*a3-a3*a3*r1*a1*a1-a3*a3*r1*b1*b1-a3*a3*r1*r2*r2
              -a3*a3*r2*a2*a2-a3*a3*r2*b2*b2-a3*a3*r2*r1*r1+a2*r3*r3*a1*r2+a2*r3*r3*a3*r1-2*a3*r2*b1*a2*b3
              +a2*r1*r1*a3*r2-a2*r3*r3*a3*r2-a1*r3*a3*a2*a2-a1*r3*a3*r1*r1+a1*r3*a3*r2*r2+a2*r1*a1*b2*b2
              -a2*r1*a1*r2*r2-a2*r1*a3*b2*b2+a2*r1*a3*r2*r2-2*a2*r1*b1*a3*b2-a2*r3*a1*b2*b2+a2*r3*a1*r2*r2
              -a2*r3*a3*a1*a1+a2*r3*a3*b1*b1+a2*r3*a3*b2*b2+a2*r3*a3*r1*r1-a2*r3*a3*r2*r2-2*a2*r3*b1*a3*b2+2*a1*r2*a3*a2*a2
              +2*a1*r2*a3*b2*b2+a1*r2*a3*r1*r1-a3*r1*a1*a2*a2+a3*r1*a1*r2*r2-2*a3*r1*b1*a2*b3
              +4*r1*a3*b2*a2*b3;
      Obj tmp=a2*b3-a3*b2-a2*b1-a1*b3+a1*b2+a3*b1;
      Obj d = sqrt((a2*a2+b2*b2-r2*r2-2*a2*a3+a3*a3-r3*r3-2*b3*b2+b3*b3+2*r3*r2)*
                   (b3*b3+b1*b1-r1*r1-r3*r3-2*b1*b3+2*r3*r1+a3*a3-2*a1*a3+a1*a1)*
                   (a1*a1-2*a1*a2-r1*r1+b1*b1+b2*b2-r2*r2-2*b2*b1+2*r2*r1+a2*a2)*
                   tmp*tmp);
      Obj v = d-b;

      if (v<0) return tlp::Circle<Obj, OTYPE>(0,0,0);

      Obj aa = -2*a3*b2*b2*a1-2*a2*b3*b3*a1
               -2*a1*a1*b3*b2+a3*a3*b2*b2+a2*a2*b3*b3-r1*r1*b3*b3-r1*r1*b2*b2+a2*a2*b1*b1+a3*a3*b1*b1-a2*a2*r1*r1
               -a2*a2*r3*r3-a3*a3*r2*r2-2*a3*b2*a2*b3+2*a3*b2*a2*b1+2*a2*b3*a3*b1-2*b1*a2*a2*b3-a3*a3*r1*r1
               -2*a3*a3*b2*b1+2*b1*r3*r3*b2+2*r1*r1*b3*b2+2*b3*b1*r2*r2-2*a2*b1*b1*a3+2*a2*a3*r1*r1-b2*b2*r3*r3-b1*b1*r3*r3
               -b1*b1*r2*r2-r2*r2*b3*b3-a1*a1*r3*r3-a1*a1*r2*r2+a1*a1*b3*b3+a1*a1*b2*b2+2*b2*b2*r3*r1+2*b1*b1*r3*r2
               +2*r1*b3*b3*r2-2*b2*r3*b1*r2-2*b2*r3*r1*b3+2*b2*r3*r2*b3+2*b1*r3*r1*b3-2*b1*r3*r1*b2
               -2*b1*r3*r2*b3-2*b1*r2*r1*b3+2*b1*r2*r1*b2-2*r1*b2*r2*b3+2*a1*r3*r3*a2+2*a1*a1*r3*r2+2*a2*a2*r1*r3
               +2*a1*r2*r2*a3+2*a3*a3*r1*r2-2*a1*r3*a2*r1+2*a1*r3*a3*r1+2*a2*r1*a1*r2-2*a2*r3*a1*r2
               -2*a2*r3*a3*r1-2*a1*r2*a3*r1-2*a1*r3*a3*r2-2*a2*r1*a3*r2+2*a2*r3*a3*r2+2*a2*b3*a1*b2
               +2*a3*b2*a1*b3+2*a2*b1*a1*b3-2*a2*b1*a1*b2-2*a1*b3*a3*b1+2*a1*b2*a3*b1;
      Obj R = 0.5*v/aa;
      Obj y = -0.5*(-2*a1*R*r2-2*a3*R*r1+2*a2*R*r1+2*a3*R*r2-2*a2*R*r3+a1*a3*a3+a1*b3*b3-a1*r3*r3
                    +a2*a1*a1+a2*b1*b1+a1*r2*r2+2*a1*R*r3-a3*b1*b1+a3*a2*a2+a3*b2*b2+a3*r1*r1-a3*r2*r2-a2*a3*a3-a2*b3*b3
                    -a2*r1*r1+a2*r3*r3-a1*a2*a2-a1*b2*b2-a3*a1*a1)/(a2*b3+a3*b1-a2*b1-a1*b3+a1*b2-a3*b2);
      Obj x = 0.5*(-a1*a1*b3+a1*a1*b2+2*R*r2*b3+b1*a3*a3+b1*b3*b3+2*b1*R*r3-2*R*r1*b3+2*R*r1*b2+b1*r2*r2
                   -b2*a3*a3-b2*b3*b3+b2*r3*r3-2*b2*R*r3-b1*r3*r3-r2*r2*b3+a2*a2*b3-r1*r1*b2-b1*b2*b2+b1*b1*b2-b1*b1*b3
                   -b1*a2*a2+b2*b2*b3+r1*r1*b3-2*b1*R*r2)/(a2*b3+a3*b1-a2*b1-a1*b3+a1*b2-a3*b2);
      return  tlp::Circle<Obj, OTYPE>(x,y,R);
    }

    void process2() {
      if (isEmpty()) {
        result=tlp::enclosingCircle((*circles)[b1],(*circles)[b2]);
      }
      else {
        unsigned selectedCircle=popBack();
        process2();

        if (!(*circles)[selectedCircle].isIncludeIn(result)) {
          result=enclosingCircle((*circles)[b1],(*circles)[b2],(*circles)[selectedCircle]);
          pushFront(selectedCircle);
        }
        else {
          pushBack(selectedCircle);
        }
      }
    }

    void process1() {
      if (isEmpty()) {
        result=(*circles)[b1];
      }
      else {
        unsigned selectedCircle=popBack();
        process1();

        if (!(*circles)[selectedCircle].isIncludeIn(result)) {
          b2=selectedCircle;
          process2();
          pushFront(selectedCircle);
        }
        else {
          pushBack(selectedCircle);
        }
      }
    }
    void process0() {
      if (isEmpty()) {
        result=Circle<Obj, OTYPE>(0,0,0);
      }
      else {
        unsigned selectedCircle=popBack();
        process0();

        if (!(*circles)[selectedCircle].isIncludeIn(result)) {
          b1=selectedCircle;
          process1();
          pushFront(selectedCircle);
        }
        else {
          pushBack(selectedCircle);
        }
      }
    }
    bool isEmpty() const {
      return first==(last+1)%enclosedCircles.size();
    }
    void pushFront(unsigned c) {
      first=(first+enclosedCircles.size()-1)%enclosedCircles.size();
      enclosedCircles[first]=c;
    }
    unsigned popBack() {
      unsigned result=enclosedCircles[last];
      last=(last+enclosedCircles.size()-1)%enclosedCircles.size();
      return result;
    }
    void pushBack(unsigned c) {
      last=(last+1)%enclosedCircles.size();
      enclosedCircles[last]=c;
    }
  public:
    OptimumCircleHull():circles(NULL),first(0),last(0),b1(0),b2(0) {}

    tlp::Circle<Obj, OTYPE> operator()(const std::vector<tlp::Circle<Obj, OTYPE> > &circlesSet) {
      circles=&circlesSet;
      enclosedCircles.resize(circlesSet.size()+1);
      first=0;
      last=circlesSet.size()-1;

      for (unsigned i=0; i<circlesSet.size(); ++i)
        enclosedCircles[i]=i;

      for (unsigned i=circlesSet.size(); i>0;) {
        unsigned idx=(unsigned)(1.0*rand()*i/RAND_MAX);
        --i;
        std::swap(enclosedCircles[idx],enclosedCircles[i]);
      }

      process0();
      return result;
    }
  };
  return OptimumCircleHull()(circles);
}

template <typename Obj, typename OTYPE>
std::ostream& tlp::operator<<(std::ostream &os,const tlp::Circle<Obj, OTYPE> &a) {
  os << "((" << a[0] << "," <<  a[1] << ")," << a.radius << ")";
  return os;
}
