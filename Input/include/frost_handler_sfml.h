/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          InputManager header           */
/*                                        */
/*                                        */


#ifndef FROST_HANDLER_SFML_H
#define FROST_HANDLER_SFML_H

#include <frost_utils.h>
#include "frost_inputmanager.h"

namespace sf { class Window; }

namespace Frost
{
    class SFMLInputHandler : public InputHandlerImpl
    {
    public :

        /// Initializes this handler.
        /** \param pWindow The window from which to receive input
        */
        SFMLInputHandler(s_ptr<sf::Window> pWindow);

        void FillKeyboardState(InputHandler::KeyboardState& mState);
        void FillMouseState(InputHandler::MouseState& mState);

    private :

        static const int lKeyFrostToSFML[100][2];

        s_float fScreenWidth_;
        s_float fScreenHeight_;

        s_ptr<sf::Window> pWindow_;
    };
}

#endif
