#ifndef FROST_DECAL_H
#define FROST_DECAL_H

#include "frost.h"
#include "scene/frost_movableobject.h"

#include <frost_utils_color.h>

namespace Frost
{

    /// A movable texture projection.
    /** This class can be used to display a selection
    *   indicator on the ground, fake shadows, any many
    *   other things.
    */
    class Decal : public MovableObject
    {
    public :

        enum Projection
        {
            PROJ_PERSPECTIVE,
            PROJ_ORTHOGRAPHIC
        };

        /// Default constructor.
        /** \param sTextureFile The file containing the texture to draw
        */
        Decal(const s_str& sTextureFile);

        /// Copy constructor.
        /** \param mDecal   The base decal to copy
        */
        Decal(const Decal& mDecal);

        /// Destructor.
        virtual ~Decal();

        /// Makes this Decal appear on the provided Material.
        /** \param pMat The material
        */
        void                    AddMaterial(s_wptr<Material> pMat);

        /// Removes this Decal from the provided Material.
        /** \param pMat The material
        *   \note If you want to temporarily hide it, use the Hide() function.
        */
        void                    RemoveMaterial(s_wptr<Material> pMat);

        /// Makes this Decal visible on its Material.
        void                    Show(s_wptr<Material> pMat = nullptr);

        /// Makes this Decal invisible on its Material.
        void                    Hide(s_wptr<Material> pMat = nullptr);

        /// Checks if this Decal is shown.
        /** \return 'true' if this Decal is shown
        */
        const s_bool&           IsShown() const;

        /// Sets the texture file to project.
        /** \param sFile The texture file
        */
        void                    SetTextureFile(const s_str& sFile);

        /// Returns this Decal's texture file name.
        /** \return This Decal's texture file name
        */
        const s_str&            GetTextureFile() const;

        /// Sets the scale of the projection.
        /** \param fScale The scale of the projection
        */
        void                    SetScale(const s_float& fScale);

        /// Returns the scale of the projection.
        /** \return The scale of the projection
        */
        const s_float&          GetScale() const;

        /// Sets this Decal's diffuse color.
        /** \param mColor The diffuse color
        */
        void                    SetDiffuse(const Color& mColor);

        /// Returns this Decal's color.
        /** \return This Decal's color
        */
        const Color&            GetDiffuse() const;

        /// Sets this Decal's self illumination color.
        /** \param mColor The self illumination color
        *   \note Alpha isn't taken into account
        */
        void                    SetSelfIllumination(const Color& mColor);

        /// Returns this Decal's self illumination color.
        /** \return This Decal's self illumination color
        */
        const Color&            GetSelfIllumination() const;

        /// Sets this Decal's ambient color.
        /** \param mColor The ambient color
        *   \note Alpha isn't taken into account
        */
        void                    SetAmbient(const Color& mColor);

        /// Returns this Decal's ambient color.
        /** \return This Decal's ambient color
        */
        const Color&            GetAmbient() const;

        /// Set the Decal's projection mode.
        /** \param mProjection The new projection mode
        */
        void                    SetProjection(const Projection& mProjection);

        /// Returns the Decal's projection mode.
        /** \return The Decal's projection mode
        */
        const Projection&       GetProjection() const;

        /// Returns this Decal's ID.
        /** \return This Decal's ID
        *   \note This ID is unique for the associated
        *         Material only.
        */
        const s_uint&           GetID() const;

        /// Returns this Decal's Ogre Frustum.
        /** \return This Decal's Ogre Frustum
        */
        s_refptr<Ogre::Frustum> GetOgreFrustum();

        static s_str CLASS_NAME;

    private :

        struct MaterialInfo
        {
            s_wptr<Material>              pMat;
            s_ptr<Ogre::Material>         pOgreMat;
            s_ptr<Ogre::Pass>             pOgrePass;
            s_ptr<Ogre::TextureUnitState> pTUS;
            s_bool                        bShown;
        };

        s_map<s_uint, MaterialInfo>   lMaterialList_;
        s_refptr<Ogre::Frustum>       pOgreFrustum_;
        s_float                       fScale_;
        s_str                         sTextureFile_;
        s_bool                        bShown_;
        Projection                    mProjection_;
        Color                         mDiffuse_;
        Color                         mSelfIllum_;
        Color                         mAmbient_;

    };
}

#endif
