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

#define EDITMENU_CLEAR           0x0000
#define EDITMENU_CUT             0x0001
#define EDITMENU_COPY            0x0002
#define EDITMENU_PASTE           0x0004
#define EDITMENU_FIND            0x0008
#define EDITMENU_SELECTALL       0x0010
#define EDITMENU_DELSELECTION    0x0020
#define EDITMENU_DESELECTALL     0x0040
#define EDITMENU_INVERTSELECTION 0x0080
#define EDITMENU_CREATEGROUP     0x0100
#define EDITMENU_CREATESUBGRAPH  0x0200
#define EDITMENU_ALL             0xffff

class QMenuBar;
class QVBoxLayout;

namespace tlp {

  class Interactor;

  /** \brief Tulip view interface class
   * 
   */
  class TLP_QT_SIMPLE_SCOPE View : public QWidget {
    
    Q_OBJECT;

  public:

    View(const std::string &name,QWidget *parent=NULL);
    virtual ~View() {}
    std::string getPluginName() {return pluginName;}
    virtual void setData(Graph *graph,DataSet dataSet) = 0;
    virtual DataSet getData() = 0;
    virtual Graph *getGraph() = 0;
    virtual void getInteractorsActionList(std::list<QAction*> &interactorsList) {}
    virtual Iterator<Interactor *> *installInteractor(const std::string &) {return NULL;}

    //edit menu
    virtual int getEditMenuFlag() {return EDITMENU_CLEAR;} 

  protected:

    virtual void constructInteractorsMap() {}
    void setCentralWidget(QWidget *widget);

    QVBoxLayout *mainLayout;
    QWidget *centralWidget;
    std::map<std::string,std::vector<Interactor *> > interactorsMap;
    

  public slots:

    void showElementProperties(unsigned int eltId, bool isNode) {
      emit showElementPropertiesSignal(eltId, isNode);
    }
    virtual void changeGraph(Graph *) {};

    virtual void draw() = 0;
    virtual void reinitAndDraw() = 0;

    virtual bool eventFilter(QObject *object, QEvent *event) = 0;

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






