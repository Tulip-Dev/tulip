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
#ifndef _Tulip_ABSTRACTVIEW_H
#define _Tulip_ABSTRACTVIEW_H

#include "View.h"


namespace tlp {

  /** \brief Abstract view provide interactors' functions
   *
   *  Abstract view provide a View with interactors' basic functions like getInteractors, pushInteractor and popInteractor
   *  You can inherit from it if you want this functions
   *  In tulip-qt GlMainView inherit from it
   */
  class TLP_QT_SCOPE AbstractView : public View {

    Q_OBJECT;

  public:

    /**
     * Basic constructor
     */
    AbstractView();
    /*
     * Basic destructor
     */
    virtual ~AbstractView();

    /*
     * Construct the abstract view
     * \warning If you want to inherit from it, you must call AbstractView::construct(QWidget *) in your new construct function
     */
    virtual QWidget *construct(QWidget *parent);
    /*
     * get the widget who will be add to workspace by the controller
     * \return the widget of the abstract view
     */
    QWidget *getWidget(){return widget;}

    /**
     * Set all interactors available forthis view
     * Interactors are create (allocate) but now view have responsibility of her destruction
     */
    virtual void setInteractors(const std::list<Interactor *> &interactorsList);
    /**
     * Return interactors of this view
     */
    virtual std::list<Interactor *> getInteractors();
    /**
     * Set active interactor on this view
     */
    virtual void setActiveInteractor(Interactor *interactor);
    /**
     * return current interactor
     */
    Interactor *getActiveInteractor(){return activeInteractor;}

  protected:
    /**
     * empty function : implement this function if you want a specific event filter in your view
     */
    virtual void specificEventFilter(QObject *,QEvent *) {}
    /**
     * empty function : implement this function if you want a context menu when you right click the mouse
     */
    virtual void buildContextMenu(QObject *object,QMouseEvent *event,QMenu *contextMenu) ;

    virtual void buildOutputImagesFormatsList(std::set<std::string>& outputFormats);
    /**
     * empty function : implement this function if you have implement buildContextMenu()
     */
    virtual void computeContextMenuAction(QAction *) {}

    /**
     * set the central widget of the view
     * call this function to set view's centralWidget
     */
    void setCentralWidget(QWidget *widget);

    QWidget *widget;
    QVBoxLayout *mainLayout;
    QWidget *centralWidget;
    std::list<Interactor *> interactors;
    Interactor *activeInteractor;

    QMenu *exportImageMenu;

  public slots:

    /**
     * this function is call by Qt
     * this function call specificEventFilter, buildContextMenu and computeContextMenu
     */
    bool eventFilter(QObject *object, QEvent *event);

    virtual void exportImage(QAction* action);

  };

}

#endif






