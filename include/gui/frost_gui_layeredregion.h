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

            LayeredRegion(lua_State* pLua);

            LayerType GetDrawLayer();
            void SetDrawLayer(LayerType mLayer);

            // Glues
            /**/ int _GetDrawLayer(lua_State*) { return 0; }
            /**/ int _SetDrawLayer(lua_State*) { return 0; }

            static const char className[];
            static Lunar<LayeredRegion>::RegType methods[];
            static const s_str CLASS_NAME;

        protected :

        };
    }
}

#endif
