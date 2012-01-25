#include <tulip/PropertyToQVariantMapper.h>

#include <tulip/TulipMetaTypes.h>
#include <tulip/TlpQtTools.h>
#include <vector>
#include <tulip/TulipFont.h>

using namespace tlp;

QVariant IntegerPropertyConverter::getValue(unsigned int id,ElementType elementType,PropertyInterface* property) const {
  IntegerProperty* p = static_cast<IntegerProperty*>(property);

  if (property->getName().compare("viewShape") == 0) {
    if(elementType == NODE) {
      return QVariant::fromValue<NodeShape>(NodeShape(p->getNodeValue(node(id))));
    }
    else {
      return QVariant::fromValue<EdgeShape>(EdgeShape(p->getEdgeValue(edge(id))));
    }
  }
  else if(property->getName().compare("viewLabelPosition")==0) {
    if(elementType == NODE) {
      return QVariant::fromValue<LabelPosition>(static_cast<LabelPosition>(p->getNodeValue(node(id))));
    }
    else {
      //This property is not used by edge.
      return QVariant(p->getNodeValue(node(id)));
    }
  }
  else if(property->getName().compare("viewTgtAnchorShape")==0 || property->getName().compare("viewSrcAnchorShape")==0) {
    if(elementType == EDGE) {
      return QVariant::fromValue<EdgeExtremityShape>(EdgeExtremityShape(p->getEdgeValue(edge(id))));
    }
    else {
      //This property is not used by nodes.
      return QVariant(p->getNodeValue(node(id)));
    }
  }
  else {
    //Default case
    if(elementType == NODE) {
      return QVariant(p->getNodeValue(node(id)));
    }
    else {
      return QVariant(p->getEdgeValue(edge(id)));
    }
  }
}


QVariant StringPropertyConverter::getValue(unsigned int id,ElementType elementType,PropertyInterface* property) const {
  StringProperty* p = static_cast<StringProperty*>(property);

  if (property->getName().compare("viewTexture") == 0) {
    TulipFileDescriptor descriptor;
    descriptor.fileFilterPattern=QString("Images (*.bmp *.png *.jpg *.jpeg)");

    if(elementType == NODE) {
      descriptor.absolutePath=tlpStringToQString(p->getNodeValue(node(id)));
    }
    else {
      descriptor.absolutePath=tlpStringToQString(p->getEdgeValue(edge(id)));
    }

    return QVariant::fromValue<TulipFileDescriptor>(descriptor);
  }
  else if(property->getName().compare("viewFont")==0) {
    std::string fontFile;

    if(elementType == NODE) {
      fontFile=p->getNodeValue(node(id));
    }
    else {
      fontFile=p->getEdgeValue(edge(id));
    }

    return QVariant::fromValue<TulipFont>(TulipFont::fromFile(tlpStringToQString(fontFile)));
  }
  else {
    //Default case
    if(elementType == NODE) {
      return QVariant(tlpStringToQString(p->getNodeValue(node(id))));
    }
    else {
      return QVariant(tlpStringToQString(p->getEdgeValue(edge(id))));
    }
  }
}

bool StringPropertyConverter::setValue(unsigned int id,ElementType elementType,PropertyInterface* property,const QVariant& data) const {
  StringProperty* p = static_cast<StringProperty*>(property);


  if (property->getName().compare("viewFont")==0) {
    TulipFont font = data.value<TulipFont>();

    if(elementType == NODE) {
      p->setNodeValue(node(id),QStringToTlpString(font.fontFile()));
    }
    else {
      p->setEdgeValue(edge(id),QStringToTlpString(font.fontFile()));
    }

    return true;
  }
  else if(property->getName().compare("viewTexture") == 0) {
    TulipFileDescriptor descriptor=data.value<TulipFileDescriptor>();

    if(elementType == NODE) {
      p->setNodeValue(node(id),QStringToTlpString(descriptor.absolutePath));
    }
    else {
      p->setEdgeValue(edge(id),QStringToTlpString(descriptor.absolutePath));
    }

    return true;
  }
  else {
    //Default case
    if(elementType == NODE) {
      p->setNodeValue(node(id),QStringToTlpString(data.toString()));
    }
    else {
      p->setEdgeValue(edge(id),QStringToTlpString(data.toString()));
    }

    return true;
  }
}

bool StringPropertyConverter::setAllValue(ElementType elementType,PropertyInterface* property,const QVariant& data) const {
  StringProperty* p = static_cast<StringProperty*>(property);

  if (property->getName().compare("viewTexture") == 0 || property->getName().compare("viewFont")==0) {
    TulipFileDescriptor descriptor=data.value<TulipFileDescriptor>();

    if(elementType == NODE) {
      p->setAllNodeValue(QStringToTlpString(descriptor.absolutePath));
    }
    else {
      p->setAllEdgeValue(QStringToTlpString(descriptor.absolutePath));
    }

    return true;
  }
  else {
    //Default case
    if(elementType == NODE) {
      p->setAllNodeValue(QStringToTlpString(data.toString()));
    }
    else {
      p->setAllEdgeValue(QStringToTlpString(data.toString()));
    }

    return true;
  }
}

PropertyToQVariantMapper::PropertyToQVariantMapper() {
  registerConverter<BooleanProperty>(new StandardPropertyToQVariantConverter<BooleanProperty,bool,bool>());
  registerConverter<ColorProperty>(new StandardPropertyToQVariantConverter<ColorProperty,Color,Color>());
  registerConverter<DoubleProperty>(new StandardPropertyToQVariantConverter<DoubleProperty,double,double>());
  registerConverter<GraphProperty>(new StandardPropertyToQVariantConverter<GraphProperty,Graph*,std::set<edge> >());
  registerConverter<IntegerProperty>(new IntegerPropertyConverter());
  registerConverter<LayoutProperty>(new StandardPropertyToQVariantConverter<LayoutProperty,Coord,std::vector<Coord> >());
  registerConverter<SizeProperty>(new StandardPropertyToQVariantConverter<SizeProperty,Size,Size>());
  registerConverter<StringProperty>(new StringPropertyConverter());
  registerConverter<BooleanVectorProperty>(new StandardPropertyToQVariantConverter<BooleanVectorProperty,std::vector<bool>,std::vector<bool> >);
  registerConverter<ColorVectorProperty>(new StandardPropertyToQVariantConverter<ColorVectorProperty,std::vector<Color>,std::vector<Color> >);
  registerConverter<CoordVectorProperty>(new StandardPropertyToQVariantConverter<CoordVectorProperty,std::vector<Coord>,std::vector<Coord> >);
  registerConverter<DoubleVectorProperty>(new StandardPropertyToQVariantConverter<DoubleVectorProperty,std::vector<double>,std::vector<double> >);
  registerConverter<IntegerVectorProperty>(new StandardPropertyToQVariantConverter<IntegerVectorProperty,std::vector<int>,std::vector<int> >);
  registerConverter<SizeVectorProperty>(new StandardPropertyToQVariantConverter<SizeVectorProperty,std::vector<Size>,std::vector<Size> >);
  registerConverter<StringVectorProperty>(new StandardPropertyToQVariantConverter<StringVectorProperty,std::vector<std::string>,std::vector<std::string> >);

}
PropertyToQVariantMapper::~PropertyToQVariantMapper() {
  for(QMap<QString,PropertyToQVariantConverter*>::iterator it = _propertyTypeToConverter.begin(); it != _propertyTypeToConverter.end() ; ++it) {
    delete it.value();
  }
}

void PropertyToQVariantMapper::registerConverter(const QString& propertyTypeName,PropertyToQVariantConverter* converter) {
  if(_propertyTypeToConverter.contains(propertyTypeName)) {
    delete _propertyTypeToConverter[propertyTypeName];
  }

  _propertyTypeToConverter[propertyTypeName] = converter;
}

const PropertyToQVariantConverter* PropertyToQVariantMapper::converter(PropertyInterface* property)const {
  QString propertyTypename = QString::fromStdString(property->getTypename());

  if(_propertyTypeToConverter.contains(propertyTypename)) {
    return _propertyTypeToConverter[propertyTypename];
  }
  else {
    return NULL;
  }
}
