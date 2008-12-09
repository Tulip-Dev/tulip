#ifndef _Tulip_VIEWINTERFACE_H
#define _Tulip_VIEWINTERFACE_H

#include <QtGui/QWidget>
#include <QtGui/QMainWindow>

#include <tulip/Plugin.h>
#include <tulip/TulipRelease.h>
#include <tulip/TemplateFactory.h>
#include <tulip/ObservableGraph.h>
#include <tulip/Vector.h>
#include "GWInteractor.h"

class QMenuBar;
class QVBoxLayout;

namespace tlp {

  class Interactor;

  /** \brief Tulip view interface class
   *
   * View class provide the skeleton of a view
   */
  class TLP_QT_SCOPE View : public QObject,public WithParameter, public WithDependency{

    Q_OBJECT;

  public:

    /**
     * empty destructor : for inheritance
     */
    virtual ~View() {}
    /**
     * Construct GUI of the view
     * \return QWidget is the main widget of the view (in MainController this widget will be add to workspace)
     */
    virtual QWidget *construct(QWidget *parent) = 0;
    /**
     * Set the graph and data to the view
     */
    virtual void setData(Graph *graph,DataSet dataSet) = 0;
    /**
     * Get the graph and data of the view
     */
    virtual void getData(Graph **graph,DataSet *dataSet) = 0;
    /**
     * get Graph attached to this view
     */
    virtual Graph *getGraph() = 0;
    /**
     * Get Interactors action (in MainController actions will be add to graphToolBar)
     * \warning : QAction* must be the same at each call
     */
    virtual std::list<QAction *> *getInteractorsActionList() = 0;
    /**
     * Install a specific interactor to the view
     * \param QAction must be an element of list<QAction *> of getInteractorsActionList(...)
     */
    virtual void installInteractor(QAction *) = 0;

  public slots:

    /**
     * Change the graph
     */
    virtual void setGraph(Graph *graph) = 0;

    /**
     * Draw the view
     */
    virtual void draw() = 0;
    /**
     * Refresh the view (data don't be modified)
     */
    virtual void refresh() = 0;
    /**
     * Reinit the view and draw
     */
    virtual void init() = 0;

  signals:
    /**
     * This signal is emit when a specific element is selected in the view
     */
    void elementSelected(unsigned int eltId, bool isNode);

  };

  class TLP_QT_SCOPE ViewContext {
  };

  class TLP_QT_SCOPE ViewFactory: public Plugin {
  public:
    virtual ~ViewFactory() {}
    ///
    virtual View *createPluginObject(ViewContext *ic)=0;
    ///
    virtual int getId() const=0;

    virtual  std::string getMajor() const {
      return tlp::getMajor(getRelease());
    }
    virtual  std::string getMinor() const  {
      return tlp::getMinor(getRelease());
    }
    virtual  std::string getTulipMajor() const {
      return tlp::getMajor(getTulipRelease());
    }
    virtual  std::string getTulipMinor() const  {
      return tlp::getMinor(getTulipRelease());
    }

    static TemplateFactory<ViewFactory,View,ViewContext *> *factory;
    static void initFactory() {
      if (!factory) {
	factory = new TemplateFactory<ViewFactory,View,ViewContext *>;
      }
    }
  };

}

#define VIEWPLUGINFACTORY(T,C,N,A,D,I,R,ID,G)     \
class C##T##Factory:public tlp::T##Factory	 \
{                                                \
public:                                          \
  C##T##Factory(){				 \
    initFactory(); 			         \
    factory->registerPlugin(this);	         \
  }       					 \
  string getName() const { return string(N);}	 \
  string getGroup() const { return string(G);}	 \
  string getAuthor() const {return string(A);}	 \
  string getDate() const {return string(D);}	 \
  string getInfo() const {return string(I);}	 \
  string getRelease() const {return string(R);}\
  string getTulipRelease() const {return string(TULIP_RELEASE);} \
  int    getId() const {return ID;}		 \
  tlp::T * createPluginObject(ViewContext *ic)		 \
  {						 \
  	C *tmp = new C();				 \
    return ((tlp::T *) tmp);			 \
  }						 \
};                                               \
extern "C" {                                            \
  C##T##Factory C##T##FactoryInitializer;               \
}

#define VIEWPLUGINOFGROUP(C,N,A,D,I,R,ID,G) VIEWPLUGINFACTORY(View,C,N,A,D,I,R,ID,G)
#define VIEWPLUGIN(C,N,A,D,I,R,ID) VIEWPLUGINOFGROUP(C,N,A,D,I,R,ID,"")

#endif






