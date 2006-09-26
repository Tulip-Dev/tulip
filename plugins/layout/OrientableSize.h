#ifndef ORIENTABLESIZE_H
#define ORIENTABLESIZE_H

#include <tulip/Size.h>
class OrientableSizeProxy;

class OrientableSize: public tlp::Size {
public:  

    OrientableSize(OrientableSizeProxy* fatherParam, const float width = 0,
                   const float height = 0, const float depth = 0);
    OrientableSize(OrientableSizeProxy* fatherParam, const tlp::Size& size);

    void set(const float width = 0, const float height = 0,
             const float depth = 0);
    void set(const tlp::Size& size);

    void setW(const float width);
    void setH(const float height);
    void setD(const float depth);

    float getW() const;
    float getH() const;
    float getD() const;

    void get(float* width, float* height, float* depth) const;

protected: 
    OrientableSizeProxy* father;
};

#endif 
