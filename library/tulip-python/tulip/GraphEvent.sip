namespace tlp {

class GraphEvent : tlp::Event {
%TypeHeaderCode
#include <tulip/Graph.h>
%End

%ConvertToSubClassCode
   if (dynamic_cast<tlp::GraphEvent*>(sipCpp)) {
        sipType = sipType_tlp_GraphEvent;
   } else {
        sipType = NULL;
    }
%End

  public:

    enum GraphEventType {
      TLP_ADD_NODE = 0, TLP_DEL_NODE, TLP_ADD_EDGE, TLP_DEL_EDGE,
      TLP_REVERSE_EDGE, TLP_BEFORE_SET_ENDS, TLP_AFTER_SET_ENDS,
      TLP_ADD_SUBGRAPH, TLP_DEL_SUBGRAPH,
      TLP_ADD_LOCAL_PROPERTY, TLP_DEL_LOCAL_PROPERTY,
      TLP_BEFORE_SET_ATTRIBUTE, TLP_AFTER_SET_ATTRIBUTE, TLP_REMOVE_ATTRIBUTE
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

class DummyEvent : tlp::Event {
%TypeHeaderCode
#include <tulip/Graph.h>
%End
public:

	DummyEvent(const tlp::Observable &sender, tlp::Event::EventType type, const std::string &msg);

	std::string getMessage() const;

};

};