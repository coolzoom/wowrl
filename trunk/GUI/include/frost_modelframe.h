/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              GUI header                */
/*                                        */
/*                                        */


#ifndef FROST_GUI_MODELFRAME_H
#define FROST_GUI_MODELFRAME_H

#include "frost.h"
#include "gui/frost_frame.h"

namespace Frost
{
    namespace GUI
    {
        /// A widget that can display a 3D model.
        class ModelFrame : public Frame
        {
        public :

            /// Constructor.
            ModelFrame();

            /// Destructor.
            virtual ~ModelFrame();

            /// Updates this widget's logic.
            virtual void   Update();

            /// Calls a script.
            /** \param sScriptName The name of the script
            *   \param pEvent      Stores scripts arguments
            */
            virtual void   On(const s_str& sScriptName, s_ptr<Event> pEvent = nullptr);

            /// Calls the OnEvent script.
            /** \param mEvent The Event that occured
            */
            virtual void   OnEvent(const Event& mEvent);

            /// Sets if this Frame can receive mouse input.
            /** \param bIsMouseEnabled 'true' to enable
            *   \param bAllowWorldInput 'true' to allow world input
            */
            virtual void   EnableMouse(const s_bool& bIsMouseEnabled, const s_bool& bAllowWorldInput = false);

            /// Sets if this Frame can receive mouse wheel input.
            /** \param bIsMouseWheelEnabled 'true' to enable
            */
            virtual void   EnableMouseWheel(const s_bool& bIsMouseWheelEnabled);

            /// Copies an UIObject's parameters into this ModelFrame (inheritance).
            /** \param pObj The UIObject to copy
            */
            virtual void   CopyFrom(s_ptr<UIObject> pObj);

            /// Updates the displayed model's animation.
            /** \param fDelta Time elapsed since last call
            */
            void           UpdateModelAnimation(const s_float& fDelta);

            /// Removes the currently displayed model.
            /** \note SetModelFile() automatically removes the model.
            */
            void           RemoveModel();

            /// Returns the current model direction.
            /** \return The current model direction
            */
            const Vector&  GetModelDirection() const;

            /// Checks if lighting is enabled.
            /** \return 'true' if lighting is enabled
            */
            const s_bool&  IsLightingEnabled() const;

            /// Returns the ambient light color.
            /** \return The ambient light color
            */
            const Color&   GetAmbientColor() const;

            /// Returns the directional light's direction.
            /** \return The directional light's direction
            */
            Vector         GetLightDirection() const;

            /// Returns the directional light's color.
            /** \return The directional light's color
            */
            Color          GetLightColor() const;

            /// Returns the currently displayed model file.
            /** \return The currently displayed model file
            */
            const s_str&   GetModelFile() const;

            /// Returns the current model scale.
            /** \return The current model scale
            */
            const s_float& GetModelScale() const;

            /// Returns the current model position.
            /** \return The current model position
            */
            const Vector&  GetModelPosition() const;

            /// Returns the animation being played.
            /** \return The animation being played
            */
            const s_uint&  GetModelAnimation() const;

            /// Hides all the model's submeshes.
            void           HideModel();

            /// Hides a particular sub mesh and all its sub entities.
            /** \param uiSubMeshID The ID of the submesh
            */
            void           HideSubMesh(const s_uint& uiSubMeshID);

            /// Hides a particular sub mesh's sub entity.
            /** \param uiSubMeshID   The ID of the submesh
            *   \param uiSubEntityID The ID of the subentity
            */
            void           HideSubEntity(const s_uint& uiSubMeshID, const s_uint& uiSubEntityID);

            /// Sets the direction the model should face.
            /** \param mDirection The direction
            */
            void           SetModelDirection(const Vector& mDirection);

            /// Enables lighting on the model.
            /** \param bLightingEnabled 'true' to enable lighting
            *   \note Enabled by default. When disabled, the model is rendered
            *         as if ambient color was completely white.
            */
            void           EnableLighting(const s_bool& bLightingEnabled);

            /// Sets the ambient light color.
            /** \param mAmbientColor The ambient color
            */
            void           SetAmbientColor(const Color& mAmbientColor);

            /// Sets the direction of the directional light.
            /** \param mLightDirection The direction (distance doesn't count)
            */
            void           SetLightDirection(const Vector& mLightDirection);

            /// Sets the light color of the directional light.
            /** \param mLightColor The light color
            */
            void           SetLightColor(const Color& mLightColor);

            /// Sets the model file to render.
            /** \param sFile The model file to render
            *   \return 'false' if the model couldn't be loaded
            *   \note Loads the provided model file, and adjusts the camera
            *         so the model fits more or less into the ModelFrame.
            */
            s_bool         SetModelFile(const s_str& sFile);

            /// Sets the scale at which the model must be rendered.
            /** \param fScale The scale
            */
            void           SetModelScale(const s_float& fScale);

            /// Sets the position at which to render the model.
            /** \param mPosition The position
            */
            void           SetModelPosition(const Vector& mPosition);

            /// Sets the animation that the model should play.
            /** \param uiAnimID The animation ID (see AnimID)
            *   \note The actually animate the model, you have to
            *         call UpdateModelAnimation() or SetModelAnimationTime().
            */
            void           SetModelAnimation(const s_uint& uiAnimID);

            /// Sets the absolute time stamps at which to play the model's animation.
            /** \param fTime The time
            */
            void           SetModelAnimationTime(const s_float& fTime);

            /// Sets the texture to display on the whole model.
            /** \param sFile        The texture file
            *   \param bAlphaReject 'true' to enable alpha reject (see Material::SetAlphaReject())
            *   \note Replace the color set with SetModelTexture(const Color& mColor).
            */
            void           SetModelTexture(const s_str& sFile, const s_bool& bAlphaReject = false);

            /// Sets the color to apply to the whole model.
            /** \param mColor The color
            *   \note Replaces the texture set with SetModelTexture(const s_str& sFile,
            *         const s_bool& bAlphaReject = false).
            */
            void           SetModelTexture(const Color& mColor);

            /// Sets the texture to display on a submesh of the model.
            /** \param uiSubMeshID  The ID of the submesh
            *   \param sFile        The texture file
            *   \param bAlphaReject 'true' to enable alpha reject (see Material::SetAlphaReject())
            *   \note Replace the color set with SetSubMeshTexture(const Color& mColor).
            */
            void           SetSubMeshTexture(
                const s_uint& uiSubMeshID, const s_str& sFile, const s_bool& bAlphaReject = false
            );

            /// Sets the color to apply to a submesh of the model.
            /** \param uiSubMeshID The ID of the submesh
            *   \param mColor      The color
            *   \note Replaces the texture set with SetSubMeshTexture(const s_str& sFile,
            *         const s_bool& bAlphaReject = false).
            */
            void           SetSubMeshTexture(const s_uint& uiSubMeshID, const Color& mColor);

            /// Sets the texture to display on a subentity of a submesh.
            /** \param uiSubMeshID   The ID of the submesh
            *   \param uiSubEntityID The ID of the subentity
            *   \param sFile         The texture file
            *   \param bAlphaReject  'true' to enable alpha reject (see Material::SetAlphaReject())
            *   \note Replace the color set with SetSubEntityTexture(const Color& mColor).
            */
            void           SetSubEntityTexture(
                const s_uint& uiSubMeshID, const s_uint& uiSubEntityID,
                const s_str& sFile, const s_bool& bAlphaReject = false
            );

            /// Sets the color to apply to a subentity of a submesh.
            /** \param uiSubMeshID   The ID of the submesh
            *   \param uiSubEntityID The ID of the subentity
            *   \param mColor        The color
            *   \note Replaces the texture set with SetSubEntityTexture(const s_str& sFile,
            *         const s_bool& bAlphaReject = false).
            */
            void           SetSubEntityTexture(
                const s_uint& uiSubMeshID, const s_uint& uiSubEntityID, const Color& mColor
            );

            /// Shows all the model's submeshes.
            void           ShowModel();

            /// Shows a particular sub mesh and all its sub entities.
            /** \param uiSubMeshID The ID of the submesh
            */
            void           ShowSubMesh(const s_uint& uiSubMeshID);

            /// Shows a particular sub mesh's sub entity.
            /** \param uiSubMeshID   The ID of the submesh
            *   \param uiSubEntityID The ID of the subentity
            */
            void           ShowSubEntity(const s_uint& uiSubMeshID, const s_uint& uiSubEntityID);

            /// Returns the Model object displayed by this ModelFrame.
            /** \return The Model object displayed by this ModelFrame
            */
            s_wptr<Model>  GetModel();

            /// Returns this widget's Lua glue.
            virtual void   CreateGlue();

            /// Parses data from an XML::Block.
            /** \param pBlock The ModelFrame's XML::Block
            */
            virtual void   ParseBlock(s_ptr<XML::Block> pBlock);

            static const s_str CLASS_NAME;

        protected :

            void ParseMaterialsBlock_(s_ptr<XML::Block> pBlock);

            void UpdateRenderTarget_();
            void SetupCamera_();

            s_str           sModelFile_;
            s_float         fModelScale_;
            Vector          mModelPosition_;
            Vector          mModelDirection_;
            s_uint          uiModelAnimation_;
            s_refptr<Model> pModel_;

            s_ptr<Camera>             pCamera_;
            s_ptr<Ogre::SceneManager> pSceneManager_;

            s_bool       bLightingEnabled_;
            Color        mAmbientColor_;
            s_ptr<Light> pLight_;
            Color        mLightColor_;
            Vector       mLightDirection_;

            s_ptr<RenderTarget> pRenderTarget_;
            s_bool              bRedrawRenderTarget_;
            s_bool              bUpdateRenderTarget_;
            s_ptr<Texture>      pRenderTexture_;

        };

        /** \cond NOT_REMOVE_FROM_DOC
        */

        class LuaModelFrame : public LuaFrame
        {
        public :

            LuaModelFrame(lua_State* pLua);

            // Glues
            int _AdvanceTime(lua_State*);
            int _ClearModel(lua_State*);
            int _GetFacing(lua_State*);
            int _GetLight(lua_State*);
            int _GetModel(lua_State*);
            int _GetModelScale(lua_State*);
            int _GetPosition(lua_State*);
            int _GetAvailableSubMeshes(lua_State*);
            int _GetSubEntityNumber(lua_State*);
            int _HideModel(lua_State*);
            int _HideSubMesh(lua_State*);
            int _HideSubEntity(lua_State*);
            int _SetFacing(lua_State*);
            int _SetLight(lua_State*);
            int _SetModel(lua_State*);
            int _SetModelScale(lua_State*);
            int _SetModelTexture(lua_State*);
            int _SetPosition(lua_State*);
            int _SetSequence(lua_State*);
            int _SetSequenceTime(lua_State*);
            int _SetSubMeshTexture(lua_State*);
            int _SetSubEntityTexture(lua_State*);
            int _ShowModel(lua_State*);
            int _ShowSubMesh(lua_State*);
            int _ShowSubEntity(lua_State*);

            static const char className[];
            static const char* classList[];
            static Lunar<LuaModelFrame>::RegType methods[];
            static const s_str CLASS_NAME;

        protected :

            s_ptr<ModelFrame> pModelFrameParent_;

        };

        /** \endcond
        */
    }
}

#endif
