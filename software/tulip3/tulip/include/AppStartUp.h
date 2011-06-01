/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#ifndef APPSTARTUP_H
#define APPSTARTUP_H
#include <string>
#include <QtGui/QMovie>
#include "ui_AppStartUp.h"

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
