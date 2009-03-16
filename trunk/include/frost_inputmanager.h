/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          InputManager header           */
/*                                        */
/*                                        */


#ifndef FROST_INPUTMANAGER_H
#define FROST_INPUTMANAGER_H

#include "frost.h"
#include "utils/frost_utils_manager.h"

#define INPUT_MOUSE_BUTTON_NUMBER 3

namespace Frost
{
    enum MouseState
    {
        MOUSE_UP = 0,
        MOUSE_DRAGGED,
        MOUSE_CLICKED,
        MOUSE_RELEASED,
        MOUSE_DOUBLE,
        MOUSE_LONG
    };

    enum MouseButton
    {
        MOUSE_LEFT,
        MOUSE_RIGHT,
        MOUSE_MIDDLE
    };

    enum KeyCode
    {
        KEY_UNASSIGNED  = 0x00,
        KEY_ESCAPE      = 0x01,
        KEY_1           = 0x02,
        KEY_2           = 0x03,
        KEY_3           = 0x04,
        KEY_4           = 0x05,
        KEY_5           = 0x06,
        KEY_6           = 0x07,
        KEY_7           = 0x08,
        KEY_8           = 0x09,
        KEY_9           = 0x0A,
        KEY_0           = 0x0B,
        KEY_MINUS       = 0x0C,    /// - on main keyboard
        KEY_EQUALS      = 0x0D,
        KEY_BACK        = 0x0E,    /// Backspace
        KEY_TAB         = 0x0F,
        KEY_Q           = 0x10,
        KEY_W           = 0x11,
        KEY_E           = 0x12,
        KEY_R           = 0x13,
        KEY_T           = 0x14,
        KEY_Y           = 0x15,
        KEY_U           = 0x16,
        KEY_I           = 0x17,
        KEY_O           = 0x18,
        KEY_P           = 0x19,
        KEY_LBRACKET    = 0x1A,
        KEY_RBRACKET    = 0x1B,
        KEY_RETURN      = 0x1C,    /// Enter on main keyboard
        KEY_LCONTROL    = 0x1D,
        KEY_A           = 0x1E,
        KEY_S           = 0x1F,
        KEY_D           = 0x20,
        KEY_F           = 0x21,
        KEY_G           = 0x22,
        KEY_H           = 0x23,
        KEY_J           = 0x24,
        KEY_K           = 0x25,
        KEY_L           = 0x26,
        KEY_SEMICOLON   = 0x27,
        KEY_APOSTROPHE  = 0x28,
        KEY_GRAVE       = 0x29,    /// Accent
        KEY_LSHIFT      = 0x2A,
        KEY_BACKSLASH   = 0x2B,
        KEY_Z           = 0x2C,
        KEY_X           = 0x2D,
        KEY_C           = 0x2E,
        KEY_V           = 0x2F,
        KEY_B           = 0x30,
        KEY_N           = 0x31,
        KEY_M           = 0x32,
        KEY_COMMA       = 0x33,
        KEY_PERIOD      = 0x34,    /// . on main keyboard
        KEY_SLASH       = 0x35,    /// / on main keyboard
        KEY_RSHIFT      = 0x36,
        KEY_MULTIPLY    = 0x37,    /// * on numeric keypad
        KEY_LMENU       = 0x38,    /// Left Alt
        KEY_SPACE       = 0x39,
        KEY_CAPITAL     = 0x3A,
        KEY_F1          = 0x3B,
        KEY_F2          = 0x3C,
        KEY_F3          = 0x3D,
        KEY_F4          = 0x3E,
        KEY_F5          = 0x3F,
        KEY_F6          = 0x40,
        KEY_F7          = 0x41,
        KEY_F8          = 0x42,
        KEY_F9          = 0x43,
        KEY_F10         = 0x44,
        KEY_NUMLOCK     = 0x45,
        KEY_SCROLL      = 0x46,    /// Scroll Lock
        KEY_NUMPAD7     = 0x47,
        KEY_NUMPAD8     = 0x48,
        KEY_NUMPAD9     = 0x49,
        KEY_SUBTRACT    = 0x4A,    /// - on numeric keypad
        KEY_NUMPAD4     = 0x4B,
        KEY_NUMPAD5     = 0x4C,
        KEY_NUMPAD6     = 0x4D,
        KEY_ADD         = 0x4E,    /// + on numeric keypad
        KEY_NUMPAD1     = 0x4F,
        KEY_NUMPAD2     = 0x50,
        KEY_NUMPAD3     = 0x51,
        KEY_NUMPAD0     = 0x52,
        KEY_DECIMAL     = 0x53,    /// . on numeric keypad
        KEY_OEM_102     = 0x56,    /// < > | on UK/Germany keyboards
        KEY_F11         = 0x57,
        KEY_F12         = 0x58,
        KEY_F13         = 0x64,    ///                     (NEC PC98)
        KEY_F14         = 0x65,    ///                     (NEC PC98)
        KEY_F15         = 0x66,    ///                     (NEC PC98)
        KEY_KANA        = 0x70,    /// (Japanese keyboard)
        KEY_ABNT_C1     = 0x73,    /// / ? on Portugese (Brazilian) keyboards
        KEY_CONVERT     = 0x79,    /// (Japanese keyboard)
        KEY_NOCONVERT   = 0x7B,    /// (Japanese keyboard)
        KEY_YEN         = 0x7D,    /// (Japanese keyboard)
        KEY_ABNT_C2     = 0x7E,    /// Numpad . on Portugese (Brazilian) keyboards
        KEY_NUMPADEQUALS= 0x8D,    /// = on numeric keypad (NEC PC98)
        KEY_PREVTRACK   = 0x90,    /// Previous Track (KEY_CIRCUMFLEX on Japanese keyboard)
        KEY_AT          = 0x91,    ///                     (NEC PC98)
        KEY_COLON       = 0x92,    ///                     (NEC PC98)
        KEY_UNDERLINE   = 0x93,    ///                     (NEC PC98)
        KEY_KANJI       = 0x94,    /// (Japanese keyboard)
        KEY_STOP        = 0x95,    ///                     (NEC PC98)
        KEY_AX          = 0x96,    ///                     (Japan AX)
        KEY_UNLABELED   = 0x97,    ///                        (J3100)
        KEY_NEXTTRACK   = 0x99,    /// Next Track
        KEY_NUMPADENTER = 0x9C,    /// Enter on numeric keypad
        KEY_RCONTROL    = 0x9D,
        KEY_MUTE        = 0xA0,    /// Mute
        KEY_CALCULATOR  = 0xA1,    /// Calculator
        KEY_PLAYPAUSE   = 0xA2,    /// Play / Pause
        KEY_MEDIASTOP   = 0xA4,    /// Media Stop
        KEY_VOLUMEDOWN  = 0xAE,    /// Volume -
        KEY_VOLUMEUP    = 0xB0,    /// Volume +
        KEY_WEBHOME     = 0xB2,    /// Web home
        KEY_NUMPADCOMMA = 0xB3,    /// , on numeric keypad (NEC PC98)
        KEY_DIVIDE      = 0xB5,    /// / on numeric keypad
        KEY_SYSRQ       = 0xB7,
        KEY_RMENU       = 0xB8,    /// Right Alt
        KEY_PAUSE       = 0xC5,    /// Pause
        KEY_HOME        = 0xC7,    /// Home on arrow keypad
        KEY_UP          = 0xC8,    /// UpArrow on arrow keypad
        KEY_PGUP        = 0xC9,    /// PgUp on arrow keypad
        KEY_LEFT        = 0xCB,    /// LeftArrow on arrow keypad
        KEY_RIGHT       = 0xCD,    /// RightArrow on arrow keypad
        KEY_END         = 0xCF,    /// End on arrow keypad
        KEY_DOWN        = 0xD0,    /// DownArrow on arrow keypad
        KEY_PGDOWN      = 0xD1,    /// PgDn on arrow keypad
        KEY_INSERT      = 0xD2,    /// Insert on arrow keypad
        KEY_DELETE      = 0xD3,    /// Delete on arrow keypad
        KEY_LWIN        = 0xDB,    /// Left Windows key
        KEY_RWIN        = 0xDC,    /// Right Windows key
        KEY_APPS        = 0xDD,    /// AppMenu key
    };

    /// Handles inputs (keyboard and mouse)
    class InputManager : public Manager<InputManager>
    {
    friend class Manager<InputManager>;
    public :

        s_bool         Initialize(s_ptr<Ogre::RenderWindow> pWindow);

        void           Update();

        // Keyboard
        char           GetChar(s_bool bFormated, s_bool bForce = false) const;
        s_bool         GetKey(s_bool bForce = false) const;
        s_bool         KeyIsDown(KeyCode mKey, s_bool bForce = false) const;
        s_bool         KeyIsDownLong(KeyCode mKey, s_bool bForce = false) const;
        s_bool         KeyIsPressed(KeyCode mKey, s_bool bForce = false) const;
        s_bool         KeyIsReleased(KeyCode mKey, s_bool bForce = false) const;

        std::string    GetKeyString(KeyCode mKey) const;

        s_uint         GetNextUp();
        s_uint         GetNextDown();

        // Special keys
        const s_bool&  AltPressed() const;
        const s_bool&  ShiftPressed() const;
        const s_bool&  CtrlPressed() const;

        // Mouse
        s_bool         MouseIsDown(MouseButton mID, s_bool bForce = false) const;
        s_bool         MouseIsDownLong(MouseButton mID, s_bool bForce = false) const;
        s_bool         MouseIsPressed(MouseButton mID, s_bool bForce = false) const;
        s_bool         MouseIsReleased(MouseButton mID, s_bool bForce = false) const;
        s_bool         MouseIsDoubleClicked(MouseButton mID, s_bool bForce = false) const;
        s_bool         WheelIsRolled(s_bool bForce = false) const;
        const s_bool&  MouseLastDragged() const;
        MouseState     GetMouseState(MouseButton mID) const;
        const s_float& GetMPosX() const;
        const s_float& GetMPosY() const;
        const s_float& GetMDPosX() const;
        const s_float& GetMDPosY() const;
        const s_int&   GetMWheel() const;

        // Parameters
        void           SetDoubleclickTime(s_double dDoubleclickTime);
        void           SetFocus(s_bool bFocus);

        static const s_str CLASS_NAME;

    protected :

        /// Default constructor
        /** \note This is a singleton class. That means that there can
        *         only be a single instance at the same time. That's why
        *         the constructor is not in public scope.<br>
        *         If you need to get a pointer to that unique instance,
        *         call InputManager::GetSingleton().<br>
        *         Note that, if GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when you close
        *         your program by calling InputManager::Delete() (this is
        *         automatically done by Engine).
        */
        InputManager();

        /// Destructor
        /** \note If GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when
        *         you close your program by calling Delete() (this is
        *         automatically done by Engine).
        */
        ~InputManager();

    private :

        s_bool bFocus_;

        // Keyboard
        s_double dDoubleclickTime_;
        s_array<s_double,3>   lDoubleclickDelay_;
        s_array<s_double,256> lKeyDelay_;
        s_array<s_bool,256>   lKeyLong_;
        s_array<s_bool,256>   lKeyBuf_;
        s_array<s_bool,256>   lKeyBufOld_;

        s_bool bCtrlPressed_;
        s_bool bShiftPressed_;
        s_bool bAltPressed_;
        s_bool bKey_;
        char   cChar_;

        std::vector<s_uint> lDownStack_;
        std::vector<s_uint> lUpStack_;

        // Mouse
        s_double   lMouseDelay_[INPUT_MOUSE_BUTTON_NUMBER];
        s_bool     lMouseLong_[INPUT_MOUSE_BUTTON_NUMBER];
        s_bool     lMouseBuf_[INPUT_MOUSE_BUTTON_NUMBER];
        s_bool     lMouseBufOld_[INPUT_MOUSE_BUTTON_NUMBER];
        MouseState lMouseState_[INPUT_MOUSE_BUTTON_NUMBER];

        s_float fMX_,  fMY_;
        s_float fDMX_, fDMY_;
        s_int   iMWheel_;
        s_bool  bWheelRolled_;
        s_str   sMouseButton_;
        s_bool  bLastDragged_;

        // OIS
        s_ptr<OIS::InputManager> pOgreInputMgr_;
        s_ptr<OIS::Keyboard>     pKeyboard_;
        s_ptr<OIS::Mouse>        pMouse_;
    };
}

#endif
