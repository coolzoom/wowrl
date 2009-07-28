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
        /// Abstract GUI renderable.
        /** \note I say "abstract", but nothing prevents you
        *         from instancing a LayeredRegion in your code.<br>
        *         Actually, it is the simplest derivate of
        *         UIObject, so if you just need a widget that has a
        *         position and a size (like Frame's title region), then
        *         this is the best choice.
        */
        class LayeredRegion : public UIObject
        {
        public :

            /// Constructor.
            LayeredRegion();

            /// Prints all relevant information about this widget in a string.
            /** \param sTab The offset to give to all lines
            *   \return All relevant information about this widget
            */
            s_str Serialize(const s_str& sTab) const;

            /// Renders this LayeredRegion.
            /** \note Does nothing.
            */
            void Render();

            /// Creates the associated Lua glue.
            void CreateGlue();

            /// Checks if the provided coordinates are inside this region.
            /** \param iX The horizontal coordinate
            *   \param iY The vertical coordinate
            *   \return 'true' if the provided coordinates are inside this region
            */
            s_bool IsInRegion(const s_int& iX, const s_int& iY);

            /// Returns this LayeredRegion's draw layer.
            /** \return this LayeredRegion's draw layer
            */
            LayerType GetDrawLayer();

            /// Sets this LayeredRegion's draw layer.
            /** \param mLayer The new layer
            */
            void SetDrawLayer(LayerType mLayer);

            /// Sets this LayeredRegion's draw layer.
            /** \param sLayer The new layer
            */
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
