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

%ModuleHeaderCode
#include <tulip/TulipViewSettings.h>
%End

namespace tlp {

class NodeShape /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/TulipViewSettings.h>
%End

public:

    enum NodeShapes {
        Billboard=7,
        ChristmasTree=28,
        Circle=14,
        Cone=3,
        Cross=8,
        Cube=0,
        CubeOutlined=1,
        CubeOutlinedTransparent=9,
        Cylinder=6,
        Diamond=5,
        GlowSphere=16,
        HalfCylinder=10,
        Hexagon=13,
        Pentagon=12,
        Ring=15,
        RoundedBox=18,
        Sphere=2 ,
        Square=4,
        Triangle=11,
        Window=17,
        Star=19
    };
};

class EdgeShape /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/TulipViewSettings.h>
%End

public:

    enum EdgeShapes {
        Polyline=0,
        BezierCurve=4,
        CatmullRomCurve=8,
        CubicBSplineCurve=16
    };
};

class EdgeExtremityShape /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/TulipViewSettings.h>
%End

public:

    enum EdgeExtremityShapes {
        None=-1,
        Arrow=50,
        Circle=14,
        Cone=3,
        Cross=8,
        Cube=0,
        CubeOutlinedTransparent=9,
        Cylinder=6,
        Diamond=5,
        GlowSphere=16,
        Hexagon=13,
        Pentagon=12,
        Ring=15,
        Sphere=2 ,
        Square=4,
        Star=19
    };
};

class LabelPosition /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/TulipViewSettings.h>
%End

public:

    enum LabelPositions {
        Center=0,
        Top,
        Bottom,
        Left,
        Right
    };
};

class TulipViewSettings : tlp::Observable {
%TypeHeaderCode
#include <tulip/TulipViewSettings.h>
%End

public:

    static tlp::TulipViewSettings &instance();

    tlp::Color defaultColor(tlp::ElementType elem) const;

    void setDefaultColor(tlp::ElementType elem, const tlp::Color& color);

    tlp::Color defaultBorderColor(tlp::ElementType elem) const;

    void setDefaultBorderColor(tlp::ElementType elem, const tlp::Color& color);

    float defaultBorderWidth(tlp::ElementType elem) const;

    void setdefaultBorderWidth(tlp::ElementType elem, float borderWidth);

    tlp::Color defaultLabelColor() const;

    void setDefaultLabelColor(const tlp::Color& color);

    tlp::Color defaultLabelBorderColor() const;

    void setDefaultLabelBorderColor(const tlp::Color& color);

    float defaultLabelBorderWidth() const;

    void setDefaultLabelBorderWidth(float borderWidth);

    int defaultLabelPosition() const;

    void setDefaultLabelPosition(int position);

    tlp::Size defaultSize(tlp::ElementType elem) const;

    void setDefaultSize(tlp::ElementType elem, const tlp::Size& size);

    int defaultShape(tlp::ElementType elem) const;

    void setDefaultShape(tlp::ElementType elem, int shape);

    int defaultEdgeExtremitySrcShape() const;

    void setDefaultEdgeExtremitySrcShape(int shape);

    int defaultEdgeExtremityTgtShape() const;

    void setDefaultEdgeExtremityTgtShape(int shape);

    tlp::Size defaultEdgeExtremitySrcSize() const;

    void setDefaultEdgeExtremitySrcSize(const tlp::Size &size);

    tlp::Size defaultEdgeExtremityTgtSize() const;

    void setDefaultEdgeExtremityTgtSize(const tlp::Size &size);

    std::string defaultFontFile() const;

    void setDefaultFontFile(const std::string &fontFile);

    int defaultFontSize() const;

    void setDefaultFontSize(int fontSize);

private:

    TulipViewSettings();
};

class ViewSettingsEvent : public tlp::Event /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/TulipViewSettings.h>
%End

%ConvertToSubClassCode
   if (dynamic_cast<tlp::ViewSettingsEvent*>(sipCpp)) {
        sipType = sipFindType("tlp::ViewSettingsEvent");
   } else {
        sipType = NULL;
   }
%End

public:

    enum ViewSettingsEventType {
        TLP_DEFAULT_COLOR_MODIFIED,
        TLP_DEFAULT_SHAPE_MODIFIED,
        TLP_DEFAULT_SIZE_MODIFIED,
        TLP_DEFAULT_LABEL_COLOR_MODIFIED
    };

    ViewSettingsEventType getType() const;

    tlp::ElementType getElementType() const;

    tlp::Color getColor() const;

    tlp::Size getSize() const;

    int getShape() const;

};
};
