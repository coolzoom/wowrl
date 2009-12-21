/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              GUI header                */
/*                                        */
/*                                        */


#ifndef FROST_GUI_SLIDER_H
#define FROST_GUI_SLIDER_H

#include "frost.h"
#include "gui/frost_frame.h"

namespace Frost
{
    namespace GUI
    {
        class Slider : public Frame
        {
        public :

            enum Orientation
            {
                ORIENT_VERTICAL,
                ORIENT_HORIZONTAL
            };

            /// Constructor.
            Slider();

            /// Destructor.
            virtual ~Slider();

            /// Prints all relevant information about this widget in a string.
            /** \param sTab The offset to give to all lines
            *   \return All relevant information about this widget
            */
            virtual s_str  Serialize(const s_str& sTab) const;

            /// Returns 'true' if this Slider can use a script.
            /** \param sScriptName The name of the script
            *   \note This method can be overriden if needed.
            */
            virtual s_bool CanUseScript(const s_str& sScriptName) const;

            /// Copies an UIObject's parameters into this Slider (inheritance).
            /** \param pObj The UIObject to copy
            */
            virtual void   CopyFrom(s_ptr<UIObject> pObj);

            /// Sets the Texture to use for the thumb.
            /** \param pTexture The new Texture
            */
            void           SetThumbTexture(s_ptr<Texture> pTexture);

            /// Returns the Texture used for the thumb.
            /** \return The Texture used for the thumb
            */
            s_ptr<Texture> GetThumbTexture() const;

            /// Sets the orientation of this Slider.
            /** \param mOrientation The orientation of this Slider
            */
            void           SetOrientation(Orientation mOrientation);

            /// Returns the orientation of this Slider.
            /** \return The orientation of this Slider
            */
            Orientation    GetOrientation() const;

            /// Sets the Slider's value range.
            /** \param fMin The minimum value
            *   \param fMax The maximum value
            */
            void           SetMinMaxValues(const s_float& fMin, const s_float& fMax);

            /// Sets this Slider's minimum value.
            /** \param fMin The minimum value
            */
            void           SetMinValue(const s_float& fMin);

            /// Sets this Slider's maximum value.
            /** \param fMax The maximum value
            */
            void           SetMaxValue(const s_float& fMax);

            /// Returns this Slider's minimum value.
            /** \return This Slider's minimum value
            */
            const s_float& GetMinValue() const;

            /// Returns this Slider's maximum value.
            /** \return This Slider's maximum value
            */
            const s_float& GetMaxValue() const;

            /// Sets this Slider's value.
            /** \param fValue  The value
            *   \param bSilent 'true' to prevent OnValueChanged to be fired
            */
            void           SetValue(const s_float& fValue, const s_bool& bSilent = false);

            /// Returns this Slider's value.
            /** \return This Slider's value
            */
            const s_float& GetValue() const;

            /// Sets this Slider's value step.
            /** \param fValueStep The new step
            */
            void           SetValueStep(const s_float& fValueStep);

            /// Returns this Slider's value step.
            /** \return This Slider's value step
            */
            const s_float& GetValueStep() const;

            /// Sets the draw layer of this Slider's thumb texture.
            /** \param mThumbLayer The layer
            */
            void           SetThumbDrawLayer(LayerType mThumbLayer);

            /// Sets the draw layer of this Slider's thumb texture.
            /** \param sBarLayer The layer
            */
            void           SetThumbDrawLayer(const s_str& sBarLayer);

            /// Returns the draw layer of this Slider's thumb texture.
            /** \return The draw layer of this Slider's thumb texture
            */
            LayerType      GetThumbDrawLayer() const;

            /// Checks if the provided coordinates are in the Frame.
            /** \param iX           The horizontal coordinate
            *   \param iY           The vertical coordinate
            *   \return 'true' if the provided coordinates are in the Frame
            *   \note The Slider version of this function also checks if the
            *         mouse is over the thumb texture.
            */
            virtual s_bool IsInFrame(const s_int& iX, const s_int& iY) const;

            /// Tells this Frame it is being overed by the mouse.
            /** \param bMouseInFrame 'true' if the mouse is above this Frame
            *   \param iX            The horizontal mouse coordinate
            *   \param iY            The vertical mouse coordinate
            */
            virtual void   NotifyMouseInFrame(const s_bool& bMouseInFrame, const s_int& iX, const s_int& iY);

            /// Calls the OnEvent script.
            /** \param mEvent The Event that occured
            */
            virtual void   OnEvent(const Event& mEvent);

            /// Returns this widget's Lua glue.
            virtual void   CreateGlue();

            /// Parses data from an XML::Block.
            /** \param pBlock The Slider's XML::Block
            */
            virtual void   ParseBlock(s_ptr<XML::Block> pBlock);

            /// Updates this widget's logic.
            virtual void   Update();

            static const s_str CLASS_NAME;

        protected :

            s_ptr<Texture> CreateThumbTexture_();
            void           FireUpdateThumbTexture_();

            virtual void UpdateBorders_();

            s_bool bUpdateThumbTexture_;

            Orientation mOrientation_;

            s_float fValue_;
            s_float fMinValue_;
            s_float fMaxValue_;
            s_float fValueStep_;

            LayerType      mThumbLayer_;
            s_ptr<Texture> pThumbTexture_;
            s_bool         bThumbMoved_;
            s_bool         bMouseInThumb_;

        };

        /** \cond NOT_REMOVE_FROM_DOC
        */

        class LuaSlider : public LuaFrame
        {
        public :

            LuaSlider(lua_State* pLua);

            // Glues
            int _GetMinMaxValues(lua_State*);
            int _GetOrientation(lua_State*);
            int _GetThumbTexture(lua_State*);
            int _GetValue(lua_State*);
            int _GetValueStep(lua_State*);
            int _SetMinMaxValues(lua_State*);
            int _SetOrientation(lua_State*);
            int _SetThumbTexture(lua_State*);
            int _SetValue(lua_State*);
            int _SetValueStep(lua_State*);

            static const char className[];
            static const char* classList[];
            static Lunar<LuaSlider>::RegType methods[];
            static const s_str CLASS_NAME;

        protected :

            s_ptr<Slider> pSliderParent_;

        };

        /** \endcond
        */
    }
}

#endif
