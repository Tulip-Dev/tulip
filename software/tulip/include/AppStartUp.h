#ifndef APPSTARTUP_H
#define APPSTARTUP_H
#include <string>
#include <QtGui/QMovie>
#include "AppStartUpData.h"

namespace tlp {
  struct TulipPluginLoader;
}

class AppStartUp : public AppStartUpForm
{ 
    Q_OBJECT

   QMovie *movie;
    int totalSteps;
    int currentFrame;

public:
  AppStartUp( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
    ~AppStartUp();
public slots:
    ///Change the label in the widget
    void setLabel(std::string label);
    ///Change the progress bar.
    void setProgress(int);
    ///Change the total step of the progress bar.
    void setTotalSteps(int);
    void initTulip(tlp::TulipPluginLoader *loader,std::string& errors);
};

#endif // APPSTARTUP_H
