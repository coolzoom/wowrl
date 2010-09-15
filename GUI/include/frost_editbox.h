/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              GUI header                */
/*                                        */
/*                                        */


#ifndef FROST_GUI_EDITBOX_H
#define FROST_GUI_EDITBOX_H

#include <frost_utils.h>
#include "frost_frame.h"

namespace Frost
{
    namespace GUI
    {
        class FontString;
        class Texture;

        class EditBox : public Frame
        {
        public :

            /// Constructor.
            EditBox(s_ptr<GUIManager> pManager);

            /// Destructor.
            virtual ~EditBox();

            /// Copies an UIObject's parameters into this EditBox (inheritance).
            /** \param pObj The UIObject to copy
            */
            virtual void            CopyFrom(s_ptr<UIObject> pObj);

            /// Updates this widget's logic.
            virtual void            Update();

            /// Calls the OnEvent script.
            /** \param mEvent The Event that occured
            */
            virtual void            OnEvent(const Event& mEvent);

            /// Calls a script.
            /** \param sScriptName The name of the script
            *   \param pEvent      Stores scripts arguments
            */
            virtual void            On(const s_str& sScriptName, s_ptr<Event> pEvent = nullptr);

            /// Returns 'true' if this Button can use a script.
            /** \param sScriptName The name of the script
            *   \note This method can be overriden if needed.
            */
            virtual s_bool          CanUseScript(const s_str& sScriptName) const;

            /// Sets the content of this EditBox.
            /** \param sText The content of this EditBox
            */
            void                    SetText(const s_str& sText);

            /// Returns the content of this EditBox.
            /** \return The content of this EditBox
            */
            const s_str&            GetText() const;

            /// Selects a portion of the content.
            /** \param uiStart      The first character to select
            *   \param uiEnd        The last character to select
            *   \param bForceUpdate 'true' to bypass all redundancy checks
            *   \note Will select (uiEnd - uiStart) characters
            */
            void                    HighlightText(
                const s_uint& uiStart = 0u, const s_uint& uiEnd = s_uint::INF, const s_bool& bForceUpdate = false
            );

            /// Deselects the selected text, if any.
            void                    UnlightText();

            /// Sets the color of the highlight quad.
            /** \param mColor The color
            */
            void                    SetHighlightColor(const Color& mColor);

            /// Inserts some text after the cursor.
            /** \param sText The text to insert
            */
            void                    InsertAfterCursor(const s_str& sText);

            /// Sets the maximum number of letters to allow in this EditBox.
            /** \param uiMaxLetters The max number of letters
            */
            void                    SetMaxLetters(const s_uint& uiMaxLetters);

            /// Returns the maximum number of letters to allow in this EditBox.
            /** \return the maximum number of letters to allow in this EditBox
            */
            const s_uint&           GetMaxLetters() const;

            /// Returns the number of letters in the content.
            /** \return The number of letters in the content
            */
            const s_uint&           GetNumLetters() const;

            /// Sets the carret's blink speed.
            /** \param dBlinkSpeed The number of seconds to wait between each blink
            */
            void                    SetBlinkSpeed(const s_double& dBlinkSpeed);

            /// Returns the carret's blink speed.
            /** \return the carret's blink speed (time in seconds between each blink)
            */
            const s_double&         GetBlinkSpeed() const;

            /// Makes this EditBox allow numeric characters only.
            /** \param bNumericOnly 'true' to only allow numeric characters
            */
            void                    SetNumericOnly(const s_bool& bNumericOnly);

            /// Makes this EditBox allow positive numbers only.
            /** \param bPositiveOnly 'true' to only allow positive numbers
            *   \note Only workds if SetNumericOnly(true) has been called.
            */
            void                    SetPositiveOnly(const s_bool& bPositiveOnly);

            /// Makes this EditBox allow integer numbers only.
            /** \param bIntegerOnly 'true' to only allow integer numbers
            *   \note Only workds if SetNumericOnly(true) has been called.
            */
            void                    SetIntegerOnly(const s_bool& bIntegerOnly);

            /// Checks if this EditBox allows numeric characters only.
            /** \return 'true' if this EditBox allows numeric characters only
            */
            const s_bool&           IsNumericOnly() const;

            /// Checks if this EditBox allows positive numbers only.
            /** \return 'true' if this EditBox allows positive numbers only
            */
            const s_bool&           IsPositiveOnly() const;

            /// Checks if this EditBox allows integer numbers only.
            /** \return 'true' if this EditBox allows integer numbers only
            */
            const s_bool&           IsIntegerOnly() const;

            /// Enables password mode.
            /** \param bEnable 'true' to enable password mode
            *   \note In password mode, the content of the EditBox is replaced
            *         by stars (*).
            */
            void                    EnablePasswordMode(const s_bool& bEnable);

            /// Checks if this EditBox is in password mode.
            /** \return 'true' if this EditBox is in password mode
            */
            const s_bool&           IsPasswordModeEnabled() const;

            /// Allows this EditBox to have several lines in it.
            /** \param bMultiLine 'true' to allow several lines in this EditBox
            *   \note The behavior of a "multi line" EditBox is very different from
            *         a single line one.<br>
            *         History lines are only available to single line EditBoxes.<br>
            *         Scrolling in a single line EditBox is done horizontally, while
            *         it is only done vertically in a multi line one.
            */
            void                    SetMultiLine(const s_bool& bMultiLine);

            /// Checks if this EditBox can have several lines in it.
            /** \return 'true' if this EditBox can have several lines in it
            */
            const s_bool&           IsMultiLine() const;

            /// Sets the maximum number of history lines this EditBox can keep.
            /** \param uiMaxHistoryLines The max number of history lines
            */
            void                    SetMaxHistoryLines(const s_uint& uiMaxHistoryLines);

            /// Returns the maximum number of history lines this EditBox can keep.
            /** \return The maximum number of history lines this EditBox can keep
            */
            const s_uint&           GetMaxHistoryLines() const;

            /// Adds a new history line to the history line list.
            /** \param sHistoryLine The content of this history line
            *   \note This option is only available to single line EditBoxes.
            */
            void                    AddHistoryLine(const s_str& sHistoryLine);

            /// Returns the history line list.
            /** \return The history line list
            *   \note This list will always be empty for multi line EditBoxes.
            */
            const s_ctnr<s_str>&    GetHistoryLines() const;

            /// Enables automatic focus for this EditBox.
            /** \param bEnable 'true' to enable auto focus
            *   \note When auto focus is enabled, this EditBox will be editable
            *         immediately after it is shown. Else, the user needs to click
            *         on it first, then edit its content.
            */
            void                    EnableAutoFocus(const s_bool& bEnable);

            /// Checks if automatic focus is enabled.
            /** \return 'true' if automatic focus is enabled
            */
            const s_bool&           IsAutoFocusEnabled() const;

            /// Gives this EditBox the focus.
            /** \param bFocus 'true' to give to focus, 'false' to remove it
            */
            void                    SetFocus(const s_bool& bFocus);

            /// Sets whether keyboard arrows move the carret or not.
            /** \param bArrowsIgnored 'true' to ignore arrow keys
            */
            void                    SetArrowsIgnored(const s_bool& bArrowsIgnored);

            /// Sets the insets used to render the content text.
            /** \param iLeft   The left inset
            *   \param iRight  The right inset
            *   \param iTop    The top inset
            *   \param iBottom The bottom inset
            *   \note Positive insets will reduce the text area, while
            *         negative ones will enlarge it
            */
            void                    SetTextInsets(const s_int& iLeft, const s_int& iRight, const s_int& iTop, const s_int& iBottom);

            /// Sets the insets used to render the content text.
            /** \param lInsets (left, right, top, bottom)
            *   \note Positive insets will reduce the text area, while
            *         negative ones will enlarge it
            */
            void                    SetTextInsets(const s_array<s_int,4>& lInsets);

            /// Returns the text insets.
            /** \return The text insets
            */
            const s_array<s_int,4>& GetTextInsets() const;

            /// Returns the FontString used to render the content.
            /** \return The FontString used to render the content
            */
            s_ptr<FontString>       GetFontString();

            /// Notifies this EditBox it has gained/lost focus.
            /** \param bFocus 'true' if the EditBox has gained focus
            *   \note This function is called by GUIManager.
            */
            void                    NotifyFocus(const s_bool& bFocus);

            /// Returns this widget's Lua glue.
            virtual void            CreateGlue();

            /// Parses data from an XML::Block.
            /** \param pBlock The EditBox's XML::Block
            */
            virtual void            ParseBlock(s_ptr<XML::Block> pBlock);

            static const s_str CLASS_NAME;

        protected :

            void ParseFontStringBlock_(s_ptr<XML::Block> pBlock);
            void ParseHighlighColorBlock_(s_ptr<XML::Block> pBlock);
            void ParseTextInsetsBlock_(s_ptr<XML::Block> pBlock);

            s_ptr<FontString> CreateFontString_();
            void              CreateHighlight_();
            void              CreateCarret_();

            void CheckText_();
            void UpdateDisplayedText_();
            void UpdateFontString_();
            void UpdateCarretPosition_();

            s_bool AddChar_(const s_str& sChar);
            s_bool RemoveChar_();
            s_uint GetLetterIDAt_(const s_int& iX, const s_int& iY);
            s_bool MoveCarretAt_(const s_int& iX, const s_int& iY);
            s_bool MoveCarretHorizontally_(const s_bool& bForward = true);
            s_bool MoveCarretVertically_(const s_bool& bDown = true);

            void ProcessKey_(const s_uint& uiKey);

            s_str            sText_;
            s_ustr           sUnicodeText_;
            s_ustr           sDisplayedText_;
            s_ustr::iterator iterCarretPos_;
            s_uint           uiDisplayPos_;
            s_uint           uiNumLetters_;
            s_uint           uiMaxLetters_;
            s_bool           bNumericOnly_;
            s_bool           bPositiveOnly_;
            s_bool           bIntegerOnly_;
            s_bool           bPasswordMode_;
            s_bool           bMultiLine_;
            s_bool           bFocus_;
            s_bool           bAutoFocus_;
            s_bool           bArrowsIgnored_;

            s_str sComboKey_;

            s_ptr<Texture> pHighlight_;
            Color          mHighlightColor_;
            s_uint         uiSelectionStartPos_;
            s_uint         uiSelectionEndPos_;
            s_bool         bSelectedText_;

            s_ptr<Texture>          pCarret_;
            s_refptr<PeriodicTimer> pCarretTimer_;
            s_double                dBlinkSpeed_;

            s_ctnr<s_str> lHistoryLineList_;
            s_uint        uiMaxHistoryLines_;

            s_ptr<FontString> pFontString_;
            s_array<s_int,4>  lTextInsets_;

            s_uint                  uiLastKeyPressed_;
            s_refptr<PeriodicTimer> pKeyRepeatTimer_;
            s_double                dKeyRepeatSpeed_;
        };

        /** \cond NOT_REMOVE_FROM_DOC
        */

        class LuaEditBox : public LuaFrame
        {
        public :

            LuaEditBox(lua_State* pLua);

            // Glues
            int _AddHistoryLine(lua_State*);
            int _ClearFocus(lua_State*);
            int _GetAltArrowKeyMode(lua_State*) { return 0; } // WBI
            int _GetBlinkSpeed(lua_State*);
            int _GetHistoryLines(lua_State*);
            int _GetInputLanguage(lua_State*) { return 0; } // WBI
            int _GetMaxBytes(lua_State*) { return 0; } // WBI
            int _GetMaxLetters(lua_State*);
            int _GetNumLetters(lua_State*);
            int _GetNumber(lua_State*);
            int _GetText(lua_State*);
            int _GetTextInsets(lua_State*);
            int _HighlithtText(lua_State*);
            int _Insert(lua_State*);
            int _IsAutoFocus(lua_State*);
            int _IsMultiLine(lua_State*);
            int _IsNumeric(lua_State*);
            int _IsPassword(lua_State*);
            int _SetAltArrowKeyMode(lua_State*) { return 0; } // WBI
            int _SetAutoFocus(lua_State*);
            int _SetBlinkSpeed(lua_State*);
            int _SetFocus(lua_State*);
            int _SetMaxHistoryLines(lua_State*);
            int _SetMaxBytes(lua_State*) { return 0; } // WBI
            int _SetMaxLetters(lua_State*);
            int _SetMultiLine(lua_State*);
            int _SetNumber(lua_State*);
            int _SetNumeric(lua_State*);
            int _SetPassword(lua_State*);
            int _SetText(lua_State*);
            int _SetTextInsets(lua_State*);
            int _ToggleInputLanguage(lua_State*) { return 0; } // WBI

            static const char className[];
            static const char* classList[];
            static Lunar<LuaEditBox>::RegType methods[];
            static const s_str CLASS_NAME;

        protected :

            s_ptr<EditBox> pEditBoxParent_;

        };

        /** \endcond
        */
    }
}

#endif
