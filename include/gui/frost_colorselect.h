/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              GUI header                */
/*                                        */
/*                                        */


#ifndef FROST_GUI_COLORSELECT_H
#define FROST_GUI_COLORSELECT_H

#include "frost.h"
#include "gui/frost_frame.h"

namespace Frost
{
    namespace GUI
    {
        class ColorSelect : public Frame
        {
        public :

            /// Constructor.
            ColorSelect();

            /// Destructor.
            virtual ~ColorSelect();

            /// Returns this widget's Lua glue.
            virtual void CreateGlue();

            /// Parses data from an XML::Block.
            /** \param pBlock The ColorSelect's XML::Block
            */
            virtual void ParseBlock(s_ptr<XML::Block> pBlock);

            static const s_str CLASS_NAME;

        protected :

        };

        /** \cond NOT_REMOVE_FROM_DOC
        */

        class LuaColorSelect : public LuaFrame
        {
        public :

            LuaColorSelect(lua_State* pLua);

            // Glues

            static const char className[];
            static const char* classList[];
            static Lunar<LuaColorSelect>::RegType methods[];
            static const s_str CLASS_NAME;

        protected :

            s_ptr<ColorSelect> pColorSelectParent_;

        };

        /** \endcond
        */
    }
}

#endif
