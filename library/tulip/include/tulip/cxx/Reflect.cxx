
//=======================================================================
//Strucdef implementation
template<typename T> void tlp::StructDef::add(const char* str, const char* inHelp,
					      const char* inDefValue, bool isMandatory) {
  std::list< std::pair<std::string, std::string> >::const_iterator it;
  for (it = data.begin(); it != data.end(); ++it) {
    if ((*it).first == str)
      break;
  }
  if (it == data.end()) {
    data.push_back(std::pair<std::string, std::string>(std::string(str), std::string(typeid(T).name())));
    if (inHelp)
      help[str] = std::string(inHelp);
    if (inDefValue)
      defValue[str] = std::string(inDefValue);
    mandatory[str] = isMandatory;
  }
#ifndef NDEBUG
  else {
    std::cerr << "StructDef::addVar " << str << " already exists" << std::endl;
  }
#endif
}
//=======================================================================
//DataSet implementation
template<typename T> bool tlp::DataSet::get(const std::string& str,T& value) const {
  for (std::list< std::pair<std::string, tlp::DataType*> >::const_iterator it =
	 data.begin(); it != data.end(); ++it) {
    const std::pair<std::string, tlp::DataType*> &p = *it;
    if (p.first == str) {
      value = *((T*) p.second->value);
      return true;
    }
  }
  return false;
}

template<typename T> bool tlp::DataSet::getAndFree(const std::string &str,T& value) {
  for (std::list< std::pair<std::string, tlp::DataType*> >::iterator it =
	 data.begin(); it != data.end(); ++it) {
    std::pair<std::string, tlp::DataType *> &p = *it;
    if (p.first == str) {
      value = *((T*) p.second->value);
      delete p.second;
      data.erase(it);
      return true;
    }
  }
  return false;
}

template<typename T>
struct DataTypeContainer :public tlp::DataType {
  DataTypeContainer(void *value, std::string str) :DataType(value, str) {}
  ~DataTypeContainer() {
    delete (T*) value;
  }
  DataType* clone() {
    return new DataTypeContainer<T>(new T(*(T*)value), typeName);
  }
};

template<typename T> void tlp::DataSet::set(const std::string &str,const T& value) {
  DataTypeContainer<T> *tmp= new DataTypeContainer<T>(new T(value), typeid(T).name());
  for (std::list< std::pair<std::string, tlp::DataType*> >::iterator it =
	 data.begin(); it != data.end(); ++it) {
    std::pair<std::string, tlp::DataType*> &p = *it;
    if (p.first == str) {
      delete p.second;
      p.second = tmp;
      return;
    }
  }
  data.push_back(std::pair<std::string, tlp::DataType*>(str, tmp));
}
//=======================================================================
