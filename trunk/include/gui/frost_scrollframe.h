/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              GUI header                */
/*                                        */
/*                                        */


#ifndef FROST_GUI_SCROLLFRAME_H
#define FROST_GUI_SCROLLFRAME_H

#include "frost.h"
#include "gui/frost_frame.h"

namespace Frost
{
    namespace GUI
    {
        class ScrollFrame : public Frame
        {
        public :

            /// Constructor.
            ScrollFrame();

            /// Destructor.
            virtual ~ScrollFrame();

            /// Returns this widget's Lua glue.
            virtual void CreateGlue();

            /// Parses data from an XML::Block.
            /** \param pBlock The ScrollFrame's XML::Block
            */
            virtual void ParseBlock(s_ptr<XML::Block> pBlock);

            static const s_str CLASS_NAME;

        protected :

        };

        /** \cond NOT_REMOVE_FROM_DOC
        */

        class LuaScrollFrame : public LuaFrame
        {
        public :

            LuaScrollFrame(lua_State* pLua);

            // Glues

            static const char className[];
            static const char* classList[];
            static Lunar<LuaScrollFrame>::RegType methods[];
            static const s_str CLASS_NAME;

        protected :

            s_ptr<ScrollFrame> pScrollFrameParent_;

        };

        /** \endcond
        */
    }
}

#endif
