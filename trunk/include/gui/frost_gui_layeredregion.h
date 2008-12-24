/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              GUI header                */
/*                                        */
/*                                        */


#ifndef FROST_GUI_LAYEREDREGION_H
#define FROST_GUI_LAYEREDREGION_H

#include "frost.h"
#include "gui/frost_gui_uiobject.h"

namespace Frost
{
    namespace GUI
    {
        class LayeredRegion : public UIObject
        {
        public :

            LayeredRegion();

            LayerType GetDrawLayer();
            void SetDrawLayer(LayerType mLayer);

            static const s_str CLASS_NAME;

        protected :

        };

        class LuaLayeredRegion : public LuaUIObject
        {
        public :

            LuaLayeredRegion(lua_State* pLua);

            /**/ int _GetDrawLayer(lua_State*) { return 0; }
            /**/ int _SetDrawLayer(lua_State*) { return 0; }

            static const char className[];
            static Lunar<LuaLayeredRegion>::RegType methods[];
            static const s_str CLASS_NAME;

        protected :

            s_ptr<LayeredRegion> pLayeredRegionParent_;

        };
    }
}

#endif
