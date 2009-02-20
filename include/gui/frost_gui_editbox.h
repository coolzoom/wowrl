/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              GUI header                */
/*                                        */
/*                                        */


#ifndef FROST_GUI_EDITBOX_H
#define FROST_GUI_EDITBOX_H

#include "frost.h"
#include "gui/frost_gui_frame.h"

namespace Frost
{
    namespace GUI
    {
        class EditBox : public Frame
        {
        public :

            EditBox();

            /// Returns this widget's Lua glue.
            virtual void        CreateGlue();

            static const s_str CLASS_NAME;

        protected :

        };

        class LuaEditBox : public LuaFrame
        {
        public :

            LuaEditBox(lua_State* pLua);

            // Glues
            /**/ int AddHistoryLine_L(lua_State*) { return 0; }
            /**/ int ClearFocus_L(lua_State*) { return 0; }
            /**/ int GetAltArrowKeyMode_L(lua_State*) { return 0; }
            /**/ int GetBlinkSpeed_L(lua_State*) { return 0; }
            /**/ int GetHistoryLines_L(lua_State*) { return 0; }
            /**/ int GetInputLanguage_L(lua_State*) { return 0; }
            /**/ int GetMaxBytes_L(lua_State*) { return 0; }
            /**/ int GetMaxLetters_L(lua_State*) { return 0; }
            /**/ int GetNumLetters_L(lua_State*) { return 0; }
            /**/ int GetNumber_L(lua_State*) { return 0; }
            /**/ int GetText_L(lua_State*) { return 0; }
            /**/ int GetTextInsets_L(lua_State*) { return 0; }
            /**/ int HighlithtText_L(lua_State*) { return 0; }
            /**/ int Insert_L(lua_State*) { return 0; }
            /**/ int IsAutoFocus_L(lua_State*) { return 0; }
            /**/ int IsMultiLine_L(lua_State*) { return 0; }
            /**/ int IsNumeric_L(lua_State*) { return 0; }
            /**/ int IsPassword_L(lua_State*) { return 0; }
            /**/ int SetAltArrowKeyMode_L(lua_State*) { return 0; }
            /**/ int SetAutoFocus_L(lua_State*) { return 0; }
            /**/ int SetBlinkSpeed_L(lua_State*) { return 0; }
            /**/ int SetFocus_L(lua_State*) { return 0; }
            /**/ int SetHistoryLines_L(lua_State*) { return 0; }
            /**/ int SetMaxBytes_L(lua_State*) { return 0; }
            /**/ int SetMaxLetters_L(lua_State*) { return 0; }
            /**/ int SetMultiLine_L(lua_State*) { return 0; }
            /**/ int SetNumber_L(lua_State*) { return 0; }
            /**/ int SetNumeric_L(lua_State*) { return 0; }
            /**/ int SetPassword_L(lua_State*) { return 0; }
            /**/ int SetText_L(lua_State*) { return 0; }
            /**/ int SetTextInsets_L(lua_State*) { return 0; }
            /**/ int ToggleInputLanguage_L(lua_State*) { return 0; }

            static const char className[];
            static Lunar<LuaEditBox>::RegType methods[];
            static const s_str CLASS_NAME;

        protected :

            s_ptr<EditBox> pEditBoxParent_;

        };
    }
}

#endif
