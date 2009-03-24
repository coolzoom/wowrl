/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          InputManager source           */
/*                                        */
/*  ## : ...                              */
/*                                        */

#include "frost_inputmanager.h"

#include <OgreRenderWindow.h>
#include <OIS/OISInputManager.h>
#include <OIS/OISMouse.h>
#include <OIS/OISKeyboard.h>

#define INPUT_LONGPRESS_DELAY 0.7f

using namespace std;

namespace Frost
{
    const s_str InputManager::CLASS_NAME = "InputManager";

    InputManager::InputManager()
    {
        dDoubleclickTime_ = 0.25;
        cChar_ = 0;

        pOgreInputMgr_ = NULL;
        pKeyboard_ = NULL;
        pMouse_ = NULL;
    }

    InputManager::~InputManager()
    {
        if (pOgreInputMgr_)
        {
            pOgreInputMgr_->destroyInputObject(pMouse_.Get());
            pOgreInputMgr_->destroyInputObject(pKeyboard_.Get());
            OIS::InputManager::destroyInputSystem(pOgreInputMgr_.Get());
            pOgreInputMgr_ = NULL;
            pKeyboard_ = NULL;
            pMouse_ = NULL;
        }
    }

    s_bool InputManager::Initialize( s_ptr<Ogre::RenderWindow> pWindow )
    {
        multimap<string, string> mPL;

        size_t iWindowHnd = 0;
        ostringstream mWindowHnd;
        pWindow->getCustomAttribute("WINDOW", &iWindowHnd);
        mWindowHnd << iWindowHnd;

        mPL.insert(make_pair(string("WINDOW"), mWindowHnd.str()));
        #ifndef FROST_LINUX
            mPL.insert(make_pair(string("w32_mouse"),    string("DISCL_FOREGROUND")));
            mPL.insert(make_pair(string("w32_mouse"),    string("DISCL_NONEXCLUSIVE")));
            mPL.insert(make_pair(string("w32_keyboard"), string("DISCL_FOREGROUND")));
            mPL.insert(make_pair(string("w32_keyboard"), string("DISCL_NONEXCLUSIVE")));
        #else
            mPL.insert(make_pair(string("x11_mouse_grab"),    string("false")));
            mPL.insert(make_pair(string("x11_mouse_hide"),    string("false")));
            mPL.insert(make_pair(string("x11_keyboard_grab"), string("false")));
            mPL.insert(make_pair(string("XAutoRepeatOn"),     string("true")));
        #endif

        pOgreInputMgr_ = OIS::InputManager::createInputSystem(mPL);

        if (pOgreInputMgr_)
        {
            pKeyboard_ = static_cast<OIS::Keyboard*>(pOgreInputMgr_->createInputObject(OIS::OISKeyboard, true));
            pMouse_ = static_cast<OIS::Mouse*>(pOgreInputMgr_->createInputObject(OIS::OISMouse, true));
            const OIS::MouseState& mState = pMouse_->getMouseState();
            mState.width = pWindow->getWidth();
            mState.height = pWindow->getHeight();

            return true;
        }
        else
        {
            Error(CLASS_NAME, "Couldn't create OIS input system.");
            return false;
        }
    }

    char InputManager::GetChar( s_bool bFormated, s_bool bForce ) const
    {
        if (!bForce && bFocus_)
            return 0;
        else if (bFormated)
        {
            // Filter non printable characters and special keys
            char cChar = cChar_;
            if ((cChar == 8)  || // Backspace
                (cChar == 9)  || // Tab
                (cChar == 13) || // Enter
                (cChar == 27)    // Escape
                )
                cChar = 0;
            return cChar;
        }
        else
            return cChar_;
    }

    s_bool InputManager::GetKey( s_bool bForce ) const
    {
        if (!bForce && bFocus_)
            return false;
        else
            return bKey_;
    }

    string InputManager::GetKeyString( KeyCode mKey ) const
    {
        return pKeyboard_->getAsString((OIS::KeyCode)mKey);
    }

    s_uint InputManager::GetNextDown()
    {
        if (!lDownStack_.empty())
        {
            s_uint uiID = lDownStack_.back();
            lDownStack_.pop_back();
            return uiID;
        }
        else
            return s_uint::INF;
    }

    s_uint InputManager::GetNextUp()
    {
        if (!lUpStack_.empty())
        {
            s_uint uiID = lUpStack_.back();
            lUpStack_.pop_back();
            return uiID;
        }
        else
            return s_uint::INF;
    }

    s_bool InputManager::KeyIsDown( KeyCode mKey, s_bool bForce ) const
    {
        if (!bForce && bFocus_)
            return false;
        else
            return lKeyBuf_[mKey];
    }

    s_bool InputManager::KeyIsDownLong( KeyCode mKey, s_bool bForce ) const
    {
        if (!bForce && bFocus_)
            return false;
        else
            return (lKeyBuf_[mKey] && lKeyLong_[mKey]);
    }

    s_bool InputManager::KeyIsPressed( KeyCode mKey, s_bool bForce ) const
    {
        if (!bForce && bFocus_)
            return false;
        else
            return (lKeyBuf_[mKey] && !lKeyBufOld_[mKey]);
    }

    s_bool InputManager::KeyIsReleased( KeyCode mKey, s_bool bForce ) const
    {
        if (!bForce && bFocus_)
            return false;
        else
            return (!lKeyBuf_[mKey] && lKeyBufOld_[mKey]);
    }


    s_bool InputManager::MouseIsDown( MouseButton mID, s_bool bForce ) const
    {
        if (!bForce && bFocus_)
            return false;
        else
            return lMouseBuf_[mID];
    }

    s_bool InputManager::MouseIsDownLong( MouseButton mID, s_bool bForce ) const
    {
        if (!bForce && bFocus_)
            return false;
        else
            return (lMouseBuf_[mID] && lMouseLong_[mID]);
    }

    s_bool InputManager::MouseIsPressed( MouseButton mID, s_bool bForce ) const
    {
        if (!bForce && bFocus_)
            return false;
        else
            return (lMouseBuf_[mID] && !lMouseBufOld_[mID]);
    }

    s_bool InputManager::MouseIsReleased( MouseButton mID, s_bool bForce ) const
    {
        if (!bForce && bFocus_)
            return false;
        else
            return (!lMouseBuf_[mID] && lMouseBufOld_[mID]);
    }

    s_bool InputManager::MouseIsDoubleClicked( MouseButton mID, s_bool bForce ) const
    {
        if (!bForce && bFocus_)
            return false;
        else
        {
            return (MouseIsPressed(mID) && lDoubleclickDelay_[mID] > 0.0);
        }
    }

    s_bool InputManager::WheelIsRolled( s_bool bForce ) const
    {
        if (!bForce && bFocus_)
            return false;
        else
        {
            return bWheelRolled_;
        }
    }

    void InputManager::Update()
    {
        #ifdef PROFILE
            Profiler* prof = TimeManager::GetSingleton()->GetProfiler(0, "InputManager::Update", false);
            Chrono c(prof);
        #endif

        pKeyboard_->capture();
        pMouse_->capture();

        // Update keys
        for (uint i = 0; i < 256; i++)
            lKeyBufOld_[i] = lKeyBuf_[i];

        for (uint i = 0; i < INPUT_MOUSE_BUTTON_NUMBER; i++)
            lMouseBufOld_[i] = lMouseBuf_[i];

        // Control extreme delta time after loading/at startup etc
        s_double dDelta = TimeManager::GetSingleton()->GetDelta();
        if ( (dDelta < 0.0) || (dDelta > 1.0) )
            dDelta = 0.05;

        for (uint i = 0; i < INPUT_MOUSE_BUTTON_NUMBER; i++)
            lDoubleclickDelay_[i] -= dDelta;

        if (lMouseBuf_[MOUSE_LEFT])
            lDoubleclickDelay_[MOUSE_LEFT] = dDoubleclickTime_;
        if (lMouseBuf_[MOUSE_RIGHT])
            lDoubleclickDelay_[MOUSE_RIGHT] = dDoubleclickTime_;
        if (lMouseBuf_[MOUSE_MIDDLE])
            lDoubleclickDelay_[MOUSE_MIDDLE] = dDoubleclickTime_;

        bKey_ = false;
        for (uint i = 0; i < 256; i++)
        {
            lKeyBuf_[i] = pKeyboard_->isKeyDown((OIS::KeyCode)i);

            if (KeyIsPressed((KeyCode)i))
                lDownStack_.push_back(i);
            if (KeyIsReleased((KeyCode)i))
                lUpStack_.push_back(i);
            if (lKeyBuf_[i])
                bKey_ = true;
        }

        OIS::MouseState mMouseState = pMouse_->getMouseState();
        for (uint i = 0; i < INPUT_MOUSE_BUTTON_NUMBER; i++)
        {
            lMouseBuf_[i] = mMouseState.buttonDown((OIS::MouseButtonID)i);
        }

        // Update delays
        for (uint i = 0; i < 256; i++)
        {
            if (lKeyBuf_[i])
            {
                lKeyDelay_[i] += dDelta;
                if (lKeyDelay_[i] >= INPUT_LONGPRESS_DELAY)
                    lKeyLong_[i] = true;
            }
            else
            {
                lKeyDelay_[i] = 0.0;
                lKeyLong_[i] = false;
            }
        }
        for (uint i = 0; i < INPUT_MOUSE_BUTTON_NUMBER; i++)
        {
            if (lMouseBuf_[i])
            {
                lMouseDelay_[i] += dDelta;
                if (lMouseDelay_[i] >= INPUT_LONGPRESS_DELAY)
                    lMouseLong_[i] = true;
            }
            else
            {
                lMouseDelay_[i] = 0.0;
                lMouseLong_[i] = false;
            }
        }

        if (KeyIsDown(KEY_LCONTROL, true) || KeyIsDown(KEY_RCONTROL, true))
            bCtrlPressed_ = true;
        else
            bCtrlPressed_ = false;

        if (KeyIsDown(KEY_LSHIFT, true) || KeyIsDown(KEY_RSHIFT, true))
            bShiftPressed_ = true;
        else
            bShiftPressed_ = false;

        if (KeyIsDown(KEY_LMENU, true))
            bAltPressed_ = true;
        else
            bAltPressed_ = false;

        // Update mouse
        fMX_ = mMouseState.X.abs;
        fMY_ = mMouseState.Y.abs;

        fDMX_ = mMouseState.X.rel;
        fDMY_ = mMouseState.Y.rel;

        iMWheel_ = mMouseState.Z.rel;
        if (iMWheel_ == 0u)
            bWheelRolled_ = false;
        else
            bWheelRolled_ = true;

        s_bool bNewDragged = false;
        for (uint i = 0; i < INPUT_MOUSE_BUTTON_NUMBER; i++)
        {
            s_bool bDragStartTest = true;
            if (lMouseState_[i] == MOUSE_DRAGGED)
                bDragStartTest = false;

            // Handle left mouse button
            if (MouseIsDown((MouseButton)i, true))
            {
                if (MouseIsPressed((MouseButton)i, true))
                {
                    lMouseState_[i] = MOUSE_CLICKED; // single pressed
                }
                else if (MouseIsDoubleClicked((MouseButton)i, true))
                {
                    lMouseState_[i] = MOUSE_DOUBLE; // double clicked
                }
                else
                {
                    bNewDragged = true;
                    if (bDragStartTest)
                        bLastDragged_ = true;

                    lMouseState_[i] = MOUSE_DRAGGED; // dragged
                }
            }
            else if (MouseIsReleased((MouseButton)i, true))
            {
                lMouseState_[i] = MOUSE_RELEASED; // released
            }
            else
            {
                lMouseState_[i] = MOUSE_UP; // no input
            }
        }

        if (!bNewDragged)
            bLastDragged_ = false;

        // Send events :
        // Keyboard
        s_ptr<EventManager> pEventMgr = EventManager::GetSingleton();
        Event mKeyboardEvent;
        mKeyboardEvent.Add(s_var(s_uint()));
        s_bool bEvent;
        for (uint i = 0; i < 256; i++)
        {
            if (KeyIsReleased((KeyCode)i))
            {
                mKeyboardEvent.SetName("KEY_RELEASED");
                bEvent = true;
            }
            else if (KeyIsPressed((KeyCode)i))
            {
                mKeyboardEvent.SetName("KEY_PRESSED");
                bEvent = true;
            }

            if (bEvent)
            {
                mKeyboardEvent[0].SetUI(s_uint(i));
                pEventMgr->FireEvent(mKeyboardEvent);
                bEvent = false;
            }

            if (KeyIsDown((KeyCode)i))
            {
                mKeyboardEvent.SetName("KEY_DOWN");
                mKeyboardEvent[0].SetUI(s_uint(i));
                pEventMgr->FireEvent(mKeyboardEvent);
            }
            if (KeyIsDownLong((KeyCode)i))
            {
                mKeyboardEvent.SetName("KEY_DOWN_LONG");
                mKeyboardEvent[0].SetUI(s_uint(i));
                pEventMgr->FireEvent(mKeyboardEvent);
            }
        }

        /*s_ptr<Event> pKeyboardEvent;
        for (uint i = 0; i < 256; i++)
        {
            if (KeyIsDownLong((KeyCode)i))
            {
                pKeyboardEvent = new Event("KEY_DOWN_LONG", true);
            }
            else if (KeyIsDown((KeyCode)i))
            {
                pKeyboardEvent = new Event("KEY_DOWN", true);
            }
            else if (KeyIsReleased((KeyCode)i))
            {
                pKeyboardEvent = new Event("KEY_RELEASED", true);
            }
            else if (KeyIsPressed((KeyCode)i))
            {
                pKeyboardEvent = new Event("KEY_PRESSED", true);
            }

            if (pKeyboardEvent)
            {
                pKeyboardEvent->Add(s_uint(i));
                pEventMgr->FireEvent(*pKeyboardEvent);
                pKeyboardEvent.Delete();
            }
        }*/

        // Mouse
        if ( (fDMX_ != 0.0f) || (fDMY_ != 0.0f) )
        {
            Event mMouseMovedEvent("MOUSE_MOVED", true);
            mMouseMovedEvent.Add(fDMX_);
            mMouseMovedEvent.Add(fDMY_);
            pEventMgr->FireEvent(mMouseMovedEvent);
        }

        Event mMouseEvent;
        mMouseEvent.Add(s_var(s_uint()));
        //s_bool bEvent;
        for (uint i = 0; i < INPUT_MOUSE_BUTTON_NUMBER; i++)
        {
            if (MouseIsReleased((MouseButton)i))
            {
                mMouseEvent.SetName("MOUSE_RELEASED");
                bEvent = true;
            }
            else if (MouseIsPressed((MouseButton)i))
            {
                mMouseEvent.SetName("MOUSE_PRESSED");
                bEvent = true;
            }

            if (bEvent)
            {
                mMouseEvent[0].SetUI(s_uint(i));
                pEventMgr->FireEvent(mMouseEvent);
                bEvent = false;
            }

            if (MouseIsDown((MouseButton)i))
            {
                mMouseEvent.SetName("MOUSE_DOWN");
                mMouseEvent[0].SetUI(s_uint(i));
                pEventMgr->FireEvent(mMouseEvent);
            }

            if (MouseIsDoubleClicked((MouseButton)i))
            {
                mMouseEvent.SetName("MOUSE_DOUBLE_CLICKED");
                mMouseEvent[0].SetUI(s_uint(i));
                pEventMgr->FireEvent(mMouseEvent);
            }

            if (MouseIsDownLong((MouseButton)i))
            {
                mMouseEvent.SetName("MOUSE_DOWN_LONG");
                mMouseEvent[0].SetUI(s_uint(i));
                pEventMgr->FireEvent(mMouseEvent);
            }
        }

        /*s_ptr<Event> pMouseEvent;
        for (uint i = 0; i < INPUT_MOUSE_BUTTON_NUMBER; i++)
        {
            if (MouseIsDownLong((MouseButton)i))
            {
                pMouseEvent = new Event("MOUSE_DOWN_LONG", true);
            }
            else if (MouseIsDown((MouseButton)i))
            {
                pMouseEvent = new Event("MOUSE_DOWN", true);
            }
            else if (MouseIsReleased((MouseButton)i))
            {
                pMouseEvent = new Event("MOUSE_RELEASED", true);
            }
            else if (MouseIsPressed((MouseButton)i))
            {
                pMouseEvent = new Event("MOUSE_PRESSED", true);
            }
            else if (MouseIsDoubleClicked((MouseButton)i))
            {
                pMouseEvent = new Event("MOUSE_DOUBLE_CLICKED", true);
            }

            if (pMouseEvent)
            {
                pMouseEvent->Add(s_uint(i));
                pEventMgr->FireEvent(*pMouseEvent);
                pMouseEvent.Delete();
            }
        }*/

        if (iMWheel_ != 0)
        {
            Event mMouseWheelEvent("MOUSE_WHEEL", true);
            mMouseWheelEvent.Add(iMWheel_);
            pEventMgr->FireEvent(mMouseWheelEvent);
        }
    }

    void InputManager::SetDoubleclickTime( s_double dDoubleclickTime )
    {
        dDoubleclickTime_ = dDoubleclickTime;
    }

    void InputManager::SetFocus( s_bool bFocus )
    {
        bFocus_ = bFocus;
    }

    const s_bool& InputManager::AltPressed() const
    {
        return bAltPressed_;
    }

    const s_bool& InputManager::ShiftPressed() const
    {
        return bShiftPressed_;
    }

    const s_bool& InputManager::CtrlPressed() const
    {
        return bCtrlPressed_;
    }

    const s_bool& InputManager::MouseLastDragged() const
    {
        return bLastDragged_;
    }

    MouseState InputManager::GetMouseState( MouseButton mID ) const
    {
        return lMouseState_[mID];
    }

    const s_float& InputManager::GetMPosX() const
    {
        return fMX_;
    }

    const s_float& InputManager::GetMPosY() const
    {
        return fMY_;
    }

    const s_float& InputManager::GetMDPosX() const
    {
        return fDMX_;
    }

    const s_float& InputManager::GetMDPosY() const
    {
        return fDMY_;
    }

    const s_int& InputManager::GetMWheel() const
    {
        return iMWheel_;
    }
}
