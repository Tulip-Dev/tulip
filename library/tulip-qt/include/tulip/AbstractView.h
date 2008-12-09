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
     * Get Interactors action (in MainController actions will be add to graphToolBar)
     * \warning : QAction* must be the same at each call
     */
    virtual std::list<QAction *> *getInteractorsActionList();

    /**
     * get interactors of widget
     * \return list of interactor installed on this widget
     */
    virtual tlp::Iterator<tlp::Interactor *> *getInteractors() const;

    /**
     * install a clone of the interactor as event filter and assign the returned id
     */
    tlp::Interactor::ID pushInteractor(tlp::Interactor *interactor);
    /**
     * remove the last added interactor from the event filters list and delete it
     */
    void popInteractor();
    /**
     * remove the interactor with id from the event filters list and delete it
     */
    void removeInteractor(tlp::Interactor::ID id);
    /**
     * remove all interactors and delete them, push a new one if any
     */
    tlp::Interactor::ID resetInteractors(tlp::Interactor *interactor = NULL);
    /**
     * remove all iteractors and delete them, then install clones of the interactors
     */
    std::vector<tlp::Interactor::ID> resetInteractors(const std::vector<tlp::Interactor *>&interactors);

  protected:
    /**
     * empty function : implement this function if you want a specific event filter in your view
     */
    virtual void specificEventFilter(QObject *object,QEvent *event) {}
    /**
     * empty function : implement this function if you want a context menu when you right click the mouse
     */
    virtual void buildContextMenu(QObject *object,QMouseEvent *event,QMenu *contextMenu) {}
    /**
     * empty function : implement this function if you have implement buildContextMenu()
     */
    virtual void computeContextMenuAction(QAction *action) {}

    /**
     * construct the storage of interactors
     */
    virtual void constructInteractorsMap() {}

    /**
     * construct the storage of interactors' action
     */
    virtual void constructInteractorsActionList() {}

    /**
     * set the central widget of the view
     * call this function to set view's centralWidget
     */
    void setCentralWidget(QWidget *widget);

    tlp::Interactor::ID _id;
    std::vector<tlp::Interactor *> _interactors;
    std::map<std::string,std::vector<Interactor *> > interactorsMap;
    std::list<QAction *> interactorsActionList;

    QWidget *widget;
    QVBoxLayout *mainLayout;
    QWidget *centralWidget;

  public slots:

    /**
     * this function is call by Qt
     * this function call specificEventFilter, buildContextMenu and computeContextMenu
     */
    bool eventFilter(QObject *object, QEvent *event);

  };

}

#endif






