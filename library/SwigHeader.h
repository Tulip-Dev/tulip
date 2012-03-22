%module Tulip
//operator []
#pragma SWIG nowarn=389
//operator =
#pragma SWIG nowarn=362
//operator <<
#pragma SWIG nowarn=503
//nested class not supported
#pragma SWIG nowarn=325

//operators new and delete
#pragma SWIG nowarn=350
#pragma SWIG nowarn=351

//specialization of non template
#pragma SWIG nowarn=317

//auto rename because used keyword
#pragma SWIG nowarn=314

%{
  #include <tulip/tulipconf.h>
  #include <tulip/Array.h>
  #include <tulip/Vector.h>
  #include <tulip/VectorCast.h>
  #include <tulip/Observable.h>
  #include <tulip/StoredType.h>
  #include <tulip/PropertyTypes.h>
  #include <tulip/PropertyInterface.h>
  #include <tulip/AbstractProperty.h>
  #include <tulip/WithParameter.h>
  #include <tulip/WithDependency.h>
  #include <tulip/PropertyAlgorithm.h>
  #include <tulip/Iterator.h>
  #include <tulip/StlIterator.h>
  #include <tulip/StableIterator.h>
  #include <tulip/SortIterator.h>
  #include <tulip/ObservableGraph.h>
  #include <tulip/ObservableProperty.h>
  #include <tulip/Bfs.h>
  #include <tulip/Ordering.h>
  #include <tulip/ConvexHull.h>
  #include <tulip/Delaunay.h>
  #include <tulip/GraphTools.h>
  #include <tulip/tuliphash.h>
  #include <tulip/Matrix.h>
  #include <tulip/Rectangle.h>
  #include <tulip/Circle.h>
  #include <tulip/SimpleTest.h>
  #include <tulip/TreeTest.h>
  #include <tulip/AcyclicTest.h>
  #include <tulip/ConnectedTest.h>
  #include <tulip/BiconnectedTest.h>
  #include <tulip/TriconnectedTest.h>
  #include <tulip/GraphProperty.h>
  #include <tulip/LayoutProperty.h>
  #include <tulip/AbstractPluginInfo.h>
  #include <tulip/Algorithm.h>
  #include <tulip/PluginLoader.h>
  #include <tulip/Size.h>
  #include <tulip/Node.h>
  #include <tulip/Edge.h>
  #include <tulip/Graph.h>
  #include <tulip/Color.h>
  #include <tulip/MethodFactory.h>
  #include <tulip/PluginLoaderTxt.h>
  #include <tulip/SizeProperty.h>
  #include <tulip/ColorProperty.h>
  #include <tulip/ImportModule.h>
  #include <tulip/DoubleProperty.h>
  #include <tulip/IntegerProperty.h>
  #include <tulip/StringProperty.h>
  #include <tulip/PluginLister.h>
  #include <tulip/Coord.h>
  #include <tulip/TlpTools.h>
  #include <tulip/PluginContext.h>
  #include <tulip/BooleanProperty.h>
  #include <tulip/ExportModule.h>
  #include <tulip/DataSet.h>
  #include <tulip/PluginProgress.h>
  #include <tulip/SimplePluginProgress.h>
  #include <tulip/GraphMeasure.h>
  #include <tulip/ConcatIterator.h>
  #include <tulip/MutableContainer.h>
  #include <tulip/PluginLibraryLoader.h>
  #include <tulip/StlFunctions.h>
  #include <tulip/StringCollection.h>
  #include <tulip/DrawingTools.h>
  #include <tulip/Face.h>
  #include <tulip/PlanarityTest.h>
  #include <tulip/OuterPlanarTest.h>
  #include <tulip/FaceIterator.h>
  #include <tulip/PlanarConMap.h>
  #include <tulip/GraphDecorator.h>
  #include <tulip/ForEach.h>
  #include <tulip/MapIterator.h>
  #include <tulip/BoundingBox.h>
  #include <tulip/ColorScale.h>
  #include <tulip/PreferenceManager.h>
  #include <tulip/QuadTree.h>
  #include <tulip/memorypool.h>
  #include <tulip/conversioniterator.h>
  #include <tulip/filteriterator.h>
  #include <tulip/uniqueiterator.h>
  #include <tulip/vectorgraph.h>
  #include <tulip/vectorgraphproperty.h>
  #include <tulip/SystemDefinition.h>

  using namespace tlp;
%}

%naturalvar;

%include <stl.i>
%include <std_string.i>

%apply const std::string& {std::string* foo};

%include <typemaps.i>

%include <tulip/tulipconf.h>

%include <tulip/Array.h>
%template(ArrayFloat3) tlp::Array<float,3>;
%template(ArrayUnsignedChar4) tlp::Array<unsigned char, 4>;

%include <tulip/Vector.h>
%template(Vect3f) tlp::Vector<float, 3>;
%template(VectorUnsignedChar4) tlp::Vector<unsigned char, 4>;

%include <tulip/VectorCast.h>

%template(CoordVectorCast) tlp::VectorCast< float,3,tlp::Coord >;
%template(SizeVectorCast) tlp::VectorCast< float,3,tlp::Size >;

%template(BoundingboxBase) tlp::Array<Vec3f,2>;

%include <tulip/Coord.h>
%include <tulip/StoredType.h>

%include <tulip/Observable.h>
%include <tulip/PropertyTypes.h>
%include <tulip/PropertyInterface.h>
%include <tulip/AbstractProperty.h>

%include <tulip/WithParameter.h>
%include <tulip/WithDependency.h>
%include <tulip/PropertyAlgorithm.h>

%define PROPERTY(NAME, NODETYPE, EDGETYPE, VECTORTYPE, ALGORITHM) 
%template(Abstract##NAME##Property) tlp::AbstractProperty<NODETYPE, EDGETYPE, ALGORITHM>; 
%template(NAME##Algorithm) tlp::TemplateAlgorithm<NAME##Property>;

// %template(##NAME##VectorPropertyBase) tlp::AbstractProperty<VECTORTYPE, VECTORTYPE>;
// %template(VectorProperty##NAME##) tlp::AbstractVectorProperty<VECTORTYPE, NODETYPE >; 
%enddef

// PROPERTY(Layout, tlp::PointType, tlp::LineType, tlp::CoordVectorType, tlp::LayoutAlgorithm);
// PROPERTY(Size, tlp::SizeType, tlp::SizeType, tlp::SizeVectorType, tlp::SizeAlgorithm);
PROPERTY(Color, tlp::ColorType, tlp::ColorType, tlp::ColorVectorType, tlp::ColorAlgorithm);
PROPERTY(Double, tlp::DoubleType, tlp::DoubleType, tlp::DoubleVectorType, tlp::DoubleAlgorithm);
PROPERTY(Integer, tlp::IntegerType, tlp::IntegerType, tlp::IntegerVectorType, tlp::IntegerAlgorithm);
PROPERTY(Boolean, tlp::BooleanType, tlp::BooleanType, tlp::BooleanVectorType, tlp::BooleanAlgorithm);
// PROPERTY(String, tlp::StringType, tlp::StringType, tlp::StringVectorType, tlp::StringAlgorithm);

// %template(AbstractGraphProperty) tlp::AbstractProperty<tlp::GraphType, tlp::EdgeSetType>;

%include <tulip/Iterator.h>

#define DOXYGEN_NOTFOR_DEVEL
%include <tulip/Node.h>
%include <tulip/Edge.h>
#undef DOXYGEN_NOTFOR_DEVEL

%template(EdgeIterator) tlp::Iterator< edge >;
%template(NodeIterator) tlp::Iterator< node >;
%template(UIntIterator) tlp::Iterator< unsigned int >;

%include <tulip/StlIterator.h>
%include <tulip/StableIterator.h>

%template(EdgeStableIterator) tlp::StableIterator< edge >;
%template(NodeStableIterator) tlp::StableIterator< node >;

%include <tulip/SortIterator.h>

#define DOXYGEN_NOTFOR_DEVEL
%include <tulip/Face.h>
#undef DOXYGEN_NOTFOR_DEVEL

%template(IteratorFace) tlp::Iterator< Face >;

%include <tulip/ObservableGraph.h>
%include <tulip/ObservableProperty.h>

#define DOXYGEN_NOTFOR_DEVEL
%include <tulip/Graph.h>
#undef DOXYGEN_NOTFOR_DEVEL

%include <tulip/GraphDecorator.h>
%include <tulip/Bfs.h>
%include <tulip/Ordering.h>
%include <tulip/ConvexHull.h>
%include <tulip/Delaunay.h>
%include <tulip/GraphTools.h>
%include <tulip/tuliphash.h>
%include <tulip/Matrix.h>
%include <tulip/Rectangle.h>
%include <tulip/Circle.h>
%include <tulip/SimpleTest.h>
%include <tulip/TreeTest.h>
%include <tulip/AcyclicTest.h>
%include <tulip/ConnectedTest.h>
%include <tulip/BiconnectedTest.h>
%include <tulip/TriconnectedTest.h>
%include <tulip/GraphProperty.h>
%include <tulip/LayoutProperty.h>
%include <tulip/AbstractPluginInfo.h>
%include <tulip/Algorithm.h>
%include <tulip/PluginLoader.h>
%include <tulip/Size.h>
%include <tulip/Color.h>
%include <tulip/MethodFactory.h>
%include <tulip/PluginLoaderTxt.h>
%include <tulip/SizeProperty.h>
%include <tulip/ColorProperty.h>
%include <tulip/ImportModule.h>
%include <tulip/DoubleProperty.h>
%include <tulip/IntegerProperty.h>
%include <tulip/StringProperty.h>
%include <tulip/PluginLister.h>
%include <tulip/TlpTools.h>
%include <tulip/PluginContext.h>
%include <tulip/BooleanProperty.h>
%include <tulip/ExportModule.h>
%include <tulip/DataSet.h>
%include <tulip/PluginProgress.h>
%include <tulip/SimplePluginProgress.h>
%include <tulip/GraphMeasure.h>
%include <tulip/ConcatIterator.h>
%include <tulip/MutableContainer.h>
%include <tulip/PluginLibraryLoader.h>
%include <tulip/StlFunctions.h>
%include <tulip/StringCollection.h>
%include <tulip/DrawingTools.h>
%include <tulip/PlanarityTest.h>
%include <tulip/OuterPlanarTest.h>
%include <tulip/FaceIterator.h>
%include <tulip/PlanarConMap.h>
%include <tulip/ForEach.h>
%include <tulip/MapIterator.h>
%include <tulip/BoundingBox.h>
%include <tulip/ColorScale.h>
%include <tulip/PreferenceManager.h>
%include <tulip/QuadTree.h>
%include <tulip/memorypool.h>
%include <tulip/conversioniterator.h>
%include <tulip/filteriterator.h>
%include <tulip/uniqueiterator.h>
%include <tulip/vectorgraph.h>
%include <tulip/vectorgraphproperty.h>
%include <tulip/SystemDefinition.h>