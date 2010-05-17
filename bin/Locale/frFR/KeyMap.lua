-- Maps keyboard buttons to strings
-- [KEY_CODE] = {normal, shift, alt}
KeyMap = {
    [KEY_UNASSIGNED]   = {"", "", ""};
    [KEY_ESCAPE]       = {"", "", ""};
    [KEY_1]            = {"&", "1", ""};
    [KEY_2]            = {"é", "2", "~"};
    [KEY_3]            = {"\"", "3", "#"};
    [KEY_4]            = {"\'", "4", "{"};
    [KEY_5]            = {"(", "5", "["};
    [KEY_6]            = {"-", "6", "|"};
    [KEY_7]            = {"è", "7", "`"};
    [KEY_8]            = {"_", "8", "\\"};
    [KEY_9]            = {"ç", "9", "^"};
    [KEY_0]            = {"à", "0", "@"};
    [KEY_MINUS]        = {")", "°", "]"}; -- '-' on main keyboard
    [KEY_EQUALS]       = {"=", "+", "}"};
    [KEY_BACK]         = {"", "", ""}; -- Backspace
    [KEY_TAB]          = {"    ", "    ", "    "};
    [KEY_Q]            = {"a", "A", "a"};
    [KEY_W]            = {"z", "Z", "z"};
    [KEY_E]            = {"e", "E", "e"};
    [KEY_R]            = {"r", "R", "r"};
    [KEY_T]            = {"t", "T", "t"};
    [KEY_Y]            = {"y", "Y", "y"};
    [KEY_U]            = {"u", "U", "u"};
    [KEY_I]            = {"i", "I", "i"};
    [KEY_O]            = {"o", "O", "o"};
    [KEY_P]            = {"p", "P", "p"};
    [KEY_LBRACKET]     = {"^", "¨", ""};
    [KEY_RBRACKET]     = {"$", "£", "¤"};
    [KEY_RETURN]       = {"", "", ""}; -- Enter on main keyboard
    [KEY_LCONTROL]     = {"", "", ""};
    [KEY_A]            = {"q", "Q", "q"};
    [KEY_S]            = {"s", "S", "s"};
    [KEY_D]            = {"d", "D", "d"};
    [KEY_F]            = {"f", "F", "f"};
    [KEY_G]            = {"g", "G", "g"};
    [KEY_H]            = {"h", "H", "h"};
    [KEY_J]            = {"j", "J", "j"};
    [KEY_K]            = {"k", "K", "k"};
    [KEY_L]            = {"l", "L", "l"};
    [KEY_SEMICOLON]    = {"m", "M", "m"};
    [KEY_APOSTROPHE]   = {"ù", "%", ""};
    [KEY_GRAVE]        = {"²", "", ""}; -- Accent
    [KEY_LSHIFT]       = {"", "", ""};
    [KEY_BACKSLASH]    = {"*", "µ", ""};
    [KEY_Z]            = {"w", "W", "w"};
    [KEY_X]            = {"x", "X", "x"};
    [KEY_C]            = {"c", "C", "c"};
    [KEY_V]            = {"v", "V", "v"};
    [KEY_B]            = {"b", "B", "b"};
    [KEY_N]            = {"n", "N", "n"};
    [KEY_M]            = {",", "?", ""};
    [KEY_COMMA]        = {";", ".", ""};
    [KEY_PERIOD]       = {":", "/", ""}; -- '.' on main keyboard
    [KEY_SLASH]        = {"!", "§", ""}; -- '/' on main keyboard
    [KEY_RSHIFT]       = {"", "", ""};
    [KEY_MULTIPLY]     = {"*", "", ""}; -- '*' on numeric keypad
    [KEY_LMENU]        = {"", "", ""}; -- Left Alt
    [KEY_SPACE]        = {" ", " ", " "};
    [KEY_CAPITAL]      = {"", "", ""};
    [KEY_F1]           = {"", "", ""};
    [KEY_F2]           = {"", "", ""};
    [KEY_F3]           = {"", "", ""};
    [KEY_F4]           = {"", "", ""};
    [KEY_F5]           = {"", "", ""};
    [KEY_F6]           = {"", "", ""};
    [KEY_F7]           = {"", "", ""};
    [KEY_F8]           = {"", "", ""};
    [KEY_F9]           = {"", "", ""};
    [KEY_F10]          = {"", "", ""};
    [KEY_NUMLOCK]      = {"", "", ""};
    [KEY_SCROLL]       = {"", "", ""}; -- Scroll Lock
    [KEY_NUMPAD7]      = {"7", "7", "7"};
    [KEY_NUMPAD8]      = {"8", "8", "8"};
    [KEY_NUMPAD9]      = {"9", "9", "9"};
    [KEY_SUBTRACT]     = {"-", "-", "-"}; -- '-' on numeric keypad
    [KEY_NUMPAD4]      = {"4", "4", "4"};
    [KEY_NUMPAD5]      = {"5", "5", "5"};
    [KEY_NUMPAD6]      = {"6", "6", "6"};
    [KEY_ADD]          = {"+", "+", "+"}; -- '+' on numeric keypad
    [KEY_NUMPAD1]      = {"1", "1", "1"};
    [KEY_NUMPAD2]      = {"2", "2", "2"};
    [KEY_NUMPAD3]      = {"3", "3", "3"};
    [KEY_NUMPAD0]      = {"0", "0", "0"};
    [KEY_DECIMAL]      = {".", ".", "."}; -- '.' on numeric keypad
    [KEY_OEM_102]      = {"", "", ""}; -- '<', '>' and '|' on UK/Germany keyboards
    [KEY_F11]          = {"", "", ""};
    [KEY_F12]          = {"", "", ""};
    [KEY_F13]          = {"", "", ""}; -- (NEC PC98)
    [KEY_F14]          = {"", "", ""}; -- (NEC PC98)
    [KEY_F15]          = {"", "", ""}; -- (NEC PC98)
    [KEY_KANA]         = {"", "", ""}; -- (Japanese keyboard)
    [KEY_ABNT_C1]      = {"", "", ""}; -- '/' and '?' on Portugese (Brazilian) keyboards
    [KEY_CONVERT]      = {"", "", ""}; -- (Japanese keyboard)
    [KEY_NOCONVERT]    = {"", "", ""}; -- (Japanese keyboard)
    [KEY_YEN]          = {"", "", ""}; -- (Japanese keyboard)
    [KEY_ABNT_C2]      = {"", "", ""}; -- Numpad '.' on Portugese (Brazilian) keyboards
    [KEY_NUMPADEQUALS] = {"", "", ""}; -- '=' on numeric keypad (NEC PC98)
    [KEY_PREVTRACK]    = {"", "", ""}; -- Previous Track ([KEY_CIRCUMFLEX on Japanese keyboard)
    [KEY_AT]           = {"", "", ""}; -- (NEC PC98)
    [KEY_COLON]        = {"", "", ""}; -- (NEC PC98)
    [KEY_UNDERLINE]    = {"", "", ""}; -- (NEC PC98)
    [KEY_KANJI]        = {"", "", ""}; -- (Japanese keyboard)
    [KEY_STOP]         = {"", "", ""}; -- (NEC PC98)
    [KEY_AX]           = {"", "", ""}; -- (Japan AX)
    [KEY_UNLABELED]    = {"", "", ""}; -- (J3100)
    [KEY_NEXTTRACK]    = {"", "", ""}; -- Next Track
    [KEY_NUMPADENTER]  = {"", "", ""}; -- Enter on numeric keypad
    [KEY_RCONTROL]     = {"", "", ""};
    [KEY_MUTE]         = {"", "", ""}; -- Mute
    [KEY_CALCULATOR]   = {"", "", ""}; -- Calculator
    [KEY_PLAYPAUSE]    = {"", "", ""}; -- Play / Pause
    [KEY_MEDIASTOP]    = {"", "", ""}; -- Media Stop
    [KEY_VOLUMEDOWN]   = {"", "", ""}; -- Volume -
    [KEY_VOLUMEUP]     = {"", "", ""}; -- Volume +
    [KEY_WEBHOME]      = {"", "", ""}; -- Web home
    [KEY_NUMPADCOMMA]  = {"", "", ""}; -- ',' on numeric keypad (NEC PC98)
    [KEY_DIVIDE]       = {"", "", ""}; -- '/' on numeric keypad
    [KEY_SYSRQ]        = {"", "", ""};
    [KEY_RMENU]        = {"", "", ""}; -- Right Alt
    [KEY_PAUSE]        = {"", "", ""}; -- Pause
    [KEY_HOME]         = {"", "", ""}; -- Home on arrow keypad
    [KEY_UP]           = {"", "", ""}; -- UpArrow on arrow keypad
    [KEY_PGUP]         = {"", "", ""}; -- PgUp on arrow keypad
    [KEY_LEFT]         = {"", "", ""}; -- LeftArrow on arrow keypad
    [KEY_RIGHT]        = {"", "", ""}; -- RightArrow on arrow keypad
    [KEY_END]          = {"", "", ""}; -- End on arrow keypad
    [KEY_DOWN]         = {"", "", ""}; -- DownArrow on arrow keypad
    [KEY_PGDOWN]       = {"", "", ""}; -- PgDn on arrow keypad
    [KEY_INSERT]       = {"", "", ""}; -- Insert on arrow keypad
    [KEY_DELETE]       = {"", "", ""}; -- Delete on arrow keypad
    [KEY_LWIN]         = {"", "", ""}; -- Left Windows key
    [KEY_RWIN]         = {"", "", ""}; -- Right Windows key
    [KEY_APPS]         = {"", "", ""}; -- AppMenu key
};

-- Lists all combo keys, such as the accent key, that are used in conjunction with other keys to create special characters
-- ["combo character"] = {[KEY_CODE] = {"letter", "shift letter", "alt letter"}, ...}
KeyCombo = {
    ["^"] = {[KEY_SPACE] = {"^", "^", "^"}, [KEY_Q] = {"â", "Â", "â"}, [KEY_E] = {"ê", "Ê", "ê"}, [KEY_I] = {"î", "Î", "î"}, [KEY_O] = {"ô", "Ô", "ô"}, [KEY_U] = {"û", "Û", "û"}},
    ["¨"] = {[KEY_SPACE] = {"¨", "¨", "¨"}, [KEY_Q] = {"ä", "Ä", "ä"}, [KEY_E] = {"ë", "Ë", "ë"}, [KEY_I] = {"ï", "Ï", "ï"}, [KEY_O] = {"ö", "Ö", "ö"}, [KEY_U] = {"ü", "Ü", "ü"}},
    ["`"] = {[KEY_SPACE] = {"`", "`", "`"}, [KEY_Q] = {"à", "À", "à"}, [KEY_E] = {"è", "È", "è"}, [KEY_I] = {"ì", "Ì", "ì"}, [KEY_O] = {"ò", "Ò", "ò"}, [KEY_U] = {"ù", "Ù", "ù"}},
    ["~"] = {[KEY_SPACE] = {"~", "~", "~"}, [KEY_Q] = {"ã", "Ã", "ã"}, [KEY_O] = {"õ", "Õ", "õ"}, [KEY_N] = {"ñ", "Ñ", "ñ"}},
}