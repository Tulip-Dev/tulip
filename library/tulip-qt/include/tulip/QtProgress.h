#ifndef QtProgress_TULIP
#define QtProgress_TULIP
#include <string>
#include <tulip/PluginProgress.h>
#include <tulip/View.h>
#include "tulip/QtProgressData.h"


namespace tlp {

  class TLP_QT_SCOPE QtProgress : public QDialog, public Ui::QtProgressData, public tlp::PluginProgress {
    
    Q_OBJECT
      
  public:
    QtProgress(QWidget* parent, std::string text, View *view=0);
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
    View *view;
  };
}

#endif
