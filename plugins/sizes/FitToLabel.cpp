//-*-c++-*-
#include <cmath>
#include <tulip/TulipPlugin.h>

using namespace std;
///
class FitToLabel:public Sizes
{ 
public:
  FitToLabel(const PropertyContext &context):Sizes(context){}
  ~FitToLabel(){}
  

  Size getNodeValue(const node n) {
    StringProxy *entryLabel=superGraph->getProperty<StringProxy>("viewLabel");
    unsigned int w;
    unsigned int h;
    const string &str=entryLabel->getNodeValue(n);
    unsigned int length=str.length();
    w=0;
    h=1;
    unsigned int cur=0;
    for (unsigned int i=0;i<length;++i) 
      if ((str[i]=='\n')) {
	h++;
	if ((i-cur)>w) w=i-cur;
	cur=i+1;
      }
    if ((length-cur)>w) w=length-cur;
    if (w==0) return (Size(1,1,1));
    else
      return Size(w*2,h*1.90,1);
  }

  bool run() {
    sizesProxy->setAllEdgeValue(Size(0.25,0.25,1));
    return true;
  }
};

SIZESPLUGIN(FitToLabel,"Fit to label","Auber","08/08/2001","0","0","1");












