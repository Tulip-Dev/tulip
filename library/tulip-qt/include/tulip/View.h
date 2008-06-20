#ifndef _Tulip_VIEWINTERFACE_H
#define _Tulip_VIEWINTERFACE_H

#include <QtGui/QWidget>
#include <QtGui/QMainWindow>

#include <tulip/Plugin.h>
#include <tulip/TulipRelease.h>
#include <tulip/TemplateFactory.h>
#include <tulip/ObservableGraph.h>
#include <tulip/Vector.h>

namespace tlp {

  class Interactor;

  /** \brief Tulip view interface class
   * 
   */
  class View : public QMainWindow, public GraphObserver {
    
    Q_OBJECT;

  public:

    View(QWidget *parent=NULL);
    virtual ~View() {}
    virtual void setData(Graph *graph,std::string *in=NULL) = 0;
    virtual Graph *getGraph() = 0;
    virtual QWidget *getWidget() = 0;
    virtual void getInteractorsActionList(std::list<QAction*> &interactorsList) {}
    virtual Iterator<Interactor *> *installInteractor(const std::string &) {return NULL;}
    virtual void getEditMenuFlags(Vector<bool, 10> &flags) {flags.fill(0);}

  protected:
    virtual void constructInteractorsMap() {}

    std::map<std::string,std::vector<Interactor *> > interactorsMap;

  signals:
    void showElementPropertiesSignal(unsigned int eltId, bool isNode);
    
  };

  /** \brief Tulip view creator interface class
   * 
   */
  class ViewCreator : public WithParameter, public WithDependency {
  
  public:
    ViewCreator() {}
    virtual ~ViewCreator() {}

    virtual View* create(QWidget *parent) = 0;
  
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






