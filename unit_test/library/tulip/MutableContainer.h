#ifndef _TLPMUTABLECONTAINER_
#define _TLPMUTABLECONTAINER_

#include <iostream>
#if (__GNUC__ < 3)
#include <hash_map>
#else
#include <ext/hash_map>
#endif
#include <deque>
#include "tulipconf.h"
#include "ReturnType.h"
#include <exception>
#include "tulip/Iterator.h"

enum State { VECT=0, HASH=1 };
static const unsigned int MAXSET=100;

class ImpossibleOperation : public std::exception {
};

//===================================================================
template <typename TYPE> 
class IteratorVector : public Iterator<unsigned int> {
 public:
  IteratorVector(const TYPE &value, std::deque<TYPE> *vData, unsigned int minIndex):
    value(value),
    pos(minIndex),
    vData(vData),
    it(vData->begin()) {
    while (it!=(*vData).end() && (*it)!=value) {
      ++it;
      ++pos;
    }
  }
  bool hasNext() {
    return (pos<INT_MAX && it!=(*vData).end());
  }
  unsigned int next() {
    unsigned int tmp = pos;
    ++pos;
    ++it;
    while (it!=(*vData).end() && (*it)!=value) {
      ++it;
      ++pos;
    }
    return tmp;
  }
 private:
  const TYPE &value;
  unsigned int pos;
  std::deque<TYPE> *vData;
  typename std::deque<TYPE>::const_iterator it;
};
//===================================================================
template <typename TYPE> 
class IteratorHash : public Iterator<unsigned int> {
 public:
  IteratorHash(const TYPE &value, stdext::hash_map<unsigned int,TYPE> *hData):
    value(value),
    hData(hData) {
    it=(*hData).begin();
    while (it!=(*hData).end() && (*it).second!=value)
      ++it;
  }
  bool hasNext() {
    return (it!=(*hData).end());
  }
  unsigned int next() {
    unsigned int tmp = (*it).first;
    ++it;
    while (it!=(*hData).end() && (*it).second!=value)
      ++it;
    return tmp;
  }
 private:
  const TYPE &value;
  stdext::hash_map<unsigned int,TYPE> *hData;
  typename stdext::hash_map<unsigned int,TYPE>::const_iterator it;
};
//===================================================================
template <typename TYPE> 
class MutableContainer {
public:
  MutableContainer();
  ~MutableContainer();
  void setAll(const TYPE &value);
  void set(const unsigned int i,const TYPE &value);  
  typename ReturnType<TYPE>::Value get(const unsigned int i) const;
  Iterator<unsigned int>* findAll(const TYPE &value) throw (ImpossibleOperation);
private:
  void vecttohash();
  void hashtovect();
  void compress();
private:
  std::deque<TYPE> *vData;
  stdext::hash_map<unsigned int,TYPE> *hData;
  unsigned int minIndex,maxIndex;
  TYPE defaultValue;
  State state;
  unsigned int toCompress;
  unsigned int elementInserted;
  unsigned int phiP,phi;
  double ratio;
};
//===================================================================
//===================================================================
template<typename TYPE> 
MutableContainer<TYPE>::MutableContainer() {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  state=VECT;
  vData=new std::deque<TYPE>();
  hData=0;
  maxIndex=INT_MAX;
  minIndex=INT_MAX;
  toCompress=0;
  elementInserted=0;
  phiP=sizeof(void *);
  phi=sizeof(TYPE);
  ratio=double(phi) / (3.0*double(phiP)+double(phi));
}
//===================================================================
template <typename TYPE> 
MutableContainer<TYPE>::~MutableContainer() {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  switch (state) {
  case VECT: 
    delete vData;
    break;
  case HASH:
    delete hData;
    break;
  default:
    std::cerr << __PRETTY_FUNCTION__ << "unexpected state value (serious bug)" << std::endl;
    break;  
  }
}
//===================================================================
template <typename TYPE> 
void MutableContainer<TYPE>::setAll(const TYPE &value) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  switch (state) {
  case VECT: 
    delete vData;
    break;
  case HASH:
    delete hData;
    break;
  default:
    std::cerr << __PRETTY_FUNCTION__ << "unexpected state value (serious bug)" << std::endl;
    break; 
  }
  defaultValue=value;
  state=VECT;
  vData=new std::deque<TYPE>();
  maxIndex=INT_MAX;
  minIndex=INT_MAX;
  toCompress=0;
  elementInserted=0;
}
//===================================================================
template <typename TYPE> 
Iterator<unsigned int>* MutableContainer<TYPE>::findAll(const TYPE &value) throw (ImpossibleOperation) {
  if (value==defaultValue) 
    throw ImpossibleOperation();
  else {
    switch (state) {
    case VECT: 
      return new IteratorVector<TYPE>(value,vData,minIndex);
      break;
    case HASH:
      return new IteratorHash<TYPE>(value,hData);
      break;
    default:
      std::cerr << __PRETTY_FUNCTION__ << "unexpected state value (serious bug)" << std::endl;
      break; 
    }
  }
}
//===================================================================
template <typename TYPE> 
void MutableContainer<TYPE>::set(const unsigned int i,const TYPE &value) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  typename stdext::hash_map<unsigned int,TYPE>::iterator it;
  if (value==defaultValue) {
    switch (state) {
    case VECT : 
      if (i<=maxIndex && i>=minIndex) {
	if ((*vData)[i - minIndex] != defaultValue) {
	  elementInserted--;
	  (*vData)[i]= defaultValue;
	}
      }
      break;
    case HASH :
      if ((it=hData->find(i))!=hData->end()) {
	hData->erase(i);
	elementInserted--;
      }
      break;
    default:
      std::cerr << __PRETTY_FUNCTION__ << "unexpected state value (serious bug)" << std::endl;
      break; 
    }
  }
  else {
    switch (state) {
    case VECT :
      if (minIndex == INT_MAX) {
	minIndex = i;
	maxIndex = i;
	(*vData).push_back(value);
      }
      else {
	if ( i > maxIndex ) {
	  (*vData).resize(i+1 - minIndex, defaultValue);
	  maxIndex = i;
	}
	while ( i < minIndex ) {
	  (*vData).push_front(defaultValue);
	  minIndex--;
	}
	if ((*vData)[i - minIndex]==defaultValue) elementInserted++;
	(*vData)[i - minIndex] = value;
      }
      break;
    case HASH :
      if (hData->find(i)==hData->end()) elementInserted++;
      (*hData)[i]= value;
      break;
    default:
      std::cerr << __PRETTY_FUNCTION__ << "unexpected state value (serious bug)" << std::endl;
      break; 
    }
    if (maxIndex < i) maxIndex = i;// maxIndex= maxIndex >? i;
  }
  
  toCompress++;
  if (toCompress>MAXSET) {
    compress();
    toCompress=0;
  }
}
//===================================================================
//const TYPE &  MutableContainer<TYPE>::get(unsigned int i) const {
template <typename TYPE>   
typename ReturnType<TYPE>::Value MutableContainer<TYPE>::get(const unsigned int i) const {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  typename stdext::hash_map<unsigned int,TYPE>::iterator it;
  switch (state) {
  case VECT:
    if (i>maxIndex || i<minIndex) 
      return defaultValue;
    else 
      return ((*vData)[i - minIndex]);
    break;
  case HASH:
    if ((it=hData->find(i))!=hData->end())
      return (*it).second;
    else
      return defaultValue;
    break;
  default:
    std::cerr << __PRETTY_FUNCTION__ << "unexpected state value (serious bug)" << std::endl;
    return defaultValue;
    break;
  }
}
//===================================================================
template <typename TYPE> 
void MutableContainer<TYPE>::vecttohash() {
  //  std::cerr << __PRETTY_FUNCTION__ << std::endl << std::flush;
  hData=new stdext::hash_map<unsigned int,TYPE>(elementInserted);
  unsigned int newMaxIndex=0;
  unsigned int newMinIndex=INT_MAX;
  for (unsigned int i=minIndex;i<=maxIndex;++i) {
    if ((*vData)[i - minIndex]!=defaultValue) {
      (*hData)[i]=(*vData)[i - minIndex];
      if (newMaxIndex < i) newMaxIndex = i;// newMaxIndex = newMaxIndex >? i;
      if (newMinIndex > i) newMinIndex = i;// newMinIndex = newMinIndex <? i;
    }
  }
  maxIndex = newMaxIndex;
  minIndex = newMaxIndex;
  delete vData;
  state=HASH;
}
//===================================================================
template <typename TYPE> 
void MutableContainer<TYPE>::hashtovect() {
  //  std::cerr << __PRETTY_FUNCTION__ << std::endl << std::flush;
  vData=new std::deque<TYPE>(maxIndex - minIndex);
  typename stdext::hash_map<unsigned int,TYPE>::iterator it;
  unsigned int current=minIndex;
  for (it=hData->begin();it!=hData->end();++it) {
    for (;current<it->first;++current)
      (*vData)[current]=defaultValue;
    (*vData)[it->first]=it->second;
    ++current;
  }
  delete hData;
  state=VECT;
}
//===================================================================
template <typename TYPE> 
void MutableContainer<TYPE>::compress() {
  // cerr << __PRETTY_FUNCTION__ << endl;
  if (maxIndex==0) return;
  double limitValue= (ratio*(double(maxIndex - minIndex)+1.0f));
  switch (state) {
  case VECT: 
    if ( double(elementInserted) < limitValue) {
      vecttohash();
    }
    break;
  case HASH:
    if ( double(elementInserted) > limitValue)
      hashtovect();
    break;
  default:
    std::cerr << __PRETTY_FUNCTION__ << "unexpected state value (serious bug)" << std::endl;
    break; 
  }
}
//===================================================================
#endif
