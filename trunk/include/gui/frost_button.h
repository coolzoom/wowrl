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
        /// A simple button.
        /** This class can handle three different states :
        *   "normal", "pushed" and "disabled". You can provide a
        *   different texture for each of these states, and
        *   two different fontstrings for "normal" and "disabled".<br>
        *   In addition, you can provide another texture/fontstring
        *   for the "highlight" state (when the mouse is over the
        *   button widget).<br>
        *   Note that there is no fontstring for the "pushed" state :
        *   in this case, the "normal" font is rendered with a slight
        *   offset that you'll have to define.
        */
        class Button : public Frame
        {
        public :

            enum State
            {
                STATE_UP,
                STATE_DOWN,
                STATE_DISABLED
            };

            /// Constructor.
            Button();

            /// Destructor.
            virtual ~Button();

            /// Creates the associated Lua glue.
            virtual void      CreateGlue();

            /// Calls a script.
            /** \param sScriptName The name of the script
            *   \param pEvent      Stores scripts arguments
            */
            virtual void      On(const s_str& sScriptName, s_ptr<Event> pEvent = NULL);

            /// Calls the OnEvent script.
            /** \param mEvent The Event that occured
            */
            virtual void      OnEvent(const Event& mEvent);

            /// Copies an UIObject's parameters into this Button (inheritance).
            /** \param pObj The UIObject to copy
            */
            virtual void      CopyFrom(s_ptr<UIObject> pObj);

            /// Sets this Button's text.
            /** \param sText The new text
            */
            virtual void      SetText(const s_str& sText);

            /// Returns this Button's text.
            /** \return This Button's text
            */
            const s_str&      GetText() const;

            /// Returns this Button's normal texture.
            /** \return This Button's normal texture
            */
            s_ptr<Texture>    GetNormalTexture();

            /// Returns this Button's pushed texture.
            /** \return This Button's pushed texture
            */
            s_ptr<Texture>    GetPushedTexture();

            /// Returns this Button's disabled texture.
            /** \return This Button's disabled texture
            */
            s_ptr<Texture>    GetDisabledTexture();

            /// Returns this Button's highlight texture.
            /** \return This Button's highlight texture
            */
            s_ptr<Texture>    GetHighlightTexture();

            /// Returns this Button's normal text.
            /** \return This Button's normal text
            */
            s_ptr<FontString> GetNormalText();

            /// Returns this Button's highlight text.
            /** \return This Button's highlight text
            */
            s_ptr<FontString> GetHighlightText();

            /// Returns this Button's disabled text.
            /** \return This Button's disabled text
            */
            s_ptr<FontString> GetDisabledText();

            /// Returns the currently displayed text object.
            /** \return The currently displayed text object
            */
            s_ptr<FontString> GetCurrentFontString();

            /// Sets this Button's normal texture.
            /** \param pTexture The new texture
            */
            void              SetNormalTexture(s_ptr<Texture> pTexture);

            /// Sets this Button's pushed texture.
            /** \param pTexture The new texture
            */
            void              SetPushedTexture(s_ptr<Texture> pTexture);

            /// Sets this Button's disabled texture.
            /** \param pTexture The new texture
            */
            void              SetDisabledTexture(s_ptr<Texture> pTexture);

            /// Sets this Button's highlight texture.
            /** \param pTexture The new texture
            */
            void              SetHighlightTexture(s_ptr<Texture> pTexture);

            /// Sets this Button's normal text.
            /** \param pFont The new text object
            */
            void              SetNormalText(s_ptr<FontString> pFont);

            /// Sets this Button's highlight text.
            /** \param pFont The new text object
            */
            void              SetHighlightText(s_ptr<FontString> pFont);

            /// Sets this Button's disabled text.
            /** \param pFont The new text object
            */
            void              SetDisabledText(s_ptr<FontString> pFont);

            /// Disables this Button.
            /** \note A disabled button doesn't receive any input.
            */
            virtual void      Disable();

            /// Enables this Button.
            virtual void      Enable();

            /// Checks if this Button is enabled.
            /** \return 'true' if this Button is enabled
            */
            s_bool            IsEnabled() const;

            /// Pushed this Button.
            /** \note This function only has a visual impact :
            *         the OnClick() handler is not called.
            */
            virtual void      Push();

            /// Releases this Button.
            /** \note This function only has a visual impact :
            *         the OnClick() handler is not called.
            */
            virtual void      Release();

            /// Highlights this Button.
            /** \note The Button will be highlighted even if the
            *         mouse is not over it. It will stop when the
            *         mouse leaves it.
            */
            virtual void      Highlight();

            /// Unlights this Button.
            /** \note The Button will be unlighted even if the
            *         mouse is over it. It will highlight again
            *         when the mouse leaves then enters its region.
            */
            virtual void      Unlight();

            /// Returns this Button's state.
            /** \return This Button's state (see ButtonState)
            */
            State             GetButtonState() const;

            /// Locks this Button's highlighting.
            /** \note The button will always be highlighted
            *         until you call UnlockHighlight().
            */
            void              LockHighlight();

            /// Unlocks this Button's highlighting.
            void              UnlockHighlight();

            /// Sets this Button's pushed text offset.
            /** \param lOffset The pused text offset
            */
            virtual void      SetPushedTextOffset(const s_array<s_int,2>& lOffset);

            /// Returns this Button's pushed text offset.
            /** \return This Button's pushed text offset
            */
            const s_array<s_int,2>& GetPushedTextOffset() const;

            /// Parses data from an XML::Block.
            /** \param pBlock The Button's XML::Block
            */
            virtual void        ParseBlock(s_ptr<XML::Block> pBlock);

            static const s_str CLASS_NAME;

        protected :

            s_ptr<Texture>    CreateNormalTexture_();
            s_ptr<Texture>    CreatePushedTexture_();
            s_ptr<Texture>    CreateDisabledTexture_();
            s_ptr<Texture>    CreateHighlightTexture_();
            s_ptr<FontString> CreateNormalText_();
            s_ptr<FontString> CreateHighlightText_();
            s_ptr<FontString> CreateDisabledText_();

            State       mState_;
            s_bool      bHighlighted_;
            s_bool      bLockHighlight_;

            s_str sText_;

            s_ptr<Texture> pNormalTexture_;
            s_ptr<Texture> pPushedTexture_;
            s_ptr<Texture> pDisabledTexture_;
            s_ptr<Texture> pHighlightTexture_;

            s_ptr<FontString> pNormalText_;
            s_ptr<FontString> pHighlightText_;
            s_ptr<FontString> pDisabledText_;
            s_ptr<FontString> pCurrentFontString_;

            s_array<s_int,2> lPushedTextOffset_;

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
            int _SetNormalFontObject(lua_State*);
            int _SetNormalTextColor(lua_State*);
            int _SetNormalTexture(lua_State*);
            int _SetPushedTextOffset(lua_State*);
            int _SetPushedTexture(lua_State*);
            int _SetText(lua_State*);

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
