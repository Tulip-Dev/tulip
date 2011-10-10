#ifndef TULIPMETATYPES_H
#define TULIPMETATYPES_H

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

#include <vector>

//Declare tulip type compatible with QVariant
Q_DECLARE_METATYPE(tlp::DataSet)

Q_DECLARE_METATYPE(tlp::DoubleProperty*)
Q_DECLARE_METATYPE(tlp::DoubleVectorProperty*)
Q_DECLARE_METATYPE(std::vector<double>)

Q_DECLARE_METATYPE(tlp::Color)
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

Q_DECLARE_METATYPE(tlp::Coord)
Q_DECLARE_METATYPE(tlp::LayoutProperty*)
Q_DECLARE_METATYPE(tlp::CoordVectorProperty*)
Q_DECLARE_METATYPE(std::vector<tlp::Coord>)

Q_DECLARE_METATYPE(tlp::Graph*)
Q_DECLARE_METATYPE(tlp::GraphProperty*)

Q_DECLARE_METATYPE(tlp::IntegerProperty*)
Q_DECLARE_METATYPE(tlp::IntegerVectorProperty*)
Q_DECLARE_METATYPE(std::vector<int>)

Q_DECLARE_METATYPE(tlp::BooleanProperty*)
Q_DECLARE_METATYPE(tlp::BooleanVectorProperty*)
Q_DECLARE_METATYPE(std::vector<bool>)

Q_DECLARE_METATYPE(tlp::PropertyInterface*)

Q_DECLARE_METATYPE(tlp::ColorScale)

Q_DECLARE_METATYPE(tlp::StringCollection)

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
  static QVariant dataTypeToQvariant(tlp::DataType *);

};
}

#endif // TULIPMETATYPES_H
