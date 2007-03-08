//-*-c++-*-
#include <stdio.h>
#include <string>

#include <tulip/TulipPlugin.h>

using namespace std;
using namespace tlp;

class IdLabels:public StringAlgorithm { 
public:
  IdLabels(const PropertyContext &context):StringAlgorithm(context){}
  ~IdLabels(){}
  string getNodeValue(const node n) {
    char tmp[255];
    sprintf(tmp,"%d",n.id);
   return string(tmp);
  }
  string getEdgeValue(const edge e) {
    char tmp[255];
    sprintf(tmp,"%d",e.id);
    return string(tmp);
  }
};
STRINGPLUGIN(IdLabels,"IdLabels","Auber","04/05/2001","","1.0");
