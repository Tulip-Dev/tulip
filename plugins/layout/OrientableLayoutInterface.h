#ifndef ORIENTABLELAYOUTINTERFACE_H
#define ORIENTABLELAYOUTINTERFACE_H
#include <vector>
#include <tulip/LayoutProperty.h>
#include "OrientableConstants.h"
#include "OrientableCoord.h"

class OrientableLayoutInterface {
public:

	typedef OrientableCoord PointType;
    typedef std::vector<OrientableCoord> LineType;

    virtual ~OrientableLayoutInterface() {}

    virtual OrientableCoord     createCoord(const float x = 0, const float y = 0,
                                  const float z = 0) = 0;
    virtual OrientableCoord     createCoord(const tlp::Coord& v) = 0;
        
    virtual void      setOrientation(orientationType mask) = 0;        

    virtual void      setAllNodeValue(const PointType& v) = 0;
    virtual void      setAllEdgeValue(const LineType& v) = 0;

    virtual void      setEdgeValue(const tlp::edge e, const LineType& v) = 0;
    virtual void      setNodeValue(tlp::node n, const PointType& v) = 0;

    virtual PointType getNodeValue(const tlp::node n) = 0;
    virtual LineType  getEdgeValue(const tlp::edge e) = 0;

    virtual PointType getNodeDefaultValue() = 0;
    virtual LineType  getEdgeDefaultValue() = 0;
};

#endif
