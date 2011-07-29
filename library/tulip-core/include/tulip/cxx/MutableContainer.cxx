//===================================================================
template<typename TYPE>
tlp::MutableContainer<TYPE>::MutableContainer(): vData(new std::deque<typename StoredType<TYPE>::Value>()),
  hData(NULL), minIndex(UINT_MAX), maxIndex(UINT_MAX), defaultValue(StoredType<TYPE>::defaultValue()), state(VECT), elementInserted(0),
  ratio(double(sizeof(TYPE)) / (3.0*double(sizeof(void *))+double(sizeof(TYPE)))),
  compressing(false) {
}
//===================================================================
template <typename TYPE>
tlp::MutableContainer<TYPE>::~MutableContainer() {
  //  cerr << __PRETTY_FUNCTION__ << endl;
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
    vData=0;
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
    hData=0;
    break;
  default:
    assert(false);
    std::cerr << __PRETTY_FUNCTION__ << "unexpected state value (serious bug)" << std::endl;
    break;
  }

  StoredType<TYPE>::destroy(defaultValue);
}
//===================================================================
template <typename TYPE>
void tlp::MutableContainer<TYPE>::setAll(const TYPE &value) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
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
    hData=0;
    vData = new std::deque<typename StoredType<TYPE>::Value>();
    break;
  default:
    assert(false);
    std::cerr << __PRETTY_FUNCTION__ << "unexpected state value (serious bug)" << std::endl;
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
      std::cerr << __PRETTY_FUNCTION__ << "unexpected state value (serious bug)" << std::endl;
      return 0;
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
void tlp::MutableContainer<TYPE>::set(const unsigned int i, const TYPE &value) {
  //  cerr << __PRETTY_FUNCTION__ << endl;

  //Test if after insertion we need to resize
  if (!compressing &&
      !StoredType<TYPE>::equal(defaultValue, value)) {
    compressing = true;
    compress (std::min(i,minIndex), std::max(i,maxIndex), elementInserted);
    compressing = false;
  }

  if (StoredType<TYPE>::equal(defaultValue, value)) {
    typename TLP_HASH_MAP<unsigned int, typename StoredType<TYPE>::Value>::iterator it;

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

      break;
    case HASH :

      if ((it=hData->find(i)) != hData->end()) {
        StoredType<TYPE>::destroy((*it).second);
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
    typename TLP_HASH_MAP<unsigned int, typename StoredType<TYPE>::Value>::iterator it;
    typename StoredType<TYPE>::Value newVal =
      StoredType<TYPE>::clone(value);

    switch (state) {
    case VECT :

      if (minIndex == UINT_MAX) {
        minIndex = i;
        maxIndex = i;
        (*vData).push_back(newVal);
        ++elementInserted;
      }
      else {
        /*  if ( i > maxIndex ) {
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

        typename StoredType<TYPE>::Value val = (*vData)[i - minIndex];
        (*vData)[i - minIndex] = newVal;

        if (val != defaultValue)
          StoredType<TYPE>::destroy(val);
        else
          ++elementInserted;
      }

      break;
    case HASH :

      if ((it=hData->find(i)) != hData->end())
        StoredType<TYPE>::destroy((*it).second);
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
void tlp::MutableContainer<TYPE>::vectset(const unsigned int i,
    typename StoredType<TYPE>::Value value) {
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

    typename StoredType<TYPE>::Value val = (*vData)[i - minIndex];
    (*vData)[i - minIndex] = value;

    if (val != defaultValue)
      StoredType<TYPE>::destroy(val);
    else
      ++elementInserted;
  }

  maxIndex = std::max(maxIndex, i);
  minIndex = std::min(minIndex, i);
}
//===================================================================
//const TYPE &  MutableContainer<TYPE>::get(unsigned int i) const {
template <typename TYPE>
typename tlp::StoredType<TYPE>::ReturnedConstValue tlp::MutableContainer<TYPE>::get(const unsigned int i) const {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  if (maxIndex == UINT_MAX) return StoredType<TYPE>::get(defaultValue);

  typename TLP_HASH_MAP<unsigned int, typename StoredType<TYPE>::Value>::iterator it;

  switch (state) {
  case VECT:

    if (i>maxIndex || i<minIndex)
      return StoredType<TYPE>::get(defaultValue);
    else
      return StoredType<TYPE>::get((*vData)[i - minIndex]);

    break;
  case HASH:

    if ((it=hData->find(i))!=hData->end())
      return StoredType<TYPE>::get((*it).second);
    else
      return StoredType<TYPE>::get(defaultValue);

    break;
  default:
    assert(false);
    std::cerr << __PRETTY_FUNCTION__ << "unexpected state value (serious bug)" << std::endl;
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
    std::cerr << __PRETTY_FUNCTION__ << "unexpected state value (serious bug)" << std::endl;
    return false;
  }
}
//===================================================================
template <typename TYPE>
typename tlp::StoredType<TYPE>::ReturnedValue tlp::MutableContainer<TYPE>::get(const unsigned int i, bool& notDefault) const {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  if (maxIndex == UINT_MAX) {
    notDefault = false;
    return StoredType<TYPE>::get(defaultValue);
  }

  typename TLP_HASH_MAP<unsigned int, typename StoredType<TYPE>::Value>::iterator it;

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

  case HASH:

    if ((it=hData->find(i))!=hData->end()) {
      notDefault = true;
      return StoredType<TYPE>::get((*it).second);
    }
    else {
      notDefault = false;
      return StoredType<TYPE>::get(defaultValue);
    }

  default:
    assert(false);
    notDefault = false;
    std::cerr << __PRETTY_FUNCTION__ << "unexpected state value (serious bug)" << std::endl;
    return StoredType<TYPE>::get(defaultValue);
  }
}
//===================================================================
template <typename TYPE>
void tlp::MutableContainer<TYPE>::vecttohash() {
  //  std::cerr << __FUNCTION__ << std::endl << std::flush;
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
  vData = 0;
  state = HASH;
}
//===================================================================
template <typename TYPE>
void tlp::MutableContainer<TYPE>::hashtovect() {
  //  std::cerr << __FUNCTION__ << std::endl << std::flush;
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
  hData = 0;
}
//===================================================================
template <typename TYPE>
void tlp::MutableContainer<TYPE>::compress(unsigned int min, unsigned int max, unsigned int nbElements) {
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

template <typename TYPE>
typename tlp::StoredType<TYPE>::ReturnedConstValue tlp::MutableContainer<TYPE>::operator[](const unsigned int i) const {
  return get(i);
}
