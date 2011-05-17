#include "TulipQVariantBuilder.h"
#include <tulip/TlpQtTools.h>
#include <tulip/GlGraphStaticData.h>
#include <tulip/EdgeExtremityGlyphManager.h>
#include "GlyphPreviewGenerator.h"
using namespace tlp;
using namespace std;

TulipQVariantBuilder::TulipQVariantBuilder()
{
}

QVariant TulipQVariantBuilder::data(Graph* graph,int displayRole,ElementType elementType,unsigned int elementId,TulipPropertyType propertyType,PropertyInterface* property)const{
    switch(displayRole){
    case Qt::DisplayRole:
        {
            switch(propertyType){
            case DOUBLEPROPERTY_RTTI:
                {
                    DoubleProperty* doubleProp = dynamic_cast<DoubleProperty*>(property);
                    double min = 0;
                    double max = 0;
                    double value = 0;
                    if(elementType == NODE){
                        min = doubleProp->getNodeMin(graph);
                        max = doubleProp->getNodeMax(graph);
                        value = doubleProp->getNodeValue(node(elementId));
                    }else{
                        min = doubleProp->getEdgeMin(graph);
                        max = doubleProp->getEdgeMax(graph);
                        value = doubleProp->getEdgeValue(edge(elementId));
                    }
                    return QVariant::fromValue< Interval<double> >(Interval<double>(min,value,max));
                }
                break;
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
            }
            break;
        }
        break;
    case Qt::EditRole:
        {
            switch(propertyType){
            case NODEGLYPHPROPERTY_RTTI:
                {
                    QStringList elements;
                    GlyphFactory::initFactory();
                    int value = elementType==NODE?((IntegerProperty*)property)->getNodeValue(node(elementId)):((IntegerProperty*)property)->getEdgeValue(edge(elementId));
                    ElementCollection collection;
                    string glyphName;
                    forEach(glyphName,GlyphFactory::factory->availablePlugins()) {
                        collection.addElement(tlpStringToQString(glyphName));
                        int elementIndex = collection.rowCount()-1;
                        int glyphIndex = GlyphManager::getInst().glyphId(glyphName);
                        //Associate the last added element to the shape id
                        collection.setData(collection.index(elementIndex,0),QVariant(glyphIndex),Qt::UserRole);
                        //If the shape id is the same than the selected mark the last added element as selected
                        collection.setElementSelection(elementIndex,glyphIndex == value);
                    }
                    return QVariant::fromValue<tlp::ElementCollection>(collection);
                }
                break;
            case EDGESHAPEPROPERTY_RTTI:
                {
                    QStringList elements;
                    ElementCollection collection;
                    int value = elementType==NODE?((IntegerProperty*)property)->getNodeValue(node(elementId)):((IntegerProperty*)property)->getEdgeValue(edge(elementId));
                    for (int i = 0; i < GlGraphStaticData::edgeShapesCount; i++){
                        collection.addElement(tlpStringToQString(GlGraphStaticData::edgeShapeName(GlGraphStaticData::edgeShapeIds[i])));
                        int elementIndex = collection.rowCount()-1;
                        //Associate the last added element to the shape id
                        collection.setData(collection.index(elementIndex,0),QVariant(GlGraphStaticData::edgeShapeIds[i]),Qt::UserRole);
                        //If the shape id is the same than the selected mark the last added element as selected
                        collection.setElementSelection(elementIndex,GlGraphStaticData::edgeShapeIds[i] == value);
                    }
                    return QVariant::fromValue<tlp::ElementCollection>(collection);
                }
                break;
            case EDGEEXTREMITYGLYPHPROPERTY_RTTI:
                {
                    QStringList elements;
                    EdgeExtremityGlyphFactory::initFactory();
                    int value = elementType==NODE?((IntegerProperty*)property)->getNodeValue(node(elementId)):((IntegerProperty*)property)->getEdgeValue(edge(elementId));
                    ElementCollection collection;
                    //Add the no edge extremity shape code to the list.
                    collection.addElement(QString("NONE"));
                    collection.setData(collection.index(0,0),QVariant(EdgeExtremityGlyphManager::NoEdgeExtremetiesId),Qt::UserRole);
                    collection.setElementSelection(0,EdgeExtremityGlyphManager::NoEdgeExtremetiesId == value);
                    string glyphName;
                    forEach(glyphName,EdgeExtremityGlyphFactory::factory->availablePlugins()) {                        
                        collection.addElement(tlpStringToQString(glyphName));
                        int elementIndex = collection.rowCount()-1;
                        int glyphIndex = EdgeExtremityGlyphManager::getInst().glyphId(glyphName);
                        //Associate the last added element to the shape id
                        collection.setData(collection.index(elementIndex,0),QVariant(glyphIndex),Qt::UserRole);
                        //If the shape id is the same than the selected mark the last added element as selected
                        collection.setElementSelection(elementIndex,glyphIndex == value);
                    }
                    return QVariant::fromValue<tlp::ElementCollection>(collection);
                }
                break;
            case INTEGER_PROPERTY_RTTI:
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
            case FONTFILEPROPERTY_RTTI:
                {
                    string value = (elementType==NODE?((StringProperty*)property)->getNodeValue(node(elementId)):((StringProperty*)property)->getEdgeValue(edge(elementId)));
                    //Create an object containing the original file and the filter options.
                    FilteredUrl url(tlpStringToQString(value),QString("Font (*.ttf)"));
                    return QVariant::fromValue<FilteredUrl>(url);
                }
                break;
            case TEXTUREPROPERTY_RTTI:
                {
                    string value = (elementType==NODE?((StringProperty*)property)->getNodeValue(node(elementId)):((StringProperty*)property)->getEdgeValue(edge(elementId)));
                    FilteredUrl url(tlpStringToQString(value),QString("Images (*.png *.jpeg *.jpg *.bmp)"));
                    return QVariant::fromValue<FilteredUrl>(url);
                }
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
        break;
    case Qt::DecorationRole:
        {
            switch(propertyType){
            case COLORPROPERTY_RTTI:
                {
                    const Color& color = (elementType == NODE ? ((ColorProperty*)property)->getNodeValue(node(elementId)):((ColorProperty*)property)->getEdgeValue(edge(elementId)));
                    return QVariant(QColor(colorToQColor(color)));
                }
                break;
                case NODEGLYPHPROPERTY_RTTI:
                return QVariant(GlyphPreviewGenerator::getInst().getPreview(((IntegerProperty*)property)->getNodeValue(node(elementId))));
                break;
            case EDGEEXTREMITYGLYPHPROPERTY_RTTI:
                    return QVariant(EdgeExtremityGlyphPreviewGenerator::getInst().getPreview(((IntegerProperty*)property)->getEdgeValue(edge(elementId))));
                break;
            default:
                return QVariant();
                break;
            }
        }
        break;
    default:
        return QVariant();
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
        {
            ElementCollection collection = value.value<ElementCollection>();
            //Get the selected element
            QList<int> selectedElements = collection.selectedElement();
            if(!selectedElements.empty()){
                //Extract the code associated with the selected element.
                return setValueToTulipPropertyFromQVariant<IntegerProperty,int,int>(collection.data(collection.index(selectedElements.front(),0),Qt::UserRole),elementType,elementId,(IntegerProperty*)property);
            }else{
                return false;
            }
        }
        break;
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
    case FONTFILEPROPERTY_RTTI:
    case TEXTUREPROPERTY_RTTI:
        {
            FilteredUrl url = value.value<FilteredUrl>();
            (elementType == NODE ?((StringProperty*)property)->setNodeValue(node(elementId),QStringToTlpString(url.path())):((StringProperty*)property)->setEdgeValue(edge(elementId),QStringToTlpString(url.path())));
            return true;
        }
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
Qt::ItemFlags TulipQVariantBuilder::flags(Qt::ItemFlags defaultFlags,ElementType elementType,unsigned int ,TulipPropertyType propertyType,PropertyInterface*)const{
    switch(propertyType){
        //If we try to display an edge extremity property for nodes desactivate the row.
    case EDGEEXTREMITYGLYPHPROPERTY_RTTI:
        if(elementType == NODE){
            return Qt::NoItemFlags;
        }else{
            return defaultFlags | Qt::ItemIsEditable | Qt::ItemIsDragEnabled;
        }
        break;
    default:
        return defaultFlags | Qt::ItemIsEditable | Qt::ItemIsDragEnabled;
        break;
    }
}
