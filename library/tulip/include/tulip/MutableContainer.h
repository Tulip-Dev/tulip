#ifndef _TLPMUTABLECONTAINER_
#define _TLPMUTABLECONTAINER_

#include <iostream>
#if (__GNUC__ < 3)
#include <hash_map>
#else
#include <ext/hash_map>
#endif
#include <deque>
#include <exception>
#include <iostream>
#include <string.h>
#include <assert.h>
#include <tulip/tulipconf.h>
#include <tulip/ReturnType.h>
#include <tulip/Iterator.h>

namespace tlp {

enum State { VECT=0, HASH=1 };

#ifndef DOXYGEN_NOTFOR_DEVEL
//===================================================================
 struct AnyValueContainer {
   AnyValueContainer() {}
 };

 template<typename TYPE> struct TypedValueContainer: public AnyValueContainer {
   TYPE value;
   TypedValueContainer() {}
   TypedValueContainer(const TYPE& val) : value(val) {}
 };
   
// add class to allow iteration on values
class IteratorValue: public Iterator<unsigned int> {
 public:
  IteratorValue() {}
  virtual ~IteratorValue() {}
  virtual unsigned int nextValue(AnyValueContainer&) = 0;
};
template <typename TYPE> 
class IteratorVector : public IteratorValue {
 public:
 IteratorVector(const TYPE &value, bool equal, std::deque<typename StoredValueType<TYPE>::Value> *vData, unsigned int minIndex):
   _value(value),
     _equal(equal),
     _pos(minIndex),
     vData(vData),
     it(vData->begin()) {
     while (it!=(*vData).end() &&
	    StoredValueType<TYPE>::equal((*it), _value) != _equal) {
       ++it;
       ++_pos;
     }
   }
   bool hasNext() {
    return (_pos<UINT_MAX && it!=(*vData).end());
  }
  unsigned int next() {
    unsigned int tmp = _pos;
    do {
      ++it;
      ++_pos;
    } while (it!=(*vData).end() &&
	     StoredValueType<TYPE>::equal((*it), _value) != _equal);
    return tmp;
  }
  unsigned int nextValue(AnyValueContainer& val) {
    ((TypedValueContainer<TYPE>&) val).value = StoredValueType<TYPE>::get(*it);
    unsigned int pos = _pos;
    do {
      ++it;
      ++_pos;
    } while (it!=(*vData).end() &&
	     StoredValueType<TYPE>::equal((*it), _value) != _equal);
    return pos;
  }
 private:
  const TYPE _value;
  bool _equal;
  unsigned int _pos;
  std::deque<typename StoredValueType<TYPE>::Value> *vData;
  typename std::deque<typename StoredValueType<TYPE>::Value>::const_iterator it;
};
//===================================================================
template <typename TYPE> 
class IteratorHash : public IteratorValue {
 public:
  IteratorHash(const TYPE &value, bool equal, stdext::hash_map<unsigned int, typename StoredValueType<TYPE>::Value> *hData):
    _value(value),
    _equal(equal),
    hData(hData) {
    it=(*hData).begin();
    while (it!=(*hData).end() &&
	   StoredValueType<TYPE>::equal((*it).second,_value) != _equal)
      ++it;
  }
  bool hasNext() {
    return (it!=(*hData).end());
  }
  unsigned int next() {
    unsigned int tmp = (*it).first;
    do {
      ++it;
    } while (it!=(*hData).end() &&
	     StoredValueType<TYPE>::equal((*it).second,_value) != _equal);
    return tmp;
  }
  unsigned nextValue(AnyValueContainer& val) {
    ((TypedValueContainer<TYPE>&) val).value =
      StoredValueType<TYPE>::get((*it).second);
    unsigned int pos = (*it).first;
    do {
      ++it;
    } while (it!=(*hData).end() &&
	     StoredValueType<TYPE>::equal((*it).second,_value) != _equal);
    return pos;
  }
 private:
  const TYPE _value;
  bool _equal;
  stdext::hash_map<unsigned int, typename StoredValueType<TYPE>::Value> *hData;
  typename stdext::hash_map<unsigned int, typename StoredValueType<TYPE>::Value>::const_iterator it;
};
#endif
//===================================================================
template <typename TYPE> 
class MutableContainer {
  friend class MutableContainerTest;
public:
  MutableContainer();
  ~MutableContainer();
  void setAll(const TYPE &value);
  void set(const unsigned int i,const TYPE &value);  
  /**
   * get the value associated to i 
   */
  typename ReturnType<TYPE>::ConstValue get(const unsigned int i) const;
  /**
   * get the value associated to i and indicates if it not the default one
   */
  typename ReturnType<TYPE>::Value get(const unsigned int i, bool& isNotDefault);
  /**
   * return a pointer on an iterator for all the elements whose associated value
   * if equal to a given value or different from the default value.
   * A null pointer is returned in case of an iteration on all the elements
   * whose value is equal to the default value.
   */
  IteratorValue* findAll(const TYPE &value, bool equal = true) const;
private:
  MutableContainer(const MutableContainer<TYPE> &){}
  void operator=(const MutableContainer<TYPE> &){}
  void vecttohash();
  void hashtovect();
  void compress(unsigned int min, unsigned int max, unsigned int nbElements);
  void vectset(const unsigned int i, typename StoredValueType<TYPE>::Value value);
 private:
  std::deque<typename StoredValueType<TYPE>::Value> *vData;
  stdext::hash_map<unsigned int, typename StoredValueType<TYPE>::Value> *hData;
  unsigned int minIndex,maxIndex;
  typename StoredValueType<TYPE>::Value defaultValue;
  State state;
  unsigned int elementInserted;
  double ratio;
  bool compressing;
};
//===================================================================
// define a template function to hold default value
template <typename TYPE> inline TYPE defaultTypeValue() {
  return TYPE();
}

// macro for basic types instantiation
#define DECL_DEFAULT_TYPE_VALUE(T)					\
template<> inline T defaultTypeValue<T>() {				\
  return (T) 0;								\
}

DECL_DEFAULT_TYPE_VALUE(void)

DECL_DEFAULT_TYPE_VALUE(bool)
DECL_DEFAULT_TYPE_VALUE(char)
DECL_DEFAULT_TYPE_VALUE(signed char)
DECL_DEFAULT_TYPE_VALUE(unsigned char)
DECL_DEFAULT_TYPE_VALUE(wchar_t)

DECL_DEFAULT_TYPE_VALUE(signed short)
DECL_DEFAULT_TYPE_VALUE(unsigned short)
DECL_DEFAULT_TYPE_VALUE(signed int)
DECL_DEFAULT_TYPE_VALUE(unsigned int)
DECL_DEFAULT_TYPE_VALUE(signed long)
DECL_DEFAULT_TYPE_VALUE(unsigned long)
#if LONGLONG_EXISTS
  DECL_DEFAULT_TYPE_VALUE(signed long long)
  DECL_DEFAULT_TYPE_VALUE(unsigned long long)
#endif  // LONGLONG_EXISTS

DECL_DEFAULT_TYPE_VALUE(float)
DECL_DEFAULT_TYPE_VALUE(double)
DECL_DEFAULT_TYPE_VALUE(long double)
//===================================================================
template<typename TYPE> 
  MutableContainer<TYPE>::MutableContainer(): vData(new std::deque<typename StoredValueType<TYPE>::Value>()),
  hData(NULL), minIndex(UINT_MAX), maxIndex(UINT_MAX), defaultValue(StoredValueType<TYPE>::copy(defaultTypeValue<TYPE>())), state(VECT), elementInserted(0),
   ratio(double(sizeof(TYPE)) / (3.0*double(sizeof(void *))+double(sizeof(TYPE)))),
   compressing(false)
     {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  /* state = VECT;
  vData = new std::deque<TYPE>();
  hData = 0;
  maxIndex = UINT_MAX;
  minIndex = UINT_MAX;
  elementInserted = 0;
  unsigned int phiP = sizeof(void *);
  unsigned int phi = sizeof(TYPE);
  ratio = double(phi) / (3.0*double(phiP)+double(phi));
  compressing = false; */
}
//===================================================================
template <typename TYPE> 
MutableContainer<TYPE>::~MutableContainer() {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  switch (state) {
  case VECT: 
    if (StoredValueType<TYPE>::isPointer) {
      // delete stored values
      typename std::deque<typename StoredValueType<TYPE>::Value>::const_iterator it =
	vData->begin();
      while (it!= vData->end()) {
	if ((*it) != defaultValue)
	  StoredValueType<TYPE>::destroy(*it);
	++it;
      }
    }
    delete vData; vData=0;
    break;
  case HASH:
    if (StoredValueType<TYPE>::isPointer) {
      // delete stored values
      typename stdext::hash_map<unsigned int, typename StoredValueType<TYPE>::Value>::const_iterator it = hData->begin();
      while (it!= hData->end()) {
	StoredValueType<TYPE>::destroy((*it).second);
	++it;
      }
    }
    delete hData; hData=0;
    break;
  default:
    assert(false);
    std::cerr << __PRETTY_FUNCTION__ << "unexpected state value (serious bug)" << std::endl;
    break;  
  }
  StoredValueType<TYPE>::destroy(defaultValue);
}
//===================================================================
template <typename TYPE> 
void MutableContainer<TYPE>::setAll(const TYPE &value) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  switch (state) {
  case VECT:
    if (StoredValueType<TYPE>::isPointer) {
      // delete stored values
      typename std::deque<typename StoredValueType<TYPE>::Value>::const_iterator it =
	vData->begin();
      while (it!= vData->end()) {
	if ((*it) != defaultValue)
	  StoredValueType<TYPE>::destroy(*it);
	++it;
      }
    }
    vData->clear();
    break;
  case HASH:
    if (StoredValueType<TYPE>::isPointer) {
      // delete stored values
      typename stdext::hash_map<unsigned int, typename StoredValueType<TYPE>::Value>::const_iterator it = hData->begin();
      while (it!= hData->end()) {
	StoredValueType<TYPE>::destroy((*it).second);
	++it;
      }
    }
    delete hData;hData=0;
    vData = new std::deque<typename StoredValueType<TYPE>::Value>();
    break;
  default:
    assert(false);
    std::cerr << __PRETTY_FUNCTION__ << "unexpected state value (serious bug)" << std::endl;
    break; 
  }
  StoredValueType<TYPE>::destroy(defaultValue);
  defaultValue = StoredValueType<TYPE>::copy(value);
  state = VECT;
  maxIndex = UINT_MAX;
  minIndex = UINT_MAX;
  elementInserted = 0;
}
//===================================================================
template <typename TYPE> 
  IteratorValue* MutableContainer<TYPE>::findAll(const TYPE &value,
						 bool equal) const {
  if (equal &&
      StoredValueType<TYPE>::equal(defaultValue, value))
    // error
    return NULL;
  else {
    switch (state) {
    case VECT: 
      return new IteratorVector<TYPE>(value, equal, vData, minIndex);
      break;
    case HASH:
      return new IteratorHash<TYPE>(value, equal, hData);
      break;
    default:
      assert(false);
      std::cerr << __PRETTY_FUNCTION__ << "unexpected state value (serious bug)" << std::endl;
      return 0;
      break; 
    }
  }
}
//===================================================================
template <typename TYPE> 
void MutableContainer<TYPE>::set(const unsigned int i, const TYPE &value) {
  //  cerr << __PRETTY_FUNCTION__ << endl;

  //Test if after insertion we need to resize
  if (!compressing &&
      !StoredValueType<TYPE>::equal(defaultValue, value)) {
    compressing = true;
    compress (std::min(i,minIndex), std::max(i,maxIndex), elementInserted);
    compressing = false;
  }
  
  if (StoredValueType<TYPE>::equal(defaultValue, value)) {
    typename stdext::hash_map<unsigned int, typename StoredValueType<TYPE>::Value>::iterator it;
    switch (state) {
    case VECT : 
      if (i<=maxIndex && i>=minIndex) {
	typename StoredValueType<TYPE>::Value val = (*vData)[i - minIndex];
	if (val != defaultValue) {
	  (*vData)[i - minIndex]= defaultValue;
	  StoredValueType<TYPE>::destroy(val);
	  --elementInserted;
	}
      }
      break;
    case HASH :
      if ((it=hData->find(i)) != hData->end()) {
	StoredValueType<TYPE>::destroy((*it).second);
	hData->erase(i);
	--elementInserted;
      }
      break;
    default:
      assert(false);
      std::cerr << __PRETTY_FUNCTION__ << "unexpected state value (serious bug)" << std::endl;
      break; 
    }
  }
  else {
    typename stdext::hash_map<unsigned int, typename StoredValueType<TYPE>::Value>::iterator it;
    typename StoredValueType<TYPE>::Value newVal =
      StoredValueType<TYPE>::copy(value);
   switch (state) {
    case VECT :
      if (minIndex == UINT_MAX) {
	minIndex = i;
	maxIndex = i;
	(*vData).push_back(newVal);
	++elementInserted;
      }
      else {
	/*	if ( i > maxIndex ) {
	  (*vData).resize(i+1 - minIndex, defaultValue);
	  maxIndex = i;
	  }*/
	while ( i > maxIndex ) {
	  (*vData).push_back(defaultValue);
	  ++maxIndex;
	}
	while ( i < minIndex ) {
	  (*vData).push_front(defaultValue);
	  --minIndex;
	}
	typename StoredValueType<TYPE>::Value val = (*vData)[i - minIndex];
	(*vData)[i - minIndex] = newVal;
	if (val != defaultValue)
	  StoredValueType<TYPE>::destroy(val);
	else
	  ++elementInserted;
      }
      break;
    case HASH :
      if ((it=hData->find(i)) != hData->end())
	StoredValueType<TYPE>::destroy((*it).second);
      else
	++elementInserted;
      (*hData)[i]= newVal;
      break;
    default:
      assert(false);
      std::cerr << __PRETTY_FUNCTION__ << "unexpected state value (serious bug)" << std::endl;
      break; 
    }
    maxIndex = std::max(maxIndex, i);
    minIndex = std::min(minIndex, i);
  }
}
//===================================================================
template <typename TYPE> 
void MutableContainer<TYPE>::vectset(const unsigned int i,
				     typename StoredValueType<TYPE>::Value value) {
  assert(value != defaultValue);

  if (minIndex == UINT_MAX) {
    minIndex = i;
    maxIndex = i;
    (*vData).push_back(value);
    ++elementInserted;
  }
  else {
    while ( i > maxIndex ) {
      (*vData).push_back(defaultValue);
      ++maxIndex;
    }
    while ( i < minIndex ) {
      (*vData).push_front(defaultValue);
      --minIndex;
    }
    typename StoredValueType<TYPE>::Value val = (*vData)[i - minIndex];
    (*vData)[i - minIndex] = value;
    if (val != defaultValue)
      StoredValueType<TYPE>::destroy(val);
    else
      ++elementInserted;
  }
  maxIndex = std::max(maxIndex, i);
  minIndex = std::min(minIndex, i);
}
//===================================================================
//const TYPE &  MutableContainer<TYPE>::get(unsigned int i) const {
template <typename TYPE>   
typename ReturnType<TYPE>::ConstValue MutableContainer<TYPE>::get(const unsigned int i) const {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  if (maxIndex == UINT_MAX) return StoredValueType<TYPE>::get(defaultValue);
  typename stdext::hash_map<unsigned int, typename StoredValueType<TYPE>::Value>::iterator it;
  switch (state) {
  case VECT:
    if (i>maxIndex || i<minIndex) 
      return StoredValueType<TYPE>::get(defaultValue);
    else 
      return StoredValueType<TYPE>::get((*vData)[i - minIndex]);
    break;
  case HASH:
    if ((it=hData->find(i))!=hData->end())
      return StoredValueType<TYPE>::get((*it).second);
    else
      return StoredValueType<TYPE>::get(defaultValue);
    break;
  default:
    assert(false);
    std::cerr << __PRETTY_FUNCTION__ << "unexpected state value (serious bug)" << std::endl;
    return StoredValueType<TYPE>::get(defaultValue);
    break;
  }
}
//===================================================================
template <typename TYPE>   
  typename ReturnType<TYPE>::Value MutableContainer<TYPE>::get(const unsigned int i, bool& notDefault) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  if (maxIndex == UINT_MAX) {
    notDefault = false;
    return StoredValueType<TYPE>::get(defaultValue);
  }
  typename stdext::hash_map<unsigned int, typename StoredValueType<TYPE>::Value>::iterator it;
  switch (state) {
  case VECT:
    if (i>maxIndex || i<minIndex) {
      notDefault = false;
      return StoredValueType<TYPE>::get(defaultValue);
    } else {
      notDefault = true;
      return StoredValueType<TYPE>::get((*vData)[i - minIndex]);
    }
  case HASH:
    if ((it=hData->find(i))!=hData->end()) {
      notDefault = true;
      return StoredValueType<TYPE>::get((*it).second);
    } else {
      notDefault = false;
      return StoredValueType<TYPE>::get(defaultValue);
    }
  default:
    assert(false);
    notDefault = false;
    std::cerr << __PRETTY_FUNCTION__ << "unexpected state value (serious bug)" << std::endl;
    return StoredValueType<TYPE>::get(defaultValue);
  }
}
//===================================================================
template <typename TYPE> 
void MutableContainer<TYPE>::vecttohash() {
  //  std::cerr << __FUNCTION__ << std::endl << std::flush;
  hData=new stdext::hash_map<unsigned int, typename StoredValueType<TYPE>::Value>(elementInserted);

  unsigned int newMaxIndex = 0;
  unsigned int newMinIndex = UINT_MAX;
  elementInserted = 0;
  for (unsigned int i=minIndex; i<=maxIndex; ++i) {
    if ((*vData)[i - minIndex] != defaultValue) {
      (*hData)[i] = (*vData)[i - minIndex];
      newMaxIndex = std::max(newMaxIndex, i);
      newMinIndex = std::min(newMinIndex, i);
      ++elementInserted;
    }
  }
  maxIndex = newMaxIndex;
  minIndex = newMinIndex;
  delete vData;vData = 0;
  state = HASH;
}
//===================================================================
template <typename TYPE> 
void MutableContainer<TYPE>::hashtovect() {
  //  std::cerr << __FUNCTION__ << std::endl << std::flush;
  vData = new std::deque<typename StoredValueType<TYPE>::Value>();
  minIndex = UINT_MAX;
  maxIndex = UINT_MAX;
  elementInserted = 0;
  state=VECT;
  typename stdext::hash_map<unsigned int, typename StoredValueType<TYPE>::Value>::const_iterator it;
  for (it=hData->begin(); it!=hData->end(); ++it) {
    if (it->second  != defaultValue)
      vectset(it->first, it->second);
  }
  delete hData; hData = 0;
}
//===================================================================
template <typename TYPE> 
void MutableContainer<TYPE>::compress(unsigned int min, unsigned int max, unsigned int nbElements) {
  //  std::cerr << __PRETTY_FUNCTION__ << std::endl;
  if (max == UINT_MAX || (max - min) < 10) return;
  double limitValue = ratio*(double(max - min + 1.0));

  switch (state) {
  case VECT: 
    if ( double(nbElements) < limitValue) {
      /*
	std::cerr << "****************************************************" << std::endl;
	std::cerr << "min : " << min << " max :" << max << " lim : ";
	std::cerr  << limitValue << " el : " << elementInserted << std::endl;
	std::cerr << "minId : " << minIndex << " maxId :" << maxIndex << " lim : ";
	std::cerr  << limitValue << " el : " << elementInserted << std::endl;
      */
      vecttohash();
      /*
	std::cerr << "minId : " << minIndex << " maxId :" << maxIndex << " lim : ";
	std::cerr  << limitValue << " el : " << elementInserted << std::endl;
	std::cerr << "****************************************************" << std::endl;
	sleep(1);
      */
    }
    break;
  case HASH:
    if ( double(nbElements) > limitValue*1.5) {
      /*
	std::cerr << "****************************************************" << std::endl;
	std::cerr << "min : " << min << " max :" << max << " lim : ";
	std::cerr  << limitValue << " el : " << elementInserted << std::endl;
	std::cerr << "minId : " << minIndex << " maxId :" << maxIndex << " lim : ";
	std::cerr  << limitValue << " el : " << elementInserted << std::endl;
      */
      hashtovect();
      /*
	std::cerr << "minId : " << minIndex << " maxId :" << maxIndex << " lim : ";
	std::cerr  << limitValue << " el : " << elementInserted << std::endl;
	std::cerr << "****************************************************" << std::endl;
	sleep(1);
      */
    }
    break;
  default:
    assert(false);
    std::cerr << __PRETTY_FUNCTION__ << "unexpected state value (serious bug)" << std::endl;
    break; 
  }
}
//===================================================================

}
#endif
