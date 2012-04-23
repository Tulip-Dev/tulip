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
namespace tlp {

    class NodeShape {

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
            Window=17
        };
    };

    class EdgeShape {

    public:

        enum EdgeShapes {
            Polyline=0,
            BezierCurve=4,
            CatmullRomCurve=8,
            CubicBSplineCurve=16
        };
    };

    class EdgeExtrimityShape {

    public:

        enum EdgeExtremityShapes {
            None=-1,
            Arrow=50,
            ChristmasTree=28,
            Circle=14,
            Cone=3,
            Cross=8,
            Cube=0,
            Cylinder=6,
            Diamond=5,
            GlowSphere=16,
            Hexagon=13,
            Pentagon=12,
            Ring=15,
            Sphere=2 ,
            Square=4,
            Triangle=11
        };
    };

    class LabelPosition {

    public:

        enum LabelPositions {
            Center=0,
            Top,
            Bottom,
            Left,
            Right
        };
    };
}
%End

namespace tlp {

    class NodeShape /NoDefaultCtors/ {

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
            Window=17
        };
    };

    class EdgeShape /NoDefaultCtors/ {

    public:

        enum EdgeShapes {
            Polyline=0,
            BezierCurve=4,
            CatmullRomCurve=8,
            CubicBSplineCurve=16
        };
    };

    class EdgeExtrimityShape /NoDefaultCtors/ {

    public:

        enum EdgeExtremityShapes {
            None=-1,
            Arrow=50,
            ChristmasTree=28,
            Circle=14,
            Cone=3,
            Cross=8,
            Cube=0,
            Cylinder=6,
            Diamond=5,
            GlowSphere=16,
            Hexagon=13,
            Pentagon=12,
            Ring=15,
            Sphere=2 ,
            Square=4,
            Triangle=11
        };
    };

    class LabelPosition /NoDefaultCtors/ {

    public:

        enum LabelPositions {
            Center=0,
            Top,
            Bottom,
            Left,
            Right
        };
    };
};
