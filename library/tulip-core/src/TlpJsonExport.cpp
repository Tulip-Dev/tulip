#include <tulip/ForEach.h>
#include <tulip/ExportModule.h>
#include <tulip/Graph.h>
#include <tulip/DataSet.h>
#include <tulip/MutableContainer.h>
#include <tulip/MethodFactory.h>

#include <sstream>

#include <QDate>

using namespace std;
using namespace tlp;

template<typename T, typename U>
inline ostream& serializeKeyValue(ostream& out, T key, U value) {
  out << "\"" << key << "\":\"" << value << "\"";
  return out;
}

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

    stringstream out;

    out << "{";

    serializeKeyValue(out, "version", "4.0") << "," << endl;
    serializeKeyValue(out, "date", QDate::currentDate().toString(Qt::ISODate).toStdString()) << "," << endl;
    serializeKeyValue(out, "comment", comment) << "," << endl;
    out << "\"graph\": {" << endl;

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

    saveGraph(out, graph->getRoot());
    out << "}" << endl;

    out << "}";

    fileOut << out.str();

    return true;
  }

  void saveGraph(ostream& out, Graph* graph) {
    node n;
    edge e;

    out << "\"" << "graphID" << "\":" << graph->getId() << "," << endl;

    if(graph == graph->getRoot()) {
      out << "\"" << "nodesNumber" << "\":" << graph->numberOfNodes() << "," << endl;
    }
    else {
      writeInterval(out, "nodesIDs", new NewValueIterator<tlp::node>(graph->getNodes(), _newNodeId));
    }

    if(graph == graph->getRoot()) {
      out << "\"edges\":[" << endl;
      forEach(e, graph->getEdges()) {
        uint source = _newNodeId.get(graph->source(e).id);
        uint target = _newNodeId.get(graph->target(e).id);
        out << "[" << source << "," << target << "]" << (_it_foreach._it->hasNext() ? "," : "") << endl;
      }
      out << "]," << endl;
    }
    else {
      writeInterval(out, "edgesIDs", new NewValueIterator<tlp::edge>(graph->getEdges(), _newEdgeId));
    }

    out << "\"properties\":{" << endl;
    //saving properties
    PropertyInterface* property;
    forEach(property, graph->getObjectProperties()) {
      out << "\"" << property->getName() << "\":{" << endl;

      serializeKeyValue(out, "type", property->getTypename()) << "," << endl;
      serializeKeyValue(out, "nodeDefault", property->getNodeDefaultStringValue()) << "," << endl;
      serializeKeyValue(out, "edgeDefault", property->getEdgeDefaultStringValue());

      Iterator<node>* nodeIt = property->getNonDefaultValuatedNodes();
      Iterator<edge>* edgeIt = property->getNonDefaultValuatedEdges();
      bool hasNonDefaultValuatedNodes = nodeIt->hasNext();
      bool hasNonDefaultValuatedEdges = edgeIt->hasNext();
      delete edgeIt;
      delete nodeIt;

      if(hasNonDefaultValuatedNodes) {
        out << ",\"nodes\":{";
        forEach(n, property->getNonDefaultValuatedNodes()) {
          serializeKeyValue(out, _newNodeId.get(n.id), property->getNodeStringValue(n)) << (_it_foreach._it->hasNext() ? "," : "") << endl;
        }
        out << "}";
      }

      if(hasNonDefaultValuatedEdges) {
        out << ",\"edges\":{";
        forEach(e, property->getNonDefaultValuatedEdges()) {
          serializeKeyValue(out, _newEdgeId.get(e.id), property->getEdgeStringValue(e)) << (_it_foreach._it->hasNext() ? "," : "") << endl;
        }
        out << "}";
      }

      out << "}" << (_it_foreach._it->hasNext() ? "," : "");
    }
    out << "},";

    out << "\"attributes\":{";
    //saving attributes
    DataSet attributes = graph->getAttributes();
    pair<string, DataType*> attribute;
    forEach(attribute, attributes.getValues()) {
      DataTypeSerializer* serializer = DataSet::typenameToSerializer(attribute.second->getTypeName());
      out << "\"" << attribute.first << "\":";
      out << "[ \"" << serializer->outputTypeName << "\" ,";
      serializer->writeData(out, attribute.second);
      out << "]";
      out << (_it_foreach._it->hasNext() ? "," : "") << endl;
    }
    out << "},";

    //saving subgraphs
    out << "\"subgraphs\":{";
    Graph* sub;
    forEach(sub, graph->getSubGraphs()) {
      saveGraph(out, sub);
      out << (_it_foreach._it->hasNext() ? "," : "") << endl;
    }
    out << "}";
  }

  void writeInterval(ostream& out, const std::string& intervalName, Iterator<uint>* iterator) {
    out << "\"" + intervalName + "\":[" << endl;
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
            out << "[" << intervalBegin << "," << intervalEnd << "]," << endl;
            intervalBegin = UINT_MAX;
            intervalEnd = UINT_MAX;
          }
          else {
            out << previousId << (_it_foreach._it->hasNext() ? "," : "") << endl;
          }
        }

        if(!_it_foreach._it->hasNext()) {
          if(intervalBegin != UINT_MAX) {
            out << "[" << intervalBegin << "," << intervalEnd << "]" << endl;
          }
          else {
            out << currentId << (_it_foreach._it->hasNext() ? "," : "") << endl;
          }
        }
      }

      previousId = currentId;
    }
    out << "]," << endl;
  }

protected:
  MutableContainer<uint> _newNodeId;
  MutableContainer<uint> _newEdgeId;
};

EXPORTPLUGIN(TlpJsonExport,"TlpJsonExport","Charles Huet","18/05/2011","Tulip JSon format","1.0")
