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
#ifndef Tulip_QTOPENGLERRORVIEWER_H
#define Tulip_QTOPENGLERRORVIEWER_H

#include <QtGui/QWidget>

#include <tulip/OpenGlErrorViewer.h>

#include "tulip/QtOpenGlErrorViewerWithAskAgainDialogData.h"

namespace tlp {

/** \brief Simple QDialog with an ask again check box
 *
 * This class is used by QtOpenGlErrorViewer to display errors with ask again check box
 */
class TLP_QT_SCOPE QtOpenGlErrorViewerWithAskAgainDialog : public QDialog,public Ui::QtOpenGlErrorViewerWithAskAgainDialogData {

public :

  QtOpenGlErrorViewerWithAskAgainDialog(QWidget *parent=0);

};

/** \brief Simple class to display OpenGl error is QDialog
 *
 * This class is used to display OpenGl error is QDialog
 */
class TLP_QT_SCOPE QtOpenGlErrorViewer : public OpenGlErrorViewer {

public :

  /**
   * Basic constructor
   */
  QtOpenGlErrorViewer(QWidget *parent=0):parent(parent){}

  /**
   * Display an error in a QDialog with title : title, and text : errorMsg
   */
  virtual void displayError(const std::string &title,const std::string &errorMsg);

  /**
   * Display an error in a QDialog with title : title, and text : errorMsg
   * In this dialog we had a ask again check box
   */
  virtual void displayErrorWithAskAgain(const std::string &title,const std::string &errorMsg);

protected :

  QWidget *parent;

};

}

#endif
