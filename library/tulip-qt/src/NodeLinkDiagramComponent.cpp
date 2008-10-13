#include "tulip/NodeLinkDiagramComponent.h"

using namespace std;

namespace tlp {

  VIEWPLUGIN(NodeLinkDiagramComponentCreator, "NodeLinkDiagramComponent", "Tulip Team", "16/04/2008", "Node link diagram", "1.0", 0);  

  View* NodeLinkDiagramComponentCreator::create(const string &pluginName,QWidget *parent){
    return new GlMainView(pluginName,parent);
  }

  //==================================================
  NodeLinkDiagramComponent::NodeLinkDiagramComponent(const string &pluginName,QWidget *parent, const char *name):
    GlMainView(pluginName,parent,name) {
    
  }
  //==================================================
  NodeLinkDiagramComponent::~NodeLinkDiagramComponent() {
  }
}

