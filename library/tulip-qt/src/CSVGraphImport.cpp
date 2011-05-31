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
#include "tulip/CSVGraphImport.h"
#include <tulip/Graph.h>
#include <tulip/PropertyInterface.h>
#include <tulip/ForEach.h>
#include <tulip/TlpQtTools.h>
using namespace tlp;
using namespace std;

const char minusChar = '-';

string PropertyTools::getPropertyTypeLabel(const string& typeName){
   return QStringToTlpString(propertyTypeToPropertyTypeLabel(typeName));
}

string PropertyTools::getPropertyTypeFromPropertyTypeLabel(const string& typeNameLabel){
    return propertyTypeLabelToPropertyType(tlpStringToQString(typeNameLabel));
}

QStringList PropertyTools::getPropertyTypeLabelsList(){
    QStringList labels;
    //Init properties list
    labels<<propertyTypeToPropertyTypeLabel("color");
    labels<<propertyTypeToPropertyTypeLabel("int");
    labels<<propertyTypeToPropertyTypeLabel("layout");
    labels<<propertyTypeToPropertyTypeLabel("double");
    labels<<propertyTypeToPropertyTypeLabel("bool");
    labels<<propertyTypeToPropertyTypeLabel("size");
    labels<<propertyTypeToPropertyTypeLabel("string");
    labels<<propertyTypeToPropertyTypeLabel("vector<bool>");
    labels<<propertyTypeToPropertyTypeLabel("vector<color>");
    labels<<propertyTypeToPropertyTypeLabel("vector<coord>");
    labels<<propertyTypeToPropertyTypeLabel("vector<double>");
    labels<<propertyTypeToPropertyTypeLabel("vector<int>");
    labels<<propertyTypeToPropertyTypeLabel("vector<size>");
    labels<<propertyTypeToPropertyTypeLabel("vector<string>");
    return labels;
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

PropertyInterface *PropertyTools::getLocalProperty(Graph* graph, const string& propertyName,
                                              const string& propertyType) {

    if (propertyType.compare("double") == 0) {
        return graph->getLocalProperty<DoubleProperty> (propertyName);
    }
    else if (propertyType.compare("layout")==0) {
        return graph->getLocalProperty<LayoutProperty> (propertyName);
    }
    else if (propertyType.compare("string")==0) {
        return graph->getLocalProperty<StringProperty> (propertyName);
    }
    else if (propertyType.compare("int")==0) {
        return graph->getLocalProperty<IntegerProperty> (propertyName);
    }
    else if (propertyType.compare("color")==0) {
        return graph->getLocalProperty<ColorProperty> (propertyName);
    }
    else if (propertyType.compare("size")==0) {
        return graph->getLocalProperty<SizeProperty> (propertyName);
    }
    else if (propertyType.compare("bool")==0) {
        return graph->getLocalProperty<BooleanProperty> (propertyName);
    }
    else if (propertyType.compare("vector<double>")==0) {
        return graph->getLocalProperty<DoubleVectorProperty> (propertyName);
    }
    else if (propertyType.compare("vector<string>")==0) {
        return graph->getLocalProperty<StringVectorProperty> (propertyName);
    }
    else if (propertyType.compare("vector<int>")==0) {
        return graph->getLocalProperty<IntegerVectorProperty> (propertyName);
    }
    else if (propertyType.compare("vector<coord>")==0) {
        return graph->getLocalProperty<CoordVectorProperty> (propertyName);
    }
    else if (propertyType.compare("vector<color>")==0) {
        return graph->getLocalProperty<ColorVectorProperty> (propertyName);
    }
    else if (propertyType.compare("BooleanVector")==0) {
        return graph->getLocalProperty<BooleanVectorProperty> (propertyName);
    }
    else if (propertyType.compare("SizeVector")==0) {
        return graph->getLocalProperty<SizeVectorProperty> (propertyName);
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
    valueToId.clear();
    //Fill map with graph values.
    fillValuesMap(type,graph,keyProperty,valueToId);
}
void AbstractCSVToGraphDataMapping::fillValuesMap(tlp::ElementType type,Graph* graph,tlp::PropertyInterface* keyProperty,map<string,unsigned int>& valuesToId){
    if(type == NODE){
        node n;
        forEach(n,graph->getNodes()){
            valuesToId[keyProperty->getNodeStringValue(n)]=n.id;
        }
    }else{
        edge e;
        forEach(e,graph->getEdges()){
            valuesToId[keyProperty->getEdgeStringValue(e)]=e.id;
        }
    }
}

void AbstractCSVToGraphDataMapping::token(unsigned int row, unsigned int column, const string& token){
    if(importRow(row) && column == columnIndex){        
        if(valueToId.find(token) == valueToId.end()){
            unsigned int id = buildIndexForRow(row,token,graph,keyProperty);
            //Store the id corresponding to the token.
            if(id!=UINT_MAX){
                valueToId[token] = id;
            }
        }else{
            //Use the las found id.
            rowToGraphId[row]=valueToId[token];
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

unsigned int CSVToGraphNodeIdMapping::buildIndexForRow(unsigned int,const string& indexKey,Graph* graph,PropertyInterface* keyProperty){
    if(createMissingNodes){
        node newNode = graph->addNode();
        keyProperty->setNodeStringValue(newNode,indexKey);
        return newNode.id;
    }else{
        return UINT_MAX;
    }
}

CSVToGraphEdgeIdMapping::CSVToGraphEdgeIdMapping(Graph* graph,unsigned int columnIndex,const string& propertyName,unsigned int firstRow,unsigned int lastRow):AbstractCSVToGraphDataMapping(graph,EDGE,columnIndex,propertyName,firstRow,lastRow){
}

unsigned int CSVToGraphEdgeIdMapping::buildIndexForRow(unsigned int ,const string& ,Graph* ,PropertyInterface* ){
    return UINT_MAX;
}

CSVToGraphEdgeSrcTgtMapping::CSVToGraphEdgeSrcTgtMapping(Graph* graph,
                                                         unsigned int srcColumnIndex,unsigned int tgtColumnIndex,const string& propertyName,
                                                         unsigned int firstRow,unsigned int lastRow,
                                                         bool createMissinNodes):
graph(graph),srcColumnIndex(srcColumnIndex),tgtColumnIndex(tgtColumnIndex),keyProperty(graph->getProperty(propertyName)),firstRow(firstRow),lastRow(lastRow),
buildMissingElements(createMissinNodes){
}

pair<ElementType,unsigned int> CSVToGraphEdgeSrcTgtMapping::getElementForRow(unsigned int row){
    //Search if an edge was registered for this row and this couple of nodes.
    map<unsigned int,edge>::iterator it = rowToEdge.find(row);
    if(it!=rowToEdge.end()){
        return make_pair(EDGE,it->second.id);
    }else{
        return make_pair(EDGE,UINT_MAX);
    }
}

void CSVToGraphEdgeSrcTgtMapping::begin(){
    valueToId.clear();
    rowToEdge.clear();
    rowToNodes.clear();
    node n;
    forEach(n,graph->getNodes()){
        valueToId[keyProperty->getNodeStringValue(n)]=n.id;
    }
}

void CSVToGraphEdgeSrcTgtMapping::token(unsigned int row, unsigned int column, const string& token){
    if(firstRow <= row && lastRow>= row){
        if(column == srcColumnIndex){
            map<string,unsigned int>::iterator it = valueToId.find(token);
            //token exists in the map
            node n;
            if(it != valueToId.end()){
                n = node(it->second);
            }else if(buildMissingElements){
                n = graph->addNode();
                keyProperty->setNodeStringValue(n,token);
                valueToId[token] = n.id;
            }
            rowToNodes[row].first = n;
        }else if(column == tgtColumnIndex){
            map<string,unsigned int>::iterator it = valueToId.find(token);
            //token exists in the map
            node n;
            if(it != valueToId.end()){
                n =node(it->second);
            }else if(buildMissingElements){
                n = graph->addNode();
                keyProperty->setNodeStringValue(n,token);
                valueToId[token] = n.id;
            }
            rowToNodes[row].second= n;
        }
        //If nodes are found create a new edge.
        pair<node,node> nodes = rowToNodes[row];
        if( graph->isElement(nodes.first) && graph->isElement(nodes.second) && rowToEdge.find(row)==rowToEdge.end()){
            rowToEdge[row]=graph->addEdge(nodes.first,nodes.second);
        }
    }
}
void CSVToGraphEdgeSrcTgtMapping::end(unsigned int , unsigned int ){
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

