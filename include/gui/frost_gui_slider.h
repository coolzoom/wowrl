/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              GUI header                */
/*                                        */
/*                                        */


#ifndef FROST_GUI_SLIDER_H
#define FROST_GUI_SLIDER_H

#include "frost.h"
#include "gui/frost_gui_frame.h"

namespace Frost
{
    namespace GUI
    {
        class Slider : public Frame
        {
        public :

            Slider();

            /// Returns this widget's Lua glue.
            virtual void        CreateGlue();

            static const s_str CLASS_NAME;

        protected :

        };

        class LuaSlider : public LuaFrame
        {
        public :

            LuaSlider(lua_State* pLua);

            // Glues
            /**/ int GetMinMaxValues(lua_State*) { return 0; }
            /**/ int GetOrientation(lua_State*) { return 0; }
            /**/ int GetThumbTexture(lua_State*) { return 0; }
            /**/ int GetValue(lua_State*) { return 0; }
            /**/ int GetValueStep(lua_State*) { return 0; }
            /**/ int SetMinMaxValues(lua_State*) { return 0; }
            /**/ int SetOrientation(lua_State*) { return 0; }
            /**/ int SetThumbTexture(lua_State*) { return 0; }
            /**/ int SetValue(lua_State*) { return 0; }
            /**/ int SetValueStep(lua_State*) { return 0; }

            static const char className[];
            static Lunar<LuaSlider>::RegType methods[];
            static const s_str CLASS_NAME;

        protected :

            s_ptr<Slider> pSliderParent_;

        };
    }
}

#endif
