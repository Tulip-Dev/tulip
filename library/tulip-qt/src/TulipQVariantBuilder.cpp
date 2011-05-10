#include "TulipQVariantBuilder.h"
#include <tulip/TlpQtTools.h>
#include <tulip/GlGraphStaticData.h>
#include <tulip/EdgeExtremityGlyphManager.h>

using namespace tlp;
using namespace std;

TulipQVariantBuilder::TulipQVariantBuilder()
{
}

QVariant TulipQVariantBuilder::displayRoleData(ElementType elementType, unsigned int elementId, TulipPropertyType propertyType, PropertyInterface* property) const
{
    switch(propertyType){
    case NODEGLYPHPROPERTY_RTTI:
        return QVariant(QString::fromUtf8(GlyphManager::getInst().glyphName(((IntegerProperty*)property)->getNodeValue(node(elementId))).c_str()));
        break;
    case EDGESHAPEPROPERTY_RTTI:
        return QVariant(QString::fromUtf8(GlGraphStaticData::edgeShapeName(
                ((IntegerProperty*)property)->getEdgeValue(edge(elementId))).c_str()));
        break;
    case EDGEEXTREMITYGLYPHPROPERTY_RTTI:
        return QVariant(QString::fromUtf8(EdgeExtremityGlyphManager::getInst().glyphName(
                ((IntegerProperty*)property)->getEdgeValue(edge(elementId))).c_str()));
        break;
    default:{
            if(elementType == NODE)
                return QVariant(QString::fromUtf8(property->getNodeStringValue(node(elementId)).c_str()));
            else return QVariant(QString::fromUtf8(property->getEdgeStringValue(edge(elementId)).c_str()));
        }
        break;
    }
}
QVariant TulipQVariantBuilder::editRoleData(ElementType elementType, unsigned int elementId, TulipPropertyType propertyType, PropertyInterface* property) const
{
    switch(propertyType){
    case INTEGER_PROPERTY_RTTI:
    case NODEGLYPHPROPERTY_RTTI:
    case EDGESHAPEPROPERTY_RTTI:
    case EDGEEXTREMITYGLYPHPROPERTY_RTTI:
        return createQVariantFromTulipProperty<IntegerProperty,int,int >(elementType,elementId,(IntegerProperty*)property);
        break;
    case BOOLEAN_PROPERTY_RTTI:
        return createQVariantFromTulipProperty<BooleanProperty,bool,bool>(elementType,elementId,(BooleanProperty*)property);
        break;
    case DOUBLEPROPERTY_RTTI:
        return createQVariantFromTulipProperty<DoubleProperty,double,double>(elementType,elementId,(DoubleProperty*)property);
        break;
    case LAYOUTPROPERTY_RTTI:
        return createQVariantFromTulipProperty<LayoutProperty,Coord,vector<Coord> >(elementType,elementId,(LayoutProperty*)property);
        break;
    case COLORPROPERTY_RTTI:
        return createQVariantFromTulipProperty<ColorProperty,Color,Color>(elementType,elementId,(ColorProperty*)property);
        break;
    case SIZEPROPERTY_RTTI:
        return createQVariantFromTulipProperty<SizeProperty,Size,Size>(elementType,elementId,(SizeProperty*)property);
        break;
    case BOOLEANVECTORPROPERTY_RTTI:
        return createQVariantFromTulipProperty<BooleanVectorProperty,vector<bool>,vector<bool> >(elementType,elementId,(BooleanVectorProperty*)property);
        break;
    case COLORVECTORPROPERTY_RTTI:
        return createQVariantFromTulipProperty<ColorVectorProperty,vector<Color>,vector<Color> >(elementType,elementId,(ColorVectorProperty*)property);
        break;
    case COORDVECTORPROPERTY_RTTI:
        return createQVariantFromTulipProperty<CoordVectorProperty,vector<Coord>,vector<Coord> >(elementType,elementId,(CoordVectorProperty*)property);
        break;
    case DOUBLEVECTORPROPERTY_RTTI:
        return createQVariantFromTulipProperty<DoubleVectorProperty,vector<double>,vector<double> >(elementType,elementId,(DoubleVectorProperty*)property);
        break;
    case INTEGERVECTORPROPERTY_RTTI:
        return createQVariantFromTulipProperty<IntegerVectorProperty,vector<int>,vector<int> >(elementType,elementId,(IntegerVectorProperty*)property);
        break;
    case SIZEVECTORPROPERTY_RTTI:
        return createQVariantFromTulipProperty<SizeVectorProperty,vector<Size>,vector<Size> >(elementType,elementId,(SizeVectorProperty*)property);
        break;
    case STRINGVECTORPROPERTY_RTTI:
        return createQVariantFromTulipProperty<StringVectorProperty,vector<string>,vector<string> >(elementType,elementId,(StringVectorProperty*)property);
        break;
    default:
        if(elementType == NODE){
            return QVariant(QString::fromUtf8(property->getNodeStringValue(node(elementId)).c_str()));
        }
        else {
            return QVariant(QString::fromUtf8(property->getEdgeStringValue(edge(elementId)).c_str()));
        }
        break;
    }
}

TulipPropertyType TulipQVariantBuilder::getPropertyType(ElementType elementType,PropertyInterface* property) const
{
    if (property->getName().compare("viewShape") == 0) {
        return elementType==NODE?NODEGLYPHPROPERTY_RTTI:EDGESHAPEPROPERTY_RTTI;
    }
    else if (property->getName().compare("viewLabelPosition")==0) {
        return LABELPOSITIONPROPERTY_RTTI;
    }
    else if (property->getName().compare("viewTexture")==0) {
        return TEXTUREPROPERTY_RTTI;
    }
    else if (property->getName().compare("viewFont")==0) {
        return FONTFILEPROPERTY_RTTI;
    }else if(property->getName().compare("viewTgtAnchorShape")==0){
        return EDGEEXTREMITYGLYPHPROPERTY_RTTI;
    }else if(property->getName().compare("viewSrcAnchorShape")==0){
        return EDGEEXTREMITYGLYPHPROPERTY_RTTI;
    }
    else if (typeid(*property) == typeid(BooleanProperty)) {
        return BOOLEAN_PROPERTY_RTTI;
    }
    else if (typeid(*property) == typeid(ColorProperty)) {
        return COLORPROPERTY_RTTI;
    }
    else if (typeid(*property) == typeid(DoubleProperty)) {
        return DOUBLEPROPERTY_RTTI;
    }else if (typeid(*property) == typeid(IntegerProperty)) {
        return INTEGER_PROPERTY_RTTI;
    }
    else if (typeid(*property) == typeid(SizeProperty)) {
        return SIZEPROPERTY_RTTI;
    }
    else if (typeid(*property) == typeid(LayoutProperty)) {
        return LAYOUTPROPERTY_RTTI;
    }
    else if (typeid(*property) == typeid(StringVectorProperty)) {
        return STRINGVECTORPROPERTY_RTTI;
    }
    else if (typeid(*property) == typeid(ColorVectorProperty)) {
        return COLORVECTORPROPERTY_RTTI;
    }
    else if (typeid(*property) == typeid(SizeVectorProperty)) {
        return SIZEVECTORPROPERTY_RTTI;
    }
    else if (typeid(*property) == typeid(BooleanVectorProperty)) {
        return BOOLEANVECTORPROPERTY_RTTI;
    }
    else if (typeid(*property) == typeid(DoubleVectorProperty)) {
        return DOUBLEVECTORPROPERTY_RTTI;
    }
    else if (typeid(*property) == typeid(IntegerVectorProperty)) {
        return INTEGERVECTORPROPERTY_RTTI;
    }
    else if (typeid(*property) == typeid(CoordVectorProperty)) {
        return COORDVECTORPROPERTY_RTTI;
    }else{
        return STRINGPROPERTY_RTTI;
    }
}

bool TulipQVariantBuilder::setData(const QVariant& value, ElementType elementType, unsigned int elementId, TulipPropertyType propertyType, PropertyInterface* property) const
{
    if(!value.isValid()){
        return false;
    }
    switch(propertyType){
    case BOOLEAN_PROPERTY_RTTI:
        return setValueToTulipPropertyFromQVariant<BooleanProperty,bool,bool>(value,elementType,elementId,(BooleanProperty*)property);
        break;
    case DOUBLEPROPERTY_RTTI:
        return setValueToTulipPropertyFromQVariant<DoubleProperty,double,double>(value,elementType,elementId,(DoubleProperty*)property);
        break;
    case NODEGLYPHPROPERTY_RTTI:
    case EDGESHAPEPROPERTY_RTTI:
    case EDGEEXTREMITYGLYPHPROPERTY_RTTI:
    case INTEGER_PROPERTY_RTTI:
        return setValueToTulipPropertyFromQVariant<IntegerProperty,int,int>(value,elementType,elementId,(IntegerProperty*)property);
        break;
    case LAYOUTPROPERTY_RTTI:
        return setValueToTulipPropertyFromQVariant<LayoutProperty,Coord,vector<Coord> >(value,elementType,elementId,(LayoutProperty*)property);
        break;
    case COLORPROPERTY_RTTI:
        return setValueToTulipPropertyFromQVariant<ColorProperty,Color,Color >(value,elementType,elementId,(ColorProperty*)property);
        break;
    case SIZEPROPERTY_RTTI:
        return setValueToTulipPropertyFromQVariant<SizeProperty,Size,Size >(value,elementType,elementId,(SizeProperty*)property);
        break;
    case BOOLEANVECTORPROPERTY_RTTI:
        return setValueToTulipPropertyFromQVariant<BooleanVectorProperty,vector<bool>,vector<bool> >(value,elementType,elementId,(BooleanVectorProperty*)property);
        break;
    case COLORVECTORPROPERTY_RTTI:
        return setValueToTulipPropertyFromQVariant<ColorVectorProperty,vector<Color>,vector<Color> >(value,elementType,elementId,(ColorVectorProperty*)property);
        break;
    case COORDVECTORPROPERTY_RTTI:
        return setValueToTulipPropertyFromQVariant<CoordVectorProperty,vector<Coord>,vector<Coord> >(value,elementType,elementId,(CoordVectorProperty*)property);
        break;
    case DOUBLEVECTORPROPERTY_RTTI:
        return setValueToTulipPropertyFromQVariant<DoubleVectorProperty,vector<double>,vector<double> >(value,elementType,elementId,(DoubleVectorProperty*)property);
        break;
    case INTEGERVECTORPROPERTY_RTTI:
        return setValueToTulipPropertyFromQVariant<IntegerVectorProperty,vector<int>,vector<int> >(value,elementType,elementId,(IntegerVectorProperty*)property);
        break;
    case SIZEVECTORPROPERTY_RTTI:
        return setValueToTulipPropertyFromQVariant<SizeVectorProperty,vector<Size>,vector<Size> >(value,elementType,elementId,(SizeVectorProperty*)property);
        break;
    case STRINGVECTORPROPERTY_RTTI:
        return setValueToTulipPropertyFromQVariant<StringVectorProperty,vector<string>,vector<string> >(value,elementType,elementId,(StringVectorProperty*)property);
        break;
    default:
        if(elementType == NODE){
            const string& oldValue = property->getNodeStringValue(node(elementId));
            string newValue = value.toString().toStdString();
            if(oldValue.compare(newValue)!=0){
                return property->setNodeStringValue(node(elementId),newValue);
            }
        }else{
            const string& oldValue = property->getEdgeStringValue(edge(elementId));
            string newValue = value.toString().toStdString();
            if(oldValue.compare(newValue)!=0){
                return property->setEdgeStringValue(edge(elementId),newValue);
            }
        }
        return false;
        break;
    }
}
