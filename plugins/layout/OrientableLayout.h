#ifndef ORIENTABLELAYOUT_H
#define ORIENTABLELAYOUT_H

#include "OrientableLayoutInterface.h"

class OrientableLayout : public OrientableLayoutInterface {
    friend class OrientableCoord;
public:

    OrientableLayout(LayoutProxy* layout, orientationType mask = ORI_DEFAULT);
    
    OrientableCoord createCoord(const float x = 0, const float y = 0,
                                const float z = 0);
    OrientableCoord createCoord(const Coord& v);
        
    void            setOrientation(orientationType mask);        

    void            setAllNodeValue(const PointType& v);
    void            setAllEdgeValue(const LineType& v);

    void            setEdgeValue(const edge e, const LineType& v);
    void            setNodeValue(node n, const PointType& v);

    PointType       getNodeValue(const node n);
    LineType        getEdgeValue(const edge e);

    PointType       getNodeDefaultValue();
    LineType        getEdgeDefaultValue();

private:
	typedef std::vector<Coord> CoordLineType;
	std::vector<OrientableCoord> convertEdgeLinetype(const std::vector<Coord>& v);
	
	LayoutProxy*    layout;
	orientationType orientation;
	
	float (OrientableCoord::*readX)() const;
	float (OrientableCoord::*readY)() const;
	float (OrientableCoord::*readZ)() const;
	void  (OrientableCoord::*writeX)(float);
	void  (OrientableCoord::*writeY)(float);
	void  (OrientableCoord::*writeZ)(float);
};

#endif
