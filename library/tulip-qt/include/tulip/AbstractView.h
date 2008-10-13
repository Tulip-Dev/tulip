#ifndef _Tulip_ABSTRACTVIEW_H
#define _Tulip_ABSTRACTVIEW_H

#include "View.h"


namespace tlp {

  /** \brief Tulip abstract view class
   * 
   */
  class TLP_QT_SIMPLE_SCOPE AbstractView : public View {
    
    Q_OBJECT;

  public:

    AbstractView(const std::string &name,QWidget *parent=NULL);
    virtual ~AbstractView();

    /**
     * return list of interactor installed on this widget
     */
    virtual tlp::Iterator<tlp::Interactor *> *getInteractors() const;

    /// install a clone of the interactor as event filter and assign the returned id
    tlp::Interactor::ID pushInteractor(tlp::Interactor *interactor);
    /// remove the last added interactor from the event filters list and delete it
    void popInteractor();
    /// remove the interactor with id from the event filters list and delete it
    void removeInteractor(tlp::Interactor::ID id);
    ///  remove all interactors and delete them, push a new one if any
    tlp::Interactor::ID resetInteractors(tlp::Interactor *interactor = NULL);
    /// remove all iteractors and delete them, then install clones of the interactors
    std::vector<tlp::Interactor::ID> resetInteractors(const std::vector<tlp::Interactor *>&interactors);

  protected:
    //eventFilter 
    virtual void specificEventFilter(QObject *object,QEvent *event) {}
    virtual void buildContextMenu(QObject *object,QMouseEvent *event,QMenu *contextMenu) {}
    virtual void computeContextMenuAction(QAction *action) {}

    virtual void constructInteractorsMap() {}
    void setCentralWidget(QWidget *widget);

    tlp::Interactor::ID _id;
    std::vector<tlp::Interactor *> _interactors;
    std::map<std::string,std::vector<Interactor *> > interactorsMap;

    QVBoxLayout *mainLayout;
    QWidget *centralWidget;

  public slots:

    bool eventFilter(QObject *object, QEvent *event);
  
  };

}

#endif






