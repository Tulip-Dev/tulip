#include <assert.h>
#include "Random.h"

LAYOUTPLUGINOFGROUP(Random,"Random","David Auber","01/12/1999","Ok","0","1","Basic");

using namespace std;

Random::Random(const PropertyContext &context):Layout(context){}

Random::~Random() {}

bool Random::run() {
  layoutProxy->setAllEdgeValue(vector<Coord>(0));
  superGraph->getLocalProperty<SizesProxy>("viewSize")->setAllNodeValue(Size(1,1,1));
  Iterator<node> *itN=superGraph->getNodes();
  while (itN->hasNext()) {
    node itn=itN->next();
    layoutProxy->setNodeValue(itn,Coord(rand()%1024,rand()%1024,rand()%1024));
  } delete itN;
  return true;
}

bool Random::check(string &erreurMsg) {
  erreurMsg="";
  return (true);
}

void Random::reset(){
}
