#ifndef PROPERTYTOQVARIANTMAPPER_H
#define PROPERTYTOQVARIANTMAPPER_H

#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QVariant>

#include <tulip/Graph.h>
#include <tulip/IntegerProperty.h>
#include <tulip/StringProperty.h>

namespace tlp{

/**
  * @brief Interface to perform data conversion between a property values and QVariant objects.
  **/
class TLP_QT_SCOPE PropertyToQVariantConverter {
public:
  virtual ~PropertyToQVariantConverter() {}
  virtual QVariant getValue(unsigned int id,tlp::ElementType elementType,tlp::PropertyInterface* property) const =0;
  virtual bool setValue(unsigned int id,tlp::ElementType elementType,tlp::PropertyInterface* property,const QVariant& data) const  =0;
  virtual bool setAllValue(tlp::ElementType elementType,tlp::PropertyInterface* property,const QVariant& data) const  =0;
};


/**
  * @brief Generic conversion from properties values to QVariant
  **/
template<typename PropertyClass, typename NodeType, typename EdgeType>
class TLP_QT_SCOPE StandardPropertyToQVariantConverter : public PropertyToQVariantConverter {
public:
  QVariant getValue(unsigned int id,tlp::ElementType elementType,tlp::PropertyInterface* property) const;
  bool setValue(unsigned int id,tlp::ElementType elementType,tlp::PropertyInterface* property,const QVariant& data) const;
  bool setAllValue(tlp::ElementType elementType,tlp::PropertyInterface* property,const QVariant& data) const;

};


/**
  * @brief Convert values from integer properties. Handle tulip visual properties likes viewShape(NodeShape,EdgeShape), viewLabelPosition(LabelPosition,int), viewTgtAnchorShape(int,EdgeExtremityShape), viewSrcAnchorShape(int,EdgeExtremityShape)
  **/
class TLP_QT_SCOPE IntegerPropertyConverter : public StandardPropertyToQVariantConverter<tlp::IntegerProperty,int,int> {
public:
  QVariant getValue(unsigned int id,tlp::ElementType elementType,tlp::PropertyInterface* property) const;

};

/**
  * @brief Convert values from integer properties. Handle tulip visual properties likes viewFont(TulipFileDescriptor,TulipFileDescriptor), viewTexture(TulipFileDescriptor,TulipFileDescriptor)
  **/
class TLP_QT_SCOPE StringPropertyConverter : public StandardPropertyToQVariantConverter<tlp::StringProperty,std::string,std::string> {

  QVariant getValue(unsigned int id,tlp::ElementType elementType,tlp::PropertyInterface* property) const;
  bool setValue(unsigned int id,tlp::ElementType elementType,tlp::PropertyInterface* property,const QVariant& data) const;
  bool setAllValue(tlp::ElementType elementType,tlp::PropertyInterface* property,const QVariant& data) const;
};

/**
  * @brief Register and perform mapping between properties and PropertyToQVariantConverter object.
  * You can overload a PropertyToQVariantConverter by registering your own using the registerConverter function.
  **/
class TLP_QT_SCOPE PropertyToQVariantMapper {
public:
  PropertyToQVariantMapper();
  ~PropertyToQVariantMapper();

  template<class PropertyInterface>
  void registerConverter(PropertyToQVariantConverter* converter) {
    registerConverter(QString::fromStdString(PropertyInterface::propertyTypename),converter);
  }

  const PropertyToQVariantConverter* converter(tlp::PropertyInterface* property)const;
private:
  void registerConverter(const QString& propertyTypeName,PropertyToQVariantConverter* converter);
  QMap<QString,PropertyToQVariantConverter*> _propertyTypeToConverter;

};
}
#include "cxx/PropertyToQVariantMapper.cxx"
#endif // PROPERTYTOQVARIANTMAPPER_H
