#include <tulip/ForEach.h>
#include "StrengthMetric.h"

METRICPLUGIN(StrengthMetric,"Strength","David Auber","26/02/2003","Alpha","0","1");

using namespace std;
using namespace stdext;

StrengthMetric::StrengthMetric(const PropertyContext &context):Metric(context) {}

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
    Iterator<node> *itN=superGraph->getInOutNodes(*itU);
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
    Iterator<node> *itN=superGraph->getInOutNodes(*itU);
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
  node u=superGraph->source(ee);
  node v=superGraph->target(ee);
  hash_set<node> Nu,Nv,Wuv;

  //Compute Nu
  Iterator<node> *itN=superGraph->getInOutNodes(u);
  while (itN->hasNext()) {
    node n=itN->next();
    if (n!=v) Nu.insert(n);
  }delete itN;
  if (Nu.size()==0) return 0;

  //Compute Nv
  itN=superGraph->getInOutNodes(v);
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

  if (norm3 > 1E-5)
    gamma3 /= norm3;
  else
    gamma3 = 0;

  if (norm4 > 1E-5)
    gamma4 /= norm4;
  else
    gamma4 = 0;

  return gamma3 + gamma4;
}
//=============================================================
double StrengthMetric::getNodeValue(const node n ) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  if (superGraph->deg(n)==0) return 0;
  double result=0;
  Iterator<edge> *itE=superGraph->getInOutEdges(n);
  while (itE->hasNext()) {
    edge ite=itE->next();
    result+=metricProxy->getEdgeValue(ite);
  } delete itE;
  return result/double(superGraph->deg(n));
}
//=============================================================
bool StrengthMetric::run() {
  edge e;
  forEach(e, superGraph->getEdges())
    metricProxy->setEdgeValue(e, getEdgeValue(e));
  node n;
  forEach(n, superGraph->getNodes())
    metricProxy->setNodeValue(n, getNodeValue(n));
  return true;
}
//=============================================================
