/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           GUIManager header            */
/*                                        */
/*                                        */


#ifndef FROST_GUIMANAGERIMPL_H
#define FROST_GUIMANAGERIMPL_H

#include <frost_utils.h>

namespace Frost
{
    namespace GUI
    {
        class  RenderTarget;
        class  SpriteManager;
        struct Quad;
    }

    /// Manages the user interface
    /** \note About : void Initialize( const s_map<s_str, s_var>& lParams );<br>
    *         Within the OGRE implementation, this function requires several parameters :
    *            - scene_manager : the name of the scene manager (s_str)
    *            - width         : the width of the screen (s_uint)
    *            - height        : the height of the screen (s_uint)
    *         Calling this function is mandatory.
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

        void                    RenderQuad(const GUI::Quad& mQuad);

        s_wptr<GUI::SpriteManager> GetSpriteManager();

        static const s_str CLASS_NAME;

    private :

        s_refptr<GUI::SpriteManager> pSpriteMgr_;

    };
}

#endif
