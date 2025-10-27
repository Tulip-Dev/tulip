#include <tulip/TulipPluginHeaders.h>

// Check base plugins implementation can be compiled
// and successfully loaded

// Algorithm plugin
class TestAlgorithmPlugin : public tlp::Algorithm {

public:
  PLUGININFORMATION("Test Algorithm Plugin", "Tulip-dev", "2019", "", "1.0", "")

  TestAlgorithmPlugin(tlp::PluginContext *context) : tlp::Algorithm(context) {}

  bool run() override {
    graph->clear();
    return true;
  }
};

PLUGIN(TestAlgorithmPlugin)

// BooleanAlgorithm plugin
class TestBooleanAlgorithmPlugin : public tlp::BooleanAlgorithm {

public:
  PLUGININFORMATION("Test Boolean Algorithm Plugin", "Tulip-dev", "2019", "", "1.0", "")

  TestBooleanAlgorithmPlugin(tlp::PluginContext *context) : tlp::BooleanAlgorithm(context) {}

  bool run() override {
    result->setAllNodeValue(false);
    result->setNodeValue(graph->getRandomNode(), true);
    return true;
  }
};

PLUGIN(TestBooleanAlgorithmPlugin)

// ColorAlgorithm plugin
class TestColorAlgorithmPlugin : public tlp::ColorAlgorithm {

public:
  PLUGININFORMATION("Test Color Algorithm Plugin", "Tulip-dev", "2019", "", "1.0", "")

  TestColorAlgorithmPlugin(tlp::PluginContext *context) : tlp::ColorAlgorithm(context) {}

  bool run() override {
    result->setAllNodeValue(tlp::Color::Red);
    result->setNodeValue(graph->getRandomNode(), tlp::Color::Green);
    return true;
  }
};

PLUGIN(TestColorAlgorithmPlugin)

// DoubleAlgorithm plugin
class TestDoubleAlgorithmPlugin : public tlp::DoubleAlgorithm {

public:
  PLUGININFORMATION("Test Double Algorithm Plugin", "Tulip-dev", "2019", "", "1.0", "")

  TestDoubleAlgorithmPlugin(tlp::PluginContext *context) : tlp::DoubleAlgorithm(context) {}

  bool run() override {
    result->setAllNodeValue(tlp::randomDouble());
    result->setNodeValue(graph->getRandomNode(), tlp::randomDouble());
    return true;
  }
};

PLUGIN(TestDoubleAlgorithmPlugin)

// IntegerAlgorithm plugin
class TestIntegerAlgorithmPlugin : public tlp::IntegerAlgorithm {

public:
  PLUGININFORMATION("Test Integer Algorithm Plugin", "Tulip-dev", "2019", "", "1.0", "")

  TestIntegerAlgorithmPlugin(tlp::PluginContext *context) : tlp::IntegerAlgorithm(context) {}

  bool run() override {
    result->setAllNodeValue(tlp::randomInteger(10000));
    result->setNodeValue(graph->getRandomNode(), tlp::randomInteger(10000));
    return true;
  }
};

PLUGIN(TestIntegerAlgorithmPlugin)

// LayoutAlgorithm plugin
class TestLayoutAlgorithmPlugin : public tlp::LayoutAlgorithm {

public:
  PLUGININFORMATION("Test Layout Algorithm Plugin", "Tulip-dev", "2019", "", "1.0", "")

  TestLayoutAlgorithmPlugin(tlp::PluginContext *context) : tlp::LayoutAlgorithm(context) {}

  bool run() override {
    result->setAllNodeValue(tlp::Coord(0.0f, 0.0f));
    result->setNodeValue(graph->getRandomNode(), tlp::Coord(10.0f, 10.0f));
    return true;
  }
};

PLUGIN(TestLayoutAlgorithmPlugin)

// SizeAlgorithm plugin
class TestSizeAlgorithmPlugin : public tlp::SizeAlgorithm {

public:
  PLUGININFORMATION("Test Size Algorithm Plugin", "Tulip-dev", "2019", "", "1.0", "")

  TestSizeAlgorithmPlugin(tlp::PluginContext *context) : tlp::SizeAlgorithm(context) {}

  bool run() override {
    result->setAllNodeValue(tlp::Size(1.0f, 1.0f, 1.0f));
    result->setNodeValue(graph->getRandomNode(), tlp::Size(10.0f, 10.0f, 10.0f));
    return true;
  }
};

PLUGIN(TestSizeAlgorithmPlugin)

// StringAlgorithm plugin
class TestStringAlgorithmPlugin : public tlp::StringAlgorithm {

public:
  PLUGININFORMATION("Test String Algorithm Plugin", "Tulip-dev", "2019", "", "1.0", "")

  TestStringAlgorithmPlugin(tlp::PluginContext *context) : tlp::StringAlgorithm(context) {}

  bool run() override {
    result->setAllNodeValue("foo");
    result->setNodeValue(graph->getRandomNode(), "bar");
    return true;
  }
};

PLUGIN(TestStringAlgorithmPlugin)

// ImportModule plugin
class TestImportModulePlugin : public tlp::ImportModule {

public:
  PLUGININFORMATION("Test Import Module Plugin", "Tulip-dev", "2019", "", "1.0", "")

  TestImportModulePlugin(tlp::PluginContext *context) : tlp::ImportModule(context) {}

  bool importGraph() override {
    graph->addNode();
    return true;
  }
};

PLUGIN(TestImportModulePlugin)

// ExportModule plugin
class TestExportModulePlugin : public tlp::ExportModule {

public:
  PLUGININFORMATION("Test Export Module Plugin", "Tulip-dev", "2019", "", "1.0", "")

  TestExportModulePlugin(tlp::PluginContext *context) : tlp::ExportModule(context, {"graph"}) {}

  bool exportGraph(std::ostream &os) override {
    os << graph;
    return true;
  }
};

PLUGIN(TestExportModulePlugin)

// Check there is no symbol issue due to Tulip multithreading
// implementation (either OpenMP or C++11 threads)
// when using a static property in a plugin code
class TestStaticPropertyPlugin : public tlp::Algorithm {

public:
  PLUGININFORMATION("Test Static Property", "Tulip-dev", "2019", "", "1.0", "")

  TestStaticPropertyPlugin(tlp::PluginContext *context) : tlp::Algorithm(context) {}

  bool run() override {
    tlp::NodeStaticProperty<double *> test(graph);
    test.setAll(nullptr);
    return true;
  }
};

PLUGIN(TestStaticPropertyPlugin)
