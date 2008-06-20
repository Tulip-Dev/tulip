//-*-c++-*-

/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 16/05/08
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef Tulip_TABWIDGET_H
#define Tulip_TABWIDGET_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/TabWidgetData.h"


namespace tlp {

  class TabWidget : public QWidget, public Ui::TabWidgetData {
  public:
    TabWidget(QWidget* parent) : QWidget(parent) {
      setupUi(this);
    }
  };

#endif

}
