
#ifndef __RGB565_H__
#define __RGB565_H__

#include "../vector/vec3f.h"

#define PGLM_RGB565_ALICEBLUE            ((u16)0xEFBFu)
#define PGLM_RGB565_ANTIQUEWHITE         ((u16)0xF75Au)
#define PGLM_RGB565_AQUA                 ((u16)0x07FFu)
#define PGLM_RGB565_AQUAMARINE           ((u16)0x7FFAu)
#define PGLM_RGB565_AZURE                ((u16)0xEFFFu)
#define PGLM_RGB565_BEIGE                ((u16)0xF7BBu)
#define PGLM_RGB565_BISQUE               ((u16)0xFF18u)
#define PGLM_RGB565_BLACK                ((u16)0x0000u)
#define PGLM_RGB565_BLANCHEDALMOND       ((u16)0xFF59u)
#define PGLM_RGB565_BLUE                 ((u16)0x001Fu)
#define PGLM_RGB565_BLUEVIOLET           ((u16)0x897Bu)
#define PGLM_RGB565_BROWN                ((u16)0xA145u)
#define PGLM_RGB565_BURLYWOOD            ((u16)0xDDB0u)
#define PGLM_RGB565_CADETBLUE            ((u16)0x64F3u)
#define PGLM_RGB565_CHARTREUSE           ((u16)0x7FE0u)
#define PGLM_RGB565_CHOCOLATE            ((u16)0xD344u)
#define PGLM_RGB565_CORAL                ((u16)0xFBEAu)
#define PGLM_RGB565_CORNFLOWERBLUE       ((u16)0x64BDu)
#define PGLM_RGB565_CORNSILK             ((u16)0xFFBBu)
#define PGLM_RGB565_CRIMSON              ((u16)0xD8A7u)
#define PGLM_RGB565_CYAN                 ((u16)0x07FFu)
#define PGLM_RGB565_DARKBLUE             ((u16)0x0011u)
#define PGLM_RGB565_DARKCYAN             ((u16)0x0451u)
#define PGLM_RGB565_DARKGOLDENROD        ((u16)0xB421u)
#define PGLM_RGB565_DARKGRAY             ((u16)0xAD55u)
#define PGLM_RGB565_DARKGREEN            ((u16)0x0320u)
#define PGLM_RGB565_DARKGREY             ((u16)0xAD55u)
#define PGLM_RGB565_DARKKHAKI            ((u16)0xBDADu)
#define PGLM_RGB565_DARKMAGENTA          ((u16)0x8811u)
#define PGLM_RGB565_DARKOLIVEGREEN       ((u16)0x5346u)
#define PGLM_RGB565_DARKORANGE           ((u16)0xFC60u)
#define PGLM_RGB565_DARKORCHID           ((u16)0x9999u)
#define PGLM_RGB565_DARKRED              ((u16)0x8800u)
#define PGLM_RGB565_DARKSALMON           ((u16)0xE4AFu)
#define PGLM_RGB565_DARKSEAGREEN         ((u16)0x8DD1u)
#define PGLM_RGB565_DARKSLATEBLUE        ((u16)0x49F1u)
#define PGLM_RGB565_DARKSLATEGRAY        ((u16)0x328Au)
#define PGLM_RGB565_DARKSLATEGREY        ((u16)0x328Au)
#define PGLM_RGB565_DARKTURQUOISE        ((u16)0x0679u)
#define PGLM_RGB565_DARKVIOLET           ((u16)0x901Au)
#define PGLM_RGB565_DEEPPINK             ((u16)0xF8B2u)
#define PGLM_RGB565_DEEPSKYBLUE          ((u16)0x05FFu)
#define PGLM_RGB565_DIMGRAY              ((u16)0x6B4Du)
#define PGLM_RGB565_DIMGREY              ((u16)0x6B4Du)
#define PGLM_RGB565_DODGERBLUE           ((u16)0x249Fu)
#define PGLM_RGB565_FIREBRICK            ((u16)0xB104u)
#define PGLM_RGB565_FLORALWHITE          ((u16)0xFFDDu)
#define PGLM_RGB565_FORESTGREEN          ((u16)0x2444u)
#define PGLM_RGB565_FUCHSIA              ((u16)0xF81Fu)
#define PGLM_RGB565_GAINSBORO            ((u16)0xDEDBu)
#define PGLM_RGB565_GHOSTWHITE           ((u16)0xF7BFu)
#define PGLM_RGB565_GOLD                 ((u16)0xFEA0u)
#define PGLM_RGB565_GOLDENROD            ((u16)0xDD24u)
#define PGLM_RGB565_GRAY                 ((u16)0x8410u)
#define PGLM_RGB565_GREEN                ((u16)0x0400u)
#define PGLM_RGB565_GREENYELLOW          ((u16)0xAFE6u)
#define PGLM_RGB565_GREY                 ((u16)0x8410u)
#define PGLM_RGB565_HONEYDEW             ((u16)0xEFFDu)
#define PGLM_RGB565_HOTPINK              ((u16)0xFB56u)
#define PGLM_RGB565_INDIANRED            ((u16)0xCAEBu)
#define PGLM_RGB565_INDIGO               ((u16)0x4810u)
#define PGLM_RGB565_IVORY                ((u16)0xFFFDu)
#define PGLM_RGB565_KHAKI                ((u16)0xEF31u)
#define PGLM_RGB565_LAVENDER             ((u16)0xE73Eu)
#define PGLM_RGB565_LAVENDERBLUSH        ((u16)0xFF7Eu)
#define PGLM_RGB565_LAWNGREEN            ((u16)0x7FC0u)
#define PGLM_RGB565_LEMONCHIFFON         ((u16)0xFFD9u)
#define PGLM_RGB565_LIGHTBLUE            ((u16)0xAEBCu)
#define PGLM_RGB565_LIGHTCORAL           ((u16)0xEC10u)
#define PGLM_RGB565_LIGHTCYAN            ((u16)0xDFFFu)
#define PGLM_RGB565_LIGHTGOLDENRODYELLOW ((u16)0xF7DAu)
#define PGLM_RGB565_LIGHTGRAY            ((u16)0xD69Au)
#define PGLM_RGB565_LIGHTGREEN           ((u16)0x9772u)
#define PGLM_RGB565_LIGHTGREY            ((u16)0xD69Au)
#define PGLM_RGB565_LIGHTPINK            ((u16)0xFDB7u)
#define PGLM_RGB565_LIGHTSALMON          ((u16)0xFD0Fu)
#define PGLM_RGB565_LIGHTSEAGREEN        ((u16)0x2595u)
#define PGLM_RGB565_LIGHTSKYBLUE         ((u16)0x867Eu)
#define PGLM_RGB565_LIGHTSLATEGRAY       ((u16)0x7453u)
#define PGLM_RGB565_LIGHTSLATEGREY       ((u16)0x7453u)
#define PGLM_RGB565_LIGHTSTEELBLUE       ((u16)0xAE1Bu)
#define PGLM_RGB565_LIGHTYELLOW          ((u16)0xFFFBu)
#define PGLM_RGB565_LIME                 ((u16)0x07E0u)
#define PGLM_RGB565_LIMEGREEN            ((u16)0x3666u)
#define PGLM_RGB565_LINEN                ((u16)0xF77Cu)
#define PGLM_RGB565_MAGENTA              ((u16)0xF81Fu)
#define PGLM_RGB565_MAROON               ((u16)0x8000u)
#define PGLM_RGB565_MEDIUMAQUAMARINE     ((u16)0x6675u)
#define PGLM_RGB565_MEDIUMBLUE           ((u16)0x0019u)
#define PGLM_RGB565_MEDIUMORCHID         ((u16)0xBABAu)
#define PGLM_RGB565_MEDIUMPURPLE         ((u16)0x939Bu)
#define PGLM_RGB565_MEDIUMSEAGREEN       ((u16)0x3D8Eu)
#define PGLM_RGB565_MEDIUMSLATEBLUE      ((u16)0x7B5Du)
#define PGLM_RGB565_MEDIUMSPRINGGREEN    ((u16)0x07D3u)
#define PGLM_RGB565_MEDIUMTURQUOISE      ((u16)0x4E99u)
#define PGLM_RGB565_MEDIUMVIOLETRED      ((u16)0xC0B0u)
#define PGLM_RGB565_MIDNIGHTBLUE         ((u16)0x18CEu)
#define PGLM_RGB565_MINTCREAM            ((u16)0xF7FEu)
#define PGLM_RGB565_MISTYROSE            ((u16)0xFF1Bu)
#define PGLM_RGB565_MOCCASIN             ((u16)0xFF16u)
#define PGLM_RGB565_NAVAJOWHITE          ((u16)0xFEF5u)
#define PGLM_RGB565_NAVY                 ((u16)0x0010u)
#define PGLM_RGB565_OLDLACE              ((u16)0xFFBCu)
#define PGLM_RGB565_OLIVE                ((u16)0x8400u)
#define PGLM_RGB565_OLIVEDRAB            ((u16)0x6C64u)
#define PGLM_RGB565_ORANGE               ((u16)0xFD20u)
#define PGLM_RGB565_ORANGERED            ((u16)0xFA20u)
#define PGLM_RGB565_ORCHID               ((u16)0xDB9Au)
#define PGLM_RGB565_PALEGOLDENROD        ((u16)0xEF35u)
#define PGLM_RGB565_PALEGREEN            ((u16)0x97D2u)
#define PGLM_RGB565_PALETURQUOISE        ((u16)0xAF7Du)
#define PGLM_RGB565_PALEVIOLETRED        ((u16)0xDB92u)
#define PGLM_RGB565_PAPAYAWHIP           ((u16)0xFF7Au)
#define PGLM_RGB565_PEACHPUFF            ((u16)0xFED6u)
#define PGLM_RGB565_PERU                 ((u16)0xCC28u)
#define PGLM_RGB565_PINK                 ((u16)0xFDF9u)
#define PGLM_RGB565_PLUM                 ((u16)0xDD1Bu)
#define PGLM_RGB565_POWDERBLUE           ((u16)0xAEFCu)
#define PGLM_RGB565_PURPLE               ((u16)0x8010u)
#define PGLM_RGB565_RED                  ((u16)0xF800u)
#define PGLM_RGB565_ROSYBROWN            ((u16)0xBC71u)
#define PGLM_RGB565_ROYALBLUE            ((u16)0x435Bu)
#define PGLM_RGB565_SADDLEBROWN          ((u16)0x8A22u)
#define PGLM_RGB565_SALMON               ((u16)0xF40Eu)
#define PGLM_RGB565_SANDYBROWN           ((u16)0xF52Cu)
#define PGLM_RGB565_SEAGREEN             ((u16)0x344Bu)
#define PGLM_RGB565_SEASHELL             ((u16)0xFFBDu)
#define PGLM_RGB565_SIENNA               ((u16)0x9A85u)
#define PGLM_RGB565_SILVER               ((u16)0xBDF7u)
#define PGLM_RGB565_SKYBLUE              ((u16)0x867Du)
#define PGLM_RGB565_SLATEBLUE            ((u16)0x6AD9u)
#define PGLM_RGB565_SLATEGRAY            ((u16)0x7412u)
#define PGLM_RGB565_SLATEGREY            ((u16)0x7412u)
#define PGLM_RGB565_SNOW                 ((u16)0xFFDEu)
#define PGLM_RGB565_SPRINGGREEN          ((u16)0x07EFu)
#define PGLM_RGB565_STEELBLUE            ((u16)0x4C16u)
#define PGLM_RGB565_TAN                  ((u16)0xD591u)
#define PGLM_RGB565_TEAL                 ((u16)0x0410u)
#define PGLM_RGB565_THISTLE              ((u16)0xD5FAu)
#define PGLM_RGB565_TOMATO               ((u16)0xFB09u)
#define PGLM_RGB565_TURQUOISE            ((u16)0x46F9u)
#define PGLM_RGB565_VIOLET               ((u16)0xEC1Du)
#define PGLM_RGB565_WHEAT                ((u16)0xF6F6u)
#define PGLM_RGB565_WHITE                ((u16)0xFFFFu)
#define PGLM_RGB565_WHITESMOKE           ((u16)0xF7BEu)
#define PGLM_RGB565_YELLOW               ((u16)0xFFE0u)
#define PGLM_RGB565_YELLOWGREEN          ((u16)0x9E66u)

inline u16 color_vec3f_to_rgb565(vec3f color) {
    const u8 red     = (color.x < 1.0f) ? (u8)(color.x * 0x1Fu) : 0x1Fu; 
    const u8 green   = (color.y < 1.0f) ? (u8)(color.y * 0x3Fu) : 0x3Fu; 
    const u8 blue    = (color.z < 1.0f) ? (u8)(color.z * 0x1Fu) : 0x1Fu; 
    const u16 rgb565 = (red << 11) | (green << 5) | blue;
    return rgb565;
}

inline u16 color_average_rgb565(u16 c1, u16 c2) {
    c1 &= 0xF7DFu;
    c1 >>= 1;
    c2 &= 0xF7DFu;
    c2 >>= 1;
    return c1 + c2;
}

#endif // __RGB565_H__
