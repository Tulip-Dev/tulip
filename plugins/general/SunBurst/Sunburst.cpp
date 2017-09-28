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
#ifdef _OPENMP
#include <omp.h>
#endif

#include <tulip/Algorithm.h>
#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/StaticProperty.h>
#include <tulip/TreeTest.h>
#include <tulip/TulipViewSettings.h>


using namespace tlp;
using namespace std;

static const char * paramHelp[] = {
  // leaf size
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "DoubleProperty" ) \
  HTML_HELP_DEF( "default", "none" ) \
  HTML_HELP_BODY() \
  "Input size of tree leaves." \
  HTML_HELP_CLOSE(),
  // real surface
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "bool" ) \
  HTML_HELP_DEF( "default", "true" ) \
  HTML_HELP_BODY() \
  "Indicates if the real surface must be computed." \
  HTML_HELP_CLOSE(),
};

class SunBurst:public tlp::Algorithm {
public:
  PLUGININFORMATION("SunBurst", "Tulip Team","07/01/2011","Alpha","0.1","Tree")
  SunBurst(const tlp::PluginContext *);
  ~SunBurst();
  bool run();
  bool check(std::string &);
  void reset();
  void layout(node n, double alpha1, double alpha2,
              double radius, double radiusIncr, bool equalSurface,
              NodeStaticProperty<double>& sizes,
              NodeStaticProperty<vector<Coord> >& polygons);
  void updateSize(node n, NodeStaticProperty<double>& sizes);
};

PLUGIN(SunBurst)

//================================================================================
SunBurst::SunBurst(const PluginContext* context):Algorithm(context) {
  addInParameter<DoubleProperty>("Leaf size", paramHelp[0], "none", false);
  addInParameter<bool>("Real surface", "", "true", false);
}
//================================================================================
SunBurst::~SunBurst() {
}

double DEFAULT_RADIUS_INCR  = 10.;
double radiusSpace = 1.;


double circleArea(double radius) {
  return M_PI * radius*radius;
}

double computeRadiusIncr(double currentStartRadius, double radiusIncr, double nexStartRadius) {
  double currentArea = circleArea(currentStartRadius+radiusIncr) - circleArea(currentStartRadius);
  double r4 = sqrt((currentArea + circleArea(nexStartRadius))/M_PI);
  return r4 - nexStartRadius;
}

/**
 * @brief sunburstLayout
 * @param tree
 * @param n
 * @param alpha1 angle 1
 * @param alpha2 angle 2
 * @param radius the start radius
 * @param radiusIncr the increment of the radius
 * @param sizes the values of the nodes.
 */
void SunBurst::layout(node n, double alpha1, double alpha2,
                      double radius, double radiusIncr, bool equalSurface,
                      NodeStaticProperty<double>& sizes,
                      NodeStaticProperty<vector<Coord> >& polygons) {
  size_t slices = rint((alpha2 - alpha1)*50./(2.* M_PI)) + 4;
  double dalpha = (alpha2 - alpha1)/double(slices-1);//Compute the delta alpha
  vector<Coord>& shape = polygons[n];
  vector<Coord>  label;
  vector<double> lsize;
  double labelLength = (alpha2-alpha1) * (radius + radiusIncr/2.); //use the minumarcsize
  double labelHeight = radiusIncr/2.;
  double labelratio  = labelLength / labelHeight; // label width / label height, perfect should be around 6;

  if (labelratio < 4.) {
    labelHeight = labelLength / 4.;
  }

  //incircle
  double alpha = alpha1;

  for (unsigned int k=0; k<slices; ++k) {
    Coord p(radius * cos(alpha), radius * sin(alpha));
    shape.push_back(p);
    alpha += dalpha;
  }

  double alpha1label = alpha1;
  double alpha2label = alpha2;
  double dalphalabel = dalpha;

  if (alpha2 - alpha1 > M_PI/2.) {
    double center = (alpha2 + alpha1)/2.;
    alpha1label = center - M_PI/4.;
    alpha2label = center + M_PI/4.;
    dalphalabel = M_PI/2. / (double)(slices);
  }

  dalphalabel = (alpha2label - alpha1label) / 20.f;

  if (alpha2label - alpha1label < M_PI/4)
    dalphalabel = (alpha2label - alpha1label) / 6.f;

  //Label Path
  alpha = alpha1label; //label path start and end doesn't touch the border

  while (alpha < alpha2label + 1E-4) {
    Coord l((radius + radiusIncr/2.) * cos(alpha), (radius + radiusIncr/2.) * sin(alpha));
    label.push_back(l);
    lsize.push_back(labelHeight);
    alpha += dalphalabel;
  }

  //outcircle
  alpha = alpha2;

  for (unsigned int k=0; k<slices; ++k) {
    Coord p((radius + radiusIncr) * cos(alpha), (radius + radiusIncr) * sin(alpha));
    shape.push_back(p);
    alpha -= dalpha;
  }

  /*CoordVectorProperty  * labelPath    = graph->getProperty<CoordVectorProperty>("viewLabelPaths");
    DoubleVectorProperty * labelPathSze = graph->getProperty<DoubleVectorProperty>("viewLabelPathsSize");

  //vertical label
  // P1 and P2 should be enough but the render path function do not work with 2 points
  if (labelratio < 1.) {

      label.resize(0);
      lsize.resize(0);
      double halpha = (alpha2+alpha1)/2.;
      double r1 = radius;
      double r2 = radius + radiusIncr;
      if (labelratio < 1./6. && labelLength>0) {
          //        double ratio2 = (radiusIncr/2.) / arclabelsize

          r1 += radiusIncr - 4. * labelLength;
          r2 -= radiusIncr - 4. * labelLength;
      }
      Coord p1(r1 * cos(halpha), r1 * sin(halpha), 0 );
      Coord p5(r2 * cos(halpha), r2 * sin(halpha), 0 );
      Coord p3 = (p1 + p5)/2.f;
      Coord p2 = (p1 + p3)/2.f;
      Coord p4 = (p3 + p5)/2.f;
      label.push_back(p1);
      label.push_back(p2);
      //label.push_back(p3);
      label.push_back(p4);
      label.push_back(p5);
      double s1 = r1 * (alpha2-alpha1) / 2.;
      double s5 = r2 * (alpha2-alpha1) / 2.;
      double s3 = (s5+s1)/2.;
      double s2 = (s3+s1)/2.;
      double s4 = (s5+s3)/2.;
      lsize.push_back(s1);
      lsize.push_back(s2);
      //lsize.push_back(s3);
      lsize.push_back(s4);
      lsize.push_back(s5);

  }

  labelPath->setNodeValue(n, label);
  labelPathSze->setNodeValue(n, lsize);*/

  //recursive call
  double curS = sizes[n];
  double deltaAlpha = (alpha2 - alpha1) / curS;
  alpha = alpha1;

  double nextStartRadius = radius + radiusIncr + radiusSpace;
  double nextRadiusIncrement = equalSurface?computeRadiusIncr(radius,radiusIncr,nextStartRadius):DEFAULT_RADIUS_INCR;

  node ni;
  forEach(ni, graph->getOutNodes(n)) {
    layout(ni, alpha, alpha + deltaAlpha * sizes[ni], nextStartRadius,
           nextRadiusIncrement, equalSurface, sizes, polygons);
    alpha += deltaAlpha * sizes[ni];
  }
}
//================================================================================
/**
 * @brief updateSize computes the values of the nodes according to the sum of its children. If the node is a leaf return it's value in the size property.
 * @param n the node to treat.
 */
void SunBurst::updateSize(node n, NodeStaticProperty<double>& sizes) {
  if (graph->outdeg(n) == 0) {//If the element is a leaf
    if (sizes[n] < 1E-5) { //algorithm fail with size <= 0.
      sizes[n] = 1E-5;
    }

    return;
  }

  double sum = 0;
  node ni;
  forEach(ni, graph->getOutNodes(n)) {
    updateSize(ni, sizes);//Compute the value of the child.
    sum += sizes[ni];//Add the child value to current value.
  }
  sizes[n] = sum;//Update node value.
}
//================================================================================
bool SunBurst ::run() {
  if (!TreeTest::isTree(graph)) {
    cout << "The graph is not a tree" << endl;
    return false;
  }

  DoubleProperty* leafSize = NULL;
  bool equalSurface = true;

  if(dataSet != NULL) {
    dataSet->get<DoubleProperty*>("Leaf size", leafSize);
    dataSet->get<bool>("Real surface", equalSurface);
  }

  NodeStaticProperty<double> sizes(graph);

  if (leafSize)
    sizes.copyFromProperty(leafSize);
  else {
    const std::vector<node>& nodes = graph->nodes();
    unsigned int nbNodes = nodes.size();
#ifdef _OPENMP
    #pragma omp parallel for
#endif

    for (OMP_ITER_TYPE i = 0; i < nbNodes; ++i)
      sizes[i] = 1E-5;
  }

  NodeStaticProperty<vector<Coord> > polygons(graph);
  node start = graph->getSource();

  updateSize(start, sizes);//Compute node values.
  layout(start, -M_PI/2., 2.*M_PI - M_PI/2., 5.,
         DEFAULT_RADIUS_INCR,equalSurface, sizes, polygons);
  LayoutProperty* layout =
    graph->getLocalProperty<LayoutProperty>("viewLayout");
  layout->setAllEdgeValue(vector<Coord>());

  IntegerProperty* shape =
    graph->getLocalProperty<IntegerProperty>("viewShape");
  shape->setAllNodeValue(NodeShape::CubeOutlined);
  SizeProperty* size = graph->getLocalProperty<SizeProperty>("viewSize");


  node n;
  forEach(n, graph->getNodes()) {
    vector<Coord>& poly = polygons[n];
    Coord minC = poly[0];
    Coord maxC = poly[0];

    for (size_t i = 1; i< poly.size(); ++i) {
      minC = Coord(minVector(minC, poly[i]));
      maxC = Coord(maxVector(maxC, poly[i]));
    }

    layout->setNodeValue(n, (maxC+minC)/2.f);
    Size tmpS(maxC-minC);
    tmpS[2] = 1;
    size->setNodeValue(n, tmpS);
  }
  return true;
}
//================================================================================
bool SunBurst::check(string &erreurMsg) {
  erreurMsg="";

  if (!TreeTest::isTree(graph)) {
    erreurMsg="The graph is not a tree";
    return false;
  }

  return true;
}
//================================================================================
void SunBurst::reset() {
}
//================================================================================
