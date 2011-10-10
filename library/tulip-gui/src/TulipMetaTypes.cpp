#include "tulip/TulipMetaTypes.h"

using namespace tlp;

#define CHECK_QVARIANT(TYPE) if (v.userType() == qMetaTypeId<TYPE>()) return new TypedData<TYPE>(new TYPE(v.value<TYPE>()));

tlp::DataType* TulipMetaTypes::qVariantToDataType(const QVariant &v) {
  CHECK_QVARIANT(tlp::GraphType::RealType);
  CHECK_QVARIANT(tlp::DoubleType::RealType);
  CHECK_QVARIANT(tlp::FloatType::RealType);
  CHECK_QVARIANT(tlp::IntegerType::RealType);
  CHECK_QVARIANT(tlp::UnsignedIntegerType::RealType);
  CHECK_QVARIANT(tlp::LongType::RealType);
  CHECK_QVARIANT(tlp::BooleanType::RealType);
  CHECK_QVARIANT(tlp::BooleanVectorType::RealType);
  CHECK_QVARIANT(tlp::PointType::RealType);
  CHECK_QVARIANT(tlp::SizeType::RealType);
  CHECK_QVARIANT(tlp::StringType::RealType);
  CHECK_QVARIANT(tlp::ColorType::RealType);

  CHECK_QVARIANT(tlp::BooleanProperty*);
  CHECK_QVARIANT(tlp::DoubleProperty*);
  CHECK_QVARIANT(tlp::LayoutProperty*);
  CHECK_QVARIANT(tlp::StringProperty*);
  CHECK_QVARIANT(tlp::IntegerProperty*);
  CHECK_QVARIANT(tlp::SizeProperty*);
  CHECK_QVARIANT(tlp::ColorProperty*);
  CHECK_QVARIANT(tlp::PropertyInterface*);

  CHECK_QVARIANT(ColorScale);

  CHECK_QVARIANT(StringCollection);
  return NULL;
}

#define CHECK_DATATYPE(TYPE) if (type.compare(typeid(TYPE).name()) == 0) return typedVariant<TYPE>(dm);

#include <QtCore/QDebug>

QVariant TulipMetaTypes::dataTypeToQvariant(tlp::DataType *dm) {
  std::string type = dm->getTypeName();

  CHECK_DATATYPE(tlp::GraphType::RealType);
  CHECK_DATATYPE(tlp::DoubleType::RealType);
  CHECK_DATATYPE(tlp::FloatType::RealType);
  CHECK_DATATYPE(tlp::IntegerType::RealType);
  CHECK_DATATYPE(tlp::UnsignedIntegerType::RealType);
  CHECK_DATATYPE(tlp::LongType::RealType);
  CHECK_DATATYPE(tlp::BooleanType::RealType);
  CHECK_DATATYPE(tlp::BooleanVectorType::RealType);
  CHECK_DATATYPE(tlp::PointType::RealType);
  CHECK_DATATYPE(tlp::SizeType::RealType);
  CHECK_DATATYPE(tlp::StringType::RealType);
  CHECK_DATATYPE(tlp::ColorType::RealType);

  CHECK_DATATYPE(tlp::BooleanProperty*);
  CHECK_DATATYPE(tlp::DoubleProperty*);
  CHECK_DATATYPE(tlp::LayoutProperty*);
  CHECK_DATATYPE(tlp::StringProperty*);
  CHECK_DATATYPE(tlp::IntegerProperty*);
  CHECK_DATATYPE(tlp::SizeProperty*);
  CHECK_DATATYPE(tlp::ColorProperty*);
  CHECK_DATATYPE(tlp::PropertyInterface*);

  CHECK_DATATYPE(tlp::ColorScale)

  CHECK_DATATYPE(tlp::StringCollection)

  return QVariant();
}
