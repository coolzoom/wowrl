/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           RenderTarget header          */
/*                                        */
/*                                        */

#ifndef FROST_RENDERTARGET_H
#define FROST_RENDERTARGET_H

#include "frost.h"

#include <OgreTexture.h>

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

        /// Clears the content of this RenderTarget.
        /** \param mColor The color to use as background
        *   \note Also clears the depth buffer.
        */
        void                      Clear(const Color& mColor);

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

        s_uint uiID_;
        s_str  sName_;
        ulong  hResourceHandle_;
        s_uint uiRealWidth_;
        s_uint uiRealHeight_;
        s_uint uiWidth_;
        s_uint uiHeight_;
        Usage  mUsage_;
        Ogre::PixelFormat mPixelFormat_;

        s_ptr<Ogre::RenderTarget> pOgreRenderTarget_;

        s_ctnr< s_ptr<Ogre::RenderTargetListener> > lListenerList_;

    };
}

#endif
