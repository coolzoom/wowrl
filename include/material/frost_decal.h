#ifndef FROST_DECAL_H
#define FROST_DECAL_H

#include "frost.h"
#include "scene/frost_movableobject.h"

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
        *   \param pOgreMat     The material on which the decal will be applied
        *   \note The last two parameters are here for the Material class to create its
        *         own decals. If you want to create a temporary decal, only use the first
        *         parameter.
        */
        Decal(const s_str& sTextureFile, s_ptr<Ogre::Material> pOgreMat = nullptr);

        /// Copy constructor.
        /** \param mDecal   The base decal to copy
        *   \param pOgreMat The material on which the decal will be applied
        *   \note You shouldn't have to call this.
        */
        Decal(const Decal& mDecal, s_ptr<Ogre::Material> pOgreMat);

        /// Destructor.
        virtual ~Decal();

        /// Makes this Decal visible on its Material.
        void                    Show();

        /// Makes this Decal invisible on its Material.
        void                    Hide();

        /// Checks if this Decal is shown.
        /** \return 'true' if this Decal is shown
        */
        const s_bool&           IsShown() const;

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
        Color                   GetDiffuse() const;

        /// Sets this Decal's self illumination color.
        /** \param mColor The self illumination color
        *   \note Alpha isn't taken into account
        */
        void                    SetSelfIllumination(const Color& mColor);

        /// Sets this Decal's ambient color.
        /** \param mColor The ambient color
        *   \note Alpha isn't taken into account
        */
        void                    SetAmbient(const Color& mColor);

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

        /// Returns the associated Material.
        /** \return The associated Material
        *   \note If none, nullptr is returned.
        */
        s_ptr<Ogre::Pass>       GetOgrePass();

        /// Returns this Decal's Ogre Frustum.
        /** \return This Decal's Ogre Frustum
        */
        s_refptr<Ogre::Frustum> GetOgreFrustum();

        /// Returns this Decal's texture file name.
        /** \return This Decal's texture file name
        */
        const s_str&            GetTextureFile() const;

        static s_str CLASS_NAME;

    private :

        Decal(const Decal& mDecal);

        s_ptr<Ogre::Material>         pOgreMat_;
        s_ptr<Ogre::Pass>             pOgrePass_;
        s_ptr<Ogre::TextureUnitState> pTUS_;
        s_uint                        uiTUSIndex_;
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
