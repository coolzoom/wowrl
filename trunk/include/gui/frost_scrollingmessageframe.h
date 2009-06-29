/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              GUI header                */
/*                                        */
/*                                        */


#ifndef FROST_GUI_SCROLLINGMESSAGEFRAME_H
#define FROST_GUI_SCROLLINGMESSAGEFRAME_H

#include "frost.h"
#include "gui/frost_frame.h"

namespace Frost
{
    namespace GUI
    {
        class ScrollingMessageFrame : public Frame
        {
        public :

            ScrollingMessageFrame();

            /// Returns this widget's Lua glue.
            void CreateGlue();

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
            /**/ int AddMessage_L(lua_State*) { return 0; }
            /**/ int AtBottom_L(lua_State*) { return 0; }
            /**/ int AtTop_L(lua_State*) { return 0; }
            /**/ int Clear_L(lua_State*) { return 0; }
            /**/ int GetCurrentLine_L(lua_State*) { return 0; }
            /**/ int GetCurrentScroll_L(lua_State*) { return 0; }
            /**/ int GetFadeDuration_L(lua_State*) { return 0; }
            /**/ int GetFading_L(lua_State*) { return 0; }
            /**/ int GetMaxLines_L(lua_State*) { return 0; }
            /**/ int GetNumLinesDisplayed_L(lua_State*) { return 0; }
            /**/ int GetNumMessages_L(lua_State*) { return 0; }
            /**/ int GetTimeVisible_L(lua_State*) { return 0; }
            /**/ int PageDown_L(lua_State*) { return 0; }
            /**/ int PageUp_L(lua_State*) { return 0; }
            /**/ int ScrollDown_L(lua_State*) { return 0; }
            /**/ int ScrollToBottom_L(lua_State*) { return 0; }
            /**/ int ScrollToTop_L(lua_State*) { return 0; }
            /**/ int ScrollUp_L(lua_State*) { return 0; }
            /**/ int SetFadeDuration_L(lua_State*) { return 0; }
            /**/ int SetFading_L(lua_State*) { return 0; }
            /**/ int SetMaxLines_L(lua_State*) { return 0; }
            /**/ int SetScrollFromBottom_L(lua_State*) { return 0; }
            /**/ int SetTimeVisible_L(lua_State*) { return 0; }
            /**/ int UpdateColorByID_L(lua_State*) { return 0; }

            static const char className[];
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
