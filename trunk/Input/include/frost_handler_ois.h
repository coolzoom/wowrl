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

#include <OISPrereqs.h>

#ifdef FROST_LINUX
#include <X11/Xlib.h>
#else
#include <windows.h>
#endif

namespace Frost
{
    class OISInputHandler : public InputHandlerImpl
    {
    public :

        /// Initializes this handler.
        /** \param sWindowHandle A string containing a formatted window handle
        *   \param fWidth        The width of the window
        *   \param fHeight       the height of the window
        *   \param bMouseGrab    'true' to take full control of the mouse and hide it
        *   \note For more infos regarding the window handle, see OIS's docs.
        */
        OISInputHandler(const s_str& sWindowHandle, const s_float& fWidth, const s_float& fHeight, s_bool bMouseGrab = false);

        void Delete();

        void SetMousePosition(const s_float& fX, const s_float& fY);

        void FillKeyboardState(InputHandler::KeyboardState& mState);
        void FillMouseState(InputHandler::MouseState& mState);

    private :

        #ifdef FROST_LINUX
        Window   mWindow_;
		Display* pDisplay_;
		#else
		HWND     mWindow_;
		#endif

        s_float fScreenWidth_;
        s_float fScreenHeight_;

        s_ptr<OIS::InputManager> pOISInputMgr_;
        s_ptr<OIS::Keyboard>     pKeyboard_;
        s_ptr<OIS::Mouse>        pMouse_;
    };
}

#endif
