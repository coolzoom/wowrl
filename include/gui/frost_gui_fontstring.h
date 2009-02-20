/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              GUI header                */
/*                                        */
/*                                        */


#ifndef FROST_GUI_FONTSTRING_H
#define FROST_GUI_FONTSTRING_H

#include "frost.h"
#include "gui/frost_gui_layeredregion.h"

namespace Frost
{
    namespace GUI
    {
        class FontString : public LayeredRegion
        {
        public :

            FontString();

            /// Returns this widget's Lua glue.
            virtual void        CreateGlue();

            static const s_str CLASS_NAME;

        protected :

        };

        class LuaFontString : public LuaLayeredRegion
        {
        public :

            LuaFontString(lua_State* pLua);

            // Glues
            // Inherits from FontInstance
            /**/ int GetFont_L(lua_State*) { return 0; }
            /**/ int GetFontObject_L(lua_State*) { return 0; }
            /**/ int GetJustifyH_L(lua_State*) { return 0; }
            /**/ int GetJustifyV_L(lua_State*) { return 0; }
            /**/ int GetShadowColor_L(lua_State*) { return 0; }
            /**/ int GetShadowOffset_L(lua_State*) { return 0; }
            /**/ int GetSpacing_L(lua_State*) { return 0; }
            /**/ int GetTextColor_L(lua_State*) { return 0; }
            /**/ int SetFont_L(lua_State*) { return 0; }
            /**/ int SetFontObject_L(lua_State*) { return 0; }
            /**/ int SetJustifyH_L(lua_State*) { return 0; }
            /**/ int SetJustifyV_L(lua_State*) { return 0; }
            /**/ int SetShadowColor_L(lua_State*) { return 0; }
            /**/ int SetShadowOffset_L(lua_State*) { return 0; }
            /**/ int SetSpacing_L(lua_State*) { return 0; }
            /**/ int SetTextColor_L(lua_State*) { return 0; }

            /**/ int CanNonSpaceWrap_L(lua_State*) { return 0; }
            /**/ int GetStringHeight_L(lua_State*) { return 0; }
            /**/ int GetStringWidth_L(lua_State*) { return 0; }
            /**/ int GetText_L(lua_State*) { return 0; }
            /**/ int SetAlphaGradient_L(lua_State*) { return 0; }
            /**/ int SetNonSpaceWrap_L(lua_State*) { return 0; }
            /**/ int SetText_L(lua_State*) { return 0; }
            /**/ int SetTextHeight_L(lua_State*) { return 0; }

            static const char className[];
            static Lunar<LuaFontString>::RegType methods[];
            static const s_str CLASS_NAME;

        protected :

            s_ptr<FontString> pFontStringParent_;

        };
    }
}

#endif
