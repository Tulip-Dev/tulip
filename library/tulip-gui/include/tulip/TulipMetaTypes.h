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
#ifndef TULIPMETATYPES_H
#define TULIPMETATYPES_H

#include <QtCore/QFileInfo>
#include <QtCore/QVariant>

#include <tulip/DataSet.h>
#include <tulip/Graph.h>
#include <tulip/DataSet.h>
#include <tulip/Color.h>
#include <tulip/Coord.h>
#include <tulip/Size.h>
#include <tulip/BooleanProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/GraphProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/ColorScale.h>
#include <tulip/GlGraphStaticData.h>
#include <tulip/GlLabel.h>
#include <tulip/TulipFont.h>

#include <vector>

// Helper class for filesystem types handling
struct TulipFileDescriptor {
  enum FileType {File,Directory};

  TulipFileDescriptor() {}
  TulipFileDescriptor(const QString& absolutePath,FileType fileType):absolutePath(absolutePath),type(fileType) {}
  TulipFileDescriptor(const TulipFileDescriptor& d) {
    absolutePath = d.absolutePath;
    type = d.type;
  }
  QString absolutePath;
  FileType type;
  QString fileFilterPattern;
};


struct NodeShape {
  unsigned int nodeShapeId;
  NodeShape(unsigned int nodeShapeId=0):nodeShapeId(nodeShapeId) {}
  operator unsigned int() const {
    return nodeShapeId;
  }
};

struct EdgeExtremityShape {
  unsigned int edgeExtremityShapeId;
  EdgeExtremityShape(unsigned int edgeExtremityShapeId=0):edgeExtremityShapeId(edgeExtremityShapeId) {}
  operator unsigned int() const {
    return edgeExtremityShapeId;
  }
};

//Declare tulip type compatible with QVariant
Q_DECLARE_METATYPE(tlp::DataSet)

Q_DECLARE_METATYPE(tlp::DoubleProperty*)
Q_DECLARE_METATYPE(tlp::DoubleVectorProperty*)
Q_DECLARE_METATYPE(std::vector<double>)

Q_DECLARE_METATYPE(tlp::ColorProperty*)
Q_DECLARE_METATYPE(tlp::ColorVectorProperty*)
Q_DECLARE_METATYPE(std::vector<tlp::Color>)

Q_DECLARE_METATYPE(tlp::Size)
Q_DECLARE_METATYPE(tlp::SizeProperty*)
Q_DECLARE_METATYPE(tlp::SizeVectorProperty*)
Q_DECLARE_METATYPE(std::vector<tlp::Size>)

Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(tlp::StringProperty*)
Q_DECLARE_METATYPE(tlp::StringVectorProperty*)
Q_DECLARE_METATYPE(std::vector<std::string>)

//Q_DECLARE_METATYPE(tlp::Coord)
Q_DECLARE_METATYPE(tlp::LayoutProperty*)
Q_DECLARE_METATYPE(tlp::CoordVectorProperty*)
//Q_DECLARE_METATYPE(std::vector<tlp::Coord>)

Q_DECLARE_METATYPE(tlp::Graph*)
Q_DECLARE_METATYPE(tlp::GraphProperty*)

Q_DECLARE_METATYPE(tlp::IntegerProperty*)
Q_DECLARE_METATYPE(tlp::IntegerVectorProperty*)
Q_DECLARE_METATYPE(std::vector<int>)

Q_DECLARE_METATYPE(tlp::BooleanProperty*)
Q_DECLARE_METATYPE(tlp::BooleanVectorProperty*)
Q_DECLARE_METATYPE(std::vector<bool>)

Q_DECLARE_METATYPE(std::set<tlp::edge>)

Q_DECLARE_METATYPE(tlp::PropertyInterface*)

Q_DECLARE_METATYPE(tlp::ColorScale)

Q_DECLARE_METATYPE(tlp::StringCollection)

Q_DECLARE_METATYPE(TulipFileDescriptor)

Q_DECLARE_METATYPE(NodeShape)
Q_DECLARE_METATYPE(tlp::EdgeShape)
Q_DECLARE_METATYPE(EdgeExtremityShape)

Q_DECLARE_METATYPE(tlp::LabelPosition)
Q_DECLARE_METATYPE(tlp::TulipFont)


namespace tlp {
class TLP_QT_SCOPE TulipMetaTypes {
  TulipMetaTypes() {}

  template<typename T>
  inline static QVariant typedVariant(tlp::DataType *dm) {
    T result;

    if (dm)
      result = *((T*)dm->value);

    return QVariant::fromValue<T>(result);
  }

public:
  static tlp::DataType *qVariantToDataType(const QVariant &);
  static QVariant dataTypeToQvariant(tlp::DataType *,const std::string& paramName);

};
}

#endif // TULIPMETATYPES_H
