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


using namespace tlp;

template<typename T>
void GraphTableModel::removeFromVector(const std::set<T>& objects,std::vector<T>& vect,TLP_HASH_MAP<T,int>& objToIndexes,bool deleteRows) {
  //Sort from the greatest to the smallest to ensure index are not invalidate during the destruction process
  std::set<int, std::greater<int> > indexes;

  for( typename std::set<T>::const_iterator it = objects.begin(); it != objects.end(); ++it) {
    assert(objToIndexes.find(*it)!=objToIndexes.end());
    indexes.insert(objToIndexes[*it]);
    objToIndexes.erase(*it);
  }

  while(!indexes.empty()) {
    //Compute the greatest range of successive indexes.
    std::set<int, std::greater<int> >::const_iterator from = indexes.begin();

    std::set<int, std::greater<int> >::const_iterator current = from;
    std::set<int, std::greater<int> >::const_iterator to=from;
    ++to;

    //If the indexes are successive delete them at the same time.
    while(to != indexes.end() && *to == (*current)-1 ) {
      current = to;
      ++to;
    }

    //Range to destruct
    int fromIndex = *current;
    int toIndex = *from;

    //Remove the range from the index list
    indexes.erase(from,to);

    if(deleteRows) {
      beginRemoveRows(QModelIndex(),fromIndex,toIndex);
    }
    else {
      beginRemoveColumns(QModelIndex(),fromIndex,toIndex);
    }

    //Erase data
    vect.erase(vect.begin()+fromIndex,vect.begin()+toIndex+1);

    //Update objToIndex map
    for(unsigned int i = fromIndex ; i < vect.size() ; ++i) {
      objToIndexes[vect[i]] = static_cast<int>(i);
    }

    if(deleteRows) {
      endRemoveRows();
    }
    else {
      endRemoveColumns();
    }
  }
}

template<typename T, class Compare>
void GraphTableModel::addToVector(const std::set<T>& objects,std::vector<T>& vect,TLP_HASH_MAP<T,int>& objToIndex,bool addRows, Compare* comp) {
  //Reserve size
  if(vect.capacity() <= vect.size() + objects.size()) {
    vect.reserve(vect.size() + objects.size());
  }

  if(comp == NULL) {
    //If there is no comparator append the elements to add at the end of the vector
    if(addRows) {
      beginInsertRows(QModelIndex(),vect.size(),vect.size()+objects.size()-1);
    }
    else {
      beginInsertColumns(QModelIndex(),vect.size(),vect.size()+objects.size()-1);
    }

    for(typename std::set<T>::const_iterator it = objects.begin(); it != objects.end() ; ++it) {
      unsigned int i = vect.size();
      objToIndex[*it]=i;
      vect.push_back((*it));
    }

    if(addRows) {
      endInsertRows();
    }
    else {
      endInsertColumns();
    }
  }
  else {
    //Order objects
    std::vector<T> sortedObjects(objects.begin(),objects.end());
    std::sort(sortedObjects.begin(),sortedObjects.end(),*comp);
    unsigned int current = 0;
    typename std::vector<T>::iterator it;

    for( it = vect.begin() , current = 0 ; it != vect.end() && !sortedObjects.empty() ; ++it, ++current) {
      //First element greater than the element to insert
      if(!(*comp)(*it,sortedObjects.front())) {
        //Compute the elements to insert in the vector
        typename std::vector<T>::iterator to = sortedObjects.begin()+1;
        unsigned int insertedElementsCount = 1;

        //While there is more elements to insert at this position
        while(to != sortedObjects.end() && !(*comp)(*it,*to)) {
          ++to;
          ++insertedElementsCount;
        }

        //Notify Qt model
        if(addRows) {
          beginInsertRows(QModelIndex(),current,current+insertedElementsCount-1);
        }
        else {
          beginInsertColumns(QModelIndex(),current,current+insertedElementsCount-1);
        }

        //Update data
        vect.insert(it,sortedObjects.begin(),to);

        //Update data index
        for(unsigned int i = current ; i < vect.size() ; ++i) {
          objToIndex[vect[i]]=i;
        }

        if(addRows) {
          endInsertRows();
        }
        else {
          endInsertColumns();
        }

        //Erase added elements.
        sortedObjects.erase(sortedObjects.begin(),to);
        it = vect.begin()+current;
      }
    }

    //If there is some elements to add insert them at the end of the data.
    if(!sortedObjects.empty()) {
      if(addRows) {
        beginInsertRows(QModelIndex(),vect.size(),vect.size()+sortedObjects.size()-1);
      }
      else {
        beginInsertColumns(QModelIndex(),vect.size(),vect.size()+sortedObjects.size()-1);
      }

      size_t from = vect.size();
      //Update data
      vect.insert(vect.end(),sortedObjects.begin(),sortedObjects.end());

      //Update data index
      for(unsigned int i = from ; i < vect.size() ; ++i) {
        objToIndex[vect[i]]=i;
      }

      if(addRows) {
        endInsertRows();
      }
      else {
        endInsertColumns();
      }
    }
  }
}

template<typename T>
void GraphTableModel::updateIndexMap(const std::vector<T>& table,TLP_HASH_MAP<T,int>& indexMap) {
  indexMap.clear();

  for(size_t i = 0 ; i < table.size(); ++i) {
    indexMap[table[i]]=i;
  }
}

template<typename T>
std::pair<unsigned int,unsigned int> GraphTableModel::computeArea(const std::set<T>& elementsToFind,const std::vector<T>& elements,const TLP_HASH_MAP<T,int>& objToIndex)const {
  //Init from and to values
  int first=elements.size()-1,last=0;

  for(typename std::set<T>::const_iterator it = elementsToFind.begin(); it != elementsToFind.end() ; ++it) {
    assert(objToIndex.find(*it) != objToIndex.end());
    int index = (objToIndex.find(*it))->second;
    first = std::min(first,index);
    last = std::max(last,index);
  }

  first = std::max(first,0);
  last = std::min(last,static_cast<int>(elements.size()-1));
  return std::make_pair(first,last);
}
