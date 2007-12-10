//-*-c++-*-
#ifndef MOUSEEDGEBENDEDITION_H
#define MOUSEEDGEBENDEDITION_H
#include <vector>
#include <tulip/GlCircle.h>
#include <tulip/Observable.h>
#include <tulip/GWInteractor.h>

/** \addtogroup Mouse_interactor */ 
/*@{*/

namespace tlp {

  /// This interactor allows to move/add/delete EdgeBends
  class TLP_QT_SCOPE MouseEdgeBendEditor:public GWInteractor {

  public:

    MouseEdgeBendEditor();
    ~MouseEdgeBendEditor() {if(glGraphWidget)glGraphWidget->getScene()->getSelectionLayer()->deleteGlEntity("EdgeBendEditorComposite");}
    bool compute(GlGraphWidget *glGraphWidget);
    bool draw(GlGraphWidget *);
    bool eventFilter(QObject *, QEvent *);
    GWInteractor *clone() { return new MouseEdgeBendEditor(); }

  private:
    enum EditOperation { NONE=0, TRANSLATE, NEW, DELETE};
    enum OperationTarget { COORD = 0, SIZE, COORD_AND_SIZE};

    Graph *_graph;
    GlGraphWidget* glGraphWidget;
    LayoutProperty *_layout;
    BooleanProperty *_selection;
    DoubleProperty *_rotation;
    SizeProperty *_sizes;
    LayoutProperty *_copyLayout;
    SizeProperty *_copySizes;
    DoubleProperty *_copyRotation;

    void initProxies(GlGraphWidget *glGraphWidget);
    void saveInfo();
    void restoreInfo();
    void initEdition();
    void undoEdition();
    void stopEdition();

    EditOperation operation;
    OperationTarget mode;

    Coord editPosition;
    
    std::vector<tlp::GlCircle> circles;
    std::vector<Coord> coordinates;
    tlp::GlCircle basicCircle;
    tlp::GlComposite circleString;
    std::vector <GlEntity * > select;
    edge mEdge;
    Coord start, end;
    std::string theCircle;
    bool belong(Coord, Coord, Coord, GlGraphWidget*);
    bool computeBendsCircles(GlGraphWidget*);
    void mMouseTranslate(double, double, GlGraphWidget*);
    void mMouseDelete();
    void mMouseCreate(double, double, GlGraphWidget*);
  };

}
/*@}*/

#endif
