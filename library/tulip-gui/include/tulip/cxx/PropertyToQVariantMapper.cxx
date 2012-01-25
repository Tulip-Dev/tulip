namespace tlp {

template<typename PropertyClass, typename NodeType, typename EdgeType>
QVariant StandardPropertyToQVariantConverter<PropertyClass,NodeType,EdgeType>::getValue(unsigned int id,tlp::ElementType elementType,tlp::PropertyInterface* property) const {
  if(elementType == tlp::NODE) {
    return QVariant::fromValue<NodeType>(static_cast<PropertyClass*>(property)->getNodeValue(tlp::node(id)));
  }
  else {
    return QVariant::fromValue<EdgeType>(static_cast<PropertyClass*>(property)->getEdgeValue(tlp::edge(id)));
  }
}

template<typename PropertyClass, typename NodeType, typename EdgeType>
bool StandardPropertyToQVariantConverter<PropertyClass,NodeType,EdgeType>::setValue(unsigned int id,tlp::ElementType elementType,tlp::PropertyInterface* property,const QVariant& data) const {
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

template<typename PropertyClass, typename NodeType, typename EdgeType>
bool StandardPropertyToQVariantConverter<PropertyClass,NodeType,EdgeType>::setAllValue(tlp::ElementType elementType,tlp::PropertyInterface* property,const QVariant& data) const {
  PropertyClass* p = static_cast<PropertyClass*>(property);

  if(elementType == tlp::NODE) {
    p->setAllNodeValue(data.value<NodeType>());
  }
  else {
    p->setAllEdgeValue(data.value<EdgeType>());
  }

  return true;
}
}
