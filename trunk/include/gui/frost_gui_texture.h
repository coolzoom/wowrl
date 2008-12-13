/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              GUI header                */
/*                                        */
/*                                        */


#ifndef FROST_GUI_TEXTURE_H
#define FROST_GUI_TEXTURE_H

#include "frost.h"
#include "gui/frost_gui_layeredregion.h"

namespace Frost
{
    namespace GUI
    {
        class Texture : public LayeredRegion
        {
        public :

            Texture(lua_State* pLua);

            // Glues
            /**/ int _GetBlendMode(lua_State*) { return 0; }
            /**/ int _GetTexCoord(lua_State*) { return 0; }
            /**/ int _GetTexCoordModifiesRect(lua_State*) { return 0; }
            /**/ int _GetTexture(lua_State*) { return 0; }
            /**/ int _GetVertexColor(lua_State*) { return 0; }
            /**/ int _GetDesaturated(lua_State*) { return 0; }
            /**/ int _SetBlendMode(lua_State*) { return 0; }
            /**/ int _SetDesaturated(lua_State*) { return 0; }
            /**/ int _SetGradient(lua_State*) { return 0; }
            /**/ int _SetGradientAlpha(lua_State*) { return 0; }
            /**/ int _SetTexCoord(lua_State*) { return 0; }
            /**/ int _SetTexCoordModifiesRect(lua_State*) { return 0; }
            /**/ int _SetTexture(lua_State*) { return 0; }
            /**/ int _SetVertexColor(lua_State*) { return 0; }

            static const char className[];
            static Lunar<Texture>::RegType methods[];
            static const s_str CLASS_NAME;

        protected :

        };
    }
}

#endif
