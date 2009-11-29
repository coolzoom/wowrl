/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            GUI Structs header          */
/*                                        */
/*                                        */

#ifndef FROST_GUISTRUCTS_H
#define FROST_GUISTRUCTS_H

#include "frost.h"
#include "material/frost_material.h"

namespace Frost
{
    /// A point in the 2D environment
    /** This structure contains several informations :
    *   - the point's coordinate on the screen
    *   - the point's coordiante on the texture (if any)
    *   - the color
    */
    struct Vertex
    {
        s_float fX, fY;
        s_float fTX, fTY;
        Color   mColor;

        Vertex(const s_float& x, const s_float& y, const s_float& u, const s_float& v, const Color& color)  :
            fX(x), fY(y), fTX(u), fTY(v), mColor(color)
        {
        }

        Vertex() : mColor(255, 255, 255)
        {

        }

        void Set(const s_float& x, const s_float& y)
        {
            fX = x.Get(); fY = y.Get();
        }

        void SetUV(const s_float& u, const s_float& v)
        {
            fTX = u.Get(); fTY = v.Get();
        }
    };

    /// An array of four vertices
    /** This is the base of UI rendering.<br>
    *   It contains the four vertices that define the shape of
    *   the thing you want to draw, and the Material that should
    *   be used to draw that shape.
    */
    struct Quad
    {
        s_array<Vertex,4>  lVertexArray;
        s_refptr<Material> pMat;

        Quad();
    };
}

#endif
