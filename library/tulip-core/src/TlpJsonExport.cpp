#include <tulip/ForEach.h>
#include <tulip/ExportModule.h>
#include <tulip/Graph.h>
#include <tulip/DataSet.h>
#include <tulip/MutableContainer.h>
#include <tulip/MethodFactory.h>

#include <sstream>

#include <QDate>
#include <tulip/YajlFacade.h>

using namespace std;
using namespace tlp;

template<typename TYPE>
class NewValueIterator : public tlp::Iterator<uint> {
public:
  NewValueIterator(Iterator<TYPE>* iterator, const MutableContainer<uint>& newValues) : _iterator(iterator), _newValues(&newValues) {
  }

  ~NewValueIterator() {
    delete _iterator;
  }

  virtual uint next() {
    return _newValues->get(_iterator->next().id);
  }

  virtual bool hasNext() {
    return _iterator->hasNext();
  }

private:
  tlp::Iterator<TYPE>* _iterator;
  const MutableContainer<uint>* _newValues;
};

class TlpJsonExport : public ExportModule {
public:
  TlpJsonExport(AlgorithmContext context) : ExportModule(context) {
  }

  virtual bool exportGraph(ostream& fileOut, Graph* graph) {

    std::string comment;
    dataSet->get<string>("comment", comment);

    _writer.writeMapOpen();
    
    _writer.writeString("version");
    _writer.writeString("4.0");

    _writer.writeString("date");
    _writer.writeString(QDate::currentDate().toString(Qt::ISODate).toStdString());

    _writer.writeString("comment");
    _writer.writeString(comment);

    _writer.writeString("graph");
    _writer.writeMapOpen();
    
    node n;
    edge e;
    int i = 0;
    forEach(n, graph->getNodes()) {
      _newNodeId.set(n.id, i++);
    }
    i = 0;
    forEach(e, graph->getEdges()) {
      _newEdgeId.set(e.id, i++);
    }

    saveGraph(graph->getRoot());
    _writer.writeMapClose(); // graph map
    _writer.writeMapClose(); // top-level map

    fileOut << _writer.generatedString();

    return true;
  }

  void saveGraph(Graph* graph) {
    node n;
    edge e;

    _writer.writeString("graphID");
    _writer.writeInteger(graph->getId());

    if(graph == graph->getRoot()) {
      _writer.writeString("nodesNumber");
      _writer.writeInteger(graph->numberOfNodes());
    }
    else {
      writeInterval("nodesIDs", new NewValueIterator<tlp::node>(graph->getNodes(), _newNodeId));
    }

    if(graph == graph->getRoot()) {
      _writer.writeString("edges");
      _writer.writeArrayOpen();
      forEach(e, graph->getEdges()) {
        uint source = _newNodeId.get(graph->source(e).id);
        uint target = _newNodeId.get(graph->target(e).id);

        _writer.writeArrayOpen();
        _writer.writeInteger(source);
        _writer.writeInteger(target);
        _writer.writeArrayClose();
      }
      _writer.writeArrayClose();
    }
    else {
      writeInterval("edgesIDs", new NewValueIterator<tlp::edge>(graph->getEdges(), _newEdgeId));
    }

    _writer.writeString("properties");
    _writer.writeMapOpen();
    //saving properties
    PropertyInterface* property;
    forEach(property, graph->getObjectProperties()) {
      _writer.writeString(property->getName());
      _writer.writeMapOpen();

      _writer.writeString("type");
      _writer.writeString(property->getTypename());

      _writer.writeString("nodeDefault");
      _writer.writeString(property->getNodeDefaultStringValue());

      _writer.writeString("edgeDefault");
      _writer.writeString(property->getEdgeDefaultStringValue());

      Iterator<node>* nodeIt = property->getNonDefaultValuatedNodes();
      Iterator<edge>* edgeIt = property->getNonDefaultValuatedEdges();
      bool hasNonDefaultValuatedNodes = nodeIt->hasNext();
      bool hasNonDefaultValuatedEdges = edgeIt->hasNext();
      delete edgeIt;
      delete nodeIt;

      if(hasNonDefaultValuatedNodes) {
        _writer.writeString("nodes");
        _writer.writeMapOpen();
        forEach(n, property->getNonDefaultValuatedNodes()) {
          stringstream temp;
          temp << _newNodeId.get(n.id);
          _writer.writeString(temp.str());
          _writer.writeString(property->getNodeStringValue(n));
        }
        _writer.writeMapClose();
      }

      if(hasNonDefaultValuatedEdges) {
        _writer.writeString("edges");
        _writer.writeMapOpen();
        forEach(e, property->getNonDefaultValuatedEdges()) {
          stringstream temp;
          temp << _newEdgeId.get(e.id);
          _writer.writeString(temp.str());
          _writer.writeString(property->getEdgeStringValue(e));
        }
        _writer.writeMapClose();
      }

      _writer.writeMapClose();
    }
    _writer.writeMapClose();

    _writer.writeString("attributes");
    _writer.writeMapOpen();
    //saving attributes
    DataSet attributes = graph->getAttributes();
    pair<string, DataType*> attribute;
    forEach(attribute, attributes.getValues()) {
      DataTypeSerializer* serializer = DataSet::typenameToSerializer(attribute.second->getTypeName());
      _writer.writeString(attribute.first);
      _writer.writeArrayOpen();
      _writer.writeString(serializer->outputTypeName);

      stringstream temp;
      serializer->writeData(temp, attribute.second);
      _writer.writeString(temp.str());
      _writer.writeArrayClose();
    }
    _writer.writeMapClose();

    //saving subgraphs
    _writer.writeString("subgraphs");
    _writer.writeMapOpen();
    Graph* sub;
    forEach(sub, graph->getSubGraphs()) {
      saveGraph(sub);
    }
    _writer.writeMapClose();
  }

  void writeInterval(const std::string& intervalName, Iterator<uint>* iterator) {
    _writer.writeString(intervalName);
    _writer.writeArrayOpen();
    uint intervalBegin = UINT_MAX;
    uint intervalEnd = UINT_MAX;
    uint previousId = UINT_MAX;
    uint currentId = UINT_MAX;
    forEach(currentId, iterator) {
      //we don't need/want to do all this on the first time we loop
      if(previousId != UINT_MAX) {

        //if the ID are continuous, define an interval, otherwise witre the IDs (either intervals or single)
        if(currentId == previousId + 1) {
          //if we have no interval being defined, set the lower bound to the previous edge ID
          //if an interval is being defined, set its end to the current edge ID
          if(intervalBegin == UINT_MAX) {
            intervalBegin = previousId;
          }

          intervalEnd = currentId;
        }
        else {
          //if an interval is defined, write it
          if(intervalBegin != UINT_MAX) {
            _writer.writeArrayOpen();
            _writer.writeInteger(intervalBegin);
            _writer.writeInteger(intervalEnd);
            _writer.writeArrayClose();
            intervalBegin = UINT_MAX;
            intervalEnd = UINT_MAX;
          }
          else {
            _writer.writeInteger(previousId);
          }
        }

        if(!_it_foreach._it->hasNext()) {
          if(intervalBegin != UINT_MAX) {
            _writer.writeArrayOpen();
            _writer.writeInteger(intervalBegin);
            _writer.writeInteger(intervalEnd);
            _writer.writeArrayClose();
          }
          else {
            _writer.writeInteger(currentId);
          }
        }
      }

      previousId = currentId;
    }
    _writer.writeArrayClose();
  }

protected:
  YajlWriteFacade _writer;
  MutableContainer<uint> _newNodeId;
  MutableContainer<uint> _newEdgeId;
};

EXPORTPLUGIN(TlpJsonExport,"TlpJsonExport","Charles Huet","18/05/2011","Tulip JSon format","1.0")
