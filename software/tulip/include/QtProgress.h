#ifndef QtProgress_TULIP
#define QtProgress_TULIP
#include <string>
#include <tulip/PluginProgress.h>
#include "QtProgressData.h"

class GlGraphWidget;

class QtProgress:public QtProgressData, public tlp::PluginProgress {

  Q_OBJECT

public:
  QtProgress(QWidget* parent,std::string text, GlGraphWidget * =0);
  virtual ~QtProgress();
  void progress_handler(int i,int j);
public slots:
  void stopCompute();
  void cancelCompute();
  void changePreview(bool);

public:
  bool firstCall;
  std::string label;
  QWidget* parent;
  GlGraphWidget *glGraphWidget;
};

#endif
