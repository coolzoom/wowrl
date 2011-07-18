/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              GUI header                */
/*                                        */
/*                                        */


#ifndef FROST_GUI_LAYEREDREGION_H
#define FROST_GUI_LAYEREDREGION_H

#include <frost_utils.h>
#include "frost_region.h"

namespace Frost
{
    namespace GUI
    {
        /// Abstract GUI renderable.
        /** \note Layered regions are contained inside Frames.<br>
        *         They are sorted by layers, hence their name.
        */
        class LayeredRegion : public Region
        {
        public :

            /// Constructor.
            LayeredRegion(s_ptr<GUIManager> pManager);

            /// Destructor.
            virtual ~LayeredRegion();

            /// Prints all relevant information about this widget in a string.
            /** \param sTab The offset to give to all lines
            *   \return All relevant information about this widget
            */
            virtual s_str  Serialize(const s_str& sTab) const;

            /// Creates the associated Lua glue.
            virtual void   CreateGlue();

            /// Changes this widget's parent.
            /** \param pParent The new parent
            *   \note Default is nullptr.
            */
            virtual void   SetParent(s_ptr<UIObject> pParent);

            /// Shows this widget.
            /** \note Its parent must be shown for it to appear on
            *         the screen.
            */
            virtual void   Show();

            /// Hides this widget.
            /** \note All its childs won't be visible on the screen
            *   anymore, even if they are still marked as shown.
            */
            virtual void   Hide();

            /// Checks if this widget can be seen on the screen.
            /** \return 'true' if this widget can be seen on the screen
            */
            virtual s_bool IsVisible() const;

            /// Returns this LayeredRegion's draw layer.
            /** \return this LayeredRegion's draw layer
            */
            LayerType      GetDrawLayer();

            /// Sets this LayeredRegion's draw layer.
            /** \param mLayer The new layer
            */
            virtual void   SetDrawLayer(LayerType mLayer);

            /// Sets this LayeredRegion's draw layer.
            /** \param sLayer The new layer
            */
            virtual void   SetDrawLayer(const s_str& sLayer);

            /// Notifies the renderer of this widget that it needs to be redrawn.
            /** \note Automatically called by any shape changing function.
            */
            virtual void   NotifyRendererNeedRedraw() const;

            /// Parses data from an XML::Block.
            /** \param pBlock The Frame's XML::Block
            */
            virtual void   ParseBlock(s_ptr<XML::Block> pBlock);

            static const s_str CLASS_NAME;

        protected :

            virtual void ParseAttributes_(s_ptr<XML::Block> pBlock);

            LayerType    mLayer_;
            s_ptr<Frame> pFrameParent_;

        };

        /** \cond NOT_REMOVE_FROM_DOC
        */

        class LuaLayeredRegion : public LuaUIObject
        {
        public :

            LuaLayeredRegion(lua_State* pLua);

            int _GetDrawLayer(lua_State*);
            int _SetDrawLayer(lua_State*);

            static const char className[];
            static const char* classList[];
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
