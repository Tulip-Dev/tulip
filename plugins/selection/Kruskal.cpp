#include "Kruskal.h"

SELECTIONPLUGIN(Kruskal,"Kruskal","Anthony DON","14/04/03","Alpha","0","1");

using namespace std;

int Kruskal::getClass(const int i)
{
  return (*classes)[i];
}

int Kruskal::makeUnion(const int p, const int q)
{
  int nNodes = superGraph->numberOfNodes();
  int x = getClass(p);
  int y = getClass(q);

  Iterator<node> *itN=superGraph->getNodes();
  for (;itN->hasNext();)
    { 
      node n=itN->next();
      if(getClass(n.id) == y)
	(*classes)[n.id] = x;
    }
  delete itN;
  
  numClasses--;
}

bool Kruskal::edgeOk(const edge &e)
{
  return (getClass(superGraph->source(e).id) !=  getClass(superGraph->target(e).id));
}

Kruskal::Kruskal(const PropertyContext &context):Selection(context) 
{
  this->context = context;
}

Kruskal::~Kruskal() 
{
}

///Calcul l'arbre couvrant minimal
bool Kruskal::run()
{
  /* Initialisation */

  int nNodes = superGraph->numberOfNodes();
  numClasses = nNodes;
  classes = new map<int, int>;
  
  Iterator<node> *itN=superGraph->getNodes();
  int classNumber = 0;
  for (;itN->hasNext();)
    { 
      node n=itN->next();
      (*classes)[n.id] = classNumber;
      classNumber++;
    }
  delete itN;

  std::list<edge> sortedEdges;
  Iterator<edge> *itE=superGraph->getEdges();
  for (;itE->hasNext();)
    { 
      edge e=itE->next();
      sortedEdges.push_back(e);
    }
  delete itE;

  selectionProxy->setAllNodeValue(true);
  selectionProxy->setAllEdgeValue(false);

  /* Calcul */
  sortedEdges.sort<ltEdge>(ltEdge(superGraph->getProperty<MetricProxy>("viewMetric")));
  while(numClasses > 1)
    {
      edge cur;
      do {
	cur = sortedEdges.front();
	sortedEdges.pop_front();
      } while(! edgeOk(cur));
      
      selectionProxy->setEdgeValue(cur, true);
      makeUnion(superGraph->source(cur).id, superGraph->target(cur).id);
    }

  delete classes;
  return true;
}





