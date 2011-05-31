/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           GUIManager header            */
/*                                        */
/*                                        */


#ifndef FROST_GUIMANAGERIMPL_H
#define FROST_GUIMANAGERIMPL_H

#include <frost_utils.h>

namespace sf
{
    class RenderImage;
    class RenderWindow;
    class RenderTarget;
}

namespace Frost
{
    namespace GUI
    {
        class RenderTarget;
    }

    /// Manages the user interface
    /** \note About : void Initialize( const s_map<s_str, s_var>& lParams );<br>
    *         Within the SFML implementation, this function doesn't require
    *         any parameter and does nothing. These is no need to call it.
    */
    class GUIManagerImpl
    {
    public :

        /// Constructor.
        /** \param lParams A map containing initialization parameters.
        *   \note This map must contain :
        *         * "window" = a pointer to the sf::RenderWindow to render
        *                      to (s_ptr<sf::RenderWindow>)
        */
        GUIManagerImpl(const s_map<s_str, s_var>& lParams);

        /// Destructor.
        ~GUIManagerImpl();

        /// Tells the underlying graphics engine to start rendering into a new target.
        /** \param pTarget The target to render to (nullptr to render to the screen)
        */
        void                    Begin(s_ptr<GUI::RenderTarget> pTarget = nullptr);

        /// Tells the underlying graphics engine we're done rendering.
        /** \note For most engines, this is when the rendering is actually
        *         done, so don't forget to call it !
        */
        void                    End();

        /// Internal method for rendering.
        /** \return The current active render target.
        */
        s_ptr<sf::RenderTarget> GetCurrentRenderTarget();

        /// Returns the main render window.
        /** \return The main render window
        *   \note This is where things are rendered when no render target
        *         is defined.
        */
        s_ptr<sf::RenderWindow> GetRenderWindow();

        static const s_str CLASS_NAME;

    private :

        s_ptr<sf::RenderWindow> pWindow_;
        s_ptr<sf::RenderImage>  pRenderImage_;
        s_ptr<sf::RenderTarget> pRenderTarget_;
    };
}


#endif
