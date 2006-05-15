#include <assert.h>
#include "Random.h"

LAYOUTPLUGIN(Random,"Random","David Auber","01/12/1999","Ok","0","1");

using namespace std;
using namespace tlp;

Random::Random(const PropertyContext &context):LayoutAlgorithm(context){}

Random::~Random() {}

bool Random::run() {
  layoutResult->setAllEdgeValue(vector<Coord>(0));
  graph->getLocalProperty<SizeProperty>("viewSize")->setAllNodeValue(Size(1,1,1));
  Iterator<node> *itN=graph->getNodes();
  while (itN->hasNext()) {
    node itn=itN->next();
    layoutResult->setNodeValue(itn,Coord(rand()%1024,rand()%1024,rand()%1024));
  } delete itN;
  return true;
}

bool Random::check(string &erreurMsg) {
  erreurMsg="";
  return (true);
}

void Random::reset(){
}
