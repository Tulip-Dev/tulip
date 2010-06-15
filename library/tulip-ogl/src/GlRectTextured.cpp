#include "tulip/GlRectTextured.h"

using namespace std;

namespace tlp {
  GlRectTextured::GlRectTextured() {
    cout << "GlRectTextured deprecated : use Gl2DRect or GlRect instead "<< endl;
  }

  GlRectTextured::GlRectTextured(float top,float bottom,float left, float right,
				 const std::string& textureName,
				 bool inPercent)
    :Gl2DRect(top,bottom,left,right,textureName,inPercent)
  {
    cout << "GlRectTextured deprecated : use Gl2DRect or GlRect instead "<< endl;
  }

  GlRectTextured::GlRectTextured(float bottom,float left,float height, float width,const std::string& textureName,bool xInv, bool yInv)
    :Gl2DRect(bottom,left,height,width,textureName,xInv,yInv)
  {
    cout << "GlRectTextured deprecated : use Gl2DRect or GlRect instead "<< endl;
  }
}
