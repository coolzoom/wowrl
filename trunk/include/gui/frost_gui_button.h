/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              GUI header                */
/*                                        */
/*                                        */


#ifndef FROST_GUI_BUTTON_H
#define FROST_GUI_BUTTON_H

#include "frost.h"
#include "gui/frost_gui_frame.h"

namespace Frost
{
    namespace GUI
    {
        class Button : public Frame
        {
        public :

            Button();

            /// Returns this widget's Lua glue.
            virtual void        CreateGlue();

            static const s_str CLASS_NAME;

        protected :

        };

        class LuaButton : public LuaFrame
        {
        public :

            LuaButton(lua_State* pLua);

            // Glues
            /**/ int Click_L(lua_State*) { return 0; }
            /**/ int Disable_L(lua_State*) { return 0; }
            /**/ int Enable_L(lua_State*) { return 0; }
            /**/ int GetButtonState_L(lua_State*) { return 0; }
            /**/ int GetDisabledFontObject_L(lua_State*) { return 0; }
            /**/ int GetDisabledTextColor_L(lua_State*) { return 0; }
            /**/ int GetDisabledTexture_L(lua_State*) { return 0; }
            /**/ int GetFont_L(lua_State*) { return 0; }
            /**/ int GetFontString_L(lua_State*) { return 0; }
            /**/ int GetHighlightFontObject_L(lua_State*) { return 0; }
            /**/ int GetHighlightTextColor_L(lua_State*) { return 0; }
            /**/ int GetHighlightTexture_L(lua_State*) { return 0; }
            /**/ int GetNormalTexture_L(lua_State*) { return 0; }
            /**/ int GetPushedTextOffset_L(lua_State*) { return 0; }
            /**/ int GetPushedTexture_L(lua_State*) { return 0; }
            /**/ int GetText_L(lua_State*) { return 0; }
            /**/ int GetTextColor_L(lua_State*) { return 0; }
            /**/ int GetTextFontObject_L(lua_State*) { return 0; }
            /**/ int GetTextHeight_L(lua_State*) { return 0; }
            /**/ int GetTextWidth_L(lua_State*) { return 0; }
            /**/ int IsEnabled_L(lua_State*) { return 0; }
            /**/ int LockHighlight_L(lua_State*) { return 0; }
            /**/ int RegisterForClicks_L(lua_State*) { return 0; }
            /**/ int SetButtonState_L(lua_State*) { return 0; }
            /**/ int SetDisabledFontObject_L(lua_State*) { return 0; }
            /**/ int SetDisabledTextColor_L(lua_State*) { return 0; }
            /**/ int SetDisabledTexture_L(lua_State*) { return 0; }
            /**/ int SetFont_L(lua_State*) { return 0; }
            /**/ int SetFontString_L(lua_State*) { return 0; }
            /**/ int SetHighlightFontObject_L(lua_State*) { return 0; }
            /**/ int SetHighlightTextColor_L(lua_State*) { return 0; }
            /**/ int SetHighlightTexture_L(lua_State*) { return 0; }
            /**/ int SetNormalTexture_L(lua_State*) { return 0; }
            /**/ int SetPushedTextOffset_L(lua_State*) { return 0; }
            /**/ int SetPushedTexture_L(lua_State*) { return 0; }
            /**/ int SetText_L(lua_State*) { return 0; }
            /**/ int SetTextColor_L(lua_State*) { return 0; }
            /**/ int SetTextFontObject_L(lua_State*) { return 0; }
            /**/ int UnlockHighlight_L(lua_State*) { return 0; }

            static const char className[];
            static Lunar<LuaButton>::RegType methods[];
            static const s_str CLASS_NAME;

        protected :

            s_ptr<Button> pButtonParent_;

        };
    }
}

#endif
