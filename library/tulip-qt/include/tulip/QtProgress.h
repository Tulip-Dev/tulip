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
