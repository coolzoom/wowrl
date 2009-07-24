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
        enum ButtonState
        {
            BUTTON_UP,
            BUTTON_DOWN,
            BUTTON_DISABLED
        };

        class Button : public Frame
        {
        public :

            Button();

            /// Returns this widget's Lua glue.
            void CreateGlue();

            /// Calls a script.
            /** \param sScriptName The name of the script
            *   \param pEvent      Stores scripts arguments
            */
            void On(const s_str& sScriptName, s_ptr<Event> pEvent = NULL);

            /// Calls the OnEvent script.
            /** \param mEvent The Event that occured
            */
            void OnEvent(const Event& mEvent);

            void SetText(const s_str& sText);
            const s_str& GetText() const;

            void CreateNormalTexture();
            void CreatePushedTexture();
            void CreateDisabledTexture();
            void CreateHighlightTexture();

            void CreateNormalText();
            void CreateHighlightText();
            void CreateDisabledText();

            s_ptr<Texture> GetNormalTexture();
            s_ptr<Texture> GetPushedTexture();
            s_ptr<Texture> GetDisabledTexture();
            s_ptr<Texture> GetHighlightTexture();

            s_ptr<FontString> GetNormalText();
            s_ptr<FontString> GetHighlightText();
            s_ptr<FontString> GetDisabledText();
            s_ptr<FontString> GetCurrentFontString();

            void Disable();
            void Enable();
            s_bool IsEnabled() const;
            ButtonState GetButtonState() const;

            void LockHighlight();
            void UnlockHighlight();


            static const s_str CLASS_NAME;

        protected :

            ButtonState mState_;
            s_bool      bHighlighted_;

            s_str sText_;

            s_ptr<Texture> pNormalTexture_;
            s_ptr<Texture> pPushedTexture_;
            s_ptr<Texture> pDisabledTexture_;
            s_ptr<Texture> pHighlightTexture_;

            s_ptr<FontString> pNormalText_;
            s_ptr<FontString> pHighlightText_;
            s_ptr<FontString> pDisabledText_;

        };

        /** \cond NOT_REMOVE_FROM_DOC
        */

        class LuaButton : public LuaFrame
        {
        public :

            LuaButton(lua_State* pLua);

            // Glues
            int _Click(lua_State*);
            int _Disable(lua_State*);
            int _Enable(lua_State*);
            int _GetButtonState(lua_State*);
            int _GetDisabledFontObject(lua_State*);
            int _GetDisabledTextColor(lua_State*);
            int _GetDisabledTexture(lua_State*);
            int _GetFont(lua_State*) { return 0; } // WBI
            int _GetFontString(lua_State*) { return 0; } // WBI
            int _GetHighlightFontObject(lua_State*);
            int _GetHighlightTextColor(lua_State*);
            int _GetHighlightTexture(lua_State*);
            int _GetNormalFontObject(lua_State*);
            int _GetNormalTexture(lua_State*);
            int _GetPushedTextOffset(lua_State*);
            int _GetPushedTexture(lua_State*);
            int _GetText(lua_State*);
            int _GetTextColor(lua_State*) { return 0; }  // WBI
            int _GetTextHeight(lua_State*);
            int _GetTextWidth(lua_State*);
            int _IsEnabled(lua_State*);
            int _LockHighlight(lua_State*);
            int _RegisterForClicks(lua_State*);
            int _SetButtonState(lua_State*);
            int _SetDisabledFontObject(lua_State*);
            int _SetDisabledTextColor(lua_State*);
            int _SetDisabledTexture(lua_State*);
            int _SetFont(lua_State*) { return 0; }  // WBI;
            int _SetFontString(lua_State*) { return 0; }  // WBI;
            int _SetHighlightFontObject(lua_State*);
            int _SetHighlightTextColor(lua_State*);
            int _SetHighlightTexture(lua_State*);
            int _SetNormalTexture(lua_State*);
            int _SetPushedTextOffset(lua_State*);
            int _SetPushedTexture(lua_State*);
            int _SetText(lua_State*);
            int _SetTextColor(lua_State*) { return 0; }  // WBI;
            int _SetTextFontObject(lua_State*) { return 0; }  // WBI;
            int _UnlockHighlight(lua_State*);

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
