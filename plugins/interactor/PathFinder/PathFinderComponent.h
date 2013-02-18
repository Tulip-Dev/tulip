/*
 * PathFinderComponent.h
 *
 *  Created on: Apr 14, 2010
 *      Author: Ludwig Fiolka
 */

#ifndef PATHFINDERCOMPONENT_H_
#define PATHFINDERCOMPONENT_H_

#include <tulip/GLInteractor.h>
#include <tulip/Node.h>

namespace tlp {
class Graph;
class GlMainView;
class BooleanProperty;
class DoubleProperty;
}
class PathHighlighter;
class PathFinder;

/**
 * @brief The main component of the PathFinder interactor. Runs the path finding algorithm when two nodes have been selected and
 * triggers the highlighters if a path has been found.
 */
struct PathFinderComponent: public tlp::GLInteractorComponent {
    PathFinderComponent(PathFinder *parent);
    virtual ~PathFinderComponent();
    bool eventFilter(QObject *, QEvent *);
    InteractorComponent *clone();

    /**
     * Install a path highlighter. This highlighter is not always activated. The component will ask its interactor parent before triggering an highlighter.
     * @param highlighter The highlighter to install.
     */
    void addHighlighter(PathHighlighter *highlighter);

    /**
     * @return A list of all the installed highlighters.
     */
    std::set<PathHighlighter *> getHighlighters();

    void clear();

private:
    tlp::node src;
    tlp::node tgt;
    PathFinder *parent;
    bool graphPopable;

    std::set<PathHighlighter *> highlighters;
    void runHighlighters(tlp::GlMainWidget *glMainWidget,tlp::BooleanProperty *selection, tlp::node src, tlp::node tgt);
    void clearHighlighters(tlp::GlMainWidget *glMainWidget);

    void selectPath(tlp::GlMainWidget *glMainWidget,tlp::Graph *graph);
    PathHighlighter *findHighlighter(const std::string &name);
};



#endif /* PATHFINDERCOMPONENT_H_ */
