#ifndef _Tulip_VIEWINTERFACE_H
#define _Tulip_VIEWINTERFACE_H

#include <QtGui/QWidget>
#include <QtGui/QMainWindow>

#include <tulip/Plugin.h>
#include <tulip/TulipRelease.h>
#include <tulip/TemplateFactory.h>
#include <tulip/ObservableGraph.h>
#include <tulip/Vector.h>

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
     * Return the widget of this view
     */
    virtual QWidget *getWidget() =0;
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
     * Return configuration widget used, this widget will be add on left of the MainController
     */
    virtual std::list<std::pair<QWidget *,std::string> > getConfigurationWidget() {return std::list<std::pair<QWidget *,std::string> >();}
    /**
     * Set all interactors available forthis view
     * Interactors are create (allocate) but now view have responsibility of her destruction
     */
    virtual void setInteractors(const std::list<Interactor *> &interactors) = 0;
    /**
     * Return interactors of this view
     */
    virtual std::list<Interactor *> getInteractors() = 0;
    /**
     * Set active interactor on this view
     */
    virtual void setActiveInteractor(Interactor *interactor) = 0;
    /**
     * Take a snapshot of the view and put it in a picture
     */
    virtual void createPicture(const std::string &pictureName,int width=0, int height=0) = 0;

    /**
     * Return the real view name (if "" the real name is the name give by plugin)
     */
    virtual std::string getRealViewName() {return "";}

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

    /**
     * This signal is emit when the view want to change its graph
     */
    void requestChangeGraph(View *,Graph *);

  };

  class TLP_QT_SCOPE ViewContext {
  };

  class TLP_QT_SCOPE ViewFactory: public Plugin {
  public:
    virtual ~ViewFactory() {}
    ///
    virtual View *createPluginObject(ViewContext *ic)=0;

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

#define VIEWPLUGINFACTORY(T,C,N,A,D,I,R,G)     \
class C##T##Factory:public tlp::T##Factory	 \
{                                                \
public:                                          \
  C##T##Factory(){				 \
    initFactory(); 			         \
    factory->registerPlugin(this);	         \
  }       					 \
  std::string getName() const { return std::string(N);}	\
  std::string getGroup() const { return std::string(G);}	 \
  std::string getAuthor() const {return std::string(A);}	 \
  std::string getDate() const {return std::string(D);}	 \
  std::string getInfo() const {return std::string(I);}	 \
  std::string getRelease() const {return std::string(R);}\
  std::string getTulipRelease() const {return std::string(TULIP_RELEASE);}	\
  tlp::T * createPluginObject(tlp::ViewContext *)			\
  {						 \
  	C *tmp = new C();				 \
    return ((tlp::T *) tmp);			 \
  }						 \
};                                               \
extern "C" {                                            \
  C##T##Factory C##T##FactoryInitializer;               \
}

#define VIEWPLUGINOFGROUP(C,N,A,D,I,R,G) VIEWPLUGINFACTORY(View,C,N,A,D,I,R,G)
#define VIEWPLUGIN(C,N,A,D,I,R) VIEWPLUGINOFGROUP(C,N,A,D,I,R,"")

#endif






