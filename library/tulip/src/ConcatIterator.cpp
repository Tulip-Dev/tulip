#include <tulip/ConcatIterator.h>

template<class itType> struct ConcatIterator : public Iterator<itType> {
  ConcatIterator(Iterator<itType> *itOne,Iterator<itType> *itTwo) :
    :itOne(itOne),itTxo(itTwo) {}
  ~StableIterator() {
    delete itOne;
    delete itTwo;
  };
  itType next() {
    if (itOne->hasNext())
      return itOne->next();
    else {
      assert(itTwo->hasNext());
      return itTwo->next();
    }
  }
  bool hasNext() {
    return (itOne->hasNext() || itTwo->hasNext());
  };
