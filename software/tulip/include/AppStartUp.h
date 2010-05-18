#ifndef APPSTARTUP_H
#define APPSTARTUP_H
#include <string>
#include <QtGui/QMovie>
#include "AppStartUpData.h"

namespace tlp {
  struct TulipPluginLoader;
}

class AppStartUp : public QDialog, public Ui::AppStartUpData { 
  Q_OBJECT

  QMovie *movie;
  int totalSteps;
  int currentFrame;
  std::string errorMgs;
  bool dontClose;
  unsigned int currentTipNumber;

public:
  AppStartUp( QWidget* parent = 0);
  ~AppStartUp();
  void addErrorMsg(const std::string &errMsg);
  
public slots:
    ///Change the label in the widget
    void setLabel(std::string label);
    ///Change the progress bar.
    void setProgress(int);
    ///Change the total step of the progress bar.
    void setTotalSteps(int);
    void initTulip(tlp::TulipPluginLoader *loader,std::string& errors);
    void nextTip();
};

#endif // APPSTARTUP_H
