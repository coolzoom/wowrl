/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          InputManager source           */
/*                                        */

#include "frost_inputmanager.h"

#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>

#include <OgreRenderWindow.h>

#define INPUT_LONGPRESS_DELAY 0.7f

using namespace std;

namespace Frost
{
    const s_str InputManager::CLASS_NAME = "InputManager";

    InputManager::InputManager()
    {
        dDoubleClickTime_ = 0.25;
        dMouseHistoryMaxLength_ = 0.03;
        fMouseSensibility_ = 1.0f;
    }

    InputManager::~InputManager()
    {
        Log("Closing "+CLASS_NAME+"...");
        if (pOgreInputMgr_)
        {
            pOgreInputMgr_->destroyInputObject(pMouse_.Get());
            pOgreInputMgr_->destroyInputObject(pKeyboard_.Get());
            OIS::InputManager::destroyInputSystem(pOgreInputMgr_.Get());
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
        #ifdef WIN32
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
            pMouse_ = static_cast<OIS::Mouse*>(pOgreInputMgr_->createInputObject(OIS::OISMouse, false));

            // Oww... these are "mutable" attributes, and can be changed even
            // if the object is declared "const". This is ugly, but it's not
            // my code ;)
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

    s_bool InputManager::ReadConfig()
    {
        s_ptr<Engine> pEngine = Engine::GetSingleton();

        if (pEngine->IsConstantDefined("DoubleClickTime"))
            dDoubleClickTime_ = s_double(pEngine->GetFloatConstant("DoubleClickTime"));

        if (pEngine->IsConstantDefined("MouseBufferLength"))
            dMouseHistoryMaxLength_ = s_double(pEngine->GetFloatConstant("MouseBufferLength"));

        if (pEngine->IsConstantDefined("MouseSensibility"))
            fMouseSensibility_ = pEngine->GetFloatConstant("MouseSensibility");

        return true;
    }

    s_bool InputManager::GetKey( s_bool bForce ) const
    {
        if (!bForce && bFocus_)
            return false;
        else
            return bKey_;
    }

    s_str InputManager::GetKeyString( KeyCode mKey ) const
    {
        return pKeyboard_->getAsString((OIS::KeyCode)mKey);
    }

    const s_ctnr<s_uint>& InputManager::GetPressedList() const
    {
        return lDownStack_;
    }

    const s_ctnr<s_uint>& InputManager::GetReleasedList() const
    {
        return lUpStack_;
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

    const s_double& InputManager::GetKeyDownDuration( KeyCode mKey ) const
    {
        return lKeyDelay_[mKey];
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

    const s_double& InputManager::GetMouseDownDuration( MouseButton mID ) const
    {
        return lMouseDelay_[mID];
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
            return (MouseIsPressed(mID) && lDoubleClickDelay_[mID] > 0.0);
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

        // Control extreme delta time after loading/at startup etc
        s_double dDelta = TimeManager::GetSingleton()->GetDelta();
        if ( (dDelta < 0.0) || (dDelta > 1.0) )
            dDelta = 0.05;

        // Update keys
        bKey_ = false;
        s_ptr<EventManager> pEventMgr = EventManager::GetSingleton();
        Event mKeyboardEvent;
        mKeyboardEvent.Add(s_var(s_uint()));

        char lTempBuff[256];
        pKeyboard_->copyKeyStates(lTempBuff);

        for (uint i = 0; i < 256; i++)
        {
            lKeyBufOld_[i] = lKeyBuf_[i];

            // Update delays
            if (lKeyBufOld_[i])
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

            // Update state
            lKeyBuf_[i] = lTempBuff[i];

            if (lKeyBuf_[i])
            {
                bKey_ = true;
                if (!lKeyBufOld_[i])
                {
                    // Key is pressed
                    lDownStack_.PushBack(i);
                }
            }
            else if (lKeyBufOld_[i])
            {
                // Key is released
                lUpStack_.PushBack(i);
            }

            // Send events
            if (!bFocus_)
            {
                if (lKeyBuf_[i])
                {
                    mKeyboardEvent.SetName("KEY_DOWN");
                    mKeyboardEvent[0] = s_uint(i);
                    pEventMgr->FireEvent(mKeyboardEvent);

                    if (!lKeyBufOld_[i])
                    {
                        mKeyboardEvent.SetName("KEY_PRESSED");
                        mKeyboardEvent[0] = s_uint(i);
                        pEventMgr->FireEvent(mKeyboardEvent);
                    }

                    if (lKeyLong_[i])
                    {
                        mKeyboardEvent.SetName("KEY_DOWN_LONG");
                        mKeyboardEvent[0] = s_uint(i);
                        pEventMgr->FireEvent(mKeyboardEvent);
                    }
                }
                else if (lKeyBufOld_[i])
                {
                    mKeyboardEvent.SetName("KEY_RELEASED");
                    mKeyboardEvent[0] = s_uint(i);
                    pEventMgr->FireEvent(mKeyboardEvent);
                }
            }
        }

        // Handle modifier keys
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

        // Update mouse state
        OIS::MouseState mMouseState = pMouse_->getMouseState();
        s_bool bNewDragged = false;
        Event mMouseEvent;
        mMouseEvent.Add(s_uint());
        mMouseEvent.Add(s_float(mMouseState.X.abs));
        mMouseEvent.Add(s_float(mMouseState.Y.abs));
        s_bool bMouseState, bOldMouseState;
        for (uint i = 0; i < INPUT_MOUSE_BUTTON_NUMBER; i++)
        {
            bOldMouseState = lMouseBufOld_[i] = lMouseBuf_[i];

            // Handle double clicking
            lDoubleClickDelay_[i] -= dDelta;

            if (bOldMouseState)
                lDoubleClickDelay_[i] = dDoubleClickTime_;

            // Update delays
            if (bOldMouseState)
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

            // Update state
            bMouseState = lMouseBuf_[i] = mMouseState.buttonDown((OIS::MouseButtonID)i);

            // Handle dragging
            s_bool bDragStartTest = true;
            if (lMouseState_[i] == MOUSE_DRAGGED)
                bDragStartTest = false;

            if (bMouseState)
            {
                if (!bOldMouseState)
                {
                    lMouseState_[i] = MOUSE_CLICKED; // single pressed

                    if (lDoubleClickDelay_[i] > 0.0)
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
                else
                {
                    bNewDragged = true;
                    if (bDragStartTest)
                        bLastDragged_ = true;

                    lMouseState_[i] = MOUSE_DRAGGED; // dragged
                }
            }
            else if (bOldMouseState)
            {
                lMouseState_[i] = MOUSE_RELEASED; // released
            }
            else
            {
                lMouseState_[i] = MOUSE_UP; // no input
            }

            // Send events
            if (!bFocus_)
            {
                mMouseEvent[0] = s_uint(i);
                if (bMouseState)
                {
                    mMouseEvent.SetName("MOUSE_DOWN");
                    pEventMgr->FireEvent(mMouseEvent);

                    if (!bOldMouseState)
                    {
                        mMouseEvent.SetName("MOUSE_PRESSED");
                        pEventMgr->FireEvent(mMouseEvent);

                        if (lDoubleClickDelay_[i] > 0.0)
                        {
                            mMouseEvent.SetName("MOUSE_DOUBLE_CLICKED");
                            pEventMgr->FireEvent(mMouseEvent);
                        }
                    }

                    if (lMouseLong_[i])
                    {
                        mMouseEvent.SetName("MOUSE_DOWN_LONG");
                        pEventMgr->FireEvent(mMouseEvent);
                    }
                }
                else if (bOldMouseState)
                {
                    mMouseEvent.SetName("MOUSE_RELEASED");
                    pEventMgr->FireEvent(mMouseEvent);
                }
            }
        }

        if (!bNewDragged)
            bLastDragged_ = false;

        // Update mouse position
        fMX_ = mMouseState.X.abs;
        fMY_ = mMouseState.Y.abs;

        fRawDMX_ = fDMX_ = mMouseState.X.rel;
        fRawDMY_ = fDMY_ = mMouseState.Y.rel;

        fDMX_ *= fMouseSensibility_;
        fDMY_ *= fMouseSensibility_;

        iMWheel_ = mMouseState.Z.rel/120;
        if (iMWheel_ == 0u)
            bWheelRolled_ = false;
        else
            bWheelRolled_ = true;

        if (dMouseHistoryMaxLength_.IsNull())
        {
            fSmoothDMX_    = fDMX_;
            fSmoothDMY_    = fDMY_;
            fSmoothMWheel_ = s_float(iMWheel_);
        }
        else
        {
            lMouseHistory_.PushFront(MakePair(
                TimeManager::GetSingleton()->GetTime(),
                s_array<s_float,3>((fDMX_, fDMY_, s_float(iMWheel_)))
            ));

            s_double dHistoryLength = lMouseHistory_.Front().First() - lMouseHistory_.Back().First();
            while (dHistoryLength > dMouseHistoryMaxLength_ && (lMouseHistory_.GetSize() > 1))
            {
                lMouseHistory_.PopBack();
                dHistoryLength = lMouseHistory_.Front().First() - lMouseHistory_.Back().First();
            }

            fSmoothDMX_ = fSmoothDMY_ = fSmoothMWheel_ = 0.0f;
            s_float fHistoryWeight = 0.0f;
            s_float fWeight = 1.0f;
            s_ctnr< s_pair< s_double, s_array<s_float,3> > >::iterator iterHistory;
            foreach (iterHistory, lMouseHistory_)
            {
                fSmoothDMX_    += iterHistory->Second()[0]*fWeight;
                fSmoothDMY_    += iterHistory->Second()[1]*fWeight;
                fSmoothMWheel_ += iterHistory->Second()[2]*fWeight;

                fHistoryWeight += fWeight;
                fWeight -= 1.0f/s_float(lMouseHistory_.GetSize());
            }

            fSmoothDMX_    /= fHistoryWeight;
            fSmoothDMY_    /= fHistoryWeight;
            fSmoothMWheel_ /= fHistoryWeight;
        }

        Log(""+TimeManager::GetSingleton()->GetTime()+", "+fRawDMX_+", "+fSmoothDMX_);

        // Send movement event
        if ( (!fDMX_.IsNull()) || (!fDMY_.IsNull()) )
        {
            Event mMouseMovedEvent("MOUSE_MOVED", true);
            mMouseMovedEvent.Add(fDMX_);
            mMouseMovedEvent.Add(fDMY_);
            mMouseMovedEvent.Add(fDMX_/s_float(Engine::GetSingleton()->GetScreenWidth()));
            mMouseMovedEvent.Add(fDMY_/s_float(Engine::GetSingleton()->GetScreenHeight()));
            pEventMgr->FireEvent(mMouseMovedEvent);
        }

        if ( (!fDMX_.IsNull()) || (!fDMY_.IsNull()) )
        {
            Event mMouseMovedEvent("MOUSE_MOVED_RAW", true);
            mMouseMovedEvent.Add(fRawDMX_);
            mMouseMovedEvent.Add(fRawDMY_);
            mMouseMovedEvent.Add(fRawDMX_/s_float(Engine::GetSingleton()->GetScreenWidth()));
            mMouseMovedEvent.Add(fRawDMY_/s_float(Engine::GetSingleton()->GetScreenHeight()));
            pEventMgr->FireEvent(mMouseMovedEvent);
        }

        if ( (!fSmoothDMX_.IsNull()) || (!fSmoothDMY_.IsNull()) )
        {
            Event mMouseMovedEvent("MOUSE_MOVED_SMOOTH", true);
            mMouseMovedEvent.Add(fSmoothDMX_);
            mMouseMovedEvent.Add(fSmoothDMY_);
            mMouseMovedEvent.Add(fSmoothDMX_/s_float(Engine::GetSingleton()->GetScreenWidth()));
            mMouseMovedEvent.Add(fSmoothDMY_/s_float(Engine::GetSingleton()->GetScreenHeight()));
            pEventMgr->FireEvent(mMouseMovedEvent);
        }

        if (bWheelRolled_)
        {
            Event mMouseWheelEvent("MOUSE_WHEEL", true);
            mMouseWheelEvent.Add(iMWheel_);
            pEventMgr->FireEvent(mMouseWheelEvent);
        }

        if (!fSmoothMWheel_.IsNull())
        {
            Event mMouseWheelEvent("MOUSE_WHEEL_SMOOTH", true);
            mMouseWheelEvent.Add(fSmoothMWheel_);
            pEventMgr->FireEvent(mMouseWheelEvent);
        }
    }

    void InputManager::SetDoubleClickTime( const s_double& dDoubleClickTime )
    {
        dDoubleClickTime_ = dDoubleClickTime;
    }

    const s_double& InputManager::GetDoubleClickTime() const
    {
        return dDoubleClickTime_;
    }

    void InputManager::SetMouseBufferDuration(const s_double& dMouseHistoryMaxLength)
    {
        dMouseHistoryMaxLength_ = dMouseHistoryMaxLength;
    }

    const s_double& InputManager::GetMouseBufferDuration() const
    {
        return dMouseHistoryMaxLength_;
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

    const s_float& InputManager::GetMousePosX() const
    {
        return fMX_;
    }

    const s_float& InputManager::GetMousePosY() const
    {
        return fMY_;
    }

    const s_float& InputManager::GetMouseRawDX() const
    {
        return fRawDMX_;
    }

    const s_float& InputManager::GetMouseRawDY() const
    {
        return fRawDMY_;
    }

    const s_float& InputManager::GetMouseDX() const
    {
        return fDMX_;
    }

    const s_float& InputManager::GetMouseDY() const
    {
        return fDMY_;
    }

    const s_float& InputManager::GetMouseSmoothDX() const
    {
        return fSmoothDMX_;
    }

    const s_float& InputManager::GetMouseSmoothDY() const
    {
        return fSmoothDMY_;
    }

    const s_int& InputManager::GetMouseWheel() const
    {
        return iMWheel_;
    }

    void InputManager::SetMouseSensibility(const s_float& fMouseSensibility)
    {
        fMouseSensibility_ = fMouseSensibility;
    }

    const s_float& InputManager::GetMouseSensibility() const
    {
        return fMouseSensibility_;
    }

    s_str InputManager::GetMouseButtonString( MouseButton mID ) const
    {
        switch (mID)
        {
            case MOUSE_LEFT : return "LeftButton";
            case MOUSE_RIGHT : return "RightButton";
            case MOUSE_MIDDLE : return "MiddleButton";
            default : return "";
        }
    }
}
