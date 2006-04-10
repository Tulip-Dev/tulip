#ifndef CONVOLUTIONCLUSTERINGSETUP_H
#define CONVOLUTIONCLUSTERINGSETUP_H
#include "ConvolutionClusteringSetupData.h"
#include "ConvolutionClustering.h"

class ConvolutionClusteringSetup : public ConvolutionClusteringSetupData { 
    Q_OBJECT

public:
    ConvolutionClusteringSetup( ConvolutionClustering *convolPlugin,QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ConvolutionClusteringSetup();

public slots:
void update();
  void paintEvent( QPaintEvent * );
  void setlog(bool);
private:
  ConvolutionClustering *convolPlugin;
  bool useLogarithmicScale;
};

#endif // CONVOLUTIONCLUSTERINGSETUP_H
