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
    /// A wrapper around Ogre::RenderTarget
    class RenderTarget
    {
    public :

        enum PixelType
        {
            PIXEL_ARGB, /// Alpha, Red, Green, Blue (8 bits each)
            PIXEL_XRGB, /// Red, Green, Blue (8 bits each, Alpha is discarded)
            PIXEL_FLOAT /// A 32 bit floating point target (depth, ...)
        };

        enum Usage
        {
            USAGE_2D, /// The render target is only used to draw 2D quads (it is not assigned any camera)
            USAGE_3D  /// The render target is used to draw 3D objects and needs a camera (a viewport acutally)
        };

        /// Constructor.
        /** \param uiID     The unique ID to give to that RenderTarget
        *   \param uiWidth  The width of the RenderTarget
        *   \param uiHeight The height of the RenderTarget
        *   \param mType    The pixel type to use
        *   \param mUsage   The render usage (2D or 3D)
        *   \note You shouldn't have to call this. Use the
        *         SpriteManager instead.
        */
        RenderTarget(
            const s_uint& uiID,
            const s_uint& uiWidth,
            const s_uint& uiHeight,
            const PixelType& mType = PIXEL_ARGB,
            const Usage& mUsage = USAGE_2D
        );

        /// Constructor.
        /** \param uiID     The unique ID to give to that RenderTarget
        *   \param sName    The name to give to this RenderTarget
        *   \param uiWidth  The width of the RenderTarget
        *   \param uiHeight The height of the RenderTarget
        *   \param mType    The pixel type to use
        *   \param mUsage   The render usage (2D or 3D)
        *   \note You shouldn't have to call this. Use the
        *         SpriteManager instead.
        */
        RenderTarget(
            const s_uint& uiID,
            const s_str& sName,
            const s_uint& uiWidth,
            const s_uint& uiHeight,
            const PixelType& mType = PIXEL_ARGB,
            const Usage& mUsage = USAGE_2D
        );

        /// Destructor.
        ~RenderTarget();

        /// Adds a new listener to this RenderTarget.
        /** \param pListener The new listener
        *   \note The listener will automatically be deleted along
        *         with this RenderTarget.
        */
        void                      AddListener(s_ptr<Ogre::RenderTargetListener> pListener);

        /// Returns this render target's width.
        /** \return This render target's width
        */
        const s_uint&             GetWidth() const;

        /// Returns this render target's height.
        /** \return This render target's height
        */
        const s_uint&             GetHeight() const;

        /// Sets this render target's dimensions.
        /** \param uiWidth This render target's width
        *   \param uiHeight This render target's height
        *   \return 'true' if the function had to re-create a
        *           new render target
        */
        s_bool                    SetDimensions(const s_uint& uiWidth, const s_uint& uiHeight);

        /// Returns this render target's real width.
        /** \return This render target's real width
        *   \note This is the physical size of the render target
        *         (the nearest power of two from GetWidth()).
        */
        const s_uint&             GetRealWidth() const;

        /// Returns this render target's real height.
        /** \return This render target's real height
        *   \note This is the physical size of the render target
        *         (the nearest power of two from GetHeight()).
        */
        const s_uint&             GetRealHeight() const;

        /// Returns the associated Ogre::RenderTarget.
        /** \return The associated Ogre::RenderTarget
        */
        s_ptr<Ogre::RenderTarget> GetOgreRenderTarget();

        /// Returns this RenderTarget's ID.
        /** \return This RenderTarget's ID
        */
        const s_uint&             GetID();

        /// Returns this RenderTarget's name.
        /** \return This RenderTarget's name
        */
        const s_str&              GetName();

        static const s_str CLASS_NAME;

    private :

        s_uint                    uiID_;
        s_str                     sName_;
        s_ptr<Ogre::RenderTarget> pOgreRenderTarget_;
        ulong                     hResourceHandle_;
        s_uint                    uiRealWidth_;
        s_uint                    uiRealHeight_;
        s_uint                    uiWidth_;
        s_uint                    uiHeight_;

        s_ctnr< s_ptr<Ogre::RenderTargetListener> > lListenerList_;

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
        Vertex             lVertexArray[4];
        s_refptr<Material> pMat;

        Quad();
    };
}

#endif
