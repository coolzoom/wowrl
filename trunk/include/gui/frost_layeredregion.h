/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              GUI header                */
/*                                        */
/*                                        */


#ifndef FROST_GUI_LAYEREDREGION_H
#define FROST_GUI_LAYEREDREGION_H

#include "frost.h"
#include "gui/frost_uiobject.h"

namespace Frost
{
    namespace GUI
    {
        class LayeredRegion : public UIObject
        {
        public :

            LayeredRegion();

            /// Prints all relevant information about this widget in a string.
            /** \param sTab The offset to give to all lines
            *   \return All relevant information about this widget
            */
            s_str Serialize(const s_str& sTab) const;
            void Render();
            void CreateGlue();

            s_bool IsInRegion(const s_int& iX, const s_int& iY);
            LayerType GetDrawLayer();
            void SetDrawLayer(LayerType mLayer);
            void SetDrawLayer(const s_str& sLayer);

            static const s_str CLASS_NAME;

        protected :

            LayerType mLayer_;

        };

        /** \cond NOT_REMOVE_FROM_DOC
        */

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

        /** \endcond
        */
    }
}

#endif
