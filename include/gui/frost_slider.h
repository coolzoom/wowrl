/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              GUI header                */
/*                                        */
/*                                        */


#ifndef FROST_GUI_SLIDER_H
#define FROST_GUI_SLIDER_H

#include "frost.h"
#include "gui/frost_frame.h"

namespace Frost
{
    namespace GUI
    {
        class Slider : public Frame
        {
        public :

            /// Constructor.
            Slider();

            /// Destructor.
            virtual ~Slider();

            /// Returns this widget's Lua glue.
            virtual void CreateGlue();

            /// Parses data from an XML::Block.
            /** \param pBlock The Slider's XML::Block
            */
            virtual void ParseBlock(s_ptr<XML::Block> pBlock);

            static const s_str CLASS_NAME;

        protected :

        };

        /** \cond NOT_REMOVE_FROM_DOC
        */

        class LuaSlider : public LuaFrame
        {
        public :

            LuaSlider(lua_State* pLua);

            // Glues
            /**/ int _GetMinMaxValues(lua_State*) { return 0; }
            /**/ int _GetOrientation(lua_State*) { return 0; }
            /**/ int _GetThumbTexture(lua_State*) { return 0; }
            /**/ int _GetValue(lua_State*) { return 0; }
            /**/ int _GetValueStep(lua_State*) { return 0; }
            /**/ int _SetMinMaxValues(lua_State*) { return 0; }
            /**/ int _SetOrientation(lua_State*) { return 0; }
            /**/ int _SetThumbTexture(lua_State*) { return 0; }
            /**/ int _SetValue(lua_State*) { return 0; }
            /**/ int _SetValueStep(lua_State*) { return 0; }

            static const char className[];
            static const char* classList[];
            static Lunar<LuaSlider>::RegType methods[];
            static const s_str CLASS_NAME;

        protected :

            s_ptr<Slider> pSliderParent_;

        };

        /** \endcond
        */
    }
}

#endif
