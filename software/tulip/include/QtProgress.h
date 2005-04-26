#ifndef QtProgress_TULIP
#define QtProgress_TULIP
#include <string>
#include <tulip/PluginProgress.h>
#include "QtProgressData.h"

class GlGraphWidget;

class MyProgress:public QtProgressData, public PluginProgress {

  Q_OBJECT

public:
  MyProgress(QWidget* parent,std::string text, GlGraphWidget * =0);
  virtual ~MyProgress();
  void progress_handler(int i,int j);
public slots:
  void stopCompute();
  void cancelCompute();

public:
  bool firstCall;
  std::string label;
  QWidget* parent;
  GlGraphWidget *glGraphWidget;
};

#endif
