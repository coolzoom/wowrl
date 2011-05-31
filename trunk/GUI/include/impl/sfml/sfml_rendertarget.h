/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           RenderTarget header          */
/*                                        */
/*                                        */

#ifndef FROST_GUI_RENDERTARGETIMPL_H
#define FROST_GUI_RENDERTARGETIMPL_H

#include <frost_utils.h>

#include <SFML/Graphics/RenderImage.hpp>

namespace sf
{
    class Image;
}

namespace Frost
{
namespace GUI
{
    class RenderTarget;

    /// A wrapper around sf::RenderImage
    class RenderTargetImpl
    {
    public :

        /// Constructor.
        /** \param uiWidth  The width of the RenderTarget
        *   \param uiHeight The height of the RenderTarget
        */
        RenderTargetImpl(const s_uint& uiWidth, const s_uint& uiHeight);

        /// Destructor.
        ~RenderTargetImpl();

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

        const sf::Image&          GetImage() const;

        sf::RenderImage&          GetRenderImage();

        /// Writes the content of this RenderTarget in a file.
        /** \param sFileName The file to save into
        *   \note File format is deduced from extension.
        */
        void                      SaveContentToFile(const s_str& sFileName) const;

        static const s_str CLASS_NAME;

    private :

        s_uint uiRealWidth_;
        s_uint uiRealHeight_;
        s_uint uiWidth_;
        s_uint uiHeight_;

        sf::RenderImage mRenderTarget_;

    };
}
}

#endif
