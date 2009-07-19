/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              GUI header                */
/*                                        */
/*                                        */


#ifndef FROST_GUI_BUTTON_H
#define FROST_GUI_BUTTON_H

#include "frost.h"
#include "gui/frost_frame.h"

namespace Frost
{
    namespace GUI
    {
        class Button : public Frame
        {
        public :

            Button();

            /// Returns this widget's Lua glue.
            virtual void CreateGlue();

            static const s_str CLASS_NAME;

        protected :

        };

        /** \cond NOT_REMOVE_FROM_DOC
        */

        class LuaButton : public LuaFrame
        {
        public :

            LuaButton(lua_State* pLua);

            // Glues
            /**/ int _Click(lua_State*) { return 0; }
            /**/ int _Disable(lua_State*) { return 0; }
            /**/ int _Enable(lua_State*) { return 0; }
            /**/ int _GetButtonState(lua_State*) { return 0; }
            /**/ int _GetDisabledFontObject(lua_State*) { return 0; }
            /**/ int _GetDisabledTextColor(lua_State*) { return 0; }
            /**/ int _GetDisabledTexture(lua_State*) { return 0; }
            /**/ int _GetFont(lua_State*) { return 0; }
            /**/ int _GetFontString(lua_State*) { return 0; }
            /**/ int _GetHighlightFontObject(lua_State*) { return 0; }
            /**/ int _GetHighlightTextColor(lua_State*) { return 0; }
            /**/ int _GetHighlightTexture(lua_State*) { return 0; }
            /**/ int _GetNormalTexture(lua_State*) { return 0; }
            /**/ int _GetPushedTextOffset(lua_State*) { return 0; }
            /**/ int _GetPushedTexture(lua_State*) { return 0; }
            /**/ int _GetText(lua_State*) { return 0; }
            /**/ int _GetTextColor(lua_State*) { return 0; }
            /**/ int _GetTextFontObject(lua_State*) { return 0; }
            /**/ int _GetTextHeight(lua_State*) { return 0; }
            /**/ int _GetTextWidth(lua_State*) { return 0; }
            /**/ int _IsEnabled(lua_State*) { return 0; }
            /**/ int _LockHighlight(lua_State*) { return 0; }
            /**/ int _RegisterForClicks(lua_State*) { return 0; }
            /**/ int _SetButtonState(lua_State*) { return 0; }
            /**/ int _SetDisabledFontObject(lua_State*) { return 0; }
            /**/ int _SetDisabledTextColor(lua_State*) { return 0; }
            /**/ int _SetDisabledTexture(lua_State*) { return 0; }
            /**/ int _SetFont(lua_State*) { return 0; }
            /**/ int _SetFontString(lua_State*) { return 0; }
            /**/ int _SetHighlightFontObject(lua_State*) { return 0; }
            /**/ int _SetHighlightTextColor(lua_State*) { return 0; }
            /**/ int _SetHighlightTexture(lua_State*) { return 0; }
            /**/ int _SetNormalTexture(lua_State*) { return 0; }
            /**/ int _SetPushedTextOffset(lua_State*) { return 0; }
            /**/ int _SetPushedTexture(lua_State*) { return 0; }
            /**/ int _SetText(lua_State*) { return 0; }
            /**/ int _SetTextColor(lua_State*) { return 0; }
            /**/ int _SetTextFontObject(lua_State*) { return 0; }
            /**/ int _UnlockHighlight(lua_State*) { return 0; }

            static const char className[];
            static Lunar<LuaButton>::RegType methods[];
            static const s_str CLASS_NAME;

        protected :

            s_ptr<Button> pButtonParent_;

        };

        /** \endcond
        */
    }
}

#endif
