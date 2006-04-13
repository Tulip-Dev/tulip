//-*-c++-*-
#include <cmath>
#include <tulip/TlpTools.h>
#include <iostream>
#include <qinputdialog.h>
#include <qstringlist.h>
#include <tulip/Selection.h>


using namespace std;
class OnValueSelection:public SelectionAlgorithm { 
private:

public:
  OnValueSelection(const PropertyContext &context):SelectionAlgorithm(context){}
  ~OnValueSelection(){}
  bool run() {
    string nameMetric; 
    string ope;
    double threshold;
    bool ok=false;
    if (dataSet==0 || !dataSet->get("property",nameMetric)) {
      QString text = QInputDialog::getText("Please enter the name of the attribute",
					   "Attribute :", QLineEdit::Normal,
					   QString::null, &ok );
      if (ok && !text.isEmpty()) 
	nameMetric=text.ascii();
      else
	nameMetric="viewMetric";
    }
    cerr << "A" << endl;
    if (dataSet==0 || !dataSet->get("operator",ope)) {
      ok=false;
      QStringList lst;
      lst << "<" << "=<" << "=" << "<>" << "=>" << ">";
      QString res = QInputDialog::getItem("Type of comparison" ,
					"Please select an item", lst, 1, false, &ok);
      if (!ok)
	ope="<";
      else
	ope=res.latin1();
    }
    cerr << "B" << endl;
    ok=false;
    if (dataSet==0 || !dataSet->get("threshold",threshold)) {
      threshold= QInputDialog::getDouble(QString("Please enter a threshold"),
				QString("Value ")+ ope.c_str(), 0, 0, 100000000, 10, &ok );
    }
    cerr << "C" << endl;
    bool inf=false;
    bool eq=false;
    bool sup=false;
    if (ope.length()<2) ope+=" ";
    if (ope[0]=='=' )
      eq=true; 
    if ((ope[0]=='<') || (ope[1]=='<'))
      inf=true;
    if ((ope[0]=='>') || (ope[1]=='>'))
      sup=true;
    cerr << "D" << endl;
    MetricProxy *metric=superGraph->getProperty<MetricProxy>(nameMetric);

    set<node> tmpSmall;
    set<node> tmpEqual;
    set<node> tmpHigh;
    Iterator<node> *itN=superGraph->getNodes();
    for (;itN->hasNext();){
      node n=itN->next();
      if (metric->getNodeValue(n)<threshold)
	  tmpSmall.insert(n);
      else 
	if (metric->getNodeValue(n)==threshold)
	  tmpEqual.insert(n);
	else 
	  tmpHigh.insert(n);
    }delete itN;
    selectionObj->setAllNodeValue(false);
    selectionObj->setAllEdgeValue(false);
    set<node>::const_iterator iN;
    if (eq) 
      for (iN=tmpEqual.begin();iN!=tmpEqual.end();++iN)
	selectionObj->setNodeValue(*iN,true);
    if (inf)
	for (iN=tmpSmall.begin();iN!=tmpSmall.end();++iN)
	  selectionObj->setNodeValue(*iN,true);
    if (sup)
	for (iN=tmpHigh.begin();iN!=tmpHigh.end();++iN)
	  selectionObj->setNodeValue(*iN,true);
    return true;
  }
  
  bool check(string &erreurMsg){
    erreurMsg="";
    return true;
  }
};
SELECTIONPLUGIN(OnValueSelection,"OnValueSelection","Maylis Delest","04/06/2003","Alpha","0","1")
