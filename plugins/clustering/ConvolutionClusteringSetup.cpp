#include "ConvolutionClusteringSetup.h"
#include <iostream>
#include <qpainter.h>
#include <qpaintdevice.h>
#include <vector>
#include <cstdlib>
#include <qslider.h>
#include <cmath>

using namespace std;
using namespace tlp;
/* 
 *  Constructs a ConvolutionClusteringSetup which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ConvolutionClusteringSetup::ConvolutionClusteringSetup( ConvolutionClustering *convolPlugin,QWidget* parent,  const char* name, bool modal, Qt::WFlags fl )
  : ConvolutionClusteringSetupData( parent, name, modal, fl ), 
    convolPlugin(convolPlugin),
    useLogarithmicScale(false)
{
  int a,b,c;
  convolPlugin->getParameters(a,b,c);
  widthSlider->setMinValue(1);
  widthSlider->setMaxValue(a);
  widthSlider->setValue(c);
  discretizationSlider->setMinValue(1);
  discretizationSlider->setMaxValue(2*a);
  discretizationSlider->setValue(a);
}

/*  
 *  Destroys the object and frees any allocated resources
 */
ConvolutionClusteringSetup::~ConvolutionClusteringSetup()
{
    // no need to delete child widgets, Qt does it all for us
}

/* 
 * public slot
 */
void ConvolutionClusteringSetup::setlog(bool b) {
  useLogarithmicScale=b;
  update();
}

void ConvolutionClusteringSetup::update() {
  // GC 4.2: >? is no longer supported
  //widthSlider->setMaxValue(discretizationSlider->value() / 2 >? 1);
  widthSlider->setMaxValue(std::max(discretizationSlider->value() / 2, 1));
  convolPlugin->setParameters(discretizationSlider->value(),0,widthSlider->value());
  paintEvent(0);
}

void ConvolutionClusteringSetup::paintEvent( QPaintEvent * ) {
  QPainter *p = new QPainter((QWidget *)Frame3);
  vector<double> &histogram = *(convolPlugin->getHistogram());
  if (histogram.size()<1) reject();
  
  double theMax,theMin;
  theMax = histogram[0];
  theMin = histogram[0];
  for (unsigned int i=1; i<histogram.size(); ++i) {
    if (theMax < histogram[i]) theMax = histogram[i]; //theMax = theMax >? histogram[i];
    if (theMin > histogram[i]) theMin = histogram[i]; //theMin = theMin <? histogram[i];
  }
  if (useLogarithmicScale) {
    theMax = log10(theMax + 1.0);
    theMin = log10(theMin + 1.0);
  }

  //compute axis position
  QFont f( "times", 12, QFont::Bold );
  p->setFont( f );
  p->setPen( Qt::black );

  //les 20 de plus permetront de placer des légendes sur les axes.
  double scale = double(histogram.size())/64.0;
  int legendWidth = (int)(20.0*scale);
  int borderWidth = (int)(10.0*scale);
  int axisWidth = (int)(15.0*scale);
  p->setWindow( 0, 0, 2*histogram.size()+legendWidth, histogram.size()+legendWidth ); // defines coordinate system
  p->fillRect(0, 0, 
	      2*histogram.size()+legendWidth, histogram.size()+legendWidth , 
	      QBrush(QColor(255,255,255)));
  // draw bars
  QColor c;
  double histoScale = double(histogram.size()) / theMax;

  for (unsigned int i=0; i<histogram.size(); i++ ) {	
    c.setHsv( (int) ((double)i*360.0/(double)histogram.size()) , 255, 255 );
    p->setBrush( c );
    int height;

    if (useLogarithmicScale)
      height = (int)(log10(1.0 + double(histogram[i])) *histoScale );
    else
      height = (int)(double(histogram[i]) * histoScale);
    if (height < 1) height = 1; // height = height >? 1;
    p->drawRect( borderWidth+i*2, borderWidth  + 1 + histogram.size() - height, 
		 2, height );
  }

  //draw axis
  p->drawLine(borderWidth, borderWidth, borderWidth, 
	      histogram.size()+borderWidth);
  p->drawLine(borderWidth, histogram.size()+borderWidth, 
	      2*histogram.size()+axisWidth, histogram.size()+borderWidth);
  c.setHsv( 359 , 255, 255 );

  //draw local minimum
  list<int> localMinimum=convolPlugin->getLocalMinimum();
  while (!localMinimum.empty()) {
    int i=localMinimum.front();
    localMinimum.pop_front();
    p->drawLine(borderWidth+i*2, borderWidth,
		borderWidth+i*2, histogram.size()+borderWidth);
  }
  delete p;
}

