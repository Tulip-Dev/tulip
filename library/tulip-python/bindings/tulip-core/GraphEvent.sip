namespace tlp {

class GraphEvent : tlp::Event {
%TypeHeaderCode
#include <tulip/Graph.h>
%End

%ConvertToSubClassCode
   if (dynamic_cast<tlp::GraphEvent*>(sipCpp)) {
        sipType = sipFindType("tlp::GraphEvent");
   } else {
        sipType = NULL;
    }
%End

  public:

    enum GraphEventType {
      TLP_ADD_NODE = 0,
      TLP_DEL_NODE = 1,
      TLP_ADD_EDGE = 2,
      TLP_DEL_EDGE = 3,
      TLP_REVERSE_EDGE = 4,
      TLP_BEFORE_SET_ENDS = 5,
      TLP_AFTER_SET_ENDS = 6,
      TLP_ADD_NODES = 7,
      TLP_ADD_EDGES = 8,
      TLP_BEFORE_ADD_DESCENDANTGRAPH = 9,
      TLP_AFTER_ADD_DESCENDANTGRAPH = 10,
      TLP_BEFORE_DEL_DESCENDANTGRAPH = 11,
      TLP_AFTER_DEL_DESCENDANTGRAPH = 12,
      TLP_BEFORE_ADD_SUBGRAPH = 13,
      TLP_AFTER_ADD_SUBGRAPH = 14,
      TLP_BEFORE_DEL_SUBGRAPH = 15,
      TLP_AFTER_DEL_SUBGRAPH = 16,
      TLP_ADD_LOCAL_PROPERTY = 17,
      TLP_BEFORE_DEL_LOCAL_PROPERTY = 18,
      TLP_AFTER_DEL_LOCAL_PROPERTY = 19,
      TLP_ADD_INHERITED_PROPERTY = 20,
      TLP_BEFORE_DEL_INHERITED_PROPERTY = 21,
      TLP_AFTER_DEL_INHERITED_PROPERTY = 22,
      TLP_BEFORE_SET_ATTRIBUTE = 23,
      TLP_AFTER_SET_ATTRIBUTE = 24,
      TLP_REMOVE_ATTRIBUTE = 25,
      TLP_BEFORE_ADD_LOCAL_PROPERTY = 26,
      TLP_BEFORE_ADD_INHERITED_PROPERTY = 27
    };

    GraphEvent(const tlp::Graph& g, GraphEventType graphEvtType, unsigned int id,
	       tlp::Event::EventType evtType = tlp::Event::TLP_MODIFICATION);
      
    GraphEvent(const tlp::Graph& g, GraphEventType graphEvtType,
	       const tlp::Graph* sg);

    GraphEvent(const tlp::Graph& g, GraphEventType graphEvtType,
	       const std::string& str,
	       tlp::Event::EventType evtType = tlp::Event::TLP_MODIFICATION);
    
    ~GraphEvent() ;

    tlp::Graph* getGraph() const;

    tlp::node getNode() const ;

    tlp::edge getEdge() const ;

    const tlp::Graph* getSubGraph() const ;

    const std::string& getAttributeName() const ;

    const std::string& getPropertyName() const ;

    GraphEventType getType() const ;
    
};

};
