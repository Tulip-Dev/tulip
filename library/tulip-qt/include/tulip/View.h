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
   */
  class View : public QWidget, public GraphObserver {
    
    Q_OBJECT;

  public:

    View(const std::string &name,QWidget *parent=NULL);
    virtual ~View() {}
    std::string getPluginName() {return pluginName;}
    virtual void setData(Graph *graph,DataSet dataSet) = 0;
    virtual DataSet getData() = 0;
    virtual Graph *getGraph() = 0;
    virtual QWidget *getWidget() = 0;
    virtual void getInteractorsActionList(std::list<QAction*> &interactorsList) {}
    virtual Iterator<Interactor *> *installInteractor(const std::string &) {return NULL;}

    virtual bool cutIsEnable() {return false;}
    virtual bool copyIsEnable() {return false;}
    virtual bool pasteIsEnable() {return false;}
    virtual bool findIsEnable() {return false;}
    virtual bool selectAllIsEnable() {return false;}
    virtual bool delSelectionIsEnable() {return false;}
    virtual bool deselectAllIsEnable() {return false;}
    virtual bool invertSelectionIsEnable() {return false;}
    virtual bool createGroupIsEnable() {return false;}
    virtual bool createSubgraphIsEnable() {return false;}

    //For plugin progress handler
    virtual bool doProgressUpdate() {return false;}
    virtual void progressUpdate() {}

  protected:
    virtual void constructInteractorsMap() {}
    void setCentralWidget(QWidget *widget);

    QVBoxLayout *mainLayout;
    QMenuBar *menuBar;
    std::map<std::string,std::vector<Interactor *> > interactorsMap;

  public slots:

    void showElementProperties(unsigned int eltId, bool isNode) {
      emit showElementPropertiesSignal(eltId, isNode);
    }
    virtual void changeGraph(Graph *) {}

    virtual void cut() {}
    virtual void copy() {}
    virtual void paste() {}
    virtual void find() {}
    virtual void selectAll() {}
    virtual void delSelection() {}
    virtual void deselectAll() {}
    virtual void invertSelection() {}
    virtual void createGroup() {}
    virtual void createSubgraph() {}

  signals:
    void showElementPropertiesSignal(unsigned int eltId, bool isNode);
    void clusterTreeNeedUpdate();

  private :
    
    std::string pluginName;
  
  };

  /** \brief Tulip view creator interface class
   * 
   */
  class ViewCreator : public WithParameter, public WithDependency {
  
  public:
    ViewCreator() {}
    virtual ~ViewCreator() {}

    virtual View* create(const std::string &name,QWidget *parent) = 0;
  
  };

  class ViewCreatorContext {
    
  };

  class ViewCreatorFactory: public Plugin {
  public:
    virtual ~ViewCreatorFactory() {}
    ///
    virtual ViewCreator *createPluginObject(ViewCreatorContext *ic)=0;
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

    static TemplateFactory<ViewCreatorFactory,ViewCreator,ViewCreatorContext *> *factory;
    static void initFactory() {
      if (!factory) {
	factory = new TemplateFactory<ViewCreatorFactory,ViewCreator,ViewCreatorContext *>;
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
  string getTulipRelease() const {return string("test"/*TULIP_RELEASE*/);} \
  int    getId() const {return ID;}		 \
  tlp::T * createPluginObject(ViewCreatorContext *ic)		 \
  {						 \
    C *tmp = new C();				 \
    return ((tlp::T *) tmp);			 \
  }						 \
};                                               \
extern "C" {                                            \
  C##T##Factory C##T##FactoryInitializer;               \
}

#define VIEWPLUGINOFGROUP(C,N,A,D,I,R,ID,G) VIEWPLUGINFACTORY(ViewCreator,C,N,A,D,I,R,ID,G)
#define VIEWPLUGIN(C,N,A,D,I,R,ID) VIEWPLUGINOFGROUP(C,N,A,D,I,R,ID,"") 

#endif






