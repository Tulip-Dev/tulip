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
//===================================================================
template <typename TYPE>
tlp::MutableContainer<TYPE>::MutableContainer(): vData(new std::deque<typename StoredType<TYPE>::Value>()),
  hData(NULL), minIndex(UINT_MAX), maxIndex(UINT_MAX), defaultValue(StoredType<TYPE>::defaultValue()), state(VECT), elementInserted(0),
  ratio(double(sizeof(typename tlp::StoredType<TYPE>::Value)) / (3.0*double(sizeof(void *))+double(sizeof(typename tlp::StoredType<TYPE>::Value)))),
  compressing(false) {
}
//===================================================================
template <typename TYPE>
tlp::MutableContainer<TYPE>::~MutableContainer() {
  switch (state) {
  case VECT:

    if (StoredType<TYPE>::isPointer) {
      // delete stored values
      typename std::deque<typename StoredType<TYPE>::Value>::const_iterator it =
        vData->begin();

      while (it!= vData->end()) {
        if ((*it) != defaultValue)
          StoredType<TYPE>::destroy(*it);

        ++it;
      }
    }

    delete vData;
    vData = NULL;
    break;

  case HASH:

    if (StoredType<TYPE>::isPointer) {
      // delete stored values
      typename TLP_HASH_MAP<unsigned int, typename StoredType<TYPE>::Value>::const_iterator it = hData->begin();

      while (it!= hData->end()) {
        StoredType<TYPE>::destroy((*it).second);
        ++it;
      }
    }

    delete hData;
    hData = NULL;
    break;

  default:
    assert(false);
    tlp::error() << __PRETTY_FUNCTION__ << "unexpected state value (serious bug)" << std::endl;
    break;
  }

  StoredType<TYPE>::destroy(defaultValue);
}
//===================================================================
template <typename TYPE>
void tlp::MutableContainer<TYPE>::setAll(const TYPE &value) {
  switch (state) {
  case VECT:

    if (StoredType<TYPE>::isPointer) {
      // delete stored values
      typename std::deque<typename StoredType<TYPE>::Value>::const_iterator it =
        vData->begin();

      while (it!= vData->end()) {
        if ((*it) != defaultValue)
          StoredType<TYPE>::destroy(*it);

        ++it;
      }
    }

    vData->clear();
    break;

  case HASH:

    if (StoredType<TYPE>::isPointer) {
      // delete stored values
      typename TLP_HASH_MAP<unsigned int, typename StoredType<TYPE>::Value>::const_iterator it = hData->begin();

      while (it!= hData->end()) {
        StoredType<TYPE>::destroy((*it).second);
        ++it;
      }
    }

    delete hData;
    hData = NULL;
    vData = new std::deque<typename StoredType<TYPE>::Value>();
    break;

  default:
    assert(false);
    tlp::error() << __PRETTY_FUNCTION__ << "unexpected state value (serious bug)" << std::endl;
    break;
  }

  StoredType<TYPE>::destroy(defaultValue);
  defaultValue = StoredType<TYPE>::clone(value);
  state = VECT;
  maxIndex = UINT_MAX;
  minIndex = UINT_MAX;
  elementInserted = 0;
}
//===================================================================
// this method is private and used as is by GraphUpdatesRecorder class
// it is also used to implement findAll
template <typename TYPE>
tlp::IteratorValue* tlp::MutableContainer<TYPE>::findAllValues(const TYPE &value,
    bool equal) const {
  if (equal &&
      StoredType<TYPE>::equal(defaultValue, value))
    // error
    return NULL;
  else {
    switch (state) {
    case VECT:
      return new IteratorVect<TYPE>(value, equal, vData, minIndex);
      break;

    case HASH:
      return new IteratorHash<TYPE>(value, equal, hData);
      break;

    default:
      assert(false);
      tlp::error() << __PRETTY_FUNCTION__ << "unexpected state value (serious bug)" << std::endl;
      return NULL;
    }
  }
}
//===================================================================
// this method is visible for any class
template <typename TYPE>
tlp::Iterator<unsigned int>* tlp::MutableContainer<TYPE>::findAll(const TYPE &value,
    bool equal) const {
  return findAllValues(value, equal);
}
//===================================================================
template <typename TYPE>
void tlp::MutableContainer<TYPE>::vectset(const unsigned int i,
    typename StoredType<TYPE>::Value value) {
  if (minIndex == UINT_MAX) {
    minIndex = i;
    maxIndex = i;
    (*vData).push_back(value);
    ++elementInserted;
  }
  else {
    // the time performance of these two attempts of nicer coding
    // in this commented code seems worse than the loops below (about 15%)
    // if ( i > maxIndex ) {
    //-- 1st attempt --
    //   vData->resize(i+1 - minIndex, defaultValue);
    //-- 2nd attempt
    //   vData->insert(vData->end(), i - maxIndex, defaultValue);
    //   maxIndex = i;
    // } else if (i < minIndex) {
    //   vData->insert(vData->begin(), minIndex - i, defaultValue);
    //   minIndex = i;
    // }
    while ( i > maxIndex ) {
      (*vData).push_back(defaultValue);
      ++maxIndex;
    }

    while ( i < minIndex ) {
      (*vData).push_front(defaultValue);
      --minIndex;
    }

    typename StoredType<TYPE>::Value val = (*vData)[i - minIndex];
    (*vData)[i - minIndex] = value;

    if (val != defaultValue)
      StoredType<TYPE>::destroy(val);
    else
      ++elementInserted;
  }
}
//===================================================================
template <typename TYPE>
void tlp::MutableContainer<TYPE>::set(const unsigned int i, const TYPE &value) {
  //Test if after insertion we need to resize
  if (!compressing &&
      !StoredType<TYPE>::equal(defaultValue, value)) {
    compressing = true;
    compress (std::min(i,minIndex), std::max(i,maxIndex), elementInserted);
    compressing = false;
  }

  if (StoredType<TYPE>::equal(defaultValue, value)) {

    switch (state) {
    case VECT :

      if (i<=maxIndex && i>=minIndex) {
        typename StoredType<TYPE>::Value val = (*vData)[i - minIndex];

        if (val != defaultValue) {
          (*vData)[i - minIndex]= defaultValue;
          StoredType<TYPE>::destroy(val);
          --elementInserted;
        }
      }

      return;

    case HASH : {
      typename TLP_HASH_MAP<unsigned int, typename StoredType<TYPE>::Value>::iterator it = hData->find(i);

      if (it!=hData->end()) {
        StoredType<TYPE>::destroy((*it).second);
        hData->erase(i);
        --elementInserted;
      }

      break;
    }

    default:
      assert(false);
      tlp::error() << __PRETTY_FUNCTION__ << "unexpected state value (serious bug)" << std::endl;
      break;
    }
  }
  else {
    typename StoredType<TYPE>::Value newVal =
      StoredType<TYPE>::clone(value);

    switch (state) {
    case VECT :

      vectset(i, newVal);

      return;

    case HASH : {
      typename TLP_HASH_MAP<unsigned int, typename StoredType<TYPE>::Value>::iterator it = hData->find(i);

      if (it!=hData->end())
        StoredType<TYPE>::destroy((*it).second);
      else
        ++elementInserted;

      (*hData)[i]= newVal;
      break;
    }

    default:
      assert(false);
      tlp::error() << __PRETTY_FUNCTION__ << "unexpected state value (serious bug)" << std::endl;
      break;
    }

    maxIndex = std::max(maxIndex, i);
    minIndex = std::min(minIndex, i);
  }
}
//===================================================================
template <typename TYPE>
void tlp::MutableContainer<TYPE>::add(const unsigned int i, TYPE val) {
  if (tlp::StoredType<TYPE>::isPointer == false) {
    if (maxIndex == UINT_MAX) {
      assert(state == VECT);
      minIndex = i;
      maxIndex = i;
      (*vData).push_back(defaultValue + val);
      ++elementInserted;
      return;
    }

    switch (state) {
    case VECT: {
      if (i > maxIndex || i < minIndex) {
        set(i, defaultValue + val);
        return;
      }

      TYPE& oldVal = (*vData)[i - minIndex];

      if (oldVal == defaultValue) {
        set(i, defaultValue + val);
        return;
      }

      oldVal += val;

      return;
    }

    case HASH: {
      typename TLP_HASH_MAP<unsigned int, typename StoredType<TYPE>::Value>::iterator it = hData->find(i);

      if (it!=hData->end()) {
        // check default value
        if ((it->second + val) == defaultValue) {
          StoredType<TYPE>::destroy((*it).second);
          hData->erase(i);
          --elementInserted;
        }
        else
          it->second += val;
      }
      else {
        set(i, defaultValue + val);
      }

      return;
    }

    default:
      assert(false);
      std::cerr << __PRETTY_FUNCTION__ << "unexpected state value (serious bug)" << std::endl;
    }
  }

  assert(false);
  std::cerr << __PRETTY_FUNCTION__ << "not implemented" << std::endl;
}
//===================================================================
template <typename TYPE>
typename tlp::StoredType<TYPE>::ReturnedConstValue tlp::MutableContainer<TYPE>::get(const unsigned int i) const {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  if (maxIndex == UINT_MAX) return StoredType<TYPE>::get(defaultValue);

  switch (state) {
  case VECT:

    if (i>maxIndex || i<minIndex)
      return StoredType<TYPE>::get(defaultValue);
    else
      return StoredType<TYPE>::get((*vData)[i - minIndex]);

  case HASH: {
    typename TLP_HASH_MAP<unsigned int, typename StoredType<TYPE>::Value>::iterator it = hData->find(i);

    if (it!=hData->end())
      return StoredType<TYPE>::get((*it).second);
    else
      return StoredType<TYPE>::get(defaultValue);
  }

  default:
    assert(false);
    tlp::error() << __PRETTY_FUNCTION__ << "unexpected state value (serious bug)" << std::endl;
    return StoredType<TYPE>::get(defaultValue);
    break;
  }
}
//===================================================================
template <typename TYPE>
typename tlp::StoredType<TYPE>::ReturnedValue tlp::MutableContainer<TYPE>::getDefault() const {
  return StoredType<TYPE>::get(defaultValue);
}
//===================================================================
template <typename TYPE>
bool tlp::MutableContainer<TYPE>::hasNonDefaultValue(const unsigned int i) const {
  if (maxIndex == UINT_MAX) return false;

  switch (state) {
  case VECT:
    return (i<=maxIndex && i>=minIndex &&
            (((*vData)[i - minIndex]) != defaultValue));

  case HASH:
    return ((hData->find(i))!=hData->end());

  default:
    assert(false);
    tlp::error() << __PRETTY_FUNCTION__ << "unexpected state value (serious bug)" << std::endl;
    return false;
  }
}
//===================================================================
template <typename TYPE>
typename tlp::StoredType<TYPE>::ReturnedValue tlp::MutableContainer<TYPE>::get(const unsigned int i, bool& notDefault) const {
  if (maxIndex == UINT_MAX) {
    notDefault = false;
    return StoredType<TYPE>::get(defaultValue);
  }

  switch (state) {
  case VECT:

    if (i>maxIndex || i<minIndex) {
      notDefault = false;
      return StoredType<TYPE>::get(defaultValue);
    }
    else {
      typename StoredType<TYPE>::Value val = (*vData)[i - minIndex];
      notDefault = val != defaultValue;
      return StoredType<TYPE>::get(val);
    }

  case HASH: {
    typename TLP_HASH_MAP<unsigned int, typename StoredType<TYPE>::Value>::iterator it = hData->find(i);

    if (it!=hData->end()) {
      notDefault = true;
      return StoredType<TYPE>::get((*it).second);
    }
    else {
      notDefault = false;
      return StoredType<TYPE>::get(defaultValue);
    }
  }

  default:
    assert(false);
    notDefault = false;
    tlp::error() << __PRETTY_FUNCTION__ << "unexpected state value (serious bug)" << std::endl;
    return StoredType<TYPE>::get(defaultValue);
  }
}
//===================================================================
template <typename TYPE>
unsigned int tlp::MutableContainer<TYPE>::numberOfNonDefaultValues() const {
  return elementInserted;
}
//===================================================================
template <typename TYPE>
void tlp::MutableContainer<TYPE>::vecttohash() {
  hData=new TLP_HASH_MAP<unsigned int, typename StoredType<TYPE>::Value>(elementInserted);

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
  delete vData;
  vData = NULL;
  state = HASH;
}
//===================================================================
template <typename TYPE>
void tlp::MutableContainer<TYPE>::hashtovect() {
  vData = new std::deque<typename StoredType<TYPE>::Value>();
  minIndex = UINT_MAX;
  maxIndex = UINT_MAX;
  elementInserted = 0;
  state=VECT;
  typename TLP_HASH_MAP<unsigned int, typename StoredType<TYPE>::Value>::const_iterator it;

  for (it=hData->begin(); it!=hData->end(); ++it) {
    if (it->second  != defaultValue)
      vectset(it->first, it->second);
  }

  delete hData;
  hData = NULL;
}
//===================================================================
template <typename TYPE>
void tlp::MutableContainer<TYPE>::compress(unsigned int min, unsigned int max, unsigned int nbElements) {
  if (max == UINT_MAX || (max - min) < 10) return;

  double limitValue = ratio*(double(max - min + 1.0));

  switch (state) {
  case VECT:

    if ( double(nbElements) < limitValue) {
      vecttohash();
    }

    break;

  case HASH:

    if ( double(nbElements) > limitValue*1.5) {
      hashtovect();
    }

    break;

  default:
    assert(false);
    tlp::error() << __PRETTY_FUNCTION__ << "unexpected state value (serious bug)" << std::endl;
    break;
  }
}

template <typename TYPE>
typename tlp::StoredType<TYPE>::ReturnedConstValue tlp::MutableContainer<TYPE>::operator[](const unsigned int i) const {
  return get(i);
}
