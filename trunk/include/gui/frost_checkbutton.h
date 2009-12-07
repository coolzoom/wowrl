/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              GUI header                */
/*                                        */
/*                                        */


#ifndef FROST_GUI_CHECKBUTTON_H
#define FROST_GUI_CHECKBUTTON_H

#include "frost.h"
#include "gui/frost_button.h"

namespace Frost
{
    namespace GUI
    {
        class CheckButton : public Button
        {
        public :

            /// Constructor.
            CheckButton();

            /// Destructor.
            virtual ~CheckButton();

            /// Returns this widget's Lua glue.
            virtual void CreateGlue();

            /// Parses data from an XML::Block.
            /** \param pBlock The CheckButton's XML::Block
            */
            virtual void ParseBlock(s_ptr<XML::Block> pBlock);

            static const s_str CLASS_NAME;

        protected :

        };

        /** \cond NOT_REMOVE_FROM_DOC
        */

        class LuaCheckButton : public LuaButton
        {
        public :

            LuaCheckButton(lua_State* pLua);

            // Glues

            static const char className[];
            static const char* classList[];
            static Lunar<LuaCheckButton>::RegType methods[];
            static const s_str CLASS_NAME;

        protected :

            s_ptr<CheckButton> pCheckButtonParent_;

        };

        /** \endcond
        */
    }
}

#endif
