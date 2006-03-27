#ifndef APPSTARTUP_H
#define APPSTARTUP_H
#include <string>
#include "AppStartUpData.h"

class AppStartUp : public AppStartUpForm
{ 
    Q_OBJECT

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
    void initTulip();
};

#endif // APPSTARTUP_H
