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
    enum MaterialType
    {
        MATERIAL_2D,
        MATERIAL_2D_PLAIN,
        MATERIAL_2D_RT,
        MATERIAL_3D,
        MATERIAL_3D_PLAIN,
        MATERIAL_UNKNOWN
    };

    /// A wrapper around Ogre's material
    class Material
    {
    public :

        /// Constructor.
        Material(const s_uint& uiID, MaterialType mType, s_ptr<Ogre::Material> pOgreMat);

        /// Destructor.
        ~Material();

        /// Sets whether alpha rejecting should be on or off.
        /** \param bEnable 'true' to enable
        *   \note Only works for 3D materials.
        */
        void            SetAlphaReject(const s_bool& bEnable);

        /// Sets whether hardware skinning should be on or off.
        /** \param bEnable 'true' to enable
        *   \note Only works for 3D materials.
        */
        void            SetHardwareSkinning(const s_bool& bEnable);

        /// Tiles the texture.
        /** \param fTileFactorH The horizontal tilling
        *   \param fTileFactorV The vertical tilling
        *   \note A factor of 1.0f doesn't change anything.<br>
        *         A factor of 2.0f shows only half of the texuture.<br>
        *         A factor of 0.5f shows the texture twice.<br><br>
        *         Mostly used for terrain textures.
        */
        void            SetTilling(const s_float& fTileFactorH, const s_float& fTileFactorV);

        /// Sets this Material's diffuse color.
        /** \param mColor The diffuse color
        */
        void            SetDiffuse(const Color& mColor);

        /// Sets this Material's self illumination color.
        /** \param mColor The self illumination color
        *   \note Alpha isn't taken into account
        */
        void            SetSelfIllumination(const Color& mColor);

        /// Sets this Material's ambient color.
        /** \param mColor The ambient color
        *   \note Alpha isn't taken into account
        */
        void            SetAmbient(const Color& mColor);

        /// Sets this Material's polygon render type.
        /** \param bWireframe 'true' if you only want to render
        *                     the polygons' edges, 'false' if you
        *                     want to render the model normally.
        */
        void            SetWireframe(const s_bool& bWireframe);

        /// Adds a new Decal to this Material.
        /** \param pDecal The new decal
        *   \return The new Decal, copied from the provided one.
        *   \note The created Decal is automatically shown.
        */
        s_ptr<Decal>    AddDecal(s_ptr<Decal> pDecal);

        /// Adds a new Decal to this Material.
        /** \param pDecal The new decal
        *   \return The new Decal, copied from the provided one.
        *   \note The created Decal is automatically shown.
        */
        s_ptr<Decal>    AddDecal(s_refptr<Decal> pDecal);

        /// Creates a new Decal and adds it to this Material.
        /** \param sTextureFile The file containing the texture to draw
        *   \return The Decal that was just created.
        *   \note You can use the returned Decal on another Material by calling
        *         AddDecal().<br>
        *         The created Decal is automatically shown.
        */
        s_ptr<Decal>    AddDecal(const s_str& sTextureFile);

        /// Removes a previously created Decal from this Material.
        /** \param pDecal The Decal to remove
        */
        void            RemoveDecal(s_ptr<Decal> pDecal);

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
        /** \return The wrapped Ogre::Material
        */
        s_ptr<Ogre::Material> GetOgreMaterial();

        /// Returns the name of the wrapped Ogre::Material.
        /** \return The name of the wrapped Ogre::Material
        */
        const s_str&    GetOgreMaterialName() const;

        /// Checks if this Material comes from a render target.
        /** \return 'true' if this Material comes from a
        *           render target
        */
        s_bool          IsRenderTarget() const;

        /// Checks if this Material has a texture.
        /** \return 'true' if this Material has no texture
        */
        s_bool          IsPlain() const;

        static const s_str CLASS_NAME;

        /// Creates a new Decal.
        /** \param sTextureFile The file containing the texture to draw
        *   \return The Decal that was just created.
        *   \note You can use the returned Decal on any Material by calling
        *         AddDecal().
        */
        static s_refptr<Decal> CreateDecal(const s_str& sTextureFile);

    private :

        s_ptr<Ogre::Pass> CreateDecalPass_();

        s_uint                 uiID_;
        MaterialType           mType_;
        s_ptr<Ogre::Material>  pOgreMat_;
        s_ptr<Ogre::Pass>      pDefaultPass_;
        s_str                  sName_;
        s_float                fWidth_;
        s_float                fHeight_;

        s_bool                 bAlphaReject_;
        s_bool                 bHardwareSkinning_;

        std::map< s_uint, s_ptr<Decal> > lDecalList_;
        s_uint                           uiDecalCounter_;
    };
}

#endif
