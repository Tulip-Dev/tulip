#include "tulip/CSVGraphImport.h"
#include <tulip/Graph.h>
#include <tulip/PropertyInterface.h>
#include <tulip/ForEach.h>
#include <tulip/TlpQtTools.h>
using namespace tlp;
using namespace std;

const char minusChar = '-';

string PropertyTools::getPropertyTypeLabel(const string& typeName){
    if(typeName.compare("graph")==0)return "Graph";
    if(typeName.compare("double")==0)return "Metric";
    if(typeName.compare("layout")==0)return "Layout";
    if(typeName.compare("string")==0)return "String";
    if(typeName.compare("int")==0)return "Integer";
    if(typeName.compare("color")==0)return "Color";
    if(typeName.compare("size")==0)return "Size";
    if(typeName.compare("bool")==0)return "Selection";
    if(typeName.compare("vector<double>")==0)return "DoubleVector";
    if(typeName.compare("vector<string>")==0)return "StringVector";
    if(typeName.compare("vector<int>")==0)return "IntegerVector";
    if(typeName.compare("vector<coord>")==0)return "CoordVector";
    if(typeName.compare("vector<color>")==0)return "ColorVector";
    if(typeName.compare("vector<size>")==0)return "SizeVector";
    if(typeName.compare("vector<bool>")==0)return "BooleanVector";
    return "";
}

string PropertyTools::getPropertyTypeFromPropertyTypeLabel(const string& typeNameLabel){
    if(typeNameLabel.compare("Graph")==0)return "graph";
    if(typeNameLabel.compare("Metric" )==0)return "double";
    if(typeNameLabel.compare("Layout")==0)return "layout";
    if(typeNameLabel.compare("String")==0)return "string";
    if(typeNameLabel.compare("Integer")==0)return  "int";
    if(typeNameLabel.compare("Color")==0)return  "color";
    if(typeNameLabel.compare("Size")==0)return  "size";
    if(typeNameLabel.compare("Selection")==0)return  "bool";
    if(typeNameLabel.compare("DoubleVector")==0)return "vector<double>";
    if(typeNameLabel.compare("StringVector")==0)return "vector<string>";
    if(typeNameLabel.compare("IntegerVector")==0)return "vector<int>";
    if(typeNameLabel.compare("CoordVector")==0)return "vector<coord>";
    if(typeNameLabel.compare("ColorVector")==0)return "vector<color>";
    if(typeNameLabel.compare("SizeVector")==0)return "vector<size>";
    if(typeNameLabel.compare("BooleanVector")==0)return "vector<bool>";
    return "";
}

QStringList PropertyTools::getPropertyTypeLabelsList(){
    QStringList lst;
    lst << "Color" << "Integer" << "Layout" << "Metric" << "Selection" << "Size" << "String" << "BooleanVector"
            << "ColorVector" << "CoordVector" << "DoubleVector" << "IntegerVector" << "SizeVector" << "StringVector";
    return lst;
}

string PropertyTools::guessDataType(const string& data, const string& decimalSeparator) {
    bool stringValue = false;
    bool intValue = false;
    bool doubleValue = false;
    for (unsigned int j = 0; j < data.length(); ++j) {
        if (isalpha(data[j])) {
            stringValue = true;
        }
        else if (isdigit(data[j]) && !stringValue) {
            if (!doubleValue) {
                intValue = true;
            }
            else {
                doubleValue = true;
            }
        }
        else if (decimalSeparator.find_first_of(data[j]) != string::npos && intValue) {
            doubleValue = true;
            intValue = false;
        }
        else if (j == 0 && data[j] == minusChar) {
            intValue = true;
        }
        else {
            stringValue = true;
            intValue = false;
            doubleValue = false;
        }
    }
    if (stringValue) {
        return "string";
    }
    else if (intValue) {
        return "int";
    }
    else if (doubleValue) {
        return "double";
    }
    else {
        return "";
    }
}

bool  PropertyTools::existingPropertyIsCompatibleWithType(Graph* graph, const string& propertyName,
                                                          const string& propertyType) {
    if(!graph->existProperty(propertyName)){
        return false;
    }
    return graph->getProperty(propertyName)->getTypename().compare(propertyType)==0;
}

PropertyInterface *PropertyTools::getProperty(Graph* graph, const string& propertyName,
                                              const string& propertyType) {

    if (propertyType.compare("double") == 0) {
        return graph->getProperty<DoubleProperty> (propertyName);
    }
    else if (propertyType.compare("layout")==0) {
        return graph->getProperty<LayoutProperty> (propertyName);
    }
    else if (propertyType.compare("string")==0) {
        return graph->getProperty<StringProperty> (propertyName);
    }
    else if (propertyType.compare("int")==0) {
        return graph->getProperty<IntegerProperty> (propertyName);
    }
    else if (propertyType.compare("color")==0) {
        return graph->getProperty<ColorProperty> (propertyName);
    }
    else if (propertyType.compare("size")==0) {
        return graph->getProperty<SizeProperty> (propertyName);
    }
    else if (propertyType.compare("bool")==0) {
        return graph->getProperty<BooleanProperty> (propertyName);
    }
    else if (propertyType.compare("vector<double>")==0) {
        return graph->getProperty<DoubleVectorProperty> (propertyName);
    }
    else if (propertyType.compare("vector<string>")==0) {
        return graph->getProperty<StringVectorProperty> (propertyName);
    }
    else if (propertyType.compare("vector<int>")==0) {
        return graph->getProperty<IntegerVectorProperty> (propertyName);
    }
    else if (propertyType.compare("vector<coord>")==0) {
        return graph->getProperty<CoordVectorProperty> (propertyName);
    }
    else if (propertyType.compare("vector<color>")==0) {
        return graph->getProperty<ColorVectorProperty> (propertyName);
    }
    else if (propertyType.compare("BooleanVector")==0) {
        return graph->getProperty<BooleanVectorProperty> (propertyName);
    }
    else if (propertyType.compare("SizeVector")==0) {
        return graph->getProperty<SizeVectorProperty> (propertyName);
    }
    else {
        return NULL;
    }
}


CSVImportParameters::CSVImportParameters(unsigned int fromLine,unsigned int toLine,const vector<CSVColumn>& columns):fromLine(fromLine),toLine(toLine),columns(columns){
}

CSVImportParameters::~CSVImportParameters(){
}

unsigned int CSVImportParameters::columnNumber()const{
    return columns.size();
}
bool CSVImportParameters::importColumn(unsigned int column)const{
    if(column < columns.size()){
        return columns[column].isUsed();
    }else{
        return false;
    }
}
string CSVImportParameters::getColumnName(unsigned int column)const{
    if(column < columns.size()){
        return columns[column].columnName();
    }
    else{
        return string();
    }
}
string CSVImportParameters::getColumnDataType(unsigned int column)const{
    if(column < columns.size()){
        return columns[column].columnDataType();
    }
    else{
        return string();
    }
}
bool CSVImportParameters::importRow(unsigned int row)const{
    return row >= fromLine && row <= toLine;
}

unsigned int CSVImportParameters::getFirstLineIndex()const{
    return fromLine;
}
unsigned int CSVImportParameters::getLastLineIndex()const{
    return toLine;
}

AbstractCSVToGraphDataMapping::AbstractCSVToGraphDataMapping(Graph* graph,ElementType type,unsigned int columnIndex,const string& propertyName,unsigned int firstRow,unsigned int lastRow):graph(graph),type(type),columnIndex(columnIndex),keyProperty(NULL),firstRow(firstRow),lastRow(lastRow){
    assert(graph!=NULL);
    assert(graph->existProperty(propertyName));
    keyProperty = graph->getProperty(propertyName);
}

bool AbstractCSVToGraphDataMapping::importRow(unsigned int row)const{
    return firstRow <= row && lastRow>= row;
}

void AbstractCSVToGraphDataMapping::begin(){
    //Clean old informations.
    rowToGraphId.clear();
}

void AbstractCSVToGraphDataMapping::token(unsigned int row, unsigned int column, const string& token){
    if(importRow(row) && column == columnIndex){
        if(!buildIndexForRow(row,token)){
        }
    }
}

void AbstractCSVToGraphDataMapping::end(unsigned int, unsigned int){
}

pair<ElementType,unsigned int> AbstractCSVToGraphDataMapping::getElementForRow(unsigned int row){
    map<unsigned int, unsigned int>::const_iterator it = rowToGraphId.find(row);
    if(it != rowToGraphId.end()){
        return make_pair(type,it->second);
    }
    else{
        return make_pair(type,UINT_MAX);
    }
}

CSVToNewNodeIdMapping::CSVToNewNodeIdMapping(Graph* graph):graph(graph){
}

pair<ElementType,unsigned int> CSVToNewNodeIdMapping::getElementForRow(unsigned int row){
    map<unsigned int, unsigned int>::const_iterator it = rowToGraphId.find(row);
    if(it != rowToGraphId.end()){
        return make_pair(NODE,it->second);
    }else{
        unsigned int newId = graph->addNode().id;
        rowToGraphId[row] = newId;
        return make_pair(NODE,newId);
    }
}

void CSVToNewNodeIdMapping::begin(){

}

void CSVToNewNodeIdMapping::token(unsigned int, unsigned int, const string&){

}

void CSVToNewNodeIdMapping::end(unsigned int, unsigned int){

}

CSVToGraphNodeIdMapping::CSVToGraphNodeIdMapping(Graph* graph,unsigned int columnIndex,const string& propertyName,unsigned int firstRow,unsigned int lastRow,bool createNode):AbstractCSVToGraphDataMapping(graph,NODE,columnIndex,propertyName,firstRow,lastRow),createMissingNodes(createNode){
}

bool CSVToGraphNodeIdMapping::buildIndexForRow(unsigned int row,const string& indexKey){
    node n;
    forEach(n,graph->getNodes()){
        if(keyProperty->getNodeStringValue(n).compare(indexKey)==0){
            rowToGraphId[row] = n.id;
            returnForEach(true);
        }
    }
    if(createMissingNodes){
        node newNode = graph->addNode();
        rowToGraphId[row] = newNode.id;
        keyProperty->setNodeStringValue(newNode,indexKey);
        return true;
    }else{
        return false;
    }
}

CSVToGraphEdgeIdMapping::CSVToGraphEdgeIdMapping(Graph* graph,unsigned int columnIndex,const string& propertyName,unsigned int firstRow,unsigned int lastRow):AbstractCSVToGraphDataMapping(graph,EDGE,columnIndex,propertyName,firstRow,lastRow){
}

bool CSVToGraphEdgeIdMapping::buildIndexForRow(unsigned int row,const string& indexKey){
    edge e;
    forEach(e,graph->getEdges()){
        if(keyProperty->getEdgeStringValue(e).compare(indexKey)==0){
            rowToGraphId[row] = e.id;
            returnForEach(true);
        }
    }    
    return false;
}

CSVToGraphEdgeSrcTgtMapping::CSVToGraphEdgeSrcTgtMapping(Graph* graph,
                                                         unsigned int srcColumnIndex,unsigned int tgtColumnIndex,
                                                         const string& srcPropertyName,const string& tgtPropertyName,
                                                         unsigned int firstRow,unsigned int lastRow,
                                                         bool createMissinNodes):
graph(graph),src(CSVToGraphNodeIdMapping(graph,srcColumnIndex,srcPropertyName,firstRow,lastRow,createMissinNodes)),
tgt(CSVToGraphNodeIdMapping(graph,tgtColumnIndex,tgtPropertyName,firstRow,lastRow,createMissinNodes)),
buildMissingElements(createMissinNodes){
}

pair<ElementType,unsigned int> CSVToGraphEdgeSrcTgtMapping::getElementForRow(unsigned int row){
    node srcNode = node(src.getElementForRow(row).second);
    node tgtNode = node(tgt.getElementForRow(row).second);
    if(!graph->isElement(srcNode) || !graph->isElement(tgtNode)){
        return make_pair(EDGE,UINT_MAX);
    }else{
        //Search if an edge was registered for this node and this couple of nodes.
        map<unsigned int,edge>::iterator it = rowToEdge.find(row);
        if(it!=rowToEdge.end()){
            return make_pair(EDGE,it->second.id);
        }else{
            return make_pair(EDGE,UINT_MAX);
        }
    }
}

void CSVToGraphEdgeSrcTgtMapping::begin(){
    src.begin();
    tgt.begin();
    rowToEdge.clear();
}

void CSVToGraphEdgeSrcTgtMapping::token(unsigned int row, unsigned int column, const string& token){
    //Found nodes in the graph.
    src.token(row,column,token);
    tgt.token(row,column,token);
    node srcNode = node(src.getElementForRow(row).second);
    node tgtNode = node(tgt.getElementForRow(row).second);
    //If nodes are found create a new edge and no previous mapping was done.
    if( graph->isElement(srcNode) && graph->isElement(tgtNode) && rowToEdge.find(row)==rowToEdge.end()){
        rowToEdge[row]=graph->addEdge(srcNode,tgtNode);
    }
}
void CSVToGraphEdgeSrcTgtMapping::end(unsigned int rowNumber, unsigned int columnNumber){
    src.end(rowNumber,columnNumber);
    tgt.end(rowNumber,columnNumber);
}

CSVImportColumnToGraphPropertyMappingProxy::CSVImportColumnToGraphPropertyMappingProxy(Graph* graph,const CSVImportParameters& importParameters,QWidget* parent):graph(graph),importParameters(importParameters),parent(parent){

}

PropertyInterface *CSVImportColumnToGraphPropertyMappingProxy::getPropertyInterface(unsigned int column,const string& token){
    TLP_HASH_MAP<unsigned int,PropertyInterface*>::iterator it = propertiesBuffer.find(column);
    //No properties
    if(it==propertiesBuffer.end()){
        string propertyType = importParameters.getColumnDataType(column);
        string propertyName = importParameters.getColumnName(column);
        //The property type is invalid or autodetect.
        if (propertyType.compare("")==0) {
            //Determine type
            propertyType = PropertyTools::guessDataType(token,string(";,."));
            //If auto detection fail set to default type : string.
            if (propertyType.empty()) {
                propertyType = "string";
            }
        }        
        PropertyInterface *interf=NULL;
        //The property already exist need to check if existing is compatible with new one.
        if (graph->existProperty(propertyName)) {
            //If the properties are compatible query if we had to override existing.
            if (PropertyTools::existingPropertyIsCompatibleWithType(graph, propertyName, propertyType)) {
                if (overwritePropertiesButton != QMessageBox::YesToAll && overwritePropertiesButton != QMessageBox::NoToAll) {
                    overwritePropertiesButton = QMessageBox::question(parent, parent->tr("Property exist."),
                                                                      parent->tr("A property with the name \"") + tlpStringToQString(propertyName) + parent->tr(
                                                                              "\" already exist. Overwrite?"), QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::No
                                                                      | QMessageBox::NoToAll, QMessageBox::Yes);
                }
                if (overwritePropertiesButton == QMessageBox::NoToAll || overwritePropertiesButton == QMessageBox::No) {
                    interf = NULL;
                }
            }
            else {
                //If the properties are not compatible skip.
                QMessageBox::critical(parent, parent->tr("Property exist."), parent->tr("A property with the name \"") + tlpStringToQString(
                        propertyName) + parent->tr("\" already exist with a different type. This property will be ignored."));
                interf = NULL;
            }
        }
        else {
            interf=PropertyTools::getProperty(graph, propertyName, propertyType);
        }
        propertiesBuffer[column] = interf;
        return interf;
    }else{
        return it->second;
    }
}

CSVGraphImport::CSVGraphImport(CSVToGraphDataMapping* mapping,CSVImportColumnToGraphPropertyMapping* properties,const CSVImportParameters& importParameters):mapping(mapping),propertiesManager(properties),importParameters(importParameters){
}
CSVGraphImport::~CSVGraphImport(){}
void CSVGraphImport::begin(){
}
void CSVGraphImport::token(unsigned int row, unsigned int column, const string& token){
    if(!importParameters.importRow(row) || !importParameters.importColumn(column)){
        return;
    }
    pair<ElementType,unsigned int> element = mapping->getElementForRow(row);
    PropertyInterface *interf = propertiesManager->getPropertyInterface(column,token);    
    if(element.second != UINT_MAX && interf != NULL){        
        if(element.first == NODE){            
            if(!interf->setNodeStringValue(node(element.second),token)){
                std::cerr<<__PRETTY_FUNCTION__<<":"<<__LINE__<<" error when importing token \""<<token<<"\" in property \""<<interf->getName()<<"\" of type \""<<interf->getTypename()<<"\""<<std::endl;
            }
        }else{
            if(!interf->setEdgeStringValue(edge(element.second),token)){
                std::cerr<<__PRETTY_FUNCTION__<<":"<<__LINE__<<" error when importing token \""<<token<<"\" in property \""<<interf->getName()<<"\" of type \""<<interf->getTypename()<<"\""<<std::endl;
            }
        }
    }
}
void CSVGraphImport::end(unsigned int, unsigned int){
}

