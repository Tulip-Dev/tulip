#ifndef _GL_INTERACTOR_H
#define _GL_INTERACTOR_H

#include <QtCore/qobject.h>
#include <QtGui/QIcon>

#include <tulip/Plugin.h>
#include <tulip/TulipRelease.h>
#include <tulip/TemplateFactory.h>

namespace tlp {

  class GlMainWidget;
  class View;

  /** \brief Tulip interactor main class
   *
   */
  class TLP_QT_SCOPE Interactor  : public QObject, public WithParameter, public WithDependency {

  public:
    typedef unsigned int ID;

  protected:
    ID id;
    View *view;

  public:
    /**
     * Default constructor
     */
    Interactor() {}
    /*
     * Defaulkt destructor
     */
    virtual ~Interactor() {}

    /**
     * This function compute the interactor visual feedback
     */
    virtual bool compute(GlMainWidget *glMainWidget) { return false; }
    /**
     * This function draw interactor
     */
    virtual bool draw(GlMainWidget *glMainWidget){ return false; }
    /**
     * Clone the interactor
     */
    virtual Interactor *clone() = 0;
    /**
     * \return the id of this interactor
     */
    ID getID() { return id; }
    /**
     * Set the id of this interactor
     */
    void setID(ID i) { id = i; }
    static const ID invalidID = 0;

    /**
     * Set interactor connected view
     */
    void setView(View *view) {this->view=view;}
    /**
     * \return the interactor connected view
     */
    View *getView() {return view;}

  };

  class TLP_QT_SCOPE InteractorContext {

  public :
    InteractorContext(){}

  };

  class TLP_QT_SCOPE InteractorFactory: public Plugin {
  public:
    virtual ~InteractorFactory() {}
    ///
    virtual Interactor *createPluginObject(InteractorContext *ic)=0;

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

    static TemplateFactory<InteractorFactory,Interactor,InteractorContext *> *factory;
    static void initFactory() {
      if (!factory) {
	factory = new TemplateFactory<InteractorFactory,Interactor,InteractorContext *>;
      }
    }
  };

}

#define INTERACTORPLUGINFACTORY(T,C,N,A,D,I,R,G)     \
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
  tlp::T * createPluginObject(InteractorContext *ic)		 \
  {						 \
    C *tmp = new C();				 \
    return ((tlp::T *) tmp);			 \
  }						 \
};                                               \
extern "C" {                                            \
  C##T##Factory C##T##FactoryInitializer;               \
}

#define INTERACTORPLUGINOFGROUP(C,N,A,D,I,R,G) INTERACTORPLUGINFACTORY(Interactor,C,N,A,D,I,R,G)
#define INTERACTORPLUGIN(C,N,A,D,I,R) INTERACTORPLUGINOFGROUP(C,N,A,D,I,R,"")

#endif






