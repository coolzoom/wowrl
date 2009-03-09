/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              GUI header                */
/*                                        */
/*                                        */


#ifndef FROST_GUI_STATUSBAR_H
#define FROST_GUI_STATUSBAR_H

#include "frost.h"
#include "gui/frost_gui_frame.h"

namespace Frost
{
    namespace GUI
    {
        class StatusBar : public Frame
        {
        public :

            StatusBar();

            /// Returns this widget's Lua glue.
            virtual void        CreateGlue();

            static const s_str CLASS_NAME;

        protected :

        };

        /** \cond NOT_REMOVE_FROM_DOC
        */

        class LuaStatusBar : public LuaFrame
        {
        public :

            LuaStatusBar(lua_State* pLua);

            // Glues
            /**/ int _GetMinMaxValues(lua_State*) { return 0; }
            /**/ int _GetOrientation(lua_State*) { return 0; }
            /**/ int _GetStatusBarColor(lua_State*) { return 0; }
            /**/ int _GetStatusBarTexture(lua_State*) { return 0; }
            /**/ int _GetValue(lua_State*) { return 0; }
            /**/ int _SetMinMaxValues(lua_State*) { return 0; }
            /**/ int _SetOrientation(lua_State*) { return 0; }
            /**/ int _SetStatusBarColor(lua_State*) { return 0; }
            /**/ int _SetStatusBarTexture(lua_State*) { return 0; }
            /**/ int _SetValue(lua_State*) { return 0; }

            static const char className[];
            static Lunar<LuaStatusBar>::RegType methods[];
            static const s_str CLASS_NAME;

        protected :

            s_ptr<StatusBar> pStatusBarParent_;

        };

        /** \endcond
        */
    }
}

#endif
