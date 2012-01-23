#include "PropertyToQVariantMapper.h"

#include <tulip/TulipMetaTypes.h>
#include <tulip/TlpQtTools.h>
#include <vector>

using namespace tlp;

/**
  * @brief Convert values from integer properties. Handle tulip visual properties likes viewShape(NodeShape,EdgeShape), viewLabelPosition(LabelPosition,int), viewTgtAnchorShape(int,EdgeExtremityShape), viewSrcAnchorShape(int,EdgeExtremityShape)
  **/
class IntegerPropertyConverter : public StandardPropertyToQVariantConverter<IntegerProperty,int,int>{
public:

    QVariant getValue(unsigned int id,tlp::ElementType elementType,tlp::PropertyInterface* property) const {
        IntegerProperty* p = static_cast<IntegerProperty*>(property);
        if (property->getName().compare("viewShape") == 0) {
            if(elementType == tlp::NODE) {
                return QVariant::fromValue<NodeShape>(NodeShape(p->getNodeValue(tlp::node(id))));
            }else{
                return QVariant::fromValue<EdgeShape>(EdgeShape(p->getEdgeValue(tlp::edge(id))));
            }
        }else if(property->getName().compare("viewLabelPosition")==0){
            if(elementType == tlp::NODE) {
                return QVariant::fromValue<LabelPosition>(static_cast<LabelPosition>(p->getNodeValue(tlp::node(id))));
            }else{
                //This property is not used by edge.
                return QVariant(p->getNodeValue(node(id)));
            }
        }else if(property->getName().compare("viewTgtAnchorShape")==0 || property->getName().compare("viewSrcAnchorShape")==0){
            if(elementType == tlp::EDGE) {
                return QVariant::fromValue<EdgeExtremityShape>(EdgeExtremityShape(p->getEdgeValue(tlp::edge(id))));
            }else{
                //This property is not used by nodes.
                return QVariant(p->getNodeValue(node(id)));
            }
        }else {
            //Default case
            if(elementType == NODE){
                return QVariant(p->getNodeValue(tlp::node(id)));
            }else{
                return QVariant(p->getEdgeValue(tlp::edge(id)));
            }
        }
    }
};

/**
  * @brief Convert values from integer properties. Handle tulip visual properties likes viewShape(NodeShape,EdgeShape), viewLabelPosition(LabelPosition,int), viewTgtAnchorShape(int,EdgeExtremityShape), viewSrcAnchorShape(int,EdgeExtremityShape)
  **/
class StringPropertyConverter : public StandardPropertyToQVariantConverter<StringProperty,std::string,std::string>{
    QVariant getValue(unsigned int id,tlp::ElementType elementType,tlp::PropertyInterface* property) const {
        StringProperty* p = static_cast<StringProperty*>(property);
        if (property->getName().compare("viewTexture") == 0) {
            TulipFileDescriptor descriptor;
            descriptor.fileFilterPattern=QString("Images (*.bmp *.png *.jpg *.jpeg)");
            if(elementType == tlp::NODE) {
                descriptor.absolutePath=tlpStringToQString(p->getNodeValue(tlp::node(id)));
            }else{
                descriptor.absolutePath=tlpStringToQString(p->getEdgeValue(tlp::edge(id)));
            }
            return QVariant::fromValue<TulipFileDescriptor>(descriptor);
        }else if(property->getName().compare("viewFont")==0){
            TulipFileDescriptor descriptor;
            descriptor.fileFilterPattern=QString("Font (*.ttf");
            if(elementType == tlp::NODE) {
                descriptor.absolutePath=tlpStringToQString(p->getNodeValue(tlp::node(id)));
            }else{
                descriptor.absolutePath=tlpStringToQString(p->getEdgeValue(tlp::edge(id)));
            }
            return QVariant::fromValue<TulipFileDescriptor>(descriptor);
        }else{
            //Default case
            if(elementType == NODE){
                return QVariant(tlpStringToQString(p->getNodeValue(tlp::node(id))));
            }else{
                return QVariant(tlpStringToQString(p->getEdgeValue(tlp::edge(id))));
            }
        }
    }

    bool setValue(unsigned int id,tlp::ElementType elementType,tlp::PropertyInterface* property,const QVariant& data) const {
       StringProperty* p = static_cast<StringProperty*>(property);
        if (property->getName().compare("viewTexture") == 0 || property->getName().compare("viewFont")==0) {
            TulipFileDescriptor descriptor=data.value<TulipFileDescriptor>();
            if(elementType == NODE){
                p->setNodeValue(node(id),QStringToTlpString(descriptor.absolutePath));
            }else{
                p->setEdgeValue(edge(id),QStringToTlpString(descriptor.absolutePath));
            }
            return true;
        }else{
            //Default case
            if(elementType == NODE){
                p->setNodeValue(node(id),QStringToTlpString(data.toString()));
            }else{
                p->setEdgeValue(edge(id),QStringToTlpString(data.toString()));
            }
            return true;
        }
    }

    virtual bool setAllValue(tlp::ElementType elementType,tlp::PropertyInterface* property,const QVariant& data) const{
        StringProperty* p = static_cast<StringProperty*>(property);
         if (property->getName().compare("viewTexture") == 0 || property->getName().compare("viewFont")==0) {
             TulipFileDescriptor descriptor=data.value<TulipFileDescriptor>();
             if(elementType == NODE){
                 p->setAllNodeValue(QStringToTlpString(descriptor.absolutePath));
             }else{
                 p->setAllEdgeValue(QStringToTlpString(descriptor.absolutePath));
             }
             return true;
         }else{
             //Default case
             if(elementType == NODE){
                 p->setAllNodeValue(QStringToTlpString(data.toString()));
             }else{
                 p->setAllEdgeValue(QStringToTlpString(data.toString()));
             }
             return true;
         }
    }
};

PropertyToQVariantMapper::PropertyToQVariantMapper()
{
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
PropertyToQVariantMapper::~PropertyToQVariantMapper(){
    for(QMap<QString,PropertyToQVariantConverter*>::iterator it = _propertyTypeToConverter.begin(); it != _propertyTypeToConverter.end() ; ++it){
        delete it.value();
    }
}

void PropertyToQVariantMapper::registerConverter(const QString& propertyTypeName,PropertyToQVariantConverter* converter){
    if(_propertyTypeToConverter.contains(propertyTypeName)){
        delete _propertyTypeToConverter[propertyTypeName];
    }
    _propertyTypeToConverter[propertyTypeName] = converter;
}

const PropertyToQVariantConverter* PropertyToQVariantMapper::converter(tlp::PropertyInterface* property)const{
    QString propertyTypename = QString::fromStdString(property->getTypename());
    if(_propertyTypeToConverter.contains(propertyTypename)){
        return _propertyTypeToConverter[propertyTypename];
    }else{
        return NULL;
    }
}
