#ifndef __CDAG_DAG_VIZ_H__
#define __CDAG_DAG_VIZ_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "cdag/c_dag.h"

namespace ncore
{
    namespace EGraphColor
    {
        typedef u32 rgb_t;

        const rgb_t AliceBlue            = 0xf0f8ff;
        const rgb_t AntiqueWhite         = 0xfaebd7;
        const rgb_t Aqua                 = 0x00ffff;
        const rgb_t AquaMarine           = 0x7fffd4;
        const rgb_t Azure                = 0xf0ffff;
        const rgb_t Beige                = 0xf5f5dc;
        const rgb_t Bisque               = 0xffe4c4;
        const rgb_t Black                = 0x000000;
        const rgb_t BlanchedAlmond       = 0xffebcd;
        const rgb_t Blue                 = 0x0000ff;
        const rgb_t BlueViolet           = 0x8a2be2;
        const rgb_t Brown                = 0xa52a2a;
        const rgb_t Burlywood            = 0xdeb887;
        const rgb_t CadetBlue            = 0x5f9ea0;
        const rgb_t Chartreuse           = 0x7fff00;
        const rgb_t Chocolate            = 0xd2691e;
        const rgb_t Coral                = 0xff7f50;
        const rgb_t CornFlowerBlue       = 0x6495ed;
        const rgb_t Cornsilk             = 0xfff8dc;
        const rgb_t Crimson              = 0xdc143c;
        const rgb_t Cyan                 = 0x00ffff;
        const rgb_t DarkBlue             = 0x00008b;
        const rgb_t DarkCyan             = 0x008b8b;
        const rgb_t DarkGoldenRod        = 0xb8860b;
        const rgb_t DarkGray             = 0xa9a9a9;
        const rgb_t DarkGreen            = 0x006400;
        const rgb_t DarkGrey             = 0xa9a9a9;
        const rgb_t DarkKhaki            = 0xbdb76b;
        const rgb_t DarkMagenta          = 0x8b008b;
        const rgb_t DarkOliveGreen       = 0x556b2f;
        const rgb_t DarkOrange           = 0xff8c00;
        const rgb_t DarkOrchid           = 0x9932cc;
        const rgb_t DarkRed              = 0x8b0000;
        const rgb_t DarkSalmon           = 0xe9967a;
        const rgb_t DarkSeaGreen         = 0x8fbc8f;
        const rgb_t DarkSlateBlue        = 0x483d8b;
        const rgb_t DarkSlateGray        = 0x2f4f4f;
        const rgb_t DarkSlateGrey        = 0x2f4f4f;
        const rgb_t DarkTurquoise        = 0x00ced1;
        const rgb_t DarkViolet           = 0x9400d3;
        const rgb_t DeepPink             = 0xff1493;
        const rgb_t DeepSkyBlue          = 0x00bfff;
        const rgb_t DimGray              = 0x696969;
        const rgb_t DimGrey              = 0x696969;
        const rgb_t DodgerBlue           = 0x1e90ff;
        const rgb_t Firebrick            = 0xb22222;
        const rgb_t FloralWhite          = 0xfffaf0;
        const rgb_t ForestGreen          = 0x228b22;
        const rgb_t Fuchsia              = 0xff00ff;
        const rgb_t Gainsboro            = 0xdcdcdc;
        const rgb_t GhostWhite           = 0xf8f8ff;
        const rgb_t Gold                 = 0xffd700;
        const rgb_t GoldenRod            = 0xdaa520;
        const rgb_t Gray                 = 0x808080;
        const rgb_t Green                = 0x008000;
        const rgb_t GreenYellow          = 0xadff2f;
        const rgb_t Grey                 = 0x808080;
        const rgb_t Honeydew             = 0xf0fff0;
        const rgb_t HotPink              = 0xff69b4;
        const rgb_t IndianRed            = 0xcd5c5c;
        const rgb_t Indigo               = 0x4b0082;
        const rgb_t Ivory                = 0xfffff0;
        const rgb_t Khaki                = 0xf0e68c;
        const rgb_t Lavender             = 0xe6e6fa;
        const rgb_t LavenderBlush        = 0xfff0f5;
        const rgb_t LawnGreen            = 0x7cfc00;
        const rgb_t Lemonchiffon         = 0xfffacd;
        const rgb_t LightBlue            = 0xadd8e6;
        const rgb_t LightCoral           = 0xf08080;
        const rgb_t LightCyan            = 0xe0ffff;
        const rgb_t LightGoldenRodYellow = 0xfafad2;
        const rgb_t LightGray            = 0xd3d3d3;
        const rgb_t LightGreen           = 0x90ee90;
        const rgb_t LightGrey            = 0xd3d3d3;
        const rgb_t LightPink            = 0xffb6c1;
        const rgb_t LightSalmon          = 0xffa07a;
        const rgb_t LightSeaGreen        = 0x20b2aa;
        const rgb_t LightSkyBlue         = 0x87cefa;
        const rgb_t LightSlateGray       = 0x778899;
        const rgb_t LightSlateGrey       = 0x778899;
        const rgb_t LightSteelBlue       = 0xb0c4de;
        const rgb_t LightYellow          = 0xffffe0;
        const rgb_t Lime                 = 0x00ff00;
        const rgb_t LimeGreen            = 0x32cd32;
        const rgb_t Linen                = 0xfaf0e6;
        const rgb_t Magenta              = 0xff00ff;
        const rgb_t Maroon               = 0x800000;
        const rgb_t MediumAquamarine     = 0x66cdaa;
        const rgb_t MediumBlue           = 0x0000cd;
        const rgb_t MediumOrchid         = 0xba55d3;
        const rgb_t MediumPurple         = 0x9370db;
        const rgb_t MediumSeaGreen       = 0x3cb371;
        const rgb_t MediumSlateBlue      = 0x7b68ee;
        const rgb_t MediumSpringGreen    = 0x00fa9a;
        const rgb_t MediumTurquoise      = 0x48d1cc;
        const rgb_t MediumVioletRed      = 0xc71585;
        const rgb_t MidnightBlue         = 0x191970;
        const rgb_t Mintcream            = 0xf5fffa;
        const rgb_t Mistyrose            = 0xffe4e1;
        const rgb_t Moccasin             = 0xffe4b5;
        const rgb_t NavajoWhite          = 0xffdead;
        const rgb_t Navy                 = 0x000080;
        const rgb_t Oldlace              = 0xfdf5e6;
        const rgb_t Olive                = 0x808000;
        const rgb_t Olivedrab            = 0x6b8e23;
        const rgb_t Orange               = 0xffa500;
        const rgb_t OrangeRed            = 0xff4500;
        const rgb_t Orchid               = 0xda70d6;
        const rgb_t PalegoldenRod        = 0xeee8aa;
        const rgb_t PaleGreen            = 0x98fb98;
        const rgb_t Paleturquoise        = 0xafeeee;
        const rgb_t PalevioletRed        = 0xdb7093;
        const rgb_t Papayawhip           = 0xffefd5;
        const rgb_t Peachpuff            = 0xffdab9;
        const rgb_t Peru                 = 0xcd853f;
        const rgb_t Pink                 = 0xffc0cb;
        const rgb_t Plum                 = 0xdda0dd;
        const rgb_t PowderBlue           = 0xb0e0e6;
        const rgb_t Purple               = 0x800080;
        const rgb_t RebeccaPurple        = 0x663399;
        const rgb_t Red                  = 0xff0000;
        const rgb_t RosyBrown            = 0xbc8f8f;
        const rgb_t RoyalBlue            = 0x4169e1;
        const rgb_t SaddleBrown          = 0x8b4513;
        const rgb_t Salmon               = 0xfa8072;
        const rgb_t SandyBrown           = 0xf4a460;
        const rgb_t SeaGreen             = 0x2e8b57;
        const rgb_t Seashell             = 0xfff5ee;
        const rgb_t Sienna               = 0xa0522d;
        const rgb_t Silver               = 0xc0c0c0;
        const rgb_t SkyBlue              = 0x87ceeb;
        const rgb_t SlateBlue            = 0x6a5acd;
        const rgb_t SlateGray            = 0x708090;
        const rgb_t SlateGrey            = 0x708090;
        const rgb_t Snow                 = 0xfffafa;
        const rgb_t SpringGreen          = 0x00ff7f;
        const rgb_t SteelBlue            = 0x4682b4;
        const rgb_t Tan                  = 0xd2b48c;
        const rgb_t Teal                 = 0x008080;
        const rgb_t Thistle              = 0xd8bfd8;
        const rgb_t Tomato               = 0xff6347;
        const rgb_t Turquoise            = 0x40e0d0;
        const rgb_t Violet               = 0xee82ee;
        const rgb_t Wheat                = 0xf5deb3;
        const rgb_t White                = 0xffffff;
        const rgb_t Whitesmoke           = 0xf5f5f5;
        const rgb_t Yellow               = 0xffff00;
        const rgb_t YellowGreen          = 0x9acd32;

        const char* ToString(rgb_t value);
    } // namespace EGraphColor

    namespace EGraphShape
    {
        typedef u32   shape_t;

        const shape_t ShapeBox       = 0;
        const shape_t ShapePolygon   = 1;
        const shape_t ShapeEllipse   = 2;
        const shape_t ShapeOval      = 3;
        const shape_t ShapeCircle    = 4;
        const shape_t ShapePoint     = 5;
        const shape_t ShapeEgg       = 6;
        const shape_t ShapeTriangle  = 7;
        const shape_t ShapeNone      = 8;
        const shape_t ShapeDiamond   = 9;
        const shape_t ShapeRectangle = 10;
        const shape_t ShapeSquare    = 11;
        const shape_t Max            = 12;

        const char* ToString(shape_t value);

        typedef u32 attribute_t;

        const attribute_t AttributeSolid     = 1;
        const attribute_t AttributeDashed    = 2;
        const attribute_t AttributeDotted    = 3;
        const attribute_t AttributeBold      = 4;
        const attribute_t AttributeRounded   = 5;
        const attribute_t AttributeFilled    = 6;
        const attribute_t AttributeStriped   = 7;

    } // namespace EGraphShape

    namespace EDagComponents
    {
        const u16 NodeVizComponent = 1;
        const u16 EdgeVizComponent = 2;
    } // namespace EDagComponents

    struct DAGNodeViz
    {
        D_DECLARE_COMPONENT_TYPE(EDagComponents::NodeVizComponent);
        DCORE_CLASS_PLACEMENT_NEW_DELETE
        EGraphColor::rgb_t   m_Color;
        EGraphShape::shape_t m_Shape;
        char                 m_Label[64];
    };

    struct DAGEdgeViz
    {
        D_DECLARE_COMPONENT_TYPE(EDagComponents::EdgeVizComponent);
        DCORE_CLASS_PLACEMENT_NEW_DELETE
        EGraphColor::rgb_t m_EdgeColor;
        char               m_Label[64];
    };

    void SetLabel(DAGNodeViz* viz, char const* label);
    void SetLabel(DAGEdgeViz* viz, char const* label);

    bool Export(DirectedAcyclicGraph const& dag, alloc_t* stack, alloc_t* heap, byte*& outData, u32& outSize);

} // namespace ncore

#endif
