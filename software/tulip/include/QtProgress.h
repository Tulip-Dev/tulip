#ifndef QtProgress_TULIP
#define QtProgress_TULIP
#include <string>
#include <tulip/PluginProgress.h>
#include "QtProgressData.h"


class QtProgress:
public QDialog, public Ui::QtProgressData, public tlp::PluginProgress {

  Q_OBJECT

public:
  QtProgress(QWidget* parent, std::string text);
  virtual ~QtProgress();
  void progress_handler(int i,int j);
  void setComment(std::string msg);
  void showPreview(bool);
public slots:
  void stopCompute();
  void cancelCompute();
  void changePreview(bool);

public:
  bool firstCall;
  std::string label;
  QWidget* parent;
  //GlMainWidget *glMainWidget;
};

#endif
