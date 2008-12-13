/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               GFX header               */
/*                                        */
/*                                        */


#ifndef FROST_GFX_H
#define FROST_GFX_H

#include "frost.h"
#include <OgreTexture.h>

namespace Frost
{
    /// A wrapper around Ogre's render target
    struct RenderTarget
    {
        s_ptr<Ogre::RenderTarget> pOgreRenderTarget;
        Ogre::TexturePtr           pResourcePtr;
        s_uint                     uiWidth;
        s_uint                     uiHeight;

        static const s_str CLASS_NAME;
    };

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

        Vertex()
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
        Vertex             lVertexArray[4];
        s_refptr<Material> pMat;

        Quad();
    };

    namespace GFX
    {
        /// Creates a new RenderTarget.
        /** \param sTargetName The name of this render target
        *   \param uiWidth     The width of this render target
        *   \param uiHeight    The height of this render target
        *   \return The new RenderTarget
        *   \note For compatibility issues, the created render target will be
        *         enlarged to have a power of two for both height and width. This won't
        *         affect the final render.<br>
        *         If you call Ogre::RenderTarget::getWidth(), you'll get the power of two
        *         just above the value you've given to that function.<br>
        *         To get the "rendered" width, use Frost::RenderTarget::uiWidth.<br>
        *         Same goes for the height.
        */
        RenderTarget CreateRenderTarget(const s_str& sTargetName, const s_uint& uiWidth, const s_uint& uiHeight);

        /// Deletes a RenderTarget
        /** \param sTargetName The name of the target you want to delete
        */
        void DeleteRenderTarget(const s_str& sTargetName);

        /// Deletes a RenderTarget
        /** \param mTarget The RenderTarget you want to delete
        */
        void DeleteRenderTarget(RenderTarget& mTarget);

        /// Converts Frost coordinate system to Ogre's
        /** \param[out] fX X coordinate
        *   \param[out] fY Y coordinate
        *   \param[out] fZ Z coordinate
        *   \note (x, y, z) -> (y, z, x)
        */
        void FrostToOgrePosition(s_float& fX, s_float& fY, s_float& fZ);

        /// Converts Ogre coordinate system to Frost's
        /** \param[out] fX X coordinate
        *   \param[out] fY Y coordinate
        *   \param[out] fZ Z coordinate
        *   \note (x, y, z) -> (z, x, y)
        */
        void OgreToFrostPosition(s_float& fX, s_float& fY, s_float& fZ);

        /// Converts a Frost color to an Ogre one.
        /** \param mColor The color to convert
        *   \return The Ogre color
        */
        Ogre::ColourValue FrostToOgreColor(const Color& mColor);
    }
}

#endif
