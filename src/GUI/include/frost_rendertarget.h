/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           RenderTarget header          */
/*                                        */
/*                                        */

#ifndef FROST_GUI_RENDERTARGET_H
#define FROST_GUI_RENDERTARGET_H

#include <frost_utils.h>

namespace Frost
{
namespace GUI
{
    class RenderTargetImpl;

    /// A place to render things (the screen, a texture, ...)
    class RenderTarget
    {
    public :

        /// Constructor.
        /** \param uiWidth  The width of the RenderTarget
        *   \param uiHeight The height of the RenderTarget
        */
        RenderTarget(const s_uint& uiWidth, const s_uint& uiHeight);

        /// Destructor.
        ~RenderTarget();

        /// Clears the content of this RenderTarget.
        /** \param mColor The color to use as background
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

        /// Writes the content of this RenderTarget in a file.
        /** \param sFileName The file to save into
        *   \note File format is deduced from extension.
        */
        void                      SaveContentToFile(const s_str& sFileName) const;

        /// Returns the implementation specific render target.
        /** \return The implementation specific render target
        *   \note Although it's here for internal purpose only,
        *         nothing prevents you from using the underlying
        *         render target as you wish. Just be carefull not
        *         to mess this RenderTarget class up !
        */
        s_wptr<RenderTargetImpl>  GetImpl() const;

        static const s_str CLASS_NAME;

    private :

        s_refptr<RenderTargetImpl> pImpl_;

    };
}
}

#endif
