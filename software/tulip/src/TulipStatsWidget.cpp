//*********************************************************************************
//** TulipStatsWidget : Panel for the computation of statistics results on graphs
//** 
//** author : Delorme Maxime
//** date   : 07/06/05
//*********************************************************************************

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <sstream>

#include "TulipStatsWidget.h"
#include <tulip/PropertyManager.h> 
#include <tulip/Metric.h> 
#include <tulip/Sizes.h> 
#include <tulip/Layout.h> 
#include <tulip/Int.h> 
#include <tulip/Iterator.h> 
#include <tulip/GlGraph.h> 
#include <tulip/TlpTools.h>
#include <tulip/StableIterator.h>
#include <tulip/ClusterTree.h>

#if (QT_REL == 3)
#include <qlistbox.h>
#include <qpushbutton.h> 
#include <qlineedit.h> 
#include <qgroupbox.h> 
#include <qcheckbox.h> 
#include <qvalidator.h> 
#include <qlabel.h> 
#include <qcombobox.h>
#include <qtabwidget.h>
#include <qmessagebox.h>
#include <qradiobutton.h>
#else
#include <QtGui/qpushbutton.h> 
#include <QtGui/qlineedit.h> 
#include <QtGui/qgroupbox.h> 
#include <QtGui/qcheckbox.h> 
#include <QtGui/qvalidator.h> 
#include <QtGui/qlabel.h> 
#include <QtGui/qcombobox.h>
#include <QtGui/qtabwidget.h>
#include <QtGui/qmessagebox.h>
#include <QtGui/qradiobutton.h>
#endif

//#include "../../../thirdparty/ftgl/FTGLPixmapFont.h"

#include <GL/glu.h> 

using namespace tlp; 
using namespace std; 

namespace tlp
{
  TulipStats::TulipStats(QWidget *parent, const char *name, WFlags fl) : TulipStatsData(parent, name, fl), glGraphWidget(0), supergraph(0)
  {
    //  cout << "[START]..." << __PRETTY_FUNCTION__ ;

    reset();
    resetComposite(false);

    statsResults = new StatisticResults();

    connect(StatsTab, SIGNAL(currentChanged(QWidget *)), SLOT(refreshMetricsSlot()));

    connect(AvaiMetricsList, SIGNAL(selectionChanged()), SLOT(enableAddMetricBtn()));
    connect(UsedMetricsList, SIGNAL(selectionChanged()), SLOT(enableRemoveMetricBtn()));

    connect(AddMetricBtn, SIGNAL(clicked()), SLOT(addMetricSlot()));
    connect(DelMetricBtn, SIGNAL(clicked()), SLOT(delMetricSlot()));

    connect(ComputeResultBtn, SIGNAL(clicked()), SLOT(computeResultsSlot()));
    connect(ChangeLayoutBtn, SIGNAL(clicked()), SLOT(changeLayoutSlot()));

    connect(DiscStep1, SIGNAL(textChanged(const QString&)), SLOT(updateDiscretizationSlot()));
    connect(DiscStep2, SIGNAL(textChanged(const QString&)), SLOT(updateDiscretizationSlot()));
    connect(DiscStep3, SIGNAL(textChanged(const QString&)), SLOT(updateDiscretizationSlot()));
    connect(DiscStep1, SIGNAL(lostFocus()), SLOT(checkStepValueSlot()));
    connect(DiscStep2, SIGNAL(lostFocus()), SLOT(checkStepValueSlot()));
    connect(DiscStep3, SIGNAL(lostFocus()), SLOT(checkStepValueSlot()));

    connect(AverageDisplayCB, SIGNAL(clicked()), SLOT(chDisplayAverageSlot()));
    connect(StdDeviationDisplayCB, SIGNAL(clicked()), SLOT(chDisplayStdDeviationSlot()));
    connect(AABBDisplayCB, SIGNAL(clicked()), SLOT(chDisplayBoundingBoxSlot()));
    connect(LinearDisplayCB, SIGNAL(clicked()), SLOT(chDisplayLinearRegressionSlot()));
    connect(EigenDisplayCB, SIGNAL(clicked()), SLOT(chDisplayEigenVectorsSlot()));
    connect(DisplayCPCB, SIGNAL(clicked()), SLOT(chDisplayClusteringPlaneSlot()));

    connect(ClusteringComboBox, SIGNAL(activated(const QString&)), SLOT(chClusteringModelSlot(const QString&)));
    connect(ClusteringComboBox, SIGNAL(activated(const QString&)), SLOT(updateClusteringPlaneSlot()));

    connect(aEdit, SIGNAL(lostFocus()), SLOT(updateClusteringPlaneSlot()));
    connect(bEdit, SIGNAL(lostFocus()), SLOT(updateClusteringPlaneSlot()));
    connect(cEdit, SIGNAL(lostFocus()), SLOT(updateClusteringPlaneSlot()));
    connect(dEdit, SIGNAL(lostFocus()), SLOT(updateClusteringPlaneSlot()));

    connect(ClusterizeBtn, SIGNAL(clicked()), SLOT(clusterizeSlot()));

    // The validator for double values: 
    QDoubleValidator *doubleValidator = new QDoubleValidator( this );
    doubleValidator->setBottom(0); 

    DiscStep1->setValidator(doubleValidator); 
    DiscStep2->setValidator(doubleValidator); 
    DiscStep3->setValidator(doubleValidator); 
    aEdit->setValidator(doubleValidator); 
    bEdit->setValidator(doubleValidator); 
    cEdit->setValidator(doubleValidator); 
    dEdit->setValidator(doubleValidator); 

    updateDiscretizationSlot(); 

    //  cout << " ...[END]" << endl;
  }

  TulipStats::~TulipStats() 
  {
  } 

  void TulipStats::setClusterTreeWidget(ClusterTree *clusterTree)
  {
    clusterTreeWidget = clusterTree;
  }

  SuperGraph* TulipStats::getSuperGraph()
  {
    //  cout << "[START]..." << __PRETTY_FUNCTION__ ;
    //  cout << " ...[END]" << endl;
    return supergraph;
  }

  GlGraphWidget* TulipStats::getGlGraphWidget()
  {
    //  cout << "[START]..." << __PRETTY_FUNCTION__ ;
    //  cout << " ...[END]" << endl;
    return glGraphWidget;
  }

  void TulipStats::setGlGraphWidget(GlGraphWidget *g)
  {
    // cout << "[START]..." << __PRETTY_FUNCTION__ ;
    if (supergraph != 0)
      reset();

    resetComposite(true);

    glGraphWidget = g;

    if (g == 0)
      return;

    /*    if (supergraph != 0)
	  supergraph->removeObserver(this); */

    supergraph = g->getSuperGraph();

    if (supergraph != 0 && supergraph != NULL)
      {
	updateMetrics();
	supergraph->addObserver(this);
      }

    // cout << " ...[END]" << endl;
  } 

  void TulipStats::destroy(SuperGraph *superGraph)
  {
    if (superGraph == supergraph)
      supergraph->removeObserver(this);
  }

  //************************************************************
  //** TulipStats : private function
  //************************************************************

  
  std::string TulipStats::vectorfToString(std::vector<float> vec, int nElem)
  {
    std::string result = "( ";
    // MAC PORT
    stringstream sstr;
    
    char val[50];
    
    for(int i=0; i < nElem-1; i++)
      {
        sstr.seekp(0);
        sstr << vec[i];
	result += sstr.str() + "; ";
      }
    
    sstr.seekp(0);
    sstr << vec[nElem - 1];
    result += sstr.str() + " )";
    
    return result;
  }

  std::string TulipStats::vectorfToString(tlp::Vector<float, 3> vec)
  {
    std::string result = "( ";
    // MAC PORT
    stringstream sstr;
    
    char val[50];
    
    for(int i=0; i < 2; i++)
      {
        sstr.seekp(0);
	sstr << vec[i];
	result += sstr.str() + "; ";
      }
    
    sstr.seekp(0);
    sstr << vec[2];
    result += sstr.str() + " )";
    
    return result;
  }
  
  void TulipStats::reset()
  {  
    // cout << "[START] ... " << __PRETTY_FUNCTION__;
    
    metrics.clear();
    nMetrics = 0;
    UsedMetricsList->clear(); 
    AvaiMetricsList->clear(); 
    
    AddMetricBtn->setEnabled(false);
    DelMetricBtn->setEnabled(false);
    ChangeLayoutBtn->setEnabled(false);
    ComputeResultBtn->setEnabled(false);

    DiscStep1->setEnabled(false);
    DiscStep2->setEnabled(false);
    DiscStep3->setEnabled(false);

    resetDisplayTab();
    resetClusteringTab();

    // cout << "... [END]" << endl;
  }

  void TulipStats::resetDisplayTab()
  {
    // cout << "[START] ... " << __PRETTY_FUNCTION__;

    AverageBox->setEnabled(false);
    AverageLbl->setText("M = ( 0 )");
    AverageDisplayCB->setEnabled(false);
    AverageDisplayCB->setChecked(false);

    VarianceBox->setEnabled(false);
    VarianceLbl->setText("V = ( 0 )");

    StdDeviationBox->setEnabled(false);
    StdDeviationLbl->setText("Sigma = ( 0 )");
    StdDeviationDisplayCB->setEnabled(false);
    StdDeviationDisplayCB->setChecked(false);

    BoundsBox->setEnabled(false);
    MinLbl->setText("Min = ( 0 )");
    MaxLbl->setText("Max = ( 0 )");
    AABBDisplayCB->setEnabled(false);
    AABBDisplayCB->setChecked(false);

    LinearRegressionBox->setEnabled(false);
    b0Lbl->setText("Ordinate in the beginning = 0");
    b1Lbl->setText("Slope = 0");
    LinearDisplayCB->setEnabled(false);
    LinearDisplayCB->setChecked(false);

    EigenBox->setEnabled(false);
    v1Lbl->setText("v1 = (0, 0, 0)");
    v2Lbl->setText("v2 = (0, 0, 0)");
    v3Lbl->setText("v3 = (0, 0, 0)");
    EigenDisplayCB->setEnabled(false);
    EigenDisplayCB->setChecked(false);

    StatsResultsFrame->setEnabled(false);
  
    // cout << " ...[END]" << endl;
  }

  void TulipStats::resetClusteringTab()
  {
    // cout << "[START] ... " << __PRETTY_FUNCTION__;

    aEdit->setText("1");
    bEdit->setText("0");
    cEdit->setText("0");
    dEdit->setText("0"); 

    DisplayCPCB->setChecked(false);

    ClusteringFrame->setEnabled(false);

    // cout << " ...[END]" << endl;
  }

  void TulipStats::resetComposite(bool resetGlComposite)
  {
    //cout << "[START] ... " << __PRETTY_FUNCTION__;

    if (AverageDisplayCB->isChecked())
      glGraphWidget->removeGlAugmentedDisplay(averagePoint);

    if (StdDeviationDisplayCB->isChecked())
      glGraphWidget->removeGlAugmentedDisplay(stdDeviationBox);

    if (AABBDisplayCB->isChecked())
      glGraphWidget->removeGlAugmentedDisplay(AABB);

    if (LinearDisplayCB->isChecked())
      glGraphWidget->removeGlAugmentedDisplay(linearRegression);

    if (EigenDisplayCB->isChecked())
      {
	for(int i=0; i < 3; i++)
	  glGraphWidget->removeGlAugmentedDisplay(eigenVectors[i]);
      }

    if (resetGlComposite)
      {
	if (DisplayCPCB->isChecked())
	  glGraphWidget->removeGlAugmentedDisplay(clusteringPlane);
      }
  
    /*  if (glGraphWidget != 0 && glGraphWidget != NULL)
	glGraphWidget->UpdateGL();*/

    //cout << " ...[END]" << endl;
  }

  void TulipStats::initClusteringTab()
  {
    // cout << "[START] ... " << __PRETTY_FUNCTION__;

    ClusteringComboBox->clear();

    ClusteringComboBox->insertItem("Custom");
  
    ClusteringComboBox->insertItem("Average (X)");

    if (nMetrics > 1)
      {
	ClusteringComboBox->insertItem("Average (Y)");
      }

    if (nMetrics > 2)
      {
	ClusteringComboBox->insertItem("Average (Z)");
      }

    if (nMetrics == 2)
      {
	ClusteringComboBox->insertItem("Linear Regression Plane");
      }

    if (nMetrics == 3)
      {
	ClusteringComboBox->insertItem("Eigenvectors plane (XY)");
	ClusteringComboBox->insertItem("Eigenvectors plane (XZ)");
	ClusteringComboBox->insertItem("Eigenvectors plane (YZ)");
      }

    aEdit->setText("1");
    bEdit->setText("1");
    cEdit->setText("1");
    dEdit->setText("1");

    ClusteringFrame->setEnabled(true);

    // cout << " ...[END]" << endl;
  }

  void TulipStats::enableAddMetricBtn()
  {
    AddMetricBtn->setEnabled(true);
  }

  void TulipStats::enableRemoveMetricBtn()
  {
    DelMetricBtn->setEnabled(true);
  }

  void TulipStats::updateMetrics()
  {
    // cout << "[START] ... " << __PRETTY_FUNCTION__;

    AvaiMetricsList->clear();

    Iterator<std::string>* properties = supergraph->getProperties();

    while (properties->hasNext())
      {
	std::string proxyName = properties->next();

	PProxy* proxy = supergraph->getProperty(proxyName);

	if (dynamic_cast<Metric*>(proxy) != 0)
	  AvaiMetricsList->insertItem(proxyName.c_str());	
      }

    delete properties;

    // we check the used metrics are still available :
    int i = 0;
  
    while (i < nMetrics)
      {
	bool found = false;
	QString item = UsedMetricsList->text(i);

	for(unsigned int j=0; j < AvaiMetricsList->count(); j++)
	  {
	    QString avai = AvaiMetricsList->text(j);

	    if (item == AvaiMetricsList->text(j))
	      {
		found = true;
		break;
	      }
	  }

	// We didn't found the used metric => We delete it
	if (!found)
	  delMetricSlot(i);
	else 
	  i++;
      }

    // cout << " ...[END]" << endl;
  }

  void TulipStats::updateClusteringPlane()
  {
    // cout << "[START] ... " << __PRETTY_FUNCTION__;

    glGraphWidget->removeGlAugmentedDisplay(clusteringPlane);
    chDisplayClusteringPlaneSlot();

    // cout << " ...[END]" << endl;
  }

  //************************************************************
  //** TulipStats : private slots
  //************************************************************

  void TulipStats::refreshMetricsSlot()
  {
    if (glGraphWidget != 0)
      updateMetrics();
    else
      reset();
  }

  void TulipStats::addMetricSlot()
  {
    // cout << "[START] ... " << __PRETTY_FUNCTION__;

    std::string proxyName = 
#if (QT_REL == 3)
      AvaiMetricsList->currentText();
#else
    AvaiMetricsList->currentText().toStdString();
#endif

    // We limit the number of proxy to 3 maximum :
    if (nMetrics == 3) 
      {
	// // cout << "... [END]" << endl;
	QMessageBox::warning(this, "No more than 3 metrics", "Sorry but you can't use more than 3 metrics");
	return;
      }
  
    Metric* metric;

    metric = supergraph->getProperty<Metric>(proxyName);

    metrics.push_back(metric); 
    nMetrics++; 

    // If we add the first Metric, then we enable the buttons "Change to Histogram" and "Compute Results" 
    if (nMetrics == 1) 
      {
	// One metric we have an histogram
	ChangeLayoutBtn->setText("Change to Histogram");

	ChangeLayoutBtn->setEnabled(true);
	ComputeResultBtn->setEnabled(true);
      }
    else // More : it's a scatter plot
      ChangeLayoutBtn->setText("Change to Scatter plot");

    if (nMetrics >= 1)
      DiscStep1->setEnabled(true);
    if (nMetrics >= 2)
      DiscStep2->setEnabled(true);
    if (nMetrics >= 3)
      DiscStep3->setEnabled(true);

    UsedMetricsList->insertItem(proxyName.c_str());

    //  // cout << " ...[END]" << endl;
  }

  // This function deletes a metric by a given id
  void TulipStats::delMetricSlot(int metricID)
  {
    // cout << "[START] ... " << __PRETTY_FUNCTION__;
    UsedMetricsList->removeItem(metricID);

    nMetrics--;

    // Could be replaced by a call to remove with an Iterator (Actually I don't like Iterators ^^)
    for(int i = metricID; i < nMetrics; i++)
      metrics[i] = metrics[i+1];
    metrics.pop_back();

    if(nMetrics == 1)
      ChangeLayoutBtn->setText("Change to Histogram");

    // If we don't have any metric in the list, we disable the buttons to avoid problems
    else if (nMetrics == 0)
      {
	ChangeLayoutBtn->setEnabled(false);
	ComputeResultBtn->setEnabled(false);
	DelMetricBtn->setEnabled(false);
      }

    if (nMetrics <= 2)
      DiscStep3->setEnabled(false);
    if (nMetrics <= 1)
      DiscStep2->setEnabled(false);
    if (nMetrics == 0)
      DiscStep1->setEnabled(false);

    // cout << " ...[END]" << endl;
  }

  // This function deletes the selected metric in the list
  void TulipStats::delMetricSlot()
  {
    // cout << "[START] ... " << __PRETTY_FUNCTION__;

    int metricID = UsedMetricsList->currentItem();
    UsedMetricsList->removeItem(metricID);

    nMetrics--;

    // Could be replaced by a call to remove with an Iterator (Actually I don't like Iterators ^^)
    for(int i=metricID; i < nMetrics; i++)
      metrics[i] = metrics[i+1];

    metrics.pop_back();

    if(nMetrics == 1)
      ChangeLayoutBtn->setText("Change to Histogram");

    // If we don't have any metric in the list, we disable the buttons to avoid problems
    else if (nMetrics == 0)
      {
	ChangeLayoutBtn->setEnabled(false);
	ComputeResultBtn->setEnabled(false);
	DelMetricBtn->setEnabled(false);
      }

    if (nMetrics <= 2)
      DiscStep3->setEnabled(false);
    if (nMetrics <= 1)
      DiscStep2->setEnabled(false);
    if (nMetrics == 0)
      DiscStep1->setEnabled(false);

    // cout << " ...[END]" << endl;
  }

  void TulipStats::computeResultsSlot()
  {
    // cout << "[START] ... " << __PRETTY_FUNCTION__;

    resetComposite(false);
    resetDisplayTab();

    statsResults = StatsNodeModule::ComputeStatisticsResults(supergraph, metrics, nMetrics);

    std::string output;

    output = "M = " + vectorfToString(statsResults->averagePoint, nMetrics);
    AverageLbl->setText(output.c_str());

    output = "V = " + vectorfToString(statsResults->variancePoint, nMetrics);
    VarianceLbl->setText(output.c_str());

    output = "Sigma = " + vectorfToString(statsResults->standardDeviationPoint, nMetrics);
    StdDeviationLbl->setText(output.c_str());

    output = "Min = " + vectorfToString(statsResults->minPoint, nMetrics);
    MinLbl->setText(output.c_str());

    output = "Max = " + vectorfToString(statsResults->maxPoint, nMetrics);
    MaxLbl->setText(output.c_str());

    StatsResultsFrame->setEnabled(true);

    AverageBox->setEnabled(true);
    VarianceBox->setEnabled(true);
    StdDeviationBox->setEnabled(true);
    BoundsBox->setEnabled(true);

    AverageDisplayCB->setEnabled(true);
    StdDeviationDisplayCB->setEnabled(true);
    AABBDisplayCB->setEnabled(true);

    if (nMetrics == 2)
      {
	// 2 metrics ? We can compute the linear regression function
	char val[50];
	// MAC PORT
	stringstream sstr;
      
	sstr << statsResults->linearRegressionFunctionb0;
	output = "Ordinate in the beginning = ";
	output += sstr.str();
	b0Lbl->setText(output.c_str());

	sstr.seekp(0);
	sstr << statsResults->linearRegressionFunctionb1;
	output = "Slope = ";
	output += sstr.str();
	b1Lbl->setText(output.c_str());

	LinearRegressionBox->setEnabled(true);
	LinearDisplayCB->setEnabled(true);
      }

    if (nMetrics == 3)
      {
	// 3 metrics ? We can compute eigenvectors
	output = "v1 = " + vectorfToString(statsResults->eigenVectors[0]);
	v1Lbl->setText(output.c_str());

	output = "v2 = " + vectorfToString(statsResults->eigenVectors[1]);
	v2Lbl->setText(output.c_str());

	output = "v3 = " + vectorfToString(statsResults->eigenVectors[2]);
	v3Lbl->setText(output.c_str());

	EigenBox->setEnabled(true);
	EigenDisplayCB->setEnabled(true);
      }

    initClusteringTab();

    // cout << " ...[END]" << endl;
  }

  void TulipStats::changeLayoutSlot() {
    // cout << "[START] ... " << __PRETTY_FUNCTION__;

    Observable::holdObservers();

    resetComposite(false);
    resetDisplayTab();
    resetClusteringTab();

    // Calling the layout plugin :
    string erreurMsg;
    bool resultBool  = true;  
    DataSet *dataSet = 0;

    // We build the data set :
    dataSet = new DataSet();
    StructDef parameter = Layout::factory->getParam("Scatter Plot");
    parameter.buildDefaultDataSet( *dataSet, supergraph );
    
    char dtxt[20] = "discretizationStep1";
    char mtxt[12] = "usedMetric1";

    for(int i=0; i < nMetrics; i++)
      {
	char n = '1' + i;

	dtxt[18] = n;
	mtxt[10] = n;

	dataSet->set(dtxt, (double)discretizationStep[i]);
	dataSet->set(mtxt, metrics[i]);
      }

    dataSet->set("nMetrics", nMetrics);
    dataSet->set("shapeConversion", ShapeConversionCB->isChecked());

    Layout dest(supergraph);
    resultBool = supergraph->computeProperty("Scatter Plot", &dest, erreurMsg, NULL, dataSet);

    if (!resultBool) 
      QMessageBox::critical( 0, "Tulip Algorithm Check Failed", QString(("Scatter Plot::" + erreurMsg).c_str()) );
    else {
      *supergraph->getLocalProperty<Layout>("viewLayout") = dest;
    }

    glGraphWidget->setDisplayEdges(false);
    glGraphWidget->goodScale();

    if (dataSet != 0) 
      delete dataSet;

    Observable::unholdObservers();

    // cout << " ...[END]" << endl;
  }

  void TulipStats::updateDiscretizationSlot()
  {
    // cout << "[START] ... " << __PRETTY_FUNCTION__;

    discretizationStep[0] = DiscStep1->text().toDouble();
    discretizationStep[1] = DiscStep2->text().toDouble();
    discretizationStep[2] = DiscStep3->text().toDouble();

    // cout << " ...[END]" << endl;
  }

  void TulipStats::checkStepValueSlot()
  {

    if (DiscStep1->text() == "0")
      {
	DiscStep1->setText("1");

	QMessageBox::warning(this, "Invalid step", "This is nos a valid discretization step(DiscStep1) !");

	updateDiscretizationSlot();
      }
 
    if (DiscStep2->text() == "0")
      {
	DiscStep2->setText("1");

	QMessageBox::warning(this, "Invalid step", "This is nos a valid discretization step(DiscStep2) !");

	updateDiscretizationSlot();
      }      
      
    if (DiscStep3->text() == "0")
      {
	DiscStep3->setText("1");

	QMessageBox::warning(this, "Invalid step", "This is nos a valid discretization step(DiscStep3) !");

	updateDiscretizationSlot();
      }

  }

  void TulipStats::updateClusteringPlaneSlot()
  {
    if (DisplayCPCB->isChecked())
      updateClusteringPlane();
  }

  void TulipStats::chDisplayAverageSlot()
  {
    // cout << "[START] ... " << __PRETTY_FUNCTION__;

    if (AverageDisplayCB->isChecked())
      {
	Coord pos;
	bool disp[3];
	float max = 0;

	for(int i=0; i < 3; i++)
	  {
	    if (i < nMetrics)
	      {
		pos[i] = statsResults->averagePoint[i];
		disp[i] = true;

		float distmin = fabs(statsResults->averagePoint[i] - statsResults->minPoint[i]) / discretizationStep[i];
		float distmax = fabs(statsResults->averagePoint[i] - statsResults->maxPoint[i]) / discretizationStep[i];

		// cout << "Dist[" << i << "] = (" << distmin << "; " << distmax << ")" << endl;

		if (distmin > max)
		  max = distmin;
		if (distmax > max)
		  max = distmax;
	      }
	    else
	      {
		pos[i] = 0;
		disp[i] = false;
	      }
	  }

	// Managing the scene if we have an histogram :
	if (nMetrics == 1)
	  {
	    pos[1]  = -0.5f;
	    max     = histoMax;
	    disp[0] = false;
	    disp[1] = true;
	  }

	for(int i=0; i < 3; i++)
	  pos[i] /= discretizationStep[i];

	// We flip the Y axis ^^
	pos[1] *= -1;

	averagePoint = new GlADAxisPoint(pos, Color(255, 0, 0, 255), max, 1, 2);
	averagePoint->setRenderState(GlAD_AlphaBlending, true);
	averagePoint->setRenderState(GlAD_ZEnable, false);
	averagePoint->setDisplayDim(disp);

	glGraphWidget->addGlAugmentedDisplay(averagePoint, "Average");
      }
    else
      glGraphWidget->removeGlAugmentedDisplay(averagePoint);


    glGraphWidget->updateGL();

    //  cout << " ...[END]" << endl;
  }

  void TulipStats::chDisplayStdDeviationSlot()
  {
    // cout << "[START] ... " << __PRETTY_FUNCTION__;

    if (StdDeviationDisplayCB->isChecked())
      {
	Coord pos;

	Coord topLeft, bottomRight, aPoint;

	for(int i=0; i < 3; i++)
	  {
	    if (i < nMetrics)
	      {
		pos[i]     = statsResults->standardDeviationPoint[i];
		aPoint[i]  = statsResults->averagePoint[i];
		pos[i]    /= discretizationStep[i];
		aPoint[i] /= discretizationStep[i];
	      }
	    else
	      {
		pos[i] = 0.5;
		aPoint[i] = 0;
	      }
	  } 

	// Managing the scene if we have an histogram :
	if (nMetrics == 1)
	  {
	    pos[1]    = histoMax / 2 + 0.5;
	    aPoint[1] = histoMax / 2;
	  }

	topLeft     = Coord(aPoint[0] - pos[0], -aPoint[1] - pos[1], aPoint[2] - pos[2]);
	bottomRight = Coord(aPoint[0] + pos[0], -aPoint[1] + pos[1], aPoint[2] + pos[2]);

	stdDeviationBox = new GlADBox(topLeft, bottomRight, Color(220, 220, 255, 80));
	stdDeviationBox->setRenderState(GlAD_AlphaBlending, true);
	stdDeviationBox->setRenderState(GlAD_Culling, false);
	stdDeviationBox->setRenderState(GlAD_ZEnable, false);
	stdDeviationBox->setRenderState(GlAD_Wireframe, true);
	stdDeviationBox->setRenderState(GlAD_Solid, true);

	glGraphWidget->addGlAugmentedDisplay(stdDeviationBox, "StdDeviation");
      }
    else
      glGraphWidget->removeGlAugmentedDisplay(stdDeviationBox);


    glGraphWidget->updateGL();

    // cout << " ...[END]" << endl;
  }

  void TulipStats::chDisplayBoundingBoxSlot()
  {
    // cout << "[START] ... " << __PRETTY_FUNCTION__;

    if (AABBDisplayCB->isChecked())
      {
	Coord min, max;

	for(int i=0; i < 3; i++)
	  {
	    if (i < nMetrics)
	      {
		min[i] = statsResults->minPoint[i];
		max[i] = statsResults->maxPoint[i];

		min[i] = (int)(min[i] / discretizationStep[i]) - 0.5f;
		max[i] = (int)(max[i] / discretizationStep[i]) + 0.5f;
	      }
	    else
	      {
		min[i] = -0.5;
		max[i] =  0.5;
	      }
	  }

	// Managing the scene if we have an histogram :
	if (nMetrics == 1)
	  max[1] = histoMax + 0.5;

	min[1] *= -1;
	max[1] *= -1;

	// Because of the flipping of the Y axis, we have to flip min[1] and max[1] :
	float tmp = min[1];
	min[1]    = max[1];
	max[1]    = tmp;

	AABB = new GlADBox(min, max, Color(255, 255, 0, 255));
	AABB->setRenderState(GlAD_Solid, false);
	AABB->setRenderState(GlAD_Wireframe, true);
	AABB->changeRenderOptions();

	// cout << "Solid ? " << AABB->getRenderState(GlAD_Solid) << endl;

	glGraphWidget->addGlAugmentedDisplay(AABB, "AABB");
      }
    else
      glGraphWidget->removeGlAugmentedDisplay(AABB);

    glGraphWidget->updateGL();

    // cout << " ...[END]" << endl;
  }

  void TulipStats::chDisplayLinearRegressionSlot()
  {
    // cout << "[START] ... " << __PRETTY_FUNCTION__;

    if (LinearDisplayCB->isChecked())
      {
	Coord start;
	Coord end;

	start[0] = statsResults->minPoint[0] / discretizationStep[0];
	start[1] = -(statsResults->linearRegressionFunctionb0 + statsResults->linearRegressionFunctionb1 * start[0]) / discretizationStep[1];
	start[2] = 0;

	end[0] = statsResults->maxPoint[0] / discretizationStep[0];
	end[1] = -(statsResults->linearRegressionFunctionb0 + statsResults->linearRegressionFunctionb1 * end[0]) / discretizationStep[1];
	end[2] = 0;

	linearRegression = new GlADLine(start, end, Color(255, 0, 255, 255), 1);
	linearRegression->setRenderState(GlAD_ZEnable, false);

	glGraphWidget->addGlAugmentedDisplay(linearRegression, "Regression");
      }
    else
      glGraphWidget->removeGlAugmentedDisplay(linearRegression);

    glGraphWidget->updateGL();

    // cout << " ...[END]" << endl;
  }

  void TulipStats::chDisplayEigenVectorsSlot()
  {
    // cout << "[START] ... " << __PRETTY_FUNCTION__;

    if (EigenDisplayCB->isChecked())
      {
	Coord center; // The center of the bounding box
      
	for(int i=0; i < 3; i++)
	  center[i] = statsResults->minPoint[i];

	for(int i=0; i < 3; i++)
	  center[i] /= discretizationStep[i];

	center[1] *= -1;

	for(int i=0; i < 3; i++)
	  {
	    Coord endPos;

	    for(int j=0; j < 3; j++)
	      endPos[j] = center[j] + statsResults->eigenVectors[i][j];

	    Color col;

	    switch(i)
	      {
	      case 0: col = Color(100, 0, 0, 255); break;
	      case 1: col = Color(0, 100, 0, 255); break;
	      case 2: col = Color(0, 0, 100, 255); break;
	      }

	    eigenVectors[i] = new GlADLine(center, endPos, col, 2);
	    
	    char n = '0' + i;
	    char key[13] = "EigenVector0";
	    key[11] = n;

	    glGraphWidget->addGlAugmentedDisplay(eigenVectors[i], key);   
	  }
      }
    else 
      {
	for(int i=0; i < 3; i++)
	  glGraphWidget->removeGlAugmentedDisplay(eigenVectors[i]); 
      }
    glGraphWidget->updateGL();

    // cout << " ...[END]" << endl;
  }

  void TulipStats::chDisplayClusteringPlaneSlot()
  {
    // cout << "[START] ... " << __PRETTY_FUNCTION__;

    if (DisplayCPCB->isChecked())
      {

	float a, b, c, d;

	a = aEdit->text().toDouble();
	b = bEdit->text().toDouble();
	c = cEdit->text().toDouble();
	d = dEdit->text().toDouble();

	Coord p1, p2, p3, p4;

	for(int i=0; i < 3; i++)
	  {
	    if (i < nMetrics)
	      {
		p1[i] = statsResults->minPoint[i];
		p3[i] = statsResults->maxPoint[i];
	      }
	    else
	      {
		p1[i] = -0.0001;
		p3[i] = 5;
	      }
	  }

	for(int i = 0; i < 3; i++)
	  {
	    p1[i] = (int)(p1[i] / discretizationStep[i]);
	    p3[i] = (int)(p3[i] / discretizationStep[i]);
	  }

	p1[1] *= -1;
	p3[1] *= -1;

	Coord min, max;

	min = p1;
	max = p3;

	Plane plane(a, b, c, d);

	if (!plane.computePlane(p1, p2, p3, p4))
	  {
	    aEdit->setText("1");
	    a = 1;
	  
	    // We disconnect the slots to avoid a second message to pop up

	    disconnect(aEdit, SIGNAL(lostFocus()), this, SLOT(updateClusteringPlaneSlot()));
	    disconnect(bEdit, SIGNAL(lostFocus()), this, SLOT(updateClusteringPlaneSlot()));
	    disconnect(cEdit, SIGNAL(lostFocus()), this, SLOT(updateClusteringPlaneSlot()));
	    disconnect(dEdit, SIGNAL(lostFocus()), this, SLOT(updateClusteringPlaneSlot()));

	    QMessageBox::warning(this, "Invalid plane", "This is not a plane !");

	    connect(aEdit, SIGNAL(lostFocus()), SLOT(updateClusteringPlaneSlot()));
	    connect(bEdit, SIGNAL(lostFocus()), SLOT(updateClusteringPlaneSlot()));
	    connect(cEdit, SIGNAL(lostFocus()), SLOT(updateClusteringPlaneSlot()));
	    connect(dEdit, SIGNAL(lostFocus()), SLOT(updateClusteringPlaneSlot()));

	    plane.computePlane(p1, p2, p3, p4);
	  }

	Coord coordsTab[4] = {p1, p2, p3, p4};

	clusteringPlane = new GlADQuad(coordsTab, Color(255, 220, 220, 80));
	clusteringPlane->setRenderState(GlAD_AlphaBlending, true);
	clusteringPlane->setRenderState(GlAD_ZEnable, false);
	clusteringPlane->setRenderState(GlAD_Culling, false);
      
	glGraphWidget->addGlAugmentedDisplay(clusteringPlane, "Clustering Plane");
      }
    else
      glGraphWidget->removeGlAugmentedDisplay(clusteringPlane);

    glGraphWidget->updateGL();

    // cout << " ...[END]" << endl;
  }

  void TulipStats::chClusteringModelSlot(const QString &string)
  {
    // cout << "[START] ... " << __PRETTY_FUNCTION__;

    // Yes : this is a really ugly function
    QString a, b, c, d;

    if (string == "Custom")
      {
	a = "1";
	b = c = d = "0";
      }

    if (string == "Average (X)")
      {
	a = "1";
	b = "0";
	c = "0";
	d = d.setNum(-statsResults->averagePoint[0] / discretizationStep[0]);
      }

    if (string == "Average (Y)")
      {
	a = "0";
	b = "1";
	c = "0";
	d = d.setNum(statsResults->averagePoint[1] / discretizationStep[1]);
      }

    if (string == "Average (Z)")
      {
	a = "0";
	b = "0";
	c = "1";
	d = d.setNum(-statsResults->averagePoint[2] / discretizationStep[2]);
      }

    if (string == "Linear Regression Plane")
      {
	a = "1";
	b = b.setNum(statsResults->linearRegressionFunctionb1);
	c = "0";
	d = d.setNum(statsResults->linearRegressionFunctionb0);
      }

    if (string == "Eigenvectors plane (XY)")
      {
	Coord center;
	for(int i=0; i < 3; i++)
	  center[i] = ((statsResults->maxPoint[i] / discretizationStep[i]) + (statsResults->minPoint[i] / discretizationStep[i])) / 2;

	center[1] = -center[1];

	// // cout << center << endl;

	a = a.setNum(statsResults->eigenVectors[2][0]);
	b = b.setNum(statsResults->eigenVectors[2][1]);
	c = c.setNum(statsResults->eigenVectors[2][2]);
	d = d.setNum(-(a.toDouble() * center[0] + b.toDouble() * center[1] + c.toDouble() * center[2]));
      }

    if (string == "Eigenvectors plane (XZ)")
      {
	Coord center;
	for(int i=0; i < 3; i++)
	  center[i] = ((statsResults->maxPoint[i] / discretizationStep[i]) + (statsResults->minPoint[i] / discretizationStep[i])) / 2;

	center[1] = -center[1];

	a = a.setNum(statsResults->eigenVectors[1][0]);
	b = b.setNum(statsResults->eigenVectors[1][1]);
	c = c.setNum(statsResults->eigenVectors[1][2]);
	d = d.setNum(-(a.toDouble() * center[0] + b.toDouble() * center[1] + c.toDouble() * center[2]));
      }

    if (string == "Eigenvectors plane (YZ)")
      {
	Coord center;
	for(int i=0; i < 3; i++)
	  center[i] = ((statsResults->maxPoint[i] / discretizationStep[i]) + (statsResults->minPoint[i] / discretizationStep[i])) / 2;

	center[1] = -center[1];

	a = a.setNum(statsResults->eigenVectors[0][0]);
	b = b.setNum(statsResults->eigenVectors[0][1]);
	c = c.setNum(statsResults->eigenVectors[0][2]);
	d = d.setNum(-(a.toDouble() * center[0] + b.toDouble() * center[1] + c.toDouble() * center[2]));
      }

    aEdit->setText(a);
    bEdit->setText(b);
    cEdit->setText(c);
    dEdit->setText(d);

    // cout << " ...[END]" << endl;
  }

  void TulipStats::clusterizeSlot()
  {
    // cout << "[START] ... " << __PRETTY_FUNCTION__;

    string name("Plane Clustering");
    string erreurMsg;
    DataSet dataSet;

    StructDef parameter = ClusteringFactory::factory->getParam(name);
    parameter.buildDefaultDataSet( dataSet, supergraph );

    float a, b, c, d;

    a = aEdit->text().toDouble();
    b = bEdit->text().toDouble();
    c = cEdit->text().toDouble();
    d = dEdit->text().toDouble();

    dataSet.set("CoordA", a);
    dataSet.set("CoordB", b);
    dataSet.set("CoordC", c);
    dataSet.set("CoordD", d);

    if (!tlp::clusterizeGraph(supergraph, erreurMsg, &dataSet, name, NULL  )) 
      QMessageBox::critical( 0, "Tulip Algorithm Check Failed",QString((name + "::" + erreurMsg).c_str()));
  
    if (clusterTreeWidget != NULL)
      clusterTreeWidget->update();

    // cout << " ...[END]" << endl;
  }
}

