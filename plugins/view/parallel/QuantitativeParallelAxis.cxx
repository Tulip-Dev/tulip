using namespace std;

template <typename T>
static string getStringFromNumber(T number) {
  ostringstream oss;
  oss << number;
  return oss.str();
}

template <typename T>
QuantitativeParallelAxis<T>::QuantitativeParallelAxis(Coord baseCoord, float height, string name, vector<T> *values, bool crescent, viewType vType) :
  ParallelAxis(baseCoord, height, name, values->size(), vType) {
  vector<T> valuesCopy(*values);
  setLabels(valuesCopy, crescent);
  computeDataPointsCoord(values);
}

template <typename T>
void QuantitativeParallelAxis<T>::bestFit(T min, T max) {
  begin = min;
  end = max;

  if (begin == end) {
    end += 10;
  }

  increment = (end - begin) / 20;

  // if data are integer, we need to prevent a null increment
  if ((typeid(min) == typeid(int)) && increment == 0) {
    increment = 1;
  }
  scale = (double) _height/(end-begin);
}

template <typename T>
void QuantitativeParallelAxis<T>::setLabels(vector<T> &val, bool crescent){
  
  this->crescent = crescent;
  sort(val.begin(),val.end());
  bestFit(val.front(),val.back());
  
  
  T label;
  float labelCoord = _baseCoord.getY();
  
  if(crescent)
    label = begin;
    
  
  else 
    label = end;
  
   
  
  float i;
  for (i = labelCoord ; i <= (_baseCoord.getY() + _height) ; i += (increment * scale)) {
    
    addLabelDrawing(getStringFromNumber(label), (float) i);
    
    if(crescent)
      label += increment;
    else
      label -= increment;
  }
  
  if (i <= (_baseCoord.getY() + _height + ((increment/2) * scale) + 1))
    addLabelDrawing(getStringFromNumber(end), _baseCoord.getY() + _height);
}


template <typename T>
void QuantitativeParallelAxis<T>::computeDataPointsCoord(vector<T> *values) {

  
  for (unsigned int i = 0 ; i < values->size() ; i++) {
    
    Coord c=Coord(_baseCoord.getX(),0,_baseCoord.getZ());
  
    if (crescent)
      c.setY(_baseCoord.getY() + ((*values)[i]-begin)*scale);
    else
      c.setY(_baseCoord.getY() + (end-(*values)[i])*scale);

    _dataCoords[i] = c;
    
  }
}
