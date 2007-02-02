
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
  for (std::list< std::pair<std::string, tlp::DataType> >::const_iterator it =
	 data.begin(); it != data.end(); ++it) {
    const std::pair<std::string, tlp::DataType> &p = *it;
    if (p.first == str) {
      value = *((T*) p.second.value);
      return true;
    }
  }
  return false;
}

template<typename T> bool tlp::DataSet::getAndFree(const std::string &str,T& value) {
  for (std::list< std::pair<std::string, tlp::DataType> >::iterator it =
	 data.begin(); it != data.end(); ++it) {
    std::pair<std::string, tlp::DataType> &p = *it;
    if (p.first == str) {
      value = *((T*) p.second.value);
      delete (T*) p.second.value;
      data.erase(it);
      return true;
    }
  }
  return false;
}

template<typename T> void tlp::DataSet::set(const std::string &str,const T& value) {
  T* tmp=new T(value);
  for (std::list< std::pair<std::string, tlp::DataType> >::iterator it =
	 data.begin(); it != data.end(); ++it) {
    std::pair<std::string, tlp::DataType> &p = *it;
    if (p.first == str) {
      delete (T*) p.second.value;
      p.second.value = (void *) tmp;
      p.second.typeName = typeid(T).name();
      return;
    }
  }
  data.push_back(std::pair<std::string, tlp::DataType>(str, tlp::DataType((void *)tmp, typeid(T).name())));

}
//=======================================================================
