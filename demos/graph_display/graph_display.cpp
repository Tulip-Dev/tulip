#include <tulip/PluginLoaderTxt.h>
#include <tulip/PluginLibraryLoader.h>
#include <tulip/GlMainWidget.h>
#include <tulip/MouseInteractors.h>
#include <tulip/TlpQtTools.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/TulipViewSettings.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/ForEach.h>

#include <QApplication>
#include <QString>

#include <iostream>

using namespace tlp;
using namespace std;

void addChildren(Graph *graph, node root, int depth, int degree) {
    if (depth > 0) {
        for (int i = 0 ; i < degree ; ++i) {
            node child = graph->addNode();
            graph->addEdge(root, child);
            addChildren(graph, child, depth-1, degree);
        }
    }
}

Graph *createCompleteTree(int depth, int degree) {
    Graph *graph = newGraph();
    node root = graph->addNode();
    addChildren(graph, root, depth, degree);
    return graph;
}

// That function sets some visual properties on a complete tree whose depth equals 5
void setTreeVisualProperties(Graph *tree) {

    // First compute a layout, we use the Bubble Tree algorithm
    LayoutProperty *viewLayout = tree->getProperty<LayoutProperty>("viewLayout");
    std::string errMsg;
    tree->applyPropertyAlgorithm("Bubble Tree", viewLayout, errMsg);

    // Then apply Auto Sizing on the nodes
    SizeProperty *viewSize = tree->getProperty<SizeProperty>("viewSize");
    tree->applyPropertyAlgorithm("Auto Sizing", viewSize, errMsg);

    // Labels the node with their id
    StringProperty *viewLabel = tree->getProperty<StringProperty>("viewLabel");
    node n;
    forEach(n, tree->getNodes()) {
        viewLabel->setNodeValue(n, QString::number(n.id).toStdString());
    }

    // Add a border to the nodes, keep the default color who is black
    DoubleProperty *viewBorderWidth = tree->getProperty<DoubleProperty>("viewBorderWidth");
    viewBorderWidth->setAllNodeValue(1);

    // Build some maps to set shapes and colors according to the dag level of a node
    std::vector<int> glyphsMap;
    glyphsMap.push_back(tlp::NodeShape::Square);
    glyphsMap.push_back(tlp::NodeShape::Circle);
    glyphsMap.push_back(tlp::NodeShape::RoundedBox);
    glyphsMap.push_back(tlp::NodeShape::Hexagon);
    glyphsMap.push_back(tlp::NodeShape::Star);
    glyphsMap.push_back(tlp::NodeShape::Ring);

    std::vector<Color> colorsMap;
    colorsMap.push_back(Color::Red);
    colorsMap.push_back(Color::Azure);
    colorsMap.push_back(Color::Lemon);
    colorsMap.push_back(Color::SpringGreen);
    colorsMap.push_back(Color::Apricot);
    colorsMap.push_back(Color::Magenta);

    // Compute the Dag Level metric, the value of each node will correspond
    // to their layer id in the tree
    DoubleProperty dagLevel(tree);
    tree->applyPropertyAlgorithm("Dag Level", &dagLevel, errMsg);

    // Sets different shapes and colors for each layer of the tree
    IntegerProperty *viewShape = tree->getProperty<IntegerProperty>("viewShape");
    ColorProperty *viewColor = tree->getProperty<ColorProperty>("viewColor");
    forEach(n, tree->getNodes()) {
        viewShape->setNodeValue(n, glyphsMap[int(dagLevel.getNodeValue(n))]);
        viewColor->setNodeValue(n, colorsMap[int(dagLevel.getNodeValue(n))]);
    }
}


// That function sets some rendering parameters on the graph to visualize
void setGraphRenderingParameters(GlGraphComposite *glGraphComposite) {
    GlGraphRenderingParameters *renderingParameters = glGraphComposite->getRenderingParametersPointer();
    // Activate the display of edge extremities (arrows by default)
    renderingParameters->setViewArrow(true);
    // No color interpolation for the edges
    renderingParameters->setEdgeColorInterpolate(false);
    // Size interpolation for the edges
    renderingParameters->setEdgeSizeInterpolate(true);
    // Scale labels to node sizes
    renderingParameters->setLabelScaled(true);
}

int main(int argc, char** argv) {

  // A QApplication must always be declared at the beginning of the main function if you intend to use the tulip-gui library
  // This must be done before calling tlp::initTulipSoftware()
  QApplication app(argc,argv);


  // Initialize the library and load all plugins
  tlp::initTulipSoftware();

  Graph* g = NULL;
  if(QApplication::arguments().size()==2) {
      // Load the file passed as first argument into a graph.
      // This method will select the default Tulip algorithm plugin (TLP)
      QString filename = QApplication::arguments()[1];
      if(!((filename.endsWith(".tlp"))||(filename.endsWith(".tlp.gz")))) {
          cout << "File " << filename.toStdString() << " not compatible. Use a tlp file or a tlp.gz file" << endl;
          exit(EXIT_FAILURE);
      }
      g = tlp::loadGraph(filename.toStdString());
  } else {
      // If no arguments were given to the command, create a complete tree of depth 5
      // and degree 2 for demo purpose
      g = createCompleteTree(5, 2);
      // Set some visual properties in order to visualize the tree
      setTreeVisualProperties(g);
  }

  // Creates the main widget that will display our graph
  GlMainWidget* mainWidget = new GlMainWidget(NULL);

  // Adds a layer to the scene
  GlLayer* mainLayer = mainWidget->getScene()->createLayer("Main");

  // Adds the graph to this layer
  mainLayer->addGraph(g,"graph");

  // Sets some rendering parameters on the graph to visualize
  setGraphRenderingParameters(mainWidget->getScene()->getGlGraphComposite());

  // Display the widget
  mainWidget->show();

  // Flush event loop in order to let paint events pass through in order for the scene to be initialized.
  QApplication::processEvents();

  // Center the camera and draw the graph
  mainWidget->centerScene();
  mainWidget->draw();

  // Adds Zoom and pan navigation to the widget
  mainWidget->installEventFilter(new MouseNKeysNavigator);
  return app.exec();
}
