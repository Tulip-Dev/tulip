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

  if (v.userType() == qMetaTypeId<NodeShape>()) {
    NodeShape desc = v.value<NodeShape>();
    return new TypedData<uint>(new uint(desc.nodeShapeId));
  }

  if (v.userType() == qMetaTypeId<EdgeExtremityShape>()) {
    EdgeExtremityShape desc = v.value<EdgeExtremityShape>();
    return new TypedData<uint>(new uint(desc.edgeExtremityShapeId));
  }

  if (v.userType() == qMetaTypeId<TulipFileDescriptor>()) {
    TulipFileDescriptor desc = v.value<TulipFileDescriptor>();
    return new TypedData<std::string>(new std::string(desc.absolutePath.toUtf8().data()));
  }

  return NULL;
}

#define CHECK_DATATYPE(TYPE) if (type.compare(typeid(TYPE).name()) == 0) return typedVariant<TYPE>(dm);

#include <QtCore/QDebug>

QVariant TulipMetaTypes::dataTypeToQvariant(tlp::DataType *dm, const std::string& paramName) {
  std::string type = dm->getTypeName();

  // First, we set up some hack to provide custom types for string data whose name starts with file:: or dir::
  QString name(paramName.c_str());

  if (type.compare(typeid(std::string).name()) == 0 && (name.startsWith("file::") || name.startsWith("dir::"))) {
    TulipFileDescriptor desc;
    desc.absolutePath = QString::fromUtf8((*((std::string*)dm->value)).c_str());
    desc.type = name.startsWith("dir::")?TulipFileDescriptor::Directory:TulipFileDescriptor::File;
    return QVariant::fromValue<TulipFileDescriptor>(desc);
  }

  // ******

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
