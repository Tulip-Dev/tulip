#ifndef ORIENTABLESIZEPROXY_H
#define ORIENTABLESIZEPROXY_H

#include <vector>
#include <tulip/Sizes.h>
#include "OrientableConstants.h"
#include "OrientableSize.h"

class OrientableSizeProxy {
    friend class OrientableSize;

public:    
    typedef OrientableSize PointType;
    typedef OrientableSize LineType;

    OrientableSizeProxy(Sizes* sizesProxy,
                        orientationType mask = ORI_DEFAULT);

    OrientableSize createSize(const float width = 0, const float height = 0,
                              const float depth = 0);
    OrientableSize createSize(const Size& v);


    void           setOrientation(orientationType mask);        

    void           setAllNodeValue(const PointType& v);
    void           setAllEdgeValue(const LineType& v);

    void           setEdgeValue(const edge e, const LineType& v);
    void           setNodeValue(node n, const PointType& v);

    PointType      getNodeValue(const node n);
    LineType       getEdgeValue(const edge e);

    PointType      getNodeDefaultValue();
    LineType       getEdgeDefaultValue();

private:    
    Sizes*     sizesProxy;
    orientationType orientation;

    float (Size::*readW)() const;
    float (Size::*readH)() const;
    float (Size::*readD)() const;
    void  (Size::*writeW)(float);
    void  (Size::*writeH)(float);
    void  (Size::*writeD)(float);
};


#endif



