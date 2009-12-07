/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              GUI header                */
/*                                        */
/*                                        */


#ifndef FROST_GUI_COOLDOWN_H
#define FROST_GUI_COOLDOWN_H

#include "frost.h"
#include "gui/frost_frame.h"

namespace Frost
{
    namespace GUI
    {
        class Cooldown : public Frame
        {
        public :

            /// Constructor.
            Cooldown();

            /// Destructor.
            virtual ~Cooldown();

            /// Returns this widget's Lua glue.
            virtual void CreateGlue();

            /// Parses data from an XML::Block.
            /** \param pBlock The Cooldown's XML::Block
            */
            virtual void ParseBlock(s_ptr<XML::Block> pBlock);

            static const s_str CLASS_NAME;

        protected :

        };

        /** \cond NOT_REMOVE_FROM_DOC
        */

        class LuaCooldown : public LuaFrame
        {
        public :

            LuaCooldown(lua_State* pLua);

            // Glues

            static const char className[];
            static const char* classList[];
            static Lunar<LuaCooldown>::RegType methods[];
            static const s_str CLASS_NAME;

        protected :

            s_ptr<Cooldown> pCooldownParent_;

        };

        /** \endcond
        */
    }
}

#endif
