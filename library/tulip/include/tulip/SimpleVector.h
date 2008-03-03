//-*-c++-*-

#ifndef DOXYGEN_NOTFOR_DEVEL
#ifndef _SIMPLE_VECTOR_H_
#define _SIMPLE_VECTOR_H_

namespace tlp {

// SimpleVector
//   simple copy constructor
//   deallocate with hand

template<typename T>
class SimpleVector {
 protected:
  T *beginP;
  T *middleP;
  T *endP;

 public:
  typedef T *iterator;
  typedef const T *const_iterator;

  SimpleVector() { beginP=middleP=endP=0; }
  SimpleVector(const SimpleVector &v):
    beginP(v.beginP),middleP(v.middleP),endP(v.endP) { }
  explicit SimpleVector(size_t s) { begin=0; resize(s); }
  void resize(size_t s) {
    middleP=beginP+s;
    if (middleP>endP || size()<capacity()/2)
      doRealloc(size());
  }
  iterator begin() { return beginP; }
  iterator end() { return middleP; }
  const_iterator begin() const { return beginP; }
  const_iterator end() const { return middleP; }
  T &front() { assert(!empty()); return *beginP; }
  const T &front() const { assert(!empty()); return *beginP; }
  T &back() { assert(!empty()); return *(middleP-1); }
  const T &back() const { assert(!empty()); return *(middleP-1); }
  size_t size() const { return size_t(middleP-beginP); }
  size_t capacity() const { return size_t(endP-beginP); }
  bool empty() const { return beginP == endP; }
  T &operator[](size_t i) { assert(i<size()); return *(beginP+i); }
  T operator[](size_t i) const { assert(i<size()); return *(beginP+i); }
  void push_back(const T &v) {
    if (middleP==endP) {
      size_t s=size();
      doRealloc(s==0?1:2*s);
    }
    *middleP=v;
    ++middleP;
  }
  void pop_back() {  
    assert(!empty());  
    --middleP; 
    if (size()<capacity()/2)
      doRealloc(size());
  }
  void clear() {
    deallocateAll();
    beginP=middleP=endP=0;
  }
  void deallocateAll() {
    free(beginP);
  }
 protected:
  void doRealloc(size_t s) {
    size_t i=middleP-beginP;
    beginP=(T *)realloc(beginP,s*sizeof(T));
    middleP=beginP+i;
    endP=beginP+s;
    assert(middleP<=endP);
  }
};

}
#endif
#endif //DOXYGEN_NOTFOR_DEVEL
