#ifndef CONVOLUTIONCLUSTERINGSETUP_H
#define CONVOLUTIONCLUSTERINGSETUP_H
#include "ConvolutionClusteringSetupData.h"
#include "ConvolutionClustering.h"

class ConvolutionClusteringSetup : public ConvolutionClusteringSetupData { 
    Q_OBJECT

public:
  ConvolutionClusteringSetup( ConvolutionClustering *convolPlugin,QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
    ~ConvolutionClusteringSetup();

  ConvolutionClustering *getPlugin() { return convolPlugin; }
  bool getLogarithmicScale() { return useLogarithmicScale; }
  void abort() { reject(); }

public slots:
  void update();
  void setlog(bool);
private:
  QWidget *histogramWidget;
  ConvolutionClustering *convolPlugin;
  bool useLogarithmicScale;
};

#endif // CONVOLUTIONCLUSTERINGSETUP_H
