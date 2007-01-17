#include <tulip/ForEach.h>
#include "StrengthMetric.h"

DOUBLEPLUGINOFGROUP(StrengthMetric,"Strength","David Auber","26/02/2003","Alpha","0","1","Graph");

using namespace std;
using namespace tlp;
using namespace stdext;

StrengthMetric::StrengthMetric(const PropertyContext &context):DoubleAlgorithm(context) {}

StrengthMetric::~StrengthMetric() {}
//=============================================================
double StrengthMetric::e(hash_set<node> &U,hash_set<node> &V) {
  hash_set<node>::const_iterator itU;
  double result=0;
  hash_set<node> *A, *B;
  if (U.size()<V.size()) {
    A = &U; B=&V;
  }
  else {
    A = &V; B=&U;
  }
  for (itU=A->begin();itU!=A->end();++itU) {
    Iterator<node> *itN=graph->getInOutNodes(*itU);
    while (itN->hasNext()) {
      node itn=itN->next();
      if (B->find(itn)!=B->end()) result+=1.0;
    }delete itN;
  }
  return result;
}
//=============================================================
double StrengthMetric::e(const hash_set<node> &U) {
  hash_set<node>::const_iterator itU;
  double result=0.0;
  for (itU=U.begin();itU!=U.end();++itU) {
    Iterator<node> *itN=graph->getInOutNodes(*itU);
    while (itN->hasNext()) {
      node itn=itN->next();
      if (U.find(itn)!=U.end()) result+=1.0;
    }delete itN;
  }
  return result/2.0;
}
//=============================================================
double StrengthMetric::s(hash_set<node> &U, hash_set<node> &V) {
  if ((U.size()==0) || (V.size()==0)) return 0;
  return (e(U,V) / double(U.size()*V.size()));
}
//=============================================================
double StrengthMetric::s(const hash_set<node> &U) {
  if (U.size()<2) return 0.0;
  return  (e(U)) * 2.0 / double(U.size()*(U.size()-1));
}
//=============================================================
double StrengthMetric::getEdgeValue(const edge ee ) {
  node u=graph->source(ee);
  node v=graph->target(ee);
  hash_set<node> Nu,Nv,Wuv;

  //Compute Nu
  Iterator<node> *itN = graph->getInOutNodes(u);
  while (itN->hasNext()) {
    node n=itN->next();
    if (n!=v) Nu.insert(n);
  } delete itN;
  if (Nu.size()==0) return 0;

  //Compute Nv
  itN=graph->getInOutNodes(v);
  while (itN->hasNext()) {
    node n=itN->next();
    if (n!=u) Nv.insert(n);
  }delete itN;
  if (Nv.size()==0) return 0;

  //Compute Wuv, choose the minimum set to minimize operation
  hash_set<node> *A, *B;
  if (Nu.size()<Nv.size()) {
    A = &Nu; B=&Nv;
  }
  else {
    A = &Nv; B=&Nu;
  }
  hash_set<node>::const_iterator itNu;
  for (itNu=A->begin();itNu!=A->end();++itNu) {
    if (B->find(*itNu)!=B->end()) Wuv.insert(*itNu);
  }

  hash_set<node> &Mu = Nu;
  hash_set<node> &Mv = Nv;
  /* Compute Mu and Mv, we do not need Nu and Nv anymore,
     thus we modify them to speed up computation
  */
  for (itNu=Wuv.begin();itNu!=Wuv.end();++itNu) {
    Mu.erase(*itNu);
    Mv.erase(*itNu);
  }

  //compute strength metric
  double gamma3 = double(Wuv.size());
  double norm3  = double((Wuv.size()+Mv.size()+Mu.size()));

  double gamma4 = (e(Mu,Wuv) +  e(Mv,Wuv) +  e(Mu,Mv) +  e(Wuv));
  double norm4  = (double(Mu.size() * Wuv.size() + 
			  Mv.size() * Wuv.size() + 
			  Mu.size() * Mv.size() ) + 
		   double(Wuv.size()*(Wuv.size()-1)) / 2.0);

  double norm = norm3 + norm4;
  double gamma = gamma3 + gamma4;
  
  if (norm > 1E-5)
    gamma /= norm;
  else
    gamma = 0;

  return gamma;
}
//=============================================================
double StrengthMetric::getNodeValue(const node n ) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  if (graph->deg(n)==0) return 0;
  double result=0;
  Iterator<edge> *itE = graph->getInOutEdges(n);
  while (itE->hasNext()) {
    edge ite = itE->next();
    result += doubleResult->getEdgeValue(ite);
  } delete itE;
  return result/double(graph->deg(n));
}
//=============================================================
bool StrengthMetric::run() {
  edge e;
  int step = 0, maxSteps = graph->numberOfEdges();
  pluginProgress->setComment("Computing Strength metric on edges...");
  forEach(e, graph->getEdges()) {
    doubleResult->setEdgeValue(e, getEdgeValue(e));
    pluginProgress->progress(++step, maxSteps);
    if (pluginProgress->state() !=TLP_CONTINUE)
      return pluginProgress->state()!= TLP_CANCEL;
  }
  node n;
  step = 0;
  maxSteps = graph->numberOfNodes();
  pluginProgress->setComment("Computing Strength metric on nodes...");
  forEach(n, graph->getNodes()) {
    doubleResult->setNodeValue(n, getNodeValue(n));
    pluginProgress->progress(++step, maxSteps);
    if (pluginProgress->state() !=TLP_CONTINUE)
      return pluginProgress->state()!= TLP_CANCEL;
  }
    
  return true;
}
//=============================================================
