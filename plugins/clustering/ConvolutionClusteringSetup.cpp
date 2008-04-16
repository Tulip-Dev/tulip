#include "ConvolutionClusteringSetup.h"
#include <iostream>
#include <qpainter.h>
#include <qpaintdevice.h>
#include <qlayout.h>
#include <vector>
#include <cstdlib>
#include <qslider.h>
#include <cmath>

using namespace std;
using namespace tlp;

class HistogramWidget : public QWidget {
  
public:
  HistogramWidget(ConvolutionClusteringSetup *dialog, QWidget *parent)
    : QWidget(parent), setupDialog(dialog)
  {
    // setFrameShape(Q3Frame::StyledPanel); setFrameShadow(Q3Frame::Raised);
  }

  void paintEvent(QPaintEvent *) {
    QPainter painter(this);
    vector<double> &histogram = *(setupDialog->getPlugin()->getHistogram());
    if (histogram.size() < 1) {
      setupDialog->abort();
      return;
    }
    
    double theMax,theMin;
    theMax = histogram[0];
    theMin = histogram[0];
    for (unsigned int i=1; i<histogram.size(); ++i) {
      if (theMax < histogram[i]) theMax = histogram[i];
      if (theMin > histogram[i]) theMin = histogram[i];
    }
    if (setupDialog->getLogarithmicScale()) {
      theMax = log10(theMax + 1.0);
      theMin = log10(theMin + 1.0);
    }

    //compute axis position
    QFont f( "times", 12, QFont::Bold );
    painter.setFont( f );
    painter.setPen( Qt::black );

    //les 20 de plus permetront de placer des légendes sur les axes.
    double scale = double(histogram.size())/64.0;
    int legendWidth = (int)(20.0*scale);
    int borderWidth = (int)(10.0*scale);
    int axisWidth = (int)(15.0*scale);
    painter.setWindow( 0, 0, 2*histogram.size()+legendWidth, histogram.size()+legendWidth ); // defines coordinate system
    painter.fillRect(0, 0, 
		     2*histogram.size()+legendWidth, histogram.size()+legendWidth , 
		     QBrush(QColor(255,255,255)));
    // draw bars
    QColor c;
    double histoScale = double(histogram.size()) / theMax;

    for (unsigned int i=0; i<histogram.size(); i++ ) {	
      c.setHsv( (int) ((double)i*360.0/(double)histogram.size()) , 255, 255 );
      painter.setBrush( c );
      int height;

      if (setupDialog->getLogarithmicScale())
        height = (int)(log10(1.0 + double(histogram[i])) *histoScale );
      else
        height = (int)(double(histogram[i]) * histoScale);
      if (height < 1) height = 1;
      painter.drawRect( borderWidth+i*2, borderWidth  + 1 + histogram.size() - height, 
			2, height );
    }

    //draw axis
    painter.drawLine(borderWidth, borderWidth, borderWidth, 
		     histogram.size()+borderWidth);
    painter.drawLine(borderWidth, histogram.size()+borderWidth, 
		     2*histogram.size()+axisWidth, histogram.size()+borderWidth);
    c.setHsv( 359 , 255, 255 );

    //draw local minimum
    list<int> localMinimum=setupDialog->getPlugin()->getLocalMinimum();
    while (!localMinimum.empty()) {
      int i=localMinimum.front();
      localMinimum.pop_front();
      painter.drawLine(borderWidth+i*2, borderWidth,
		       borderWidth+i*2, histogram.size()+borderWidth);
    }
  }
private:
  ConvolutionClusteringSetup *setupDialog;
};


/* 
 *  Constructs a ConvolutionClusteringSetup which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ConvolutionClusteringSetup::ConvolutionClusteringSetup( ConvolutionClustering *convolPlugin, QWidget* parent)
  : QDialog( parent), 
    convolPlugin(convolPlugin),
    useLogarithmicScale(false) {
  setupUi(this);
  histogramWidget = new HistogramWidget(this, Frame3);
  QGridLayout *flayout = new QGridLayout(Frame3, 1, 1, 1);
  flayout->addWidget(histogramWidget, 0, 0);

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
  if (histogramWidget)
    histogramWidget->update();
  QWidget::update();
}
