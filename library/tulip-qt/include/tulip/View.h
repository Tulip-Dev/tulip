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
   * The View class provides the skeleton of a view.
   * You normally never have to construct a View object yourself, the Controller takes care of this (and manages the interactors, etc...).
   */
  class TLP_QT_SCOPE View : public QObject,public WithParameter, public WithDependency{

    Q_OBJECT;

  public:

    /**
     * empty destructor : for inheritance
     */
    virtual ~View() {}
    
    /**
     * @brief Construct GUI of the view
     * @param parent The parent widget of this view.
     * @return QWidget* the main widget of the view (in MainController this widget will be added to the workspace).
     **/
    virtual QWidget *construct(QWidget *parent) = 0;
    
    /**
     * @brief A getter on the main widget of the view.
     *
     * @return QWidget* the main widget of this view.
     **/
    virtual QWidget *getWidget() =0;
    
    /**
     * @brief Set the View's Graph and DataSet.
     *
     * @param graph The view's new Graph.
     * @param dataSet The view's new DataSet.
     * @return void
     **/
    virtual void setData(Graph *graph,DataSet dataSet) = 0;
    
    /**
     * @brief Gets the Graph and DataSet of the view.
     *
     * @param graph A pointer whose value will be modified to point to the Graph* of the view.
     * @param dataSet A pointer whose value will be modified to point to the DataSet of the view.
     * @return void
     **/
    virtual void getData(Graph **graph,DataSet *dataSet) = 0;
    
    /**
     * @brief A getter on this view's Graph.
     *
     * @return :Graph* The Graph this View displays.
     **/
    virtual Graph *getGraph() = 0;
    
    /**
     * @brief Returns the configuration widgets this view should use.
     * These widgets will be displayed in the left dock of the MainController, in the View Editor tab.
     * Each widget will be in a tab whose text is the string of the pair.
     *
     * @return :list< std::pair<QWidget* , std::string > > Each element of this list is a pair of configuration widget and widget title.
     **/
    virtual std::list<std::pair<QWidget *,std::string> > getConfigurationWidget() {return std::list<std::pair<QWidget *,std::string> >();}
    
    /**
     * @brief Set the interactors on this view.
     * The Interactors are created, but now the View has ownership on them (responsibility for their destruction).
     *
     * @param interactors The interactors that can be used with this View.
     * @return void
     **/
    virtual void setInteractors(const std::list<Interactor *> &interactors) = 0;
    
    /**
     * @brief Returns the interactors this view uses.
     *
     * @return :list< tlp::Interactor* > the interactors of this View
     **/
    virtual std::list<Interactor *> getInteractors() = 0;
    
    /**
     * @brief Sets the active interactor on this view.
     * The interactor must be in the interactors list of this View.
     *
     * @param interactor The Interactor to render active.
     * @return void
     **/
    virtual void setActiveInteractor(Interactor *interactor) = 0;
    
    /**
     * @brief Gets the active interactor of this view.
     *
     * @return :Interactor* The active Interactor of this View.
     **/
    virtual Interactor *getActiveInteractor() = 0;
    
    /**
     * @brief Take a snapshot of the view and put it in a picture
     * 
     * @deprecated In Tulip 4.x.y this function will be replaced by void createPicture(const string &pictureName,int width, int height, bool center, int zoom, int xOffset, int yOffset)
     *
     * @param pictureName filename to use for the picture, with extension (extension is used to infer image type).
     * @param width width of the picture Defaults to 0.
     * @param height height of the picture Defaults to 0.
     * @return void
     **/
    virtual void _DEPRECATED createPicture(const std::string &pictureName,int width=0, int height=0);

    /**      
     * @brief Take a snapshot of the view, and save it to a picture on disk.
     *
     * @deprecated In Tulip 4.x.y this function will be replaced by bool savePicture(const string &pictureName,int width, int height, bool center, int zoom, int xOffset, int yOffset).
     * \param pictureName : filename to use for the picture, with extension (extension is used to infer image type).
     * \param width : width of the picture
     * \param height : height of the picture
     * \param center : whether we should center the view before creating the picture, or use the current zoom and pan.
     * \param zoom : creates a picture of a sub part of the view. With zoom=1 creates only one picture with entire view; with zoom=N : the view is cut into 2^(N-1) part in width and height. Defaults to 1.
     * \param xOffset : which part of the view to render if zoom!=1. xOffset must be 0 <= xOffset < 2^(zoom-1). Defaults to 0.
     * \param yOffset : which part of the view to render if zoom!=1. yOffset must be 0 <= yOffset < 2^(zoom-1). Defaults to 0.
     * \return bool : Whether the picture can be created or not.
     */
    virtual bool _DEPRECATED createPicture(const std::string& pictureName, int width, int height, bool center, int zoom = 1, int xOffset = 0, int yOffset = 0);

    /**
     * @brief Take a snapshot of the view, and save it to a picture on disk.
     *
     * \param pictureName : filename to use for the picture, with extension (extension is used to infer image type).
     * \param width : width of the picture
     * \param height : height of the picture
     * \param center : whether we should center the view before creating the picture, or use the current zoom and pan.
     * \param zoom : creates a picture of a sub part of the view. With zoom=1 creates only one picture with entire view; with zoom=N : the view is cut into 2^(N-1) part in width and height. Defaults to 1.
     * \param xOffset : which part of the view to render if zoom!=1. xOffset must be 0 <= xOffset < 2^(zoom-1). Defaults to 0.
     * \param yOffset : which part of the view to render if zoom!=1. yOffset must be 0 <= yOffset < 2^(zoom-1). Defaults to 0.
     * \return bool : Whether the picture can be created or not.
     */
    virtual bool savePicture(const std::string& pictureName, int width, int height, bool center, int zoom = 1, int xOffset = 0, int yOffset = 0);

    /**
     * @brief Take a snapshot of the view, and return a QImage
     * \param width : width of the picture
     * \param height : height of the picture
     * \param center : whether we should center the view before creating the picture, or use the current zoom and pan.
     * \param zoom : creates a picture of a sub part of the view. With zoom=1 creates only one picture with entire view; with zoom=N : the view is cut into 2^(N-1) part in width and height. Defaults to 1.
     * \param xOffset : which part of the view to render if zoom!=1. xOffset must be 0 <= xOffset < 2^(zoom-1). Defaults to 0.
     * \param yOffset : which part of the view to render if zoom!=1. yOffset must be 0 <= yOffset < 2^(zoom-1). Defaults to 0.
     * \return Qimage : snapshot image.
     */
    virtual QImage createPicture(int width, int height, bool center, int zoom = 1, int xOffset = 0, int yOffset = 0);
    
    /**
     * @brief Return the real view name (if "" the real name is the name given by the plugin).
     *
     * @return :string the View's real name.
     **/
    virtual std::string getRealViewName() const {return "";}

    
    /**
     * @brief This function is called when an undo is performed by the controller.
     *
     * @return void
     **/
    virtual void undoIsDone() {}

  public slots:

    
    /**
     * @brief Sets the Graph on this View.
     *
     * @param graph the new Graph to be displayed by this View.
     * @return void
     **/
    virtual void setGraph(Graph *graph) = 0;

    
    /**
     * @brief Draws the View from scratch, i.e. reading the Graph, and drawind all the nodes and edges depending on their position, colors, etc...
     * This drawing is then stored to be re-used when a refresh is all you need.
     * Then, the interactors are drawn.
     * This is pretty resource-hungry, if you just need to redraw the interactors or only the selection changed, use refresh.
     *
     * @return void
     **/
    virtual void draw() = 0;
    
    /**
     * @brief Refreshes the view, using the backed up drawing that draw stored.
     * Then the interactors are drawn.
     * This is usefull when the selection changed, but nothing else in the Graph changed (color, size, layout, ...), or when an interactor changed and needs re-drawing.
     * This is way less resource-hungry than draw().
     *
     * @return void
     **/
    
    virtual void refresh() = 0;
    
    /**
     * @brief Centers and draws the view.
     * Calls centerview() [where applicable], then draw().
     * This might be removed in a future version (3.6 or later) as all this does is calling centerview on the GlMainWidget if this is a GlMainView.
     * Adding a centerView function in View would allow to remove this function.
     *
     * @return void
     **/
    virtual void init() = 0;

  signals:
    /**
     * @brief This signal is emitted when a specific element is selected in the view
     *
     * @param eltId The identifier of the selected element.
     * @param isNode Whether or not the selected element is a node. It can only be a node or an edge.
     * @return void
     **/
    void elementSelected(unsigned int eltId, bool isNode);

    /**
     * @brief This signal is emitted when the view wants to change its graph.
     *
     * @param view ...
     * @param graph ...
     * @return void
     **/
    void requestChangeGraph(tlp::View* view,tlp::Graph* graph);
  };

  class TLP_QT_SCOPE ViewContext {
  };

  /**
   * @brief This class should only be used by the plugin macros.
   **/
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






