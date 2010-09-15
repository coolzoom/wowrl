/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            Material header             */
/*                                        */
/*                                        */


#ifndef FROST_MATERIAL_H
#define FROST_MATERIAL_H

#include "frost.h"

namespace Frost
{
    /// A wrapper around Ogre's material
    class Material
    {
    public :

        enum Type
        {
            TYPE_2D,
            TYPE_2D_PLAIN,
            TYPE_2D_RT,
            TYPE_3D,
            TYPE_3D_PLAIN,
            TYPE_UNKNOWN
        };

        enum GetMaterialFlag
        {
            FLAG_NONE,                /// Has no special effect (the function is a regular getter)
            FLAG_TRANSFER_OWNERSHIP   /// Notifies the Material that it no longer has ownership of the Ogre::Material
        };

        /// Constructor.
        /** \param uiID     The unique ID associated to this Material
        *   \param mType    The underlying type of this Material
        *   \param pOgreMat The wrapped Ogre::Material
        *   \param bVanilla 'true' if this Material uses a shared Ogre::Material
        *   \note Materials flagged as "vanilla" use a pre-build Ogre::Material that
        *         is shared by other Materials. When a changes occur to a vanilla
        *         Material, it creates a new similar Ogre::Material and operates on it,
        *         keeping the original Ogre::Material unchanged.
        */
        Material(const s_uint& uiID, Type mType, s_ptr<Ogre::Material> pOgreMat, const s_bool& bVanilla = false);

        /// Destructor.
        ~Material();

        /// Sets whether alpha rejecting should be on or off for the current pass.
        /** \param bEnable 'true' to enable
        *   \note Only works for 3D materials.
        */
        void            SetAlphaReject(const s_bool& bEnable);

        /// Sets texture tiling for the current pass.
        /** \param fTileFactorH The horizontal tilling
        *   \param fTileFactorV The vertical tilling
        *   \note A factor of 1.0f doesn't change anything.<br>
        *         A factor of 2.0f shows only half of the texuture.<br>
        *         A factor of 0.5f shows the texture twice.<br><br>
        *         Mostly used for terrain textures.
        */
        void            SetTilling(const s_float& fTileFactorH, const s_float& fTileFactorV);

        /// Sets the current pass's diffuse color.
        /** \param mColor The diffuse color
        */
        void            SetDiffuse(const Color& mColor);

        /// Sets the current pass's self illumination color.
        /** \param mColor The self illumination color
        *   \note Alpha isn't taken into account
        */
        void            SetSelfIllumination(const Color& mColor);

        /// Sets the current pass's ambient color.
        /** \param mColor The ambient color
        *   \note Alpha isn't taken into account
        */
        void            SetAmbient(const Color& mColor);

        /// Sets the current pass's polygon render type.
        /** \param bWireframe 'true' if you only want to render the polygons' edges,
        *                     'false' if you want to render the model normally.
        */
        void            SetWireframe(const s_bool& bWireframe);

        /// Sets whether the current pass should read the depth buffer.
        /** \param bDepthCheck 'false' to disable depth checking
        *   \note Enabled by default.
        */
        void            SetDepthCheck(const s_bool& bDepthCheck);

        /// Sets whether the current pass should write to the depth buffer.
        /** \param bDepthWrite 'false' to disable depth writing
        *   \note Enabled by default.
        */
        void            SetDepthWrite(const s_bool& bDepthWrite);

        /// Returns this Material's width.
        /** \return This Material's width
        *   \note Doesn't work for plain materials.
        */
        const s_float&  GetWidth() const;

        /// Returns this Material's width.
        /** \return This Material's width
        *   \note Doesn't work for plain materials.
        */
        const s_float&  GetHeight() const;

        /// Returns the wrapped Ogre::Material.
        /** \param mFlag See GetMaterialFlag
        *   \return The wrapped Ogre::Material
        */
        s_ptr<Ogre::Material> GetOgreMaterial(GetMaterialFlag mFlag = FLAG_NONE);

        /// Sets vertex and pixel shaders to use for the default pass.
        /** \param sSName The shader name
        *   \note The vertex and pixel shaders must have the same name
        *         for this method to work.<br>
        *         Use "" to disable shaders.
        */
        void            SetShaders(const s_str& sSName);

        /// Sets the vertex shader to use for the default pass.
        /** \param pVS The vertex shader
        *   \note Use nullptr to remove the vertex shader
        */
        void            SetVertexShader(s_ptr<VertexShader> pVS);

        /// Sets the vertex shader to use for the default pass.
        /** \param sVSName The name of the vertex shader
        *   \note Use "" to remove the vertex shader
        */
        void            SetVertexShader(const s_str& sVSName);

        /// Removes the linked vertex shader and use the fixed pipeline.
        void            RemoveVertexShader();

        /// Sets the pixel shader to use for the default pass.
        /** \param pPS The pixel shader
        *   \note Use nullptr to remove the pixel shader
        */
        void            SetPixelShader(s_ptr<PixelShader> pPS);

        /// Sets the pixel shader to use for the default pass.
        /** \param sPSName The name of the pixel shader
        *   \note Use "" to remove the pixel shader
        */
        void            SetPixelShader(const s_str& sPSName);

        /// Removes the linked vertex shader and use the fixed pipeline.
        void            RemovePixelShader();

        /// Creates a new pass.
        /** \return The index of the new pass
        */
        s_uint          CreatePass();

        /// Sets a new default pass.
        /** \param uiIndex The index of the new pass
        */
        void            SetDefaultPass(const s_uint& uiIndex);

        /// Returns the default Ogre::Pass.
        /** \return The default Ogre::Pass
        *   \note This is simply the first pass.
        */
        s_ptr<Ogre::Pass> GetDefaultPass();

        /// Returns the name of the wrapped Ogre::Material.
        /** \param mFlag See GetMaterialFlag
        *   \return The name of the wrapped Ogre::Material
        */
        const s_str&    GetOgreMaterialName(GetMaterialFlag mFlag = FLAG_NONE) const;

        /// Checks if this Material comes from a render target.
        /** \return 'true' if this Material comes from a
        *           render target
        */
        s_bool          IsRenderTarget() const;

        /// Checks if this Material has a texture.
        /** \return 'true' if this Material has no texture
        */
        s_bool          IsPlain() const;

        /// Returns this Material's unique ID.
        /** \return This Material's unique ID
        */
        const s_uint&   GetID() const;

        static const s_str CLASS_NAME;

    private :

        void CheckVanilla_();

        struct PassInfo
        {
            s_ptr<Ogre::Pass> pPass;
            s_ptr<VertexShader> pVS;
            s_ptr<PixelShader>  pPS;

            s_bool bAlphaReject;
            s_bool bHardwareSkinning;
            s_bool bIsDesaturated;
            s_uint uiPassID;
        };

        s_uint  uiID_;
        Type    mType_;
        s_str   sName_;
        s_float fWidth_;
        s_float fHeight_;

        mutable s_bool bOwner_;
        mutable s_bool bVanilla_;

        s_ptr<Ogre::Material> pOgreMat_;
        s_ptr<PassInfo>       pDefaultPass_;

        s_ctnr<PassInfo> lPassList_;
    };
}

#endif
