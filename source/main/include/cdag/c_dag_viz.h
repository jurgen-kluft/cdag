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

        const rgb_t Aliceblue            = 0xf0f8ff;
        const rgb_t Antiquewhite         = 0xfaebd7;
        const rgb_t Aqua                 = 0x00ffff;
        const rgb_t Aquamarine           = 0x7fffd4;
        const rgb_t Azure                = 0xf0ffff;
        const rgb_t Beige                = 0xf5f5dc;
        const rgb_t Bisque               = 0xffe4c4;
        const rgb_t Black                = 0x000000;
        const rgb_t Blanchedalmond       = 0xffebcd;
        const rgb_t Blue                 = 0x0000ff;
        const rgb_t Blueviolet           = 0x8a2be2;
        const rgb_t Brown                = 0xa52a2a;
        const rgb_t Burlywood            = 0xdeb887;
        const rgb_t Cadetblue            = 0x5f9ea0;
        const rgb_t Chartreuse           = 0x7fff00;
        const rgb_t Chocolate            = 0xd2691e;
        const rgb_t Coral                = 0xff7f50;
        const rgb_t Cornflowerblue       = 0x6495ed;
        const rgb_t Cornsilk             = 0xfff8dc;
        const rgb_t Crimson              = 0xdc143c;
        const rgb_t Cyan                 = 0x00ffff;
        const rgb_t Darkblue             = 0x00008b;
        const rgb_t Darkcyan             = 0x008b8b;
        const rgb_t Darkgoldenrod        = 0xb8860b;
        const rgb_t Darkgray             = 0xa9a9a9;
        const rgb_t Darkgreen            = 0x006400;
        const rgb_t Darkgrey             = 0xa9a9a9;
        const rgb_t Darkkhaki            = 0xbdb76b;
        const rgb_t Darkmagenta          = 0x8b008b;
        const rgb_t Darkolivegreen       = 0x556b2f;
        const rgb_t Darkorange           = 0xff8c00;
        const rgb_t Darkorchid           = 0x9932cc;
        const rgb_t Darkred              = 0x8b0000;
        const rgb_t Darksalmon           = 0xe9967a;
        const rgb_t Darkseagreen         = 0x8fbc8f;
        const rgb_t Darkslateblue        = 0x483d8b;
        const rgb_t Darkslategray        = 0x2f4f4f;
        const rgb_t Darkslategrey        = 0x2f4f4f;
        const rgb_t Darkturquoise        = 0x00ced1;
        const rgb_t Darkviolet           = 0x9400d3;
        const rgb_t Deeppink             = 0xff1493;
        const rgb_t Deepskyblue          = 0x00bfff;
        const rgb_t Dimgray              = 0x696969;
        const rgb_t Dimgrey              = 0x696969;
        const rgb_t Dodgerblue           = 0x1e90ff;
        const rgb_t Firebrick            = 0xb22222;
        const rgb_t Floralwhite          = 0xfffaf0;
        const rgb_t Forestgreen          = 0x228b22;
        const rgb_t Fuchsia              = 0xff00ff;
        const rgb_t Gainsboro            = 0xdcdcdc;
        const rgb_t Ghostwhite           = 0xf8f8ff;
        const rgb_t Gold                 = 0xffd700;
        const rgb_t Goldenrod            = 0xdaa520;
        const rgb_t Gray                 = 0x808080;
        const rgb_t Green                = 0x008000;
        const rgb_t Greenyellow          = 0xadff2f;
        const rgb_t Grey                 = 0x808080;
        const rgb_t Honeydew             = 0xf0fff0;
        const rgb_t Hotpink              = 0xff69b4;
        const rgb_t Indianred            = 0xcd5c5c;
        const rgb_t Indigo               = 0x4b0082;
        const rgb_t Ivory                = 0xfffff0;
        const rgb_t Khaki                = 0xf0e68c;
        const rgb_t Lavender             = 0xe6e6fa;
        const rgb_t Lavenderblush        = 0xfff0f5;
        const rgb_t Lawngreen            = 0x7cfc00;
        const rgb_t Lemonchiffon         = 0xfffacd;
        const rgb_t Lightblue            = 0xadd8e6;
        const rgb_t Lightcoral           = 0xf08080;
        const rgb_t Lightcyan            = 0xe0ffff;
        const rgb_t Lightgoldenrodyellow = 0xfafad2;
        const rgb_t Lightgray            = 0xd3d3d3;
        const rgb_t Lightgreen           = 0x90ee90;
        const rgb_t Lightgrey            = 0xd3d3d3;
        const rgb_t Lightpink            = 0xffb6c1;
        const rgb_t Lightsalmon          = 0xffa07a;
        const rgb_t Lightseagreen        = 0x20b2aa;
        const rgb_t Lightskyblue         = 0x87cefa;
        const rgb_t Lightslategray       = 0x778899;
        const rgb_t Lightslategrey       = 0x778899;
        const rgb_t Lightsteelblue       = 0xb0c4de;
        const rgb_t Lightyellow          = 0xffffe0;
        const rgb_t Lime                 = 0x00ff00;
        const rgb_t Limegreen            = 0x32cd32;
        const rgb_t Linen                = 0xfaf0e6;
        const rgb_t Magenta              = 0xff00ff;
        const rgb_t Maroon               = 0x800000;
        const rgb_t Mediumaquamarine     = 0x66cdaa;
        const rgb_t Mediumblue           = 0x0000cd;
        const rgb_t Mediumorchid         = 0xba55d3;
        const rgb_t Mediumpurple         = 0x9370db;
        const rgb_t Mediumseagreen       = 0x3cb371;
        const rgb_t Mediumslateblue      = 0x7b68ee;
        const rgb_t Mediumspringgreen    = 0x00fa9a;
        const rgb_t Mediumturquoise      = 0x48d1cc;
        const rgb_t Mediumvioletred      = 0xc71585;
        const rgb_t Midnightblue         = 0x191970;
        const rgb_t Mintcream            = 0xf5fffa;
        const rgb_t Mistyrose            = 0xffe4e1;
        const rgb_t Moccasin             = 0xffe4b5;
        const rgb_t Navajowhite          = 0xffdead;
        const rgb_t Navy                 = 0x000080;
        const rgb_t Oldlace              = 0xfdf5e6;
        const rgb_t Olive                = 0x808000;
        const rgb_t Olivedrab            = 0x6b8e23;
        const rgb_t Orange               = 0xffa500;
        const rgb_t Orangered            = 0xff4500;
        const rgb_t Orchid               = 0xda70d6;
        const rgb_t Palegoldenrod        = 0xeee8aa;
        const rgb_t Palegreen            = 0x98fb98;
        const rgb_t Paleturquoise        = 0xafeeee;
        const rgb_t Palevioletred        = 0xdb7093;
        const rgb_t Papayawhip           = 0xffefd5;
        const rgb_t Peachpuff            = 0xffdab9;
        const rgb_t Peru                 = 0xcd853f;
        const rgb_t Pink                 = 0xffc0cb;
        const rgb_t Plum                 = 0xdda0dd;
        const rgb_t Powderblue           = 0xb0e0e6;
        const rgb_t Purple               = 0x800080;
        const rgb_t Rebeccapurple        = 0x663399;
        const rgb_t Red                  = 0xff0000;
        const rgb_t Rosybrown            = 0xbc8f8f;
        const rgb_t Royalblue            = 0x4169e1;
        const rgb_t Saddlebrown          = 0x8b4513;
        const rgb_t Salmon               = 0xfa8072;
        const rgb_t Sandybrown           = 0xf4a460;
        const rgb_t Seagreen             = 0x2e8b57;
        const rgb_t Seashell             = 0xfff5ee;
        const rgb_t Sienna               = 0xa0522d;
        const rgb_t Silver               = 0xc0c0c0;
        const rgb_t Skyblue              = 0x87ceeb;
        const rgb_t Slateblue            = 0x6a5acd;
        const rgb_t Slategray            = 0x708090;
        const rgb_t Slategrey            = 0x708090;
        const rgb_t Snow                 = 0xfffafa;
        const rgb_t Springgreen          = 0x00ff7f;
        const rgb_t Steelblue            = 0x4682b4;
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
        const rgb_t Yellowgreen          = 0x9acd32;

        const char* ToString(rgb_t value);
    } // namespace EGraphColor

    namespace EGraphShape
    {
        typedef u32   shape_t;
        const shape_t Rectangle = 0;
        const shape_t Max       = 1;

        const char* ToString(shape_t value);
    } // namespace EGraphShape

    namespace EDagComponents
    {
        const u16 NodeVizComponent = 0;
        const u16 EdgeVizComponent = 1;
    } // namespace EDagComponents

    struct DAGNodeViz
    {
        DCORE_CLASS_PLACEMENT_NEW_DELETE
        EGraphColor::rgb_t   m_NodeColor;
        EGraphColor::rgb_t   m_EdgeColor;
        EGraphShape::shape_t m_Shape;
        char                 m_Label[64];
    };

    struct DAGEdgeViz
    {
        DCORE_CLASS_PLACEMENT_NEW_DELETE
        EGraphColor::rgb_t m_EdgeColor;
        char               m_Label[64];
    };

    bool Export(DirectedAcyclicGraph const& dag, alloc_t* stack, alloc_t* heap, void*& outData, u32& outSize);

} // namespace ncore

#endif
