#include<vector>
#include<tulip/StringCollection.h>

using namespace std;
using namespace tlp;

StringCollection::StringCollection() {     
}
    
StringCollection::StringCollection(const vector<string> &vectorParam) 
  : vector<string>(vectorParam)  {     
  current = 0;
}

StringCollection::StringCollection(const string param) {
  string temp;
  string::const_iterator itChar = param.begin();
  while (itChar != param.end()) {
    if ( *itChar == ';') {
      this->push_back(temp);
      temp = "";
    }
    else 
      temp += *itChar;
    itChar++;        
  }
  if (temp.size())
    this->push_back(temp);
  current = 0;
} 
     
StringCollection::StringCollection(const vector<string>&  vectorParam, 
                           int currentParam)
  : vector<string>(vectorParam) {
  if (currentParam < int(size())) 
    current = currentParam;
  else 
    current = 0;
}

StringCollection::StringCollection(const vector<string>& vectorParam, 
                        string currentString) 
  : vector<string>(vectorParam)  {

  iterator itString = find(begin(),end(),currentString);
  if (itString != end())
    current = itString-begin();
  else
    current = 0;
}

    
string StringCollection::getCurrentString() {
  return at(current);
}


bool StringCollection::setCurrent(unsigned int param) {
  if (param < size()) {
    current =  param;
    return true;
  }
  return false;
}

bool StringCollection::setCurrent(string param) {
  for (unsigned int i = 0; i< size(); i++) {
    if (at(i) == param ) {
      current = i;
      return true;
    }
  }
  return false;
}

int StringCollection::getCurrent() {
  return current;
} 
