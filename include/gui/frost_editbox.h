/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              GUI header                */
/*                                        */
/*                                        */


#ifndef FROST_GUI_EDITBOX_H
#define FROST_GUI_EDITBOX_H

#include "frost.h"
#include "gui/frost_frame.h"

namespace Frost
{
    namespace GUI
    {
        class EditBox : public Frame
        {
        public :

            /// Constructor.
            EditBox();

            /// Destructor.
            virtual ~EditBox();

            /// Copies an UIObject's parameters into this EditBox (inheritance).
            /** \param pObj The UIObject to copy
            */
            virtual void CopyFrom(s_ptr<UIObject> pObj);

            /// Returns this widget's Lua glue.
            virtual void CreateGlue();

            /// Parses data from an XML::Block.
            /** \param pBlock The EditBox's XML::Block
            */
            virtual void ParseBlock(s_ptr<XML::Block> pBlock);

            static const s_str CLASS_NAME;

        protected :

        };

        /** \cond NOT_REMOVE_FROM_DOC
        */

        class LuaEditBox : public LuaFrame
        {
        public :

            LuaEditBox(lua_State* pLua);

            // Glues
            /**/ int _AddHistoryLine(lua_State*) { return 0; }
            /**/ int _ClearFocus(lua_State*) { return 0; }
            /**/ int _GetAltArrowKeyMode(lua_State*) { return 0; }
            /**/ int _GetBlinkSpeed(lua_State*) { return 0; }
            /**/ int _GetHistoryLines(lua_State*) { return 0; }
            /**/ int _GetInputLanguage(lua_State*) { return 0; }
            /**/ int _GetMaxBytes(lua_State*) { return 0; }
            /**/ int _GetMaxLetters(lua_State*) { return 0; }
            /**/ int _GetNumLetters(lua_State*) { return 0; }
            /**/ int _GetNumber(lua_State*) { return 0; }
            /**/ int _GetText(lua_State*) { return 0; }
            /**/ int _GetTextInsets(lua_State*) { return 0; }
            /**/ int _HighlithtText(lua_State*) { return 0; }
            /**/ int _Insert(lua_State*) { return 0; }
            /**/ int _IsAutoFocus(lua_State*) { return 0; }
            /**/ int _IsMultiLine(lua_State*) { return 0; }
            /**/ int _IsNumeric(lua_State*) { return 0; }
            /**/ int _IsPassword(lua_State*) { return 0; }
            /**/ int _SetAltArrowKeyMode(lua_State*) { return 0; }
            /**/ int _SetAutoFocus(lua_State*) { return 0; }
            /**/ int _SetBlinkSpeed(lua_State*) { return 0; }
            /**/ int _SetFocus(lua_State*) { return 0; }
            /**/ int _SetHistoryLines(lua_State*) { return 0; }
            /**/ int _SetMaxBytes(lua_State*) { return 0; }
            /**/ int _SetMaxLetters(lua_State*) { return 0; }
            /**/ int _SetMultiLine(lua_State*) { return 0; }
            /**/ int _SetNumber(lua_State*) { return 0; }
            /**/ int _SetNumeric(lua_State*) { return 0; }
            /**/ int _SetPassword(lua_State*) { return 0; }
            /**/ int _SetText(lua_State*) { return 0; }
            /**/ int _SetTextInsets(lua_State*) { return 0; }
            /**/ int _ToggleInputLanguage(lua_State*) { return 0; }

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
