//-*-c++-*-

/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 02/06/2008
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef Tulip_RENDERINGPARAMETERSDIALOG_H
#define Tulip_RENDERINGPARAMETERSDIALOG_H

#include <QtGui/QWidget>

#include "RenderingParametersDialogData.h"
#include "GlMainView.h"

namespace tlp {

  class RenderingParametersDialog : public QDialog, public Ui::RenderingParametersDialogData {
    
    Q_OBJECT

    GlMainView *mainView;

  public:

    RenderingParametersDialog(QWidget* parent);
    void setGlMainView(GlMainView *view);
    void windowActivationChange(bool oldActive);

  public slots:

    void updateView();
    void backColor();
    void setBackgroundColor(QColor tmp);
  
  };

}

#endif
