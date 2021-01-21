/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          InputManager source           */
/*                                        */


#include "frost_handler_ois.h"

#include <OIS.h>

using namespace std;

namespace Frost
{
    OISInputHandler::OISInputHandler( const s_str& sWindowHandle, const s_float& fWidth, const s_float& fHeight, s_bool bMouseGrab )
    {
        fScreenWidth_ = fWidth;
        fScreenHeight_ = fHeight;

        multimap<string, string> mPL;
        mPL.insert(make_pair(string("WINDOW"), sWindowHandle.Get()));

        #ifdef WIN32

            mWindow_ = (HWND)strtoul(sWindowHandle.c_str(), 0, 10);

            mPL.insert(make_pair(string("w32_mouse"),    string("DISCL_FOREGROUND")));
            mPL.insert(make_pair(string("w32_mouse"),    string("DISCL_NONEXCLUSIVE")));
            mPL.insert(make_pair(string("w32_keyboard"), string("DISCL_FOREGROUND")));
            mPL.insert(make_pair(string("w32_keyboard"), string("DISCL_NONEXCLUSIVE")));

        #else

            pDisplay_ = XOpenDisplay(nullptr);
            mWindow_ = strtoul(sWindowHandle.c_str(), 0, 10);

            if (bMouseGrab)
            {
                mPL.insert(make_pair(string("x11_mouse_grab"), string("true")));
                mPL.insert(make_pair(string("x11_mouse_hide"), string("true")));
            }
            else
            {
                mPL.insert(make_pair(string("x11_mouse_grab"), string("false")));
                mPL.insert(make_pair(string("x11_mouse_hide"), string("false")));
            }
            mPL.insert(make_pair(string("x11_keyboard_grab"), string("false")));
            mPL.insert(make_pair(string("XAutoRepeatOn"),     string("true")));

        #endif

        pOISInputMgr_ = OIS::InputManager::createInputSystem(mPL);

        if (pOISInputMgr_)
        {
            pKeyboard_ = static_cast<OIS::Keyboard*>(pOISInputMgr_->createInputObject(OIS::OISKeyboard, true));
            pMouse_ = static_cast<OIS::Mouse*>(pOISInputMgr_->createInputObject(OIS::OISMouse, false));

            // Oww... these are "mutable" attributes, and can be changed even
            // if the object is declared "const". This is ugly, but it's not
            // my code ;)
            const OIS::MouseState& mState = pMouse_->getMouseState();
            mState.width = fWidth.Get();
            mState.height = fHeight.Get();
        }
        else
        {
            throw Exception("InputManager", "Couldn't create OIS input system.");
        }

        if (bMouseGrab)
            SetMousePosition(fScreenWidth_/2.0f, fScreenHeight_/2.0f);
    }

    void OISInputHandler::SetMousePosition( const s_float& fX, const s_float& fY )
    {
    #ifdef FROST_LINUX
        XWarpPointer(pDisplay_, None, mWindow_, 0, 0, 0, 0, fX.Get(), fY.Get());
        XFlush(pDisplay_);
    #else
        POINT position = {(LONG)fX.Get(), (LONG)fY.Get()};
        ClientToScreen(mWindow_, &position);
        SetCursorPos(position.x, position.y);
    #endif
    }

    void OISInputHandler::Delete()
    {
        if (pOISInputMgr_)
        {
            #ifdef FROST_LINUX
                XCloseDisplay(pDisplay_);
            #endif
            pOISInputMgr_->destroyInputObject(pMouse_.Get());
            pOISInputMgr_->destroyInputObject(pKeyboard_.Get());
            OIS::InputManager::destroyInputSystem(pOISInputMgr_.Get());
        }
    }

    void OISInputHandler::FillKeyboardState( InputHandler::KeyboardState& mState )
    {
        pKeyboard_->capture();

        char lTempBuff[KEY_MAXKEY];
        pKeyboard_->copyKeyStates(lTempBuff);

        for (int i = 0; i < KEY_MAXKEY; ++i)
            mState.lKeyState[i] = (lTempBuff[i] != 0);
    }

    void OISInputHandler::FillMouseState( InputHandler::MouseState& mState )
    {
        pMouse_->capture();

        OIS::MouseState mMouseState = pMouse_->getMouseState();
        mState.fAbsX = mMouseState.X.abs;
        mState.fAbsY = mMouseState.Y.abs;
        mState.fRelX = mMouseState.X.abs/fScreenWidth_.Get();
        mState.fRelY = mMouseState.Y.abs/fScreenHeight_.Get();
        mState.bHasDelta = true;
        mState.fDX = mMouseState.X.rel;
        mState.fDY = mMouseState.Y.rel;

        mState.fRelWheel = mMouseState.Z.rel/120.0f;

        for (int i = 0; i < INPUT_MOUSE_BUTTON_NUMBER; ++i)
            mState.lButtonState[i] = mMouseState.buttonDown((OIS::MouseButtonID)i);
    }
}
