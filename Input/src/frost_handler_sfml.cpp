/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          InputManager source           */
/*                                        */


#include "frost_handler_sfml.h"

#include <SFML/Window/Window.hpp>
#include <SFML/Window/Input.hpp>

using namespace std;
using namespace sf::Key;

namespace Frost
{
    const int SFMLInputHandler::lKeyFrostToSFML[100][2] =
    {
        {KEY_ESCAPE,Escape},
        {KEY_0,Num0},
        {KEY_1,Num1},
        {KEY_2,Num2},
        {KEY_3,Num3},
        {KEY_4,Num4},
        {KEY_5,Num5},
        {KEY_6,Num6},
        {KEY_7,Num7},
        {KEY_8,Num8},
        {KEY_9,Num9},
        {KEY_MINUS,Dash},
        {KEY_EQUALS,Equal},
        {KEY_BACK,Back},
        {KEY_TAB,Tab},
        {KEY_Q,'q'},
        {KEY_W,'w'},
        {KEY_E,'e'},
        {KEY_R,'r'},
        {KEY_T,'t'},
        {KEY_Y,'y'},
        {KEY_U,'u'},
        {KEY_I,'i'},
        {KEY_O,'o'},
        {KEY_P,'p'},
        {KEY_LBRACKET,LBracket},
        {KEY_RBRACKET,RBracket},
        {KEY_RETURN,Return},
        {KEY_LCONTROL,LControl},
        {KEY_A,'a'},
        {KEY_S,'s'},
        {KEY_D,'d'},
        {KEY_F,'f'},
        {KEY_G,'g'},
        {KEY_H,'h'},
        {KEY_J,'j'},
        {KEY_K,'k'},
        {KEY_L,'l'},
        {KEY_SEMICOLON,SemiColon},
        {KEY_APOSTROPHE,Quote},
        //{KEY_GRAVE, },
        {KEY_LSHIFT,LShift},
        {KEY_BACKSLASH,BackSlash},
        {KEY_Z,'z'},
        {KEY_X,'x'},
        {KEY_C,'c'},
        {KEY_V,'v'},
        {KEY_B,'b'},
        {KEY_N,'n'},
        {KEY_M,'m'},
        {KEY_COMMA,Comma},
        {KEY_PERIOD,Period},
        {KEY_SLASH,Slash},
        {KEY_RSHIFT,RShift},
        {KEY_MULTIPLY,Multiply},
        {KEY_LMENU,LAlt},
        {KEY_SPACE,Space},
        //{KEY_CAPITAL, }, // CapsLock
        {KEY_F1,F1},
        {KEY_F2,F2},
        {KEY_F3,F3},
        {KEY_F4,F4},
        {KEY_F5,F5},
        {KEY_F6,F6},
        {KEY_F7,F7},
        {KEY_F8,F8},
        {KEY_F9,F9},
        {KEY_F10,F10},
        {KEY_NUMPAD7,Numpad7},
        {KEY_NUMPAD8,Numpad8},
        {KEY_NUMPAD9,Numpad9},
        {KEY_SUBTRACT,Subtract},
        {KEY_NUMPAD4,Numpad4},
        {KEY_NUMPAD5,Numpad5},
        {KEY_NUMPAD6,Numpad6},
        {KEY_ADD,Add},
        {KEY_NUMPAD1,Numpad1},
        {KEY_NUMPAD2,Numpad2},
        {KEY_NUMPAD3,Numpad3},
        {KEY_NUMPAD0,Numpad0},
        //{KEY_DECIMAL,"Decimal"},
        //{KEY_OEM_102,"<>"},
        {KEY_F11,F11},
        {KEY_F12,F12},
        {KEY_F13,F13},
        {KEY_F14,F14},
        {KEY_F15,F15},
        {KEY_RCONTROL,RControl},
        {KEY_DIVIDE,Divide},
        {KEY_RMENU,RAlt},
        {KEY_PAUSE,Pause},
        {KEY_HOME,Home},
        {KEY_UP,Up},
        {KEY_PGUP,PageUp},
        {KEY_LEFT,Left},
        {KEY_RIGHT,Right},
        {KEY_END,End},
        {KEY_DOWN,Down},
        {KEY_PGDOWN,PageDown},
        {KEY_INSERT,Insert},
        {KEY_DELETE,sf::Key::Delete},
        {KEY_LWIN,LSystem},
        {KEY_RWIN,RSystem},
        {KEY_APPS,Menu}
    };

    SFMLInputHandler::SFMLInputHandler( s_ptr<sf::Window> pWindow )
    {
        pWindow_ = pWindow;
    }

    void SFMLInputHandler::SetMousePosition( const s_float& fX, const s_float& fY )
    {
        pWindow_->SetCursorPosition(fX.Get(), fY.Get());
    }

    void SFMLInputHandler::FillKeyboardState( InputHandler::KeyboardState& mState )
    {
        for (int i = 0; i < 100; ++i)
            mState.lKeyState[lKeyFrostToSFML[i][0]] = pWindow_->GetInput().IsKeyDown((sf::Key::Code)lKeyFrostToSFML[i][1]);
    }

    void SFMLInputHandler::FillMouseState( InputHandler::MouseState& mState )
    {
        mState.fAbsX = pWindow_->GetInput().GetMouseX();
        mState.fAbsY = pWindow_->GetInput().GetMouseY();
        mState.fRelX = mState.fAbsX / pWindow_->GetWidth();
        mState.fRelY = mState.fAbsY / pWindow_->GetHeight();
        mState.bHasDelta = true;
        mState.fDX = pWindow_->GetInput().GetMouseDX();
        mState.fDY = pWindow_->GetInput().GetMouseDY();
        s_float fMouseWheel = pWindow_->GetInput().GetMouseWheel();
        mState.fRelWheel = fMouseWheel - fOldMouseWheel_;
        fOldMouseWheel_ = fMouseWheel;

        for (int i = 0; i < INPUT_MOUSE_BUTTON_NUMBER; ++i)
            mState.lButtonState[i] = pWindow_->GetInput().IsMouseButtonDown((sf::Mouse::Button)i);
    }
}
