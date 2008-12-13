#ifndef FROST_DECAL_H
#define FROST_DECAL_H

#include "frost.h"
#include "frost_movableobject.h"

namespace Frost
{
    class Decal : public MovableObject
    {
    public :

        /// Default constructor.
        /** \param sTextureFile The file containing the texture to draw
        *   \param uiID         The ID to give to that decal (must be unique in the Material)
        *   \param pOgrePass    The material pass on which the decal will be applied
        *   \note The last two parameters are here for the Material class to create its
        *         own decals. If you want to create a temporary decal, only use the first
        *         parameter.
        */
        Decal(const s_str& sTextureFile, const s_uint& uiID = s_uint::INF, s_ptr<Ogre::Pass> pOgrePass = NULL);

        /// Copy constructor.
        /** \param mDecal    The base decal to copy
        *   \param uiID      The ID to give to that decal (must be unique in the Material)
        *   \param pOgrePass The material pass on which the decal will be applied
        *   \note You shouldn't have to call this.
        */
        Decal(const Decal& mDecal, const s_uint& uiID, s_ptr<Ogre::Pass> pPass);

        /// Destructor.
        virtual ~Decal();

        /// Makes this Decal visible on its Material.
        void                    Show();

        /// Makes this Decal invisible on its Material.
        void                    Hide();

        /// Returns this Decal's ID.
        /** \return This Decal's ID
        *   \note This ID is unique for the associated
        *         Material only.
        */
        const s_uint&           GetID() const;

        /// Returns the associated Material.
        /** \return The associated Material
        *   \note If none, NULL is returned.
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

        s_uint uiID_;

        s_ptr<Ogre::Pass>             pOgrePass_;
        s_ptr<Ogre::TextureUnitState> pTUS_;
        s_uint                        uiTUSIndex_;
        s_refptr<Ogre::Frustum>       pOgreFrustum_;
        s_str                         sTextureFile_;
        s_bool                        bShown_;

    };
}

#endif
