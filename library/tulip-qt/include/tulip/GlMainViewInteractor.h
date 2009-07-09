#ifndef _Tulip_GLMAINVIEWINTERACTOR_H_
#define _Tulip_GLMAINVIEWINTERACTOR_H_

#include "InteractorChainOfResponsibility.h"

namespace tlp {

  /** \brief Tulip interactor abstract class for NodeLinkDiagramComponent
   *
   */
  class TLP_QT_SCOPE NodeLinkDiagramComponentInteractor  : public InteractorChainOfResponsibility {

  public :

    NodeLinkDiagramComponentInteractor(const QString &iconPath, const QString &text);
    /**
     * return if this interactor is compatible with given View
     */
    virtual bool isCompatible(const std::string &viewName) {
      if(viewName=="Node Link Diagram view")
        return true;
      return false;
    }

  };

  /** \brief Tulip interactor to navigate on the graph
   *
   */
  class TLP_QT_SCOPE InteractorNavigation  : public NodeLinkDiagramComponentInteractor {

  public:

    /**
     * Default constructor
     */
    InteractorNavigation();

    /**
     * Construct chain of responsibility
     */
    void construct();

  };

  /** \brief Tulip interactor to get information about an element of the graph
   *
   */
  class TLP_QT_SCOPE InteractorGetInformation  : public NodeLinkDiagramComponentInteractor {

  public:

    /**
     * Default constructor
     */
    InteractorGetInformation();

    /**
     * Construct chain of responsibility
     */
    void construct();
  };

  /** \brief Tulip interactor to select par of the graph
   *
   */
  class TLP_QT_SCOPE InteractorSelection  : public NodeLinkDiagramComponentInteractor {

  public:

    /**
     * Default constructor
     */
    InteractorSelection();

    /**
     * Construct chain of responsibility
     */
    void construct();
  };

  /** \brief Tulip interactor to move/reshape
   *
   */
  class TLP_QT_SCOPE InteractorSelectionModifier  : public NodeLinkDiagramComponentInteractor {

  public:

    /**
     * Default constructor
     */
    InteractorSelectionModifier();

    /**
     * Construct chain of responsibility
     */
    void construct();
  };

  /** \brief Tulip interactor to do a magic selection
   *
   */
  class TLP_QT_SCOPE InteractorMagicSelection  : public NodeLinkDiagramComponentInteractor {

  public:

    /**
     * Default constructor
     */
    InteractorMagicSelection();

    /**
     * Construct chain of responsibility
     */
    void construct();
  };

  /** \brief Tulip interactor to do a rectangle zoom
   *
   */
  class TLP_QT_SCOPE InteractorRectangleZoom  : public NodeLinkDiagramComponentInteractor {

  public:

    /**
     * Default constructor
     */
    InteractorRectangleZoom();

    /**
     * Construct chain of responsibility
     */
    void construct();
  };

  /** \brief Tulip interactor to delete an element
   *
   */
  class TLP_QT_SCOPE InteractorDeleteElement  : public NodeLinkDiagramComponentInteractor {

  public:

    /**
     * Default constructor
     */
    InteractorDeleteElement();

    /**
     * Construct chain of responsibility
     */
    void construct();
  };

  /** \brief Tulip interactor to add a node
   *
   */
  class TLP_QT_SCOPE InteractorAddNode  : public NodeLinkDiagramComponentInteractor {

  public:

    /**
     * Default constructor
     */
    InteractorAddNode();

    /**
     * Construct chain of responsibility
     */
    void construct();
  };

  /** \brief Tulip interactor to add edges
   *
   */
  class TLP_QT_SCOPE InteractorAddEdge  : public NodeLinkDiagramComponentInteractor {

  public:

    /**
     * Default constructor
     */
    InteractorAddEdge();

    /**
     * Construct chain of responsibility
     */
    void construct();
  };

  /** \brief Tulip interactor to edit edge bends
   *
   */
  class TLP_QT_SCOPE InteractorEditEdgeBends  : public NodeLinkDiagramComponentInteractor {

  public:

    /**
     * Default constructor
     */
    InteractorEditEdgeBends();

    /**
     * Construct chain of responsibility
     */
    void construct();
  };

}

#endif






