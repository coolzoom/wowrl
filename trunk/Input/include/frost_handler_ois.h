/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          InputManager header           */
/*                                        */
/*                                        */


#ifndef FROST_HANDLER_OIS_H
#define FROST_HANDLER_OIS_H

#include <frost_utils.h>
#include "frost_inputmanager.h"

#include <OIS/OISPrereqs.h>

namespace Frost
{
    class OISInputHandler : public InputHandlerImpl
    {
    public :

        /// Initializes this handler.
        /** \param sWindowHandle A string containing a formatted window handle
        *   \param fWidth        The width of the window
        *   \param fHeight       the height of the window
        *   \note For more infos regarding the window handle, see OIS's docs.
        */
        OISInputHandler(const s_str& sWindowHandle, const s_float& fWidth, const s_float& fHeight);

        void Delete();

        void FillKeyboardState(InputHandler::KeyboardState& mState);
        void FillMouseState(InputHandler::MouseState& mState);

    private :

        s_float fScreenWidth_;
        s_float fScreenHeight_;

        s_ptr<OIS::InputManager> pOISInputMgr_;
        s_ptr<OIS::Keyboard>     pKeyboard_;
        s_ptr<OIS::Mouse>        pMouse_;
    };
}

#endif
