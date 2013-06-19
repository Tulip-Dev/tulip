namespace tlp {

class PropertyEvent : tlp::Event {
%TypeHeaderCode
#include <tulip/PropertyInterface.h>
%End

%ConvertToSubClassCode
   if (dynamic_cast<tlp::PropertyEvent*>(sipCpp)) {
        sipType = sipFindType("tlp::PropertyEvent");
   } else {
        sipType = NULL;
    }
%End

public:

  enum PropertyEventType {TLP_BEFORE_SET_NODE_VALUE = 0,
                          TLP_AFTER_SET_NODE_VALUE,
                          TLP_BEFORE_SET_ALL_NODE_VALUE,
                          TLP_AFTER_SET_ALL_NODE_VALUE,
                          TLP_BEFORE_SET_ALL_EDGE_VALUE,
                          TLP_AFTER_SET_ALL_EDGE_VALUE,
                          TLP_BEFORE_SET_EDGE_VALUE,
                          TLP_AFTER_SET_EDGE_VALUE
                         };

  PropertyEvent(const tlp::PropertyInterface& prop, PropertyEventType propEvtType,
                tlp::Event::EventType evtType = tlp::Event::TLP_MODIFICATION,
                unsigned int id = UINT_MAX);

  tlp::PropertyInterface* getProperty() const;

  tlp::node getNode() const;

  tlp::edge getEdge() const;

  PropertyEventType getType() const;

};

};
