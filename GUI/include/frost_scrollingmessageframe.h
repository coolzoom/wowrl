/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              GUI header                */
/*                                        */
/*                                        */


#ifndef FROST_GUI_SCROLLINGMESSAGEFRAME_H
#define FROST_GUI_SCROLLINGMESSAGEFRAME_H

#include <frost_utils.h>
#include "frost_frame.h"

namespace Frost
{
    namespace GUI
    {
        class ScrollingMessageFrame : public Frame
        {
        public :

            /// Constructor.
            ScrollingMessageFrame(s_ptr<GUIManager> pManager);

            /// Destructor.
            virtual ~ScrollingMessageFrame();

            /// Copies an UIObject's parameters into this ScrollingMessageFrame (inheritance).
            /** \param pObj The UIObject to copy
            */
            virtual void CopyFrom(s_ptr<UIObject> pObj);

            /// Returns this widget's Lua glue.
            virtual void CreateGlue();

            /// Parses data from an XML::Block.
            /** \param pBlock The ScrollingMessageFrame's XML::Block
            */
            virtual void ParseBlock(s_ptr<XML::Block> pBlock);

            static const s_str CLASS_NAME;

        protected :

        };

        /** \cond NOT_REMOVE_FROM_DOC
        */

        class LuaScrollingMessageFrame : public LuaFrame
        {
        public :

            LuaScrollingMessageFrame(lua_State* pLua);

            // Glues
            /**/ int _AddMessage(lua_State*) { return 0; }
            /**/ int _AtBottom(lua_State*) { return 0; }
            /**/ int _AtTop(lua_State*) { return 0; }
            /**/ int _Clear(lua_State*) { return 0; }
            /**/ int _GetCurrentLine(lua_State*) { return 0; }
            /**/ int _GetCurrentScroll(lua_State*) { return 0; }
            /**/ int _GetFadeDuration(lua_State*) { return 0; }
            /**/ int _GetFading(lua_State*) { return 0; }
            /**/ int _GetMaxLines(lua_State*) { return 0; }
            /**/ int _GetNumLinesDisplayed(lua_State*) { return 0; }
            /**/ int _GetNumMessages(lua_State*) { return 0; }
            /**/ int _GetTimeVisible(lua_State*) { return 0; }
            /**/ int _PageDown(lua_State*) { return 0; }
            /**/ int _PageUp(lua_State*) { return 0; }
            /**/ int _ScrollDown(lua_State*) { return 0; }
            /**/ int _ScrollToBottom(lua_State*) { return 0; }
            /**/ int _ScrollToTop(lua_State*) { return 0; }
            /**/ int _ScrollUp(lua_State*) { return 0; }
            /**/ int _SetFadeDuration(lua_State*) { return 0; }
            /**/ int _SetFading(lua_State*) { return 0; }
            /**/ int _SetMaxLines(lua_State*) { return 0; }
            /**/ int _SetScrollFromBottom(lua_State*) { return 0; }
            /**/ int _SetTimeVisible(lua_State*) { return 0; }
            /**/ int _UpdateColorByID(lua_State*) { return 0; }

            static const char className[];
            static const char* classList[];
            static Lunar<LuaScrollingMessageFrame>::RegType methods[];
            static const s_str CLASS_NAME;

        protected :

            s_ptr<ScrollingMessageFrame> pSMFParent_;

        };

        /** \endcond
        */
    }
}

#endif
