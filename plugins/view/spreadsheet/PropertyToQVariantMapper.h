#ifndef PROPERTYTOQVARIANTMAPPER_H
#define PROPERTYTOQVARIANTMAPPER_H

#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QVariant>

#include <tulip/Graph.h>


class PropertyToQVariantConverter {
public:
  virtual ~PropertyToQVariantConverter() {}
  virtual QVariant getValue(unsigned int id,tlp::ElementType elementType,tlp::PropertyInterface* property) const =0;
  virtual bool setValue(unsigned int id,tlp::ElementType elementType,tlp::PropertyInterface* property,const QVariant& data) const  =0;
  virtual bool setAllValue(tlp::ElementType elementType,tlp::PropertyInterface* property,const QVariant& data) const  =0;
};


template<typename PropertyClass, typename NodeType, typename EdgeType>
class StandardPropertyToQVariantConverter : public PropertyToQVariantConverter {
public:
  QVariant getValue(unsigned int id,tlp::ElementType elementType,tlp::PropertyInterface* property) const {
    if(elementType == tlp::NODE) {
      return QVariant::fromValue<NodeType>(static_cast<PropertyClass*>(property)->getNodeValue(tlp::node(id)));
    }
    else {
      return QVariant::fromValue<EdgeType>(static_cast<PropertyClass*>(property)->getEdgeValue(tlp::edge(id)));
    }
  }

  bool setValue(unsigned int id,tlp::ElementType elementType,tlp::PropertyInterface* property,const QVariant& data) const {
    PropertyClass* p = static_cast<PropertyClass*>(property);

    if(elementType == tlp::NODE) {
      const NodeType& oldValue = p->getNodeValue(tlp::node(id));
      NodeType newValue = data.value<NodeType>();

      if(oldValue != newValue) {
        p->setNodeValue(tlp::node(id),newValue);
        return true;
      }
    }
    else {
      const EdgeType& oldValue = p->getEdgeValue(tlp::edge(id));
      EdgeType newValue = data.value<EdgeType>();

      if(oldValue != newValue) {
        p->setEdgeValue(tlp::edge(id),data.value<EdgeType>());
        return true;
      }
    }

    return false;
  }

  virtual bool setAllValue(tlp::ElementType elementType,tlp::PropertyInterface* property,const QVariant& data) const {
    PropertyClass* p = static_cast<PropertyClass*>(property);

    if(elementType == tlp::NODE) {
      p->setAllNodeValue(data.value<NodeType>());
    }
    else {
      p->setAllEdgeValue(data.value<EdgeType>());
    }

    return true;
  }

};

class PropertyToQVariantMapper {
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

#endif // PROPERTYTOQVARIANTMAPPER_H
