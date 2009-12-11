/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              GUI header                */
/*                                        */
/*                                        */


#ifndef FROST_GUI_STATUSBAR_H
#define FROST_GUI_STATUSBAR_H

#include "frost.h"
#include "gui/frost_frame.h"

namespace Frost
{
    namespace GUI
    {
        /// A variable length bar
        /** This is a very simple widget : you give it three
        *   values : a minimum and a maximum, and a value
        *   between those two. It then renders a bar that will
        *   be full, empty, or anything in between depending on
        *   the value.<br>
        *   Can be used to display health or cast bars.
        */
        class StatusBar : public Frame
        {
        public :

            enum Orientation
            {
                ORIENT_HORIZONTAL,
                ORIENT_VERTICAL
            };

            /// Constructor.
            StatusBar();

            /// Destructor.
            virtual ~StatusBar();

            /// Prints all relevant information about this widget in a string.
            /** \param sTab The offset to give to all lines
            *   \return All relevant information about this widget
            */
            virtual s_str  Serialize(const s_str& sTab) const;

            /// Returns 'true' if this StatusBar can use a script.
            /** \param sScriptName The name of the script
            *   \note This method can be overriden if needed.
            */
            virtual s_bool CanUseScript(const s_str& sScriptName) const;

            /// Copies an UIObject's parameters into this StatusBar (inheritance).
            /** \param pObj The UIObject to copy
            */
            virtual void   CopyFrom(s_ptr<UIObject> pObj);

            /// Sets this StatusBar's minimum value.
            /** \param fMin The minimum value
            */
            void           SetMinValue(const s_float& fMin);

            /// Sets this StatusBar's maximum value.
            /** \param fMax The maximum value
            */
            void           SetMaxValue(const s_float& fMax);

            /// Sets this StatusBar's value range.
            /** \param fMin The minimum value
            *   \param fMax The maximum value
            */
            void           SetMinMaxValues(const s_float& fMin, const s_float& fMax);

            /// Sets this StatusBar's value.
            /** \param fValue The value
            */
            void           SetValue(const s_float& fValue);

            /// Sets the draw layer of this StatusBar's bar texture.
            /** \param mBarLayer The layer
            */
            void           SetBarDrawLayer(LayerType mBarLayer);

            /// Sets the draw layer of this StatusBar's bar texture.
            /** \param sBarLayer The layer
            */
            void           SetBarDrawLayer(const s_str& sBarLayer);

            /// Sets this StatusBar's bar texture.
            /** \param pBarTexture The bar texture
            */
            void           SetBarTexture(s_ptr<Texture> pBarTexture);

            /// Sets this StatusBar's bar color.
            /** \param pBarTexture The bar color
            */
            void           SetBarColor(const Color& mBarColor);

            /// Sets this StatusBar's orientation.
            /** \param mOrient The orientation
            */
            void           SetOrientation(Orientation mOrient);

            /// Returns this StatusBar's minimum value.
            /** \return This StatusBar's minimum value
            */
            const s_float& GetMinValue() const;

            /// Returns this StatusBar's maximum value.
            /** \return This StatusBar's maximum value
            */
            const s_float& GetMaxValue() const;

            /// Returns this StatusBar's value.
            /** \return This StatusBar's value
            */
            const s_float& GetValue() const;

            /// Returns the draw layer of StatusBar's bar texture.
            /** \return The draw layer of StatusBar's bar texture
            */
            LayerType      GetBarDrawLayer() const;

            /// Returns this StatusBar's bar texture.
            /** \return This StatusBar's bar texture
            */
            s_ptr<Texture> GetBarTexture() const;

            /// Returns this StatusBar's bar color.
            /** \return This StatusBar's bar color
            */
            const Color&   GetBarColor() const;

            /// Returns this StatusBar's orientation.
            /** \return This StatusBar's orientation
            */
            Orientation    GetOrientation() const;

            /// Returns this widget's Lua glue.
            virtual void   CreateGlue();

            /// Parses data from an XML::Block.
            /** \param pBlock The StatusBar's XML::Block
            */
            virtual void   ParseBlock(s_ptr<XML::Block> pBlock);

            /// Updates this widget's logic.
            virtual void   Update();

            static const s_str CLASS_NAME;

        protected :

            s_ptr<Texture> CreateBarTexture_();
            void           FireUpdateBarTexture_();

            s_bool bUpdateBarTexture_;

            Orientation mOrientation_;

            s_float fValue_;
            s_float fMinValue_;
            s_float fMaxValue_;

            Color          mBarColor_;
            LayerType      mBarLayer_;
            s_ptr<Texture> pBarTexture_;

        };

        /** \cond NOT_REMOVE_FROM_DOC
        */

        class LuaStatusBar : public LuaFrame
        {
        public :

            LuaStatusBar(lua_State* pLua);

            // Glues
            int _GetMinMaxValues(lua_State*);
            int _GetOrientation(lua_State*);
            int _GetStatusBarColor(lua_State*);
            int _GetStatusBarTexture(lua_State*);
            int _GetValue(lua_State*);
            int _SetMinMaxValues(lua_State*);
            int _SetOrientation(lua_State*);
            int _SetStatusBarColor(lua_State*);
            int _SetStatusBarTexture(lua_State*);
            int _SetValue(lua_State*);

            static const char className[];
            static const char* classList[];
            static Lunar<LuaStatusBar>::RegType methods[];
            static const s_str CLASS_NAME;

        protected :

            s_ptr<StatusBar> pStatusBarParent_;

        };

        /** \endcond
        */
    }
}

#endif
