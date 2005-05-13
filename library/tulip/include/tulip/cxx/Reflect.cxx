

//=======================================================================
//Strucdef implementation
template<typename T> void StructDef::add(std::string str, const char * inHelp, std::string inDefValue) {
  if (data.find(str)==data.end()) {
    data[str]=typeid(T).name();
    if( inHelp )
      help[str] = std::string(inHelp);
    if( inDefValue.c_str() )
      defValue[str] = inDefValue;
  }
#ifndef NDEBUG
  else {
    std::cerr << "StructDef::addVar " << str << " already exists" << std::endl;
  }
#endif
}
//=======================================================================
//DataSet implementation
template<typename T> bool DataSet::get(const std::string str,T& value) const {
  if (data.find(str)!=data.end()) {
    //     Do not work if T do not contain a virtual function
    //     DataType dt = (*(data.find(str))).second;
    //     std::cerr << __PRETTY_FUNCTION__ << ": data.find.typeName=" << dt.typeName;
    //     std::cerr << "; typeid(T).name()=" << typeid(T).name() << std::endl;
    //    assert((*(data.find(str))).second.typeName==typeid(T).name());
    value=*((T*)((*(data.find(str))).second.value));
    return true;
  }
  else
    return false;
}
template<typename T> bool DataSet::getAndFree(const std::string &str,T& value) {
  if (get(str,value)) {
    delete ((T*)(data[str].value));
    data.erase(str);
    return true;
  }
  else
    return false;
}
template<typename T> void DataSet::set(const std::string &str,const T& value) {
  if (data.find(str)!=data.end()) {
    //    assert(data[str].typeName==typeid(T).name()); Do not work if T do not contain a virtual function
    delete (T*)(data[str].value);
  }
  T* tmp=new T(value);
  data[str]=DataType((void *)tmp,typeid(T).name());
}
//=======================================================================
