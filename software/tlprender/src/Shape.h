#ifndef SHAPE_H // -*-c++-*-
#define SHAPE_H

#if (__GNUC__ < 3)
#include <ostream.h>
#else
#include <ostream>
#endif

#include <functional>
#include <vector>

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

namespace tlprender {

  class Point {
    friend std::ostream& operator<<(std::ostream &,const Point &);
  public:
    int x, y;
    GLfloat z;
  
    inline Point(const int x=0, const int y=0, const GLfloat z=0.0): x(x), y(y), z(z) {}
    ~Point() {}

    inline bool operator ==(const Point &p) const {
      return (x==p.x) && (y==p.y) && (z==p.z);
    }
    inline bool operator !=(const Point &p) const {
      return !(*this==p);
    }
  };

  class Shape {
    friend std::ostream& operator<<(std::ostream &,const Shape &);
  private:
    bool valid;
    std::vector<Point> vertices;
    GLfloat avgZ;
  public:
    unsigned int nodeNum;
    Shape();
    ~Shape();
    bool isValid() const;
    void begin();
    void add(const int x, const int y, const GLfloat z);
    inline void add(const Point p) {vertices.push_back(p);}
   
    bool end();
    
    //extend one Shape with another
    bool clip(const Shape& p);

    inline bool operator<=(const Shape &p) const {
      return (avgZ <= p.avgZ);
    }
  };

  std::ostream& operator<<(std::ostream &os, const Shape &p);
  std::ostream& operator<<(std::ostream &os, const Point &p);
} //namespace tlprender

namespace std {
  template<>
  struct less<tlprender::Shape *> {
    bool operator()(tlprender::Shape *&p1, tlprender::Shape *&p2);
  };
  template<>
  struct less<tlprender::Shape> {
    bool operator()(const tlprender::Shape &p1, const tlprender::Shape &p2);
  };
}
#endif //SHAPE_H
