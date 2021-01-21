/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               Quad header              */
/*                                        */
/*                                        */


#ifndef FROST_GUI_QUAD_H
#define FROST_GUI_QUAD_H

#include <frost_utils.h>
#include "frost_material.h"

namespace Frost
{
namespace GUI
{
    struct Vertex;

    /// An array of four vertices
    /** This is the base of UI rendering.<br>
    *   It contains the four vertices that define the shape of
    *   the thing you want to draw, and the Material that should
    *   be used to draw that shape.
    */
    struct Quad
    {
        s_array<Vertex,4>  lVertexArray;
        Material mMat;

        Quad() {}
    };
}
}

#endif
