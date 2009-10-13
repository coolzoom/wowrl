/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           TerrainChunk header          */
/*                                        */
/*                                        */


#ifndef FROST_TERRAINCHUNK_H
#define FROST_TERRAINCHUNK_H

#include "frost.h"

namespace Frost
{
    /// A 'terrain unit'.
    class TerrainChunk
    {
    public :

        /// Constructor.
        /** \param pZone The zone this chunk belongs to
        */
        TerrainChunk(s_ptr<Zone> pZone);

        /// Destructor.
        ~TerrainChunk();

        /// Loads this chunk.
        /** \note ... if it has not already be loaded.
        */
        void Load();

        /// Unloads this chunk.
        void Unload();

        /// Shows this chunk.
        void Show();

        /// Hides this chunk.
        void Hide();

        /// Checks if this chunk is shown.
        /** \return 'true' if this chunk is shown
        */
        s_bool IsShown();

        /// Sets this chunk's terrain file.
        /** \param sTerrainFile The terrain file
        */
        void SetTerrainFile(const s_str& sTerrainFile);

        /// Sets this chunk's plane parameters
        /** \param fXSize   The size of the plane on the X axis
        *   \param fZSize   The size of the plane on the Z axis
        *   \param fDensity The number of subdivision per world unit
        */
        void SetPlane(const s_float& fXSize, const s_float& fZSize, const s_float& fDensity);

        /// Sets this chunk's Material.
        /** \param pMat The Material to use
        */
        void SetMaterial(s_refptr<Material> pMat);

        /// Sets if this chunk should always be visible.
        /** \param bAlwaysVisible 'true' to always show it
        */
        void SetAlwaysVisible(const s_bool& bAlwaysVisible);

        /// Checks if this chunk is always visible.
        /** \return 'true' if this chunk is always visible
        */
        const s_bool& IsAlwaysVisible() const;

        /// Sets this chunk's position.
        /** \param mPosition The position of this chunk
        *   \note Position is used for rendering and visibility check.
        */
        void SetPosition(const Vector& mPosition);

        /// Sets this chunk's size.
        /** \param mSize The size of this chunk
        *   \note Size is only used for visibility check : it doesn't
        *         change this chunk's real size.
        */
        void SetSize(const Vector& mSize);

        /// Returns this chunk's position.
        /** \return This chunk's position
        */
        const Vector& GetPosition() const;

        /// Returns this chunk's size.
        /** \return This chunk's size
        */
        const Vector& GetSize() const;

        /// Returns the height of a point in the 2D space (X, Z).
        /** \param fX The X coordinate
        *   \param fZ The Z coordinate
        *   \return The height of this point
        */
        s_float       GetPointHeight(const s_float& fX, const s_float& fZ) const;

        /// Returns this chunk's unique ID.
        /** \return This chunk's unique ID
        */
        const s_uint& GetID() const;

        static const s_str CLASS_NAME;

    private :

        s_uint uiID_;
        s_ptr<Zone> pZone_;

        Vector mPosition_;
        Vector mSize_;
        s_bool bAlwaysVisible_;

        s_bool bLoaded_;
        s_bool bShown_;

        s_bool  bPlane_;

        s_float fXSize_;
        s_float fZSize_;
        s_float fDensity_;

        s_str   sTerrainFile_;

        s_refptr<Material> pMat_;

        s_ptr<Obstacle>        pObstacle_;
        s_ptr<Ogre::Mesh>      pMesh_;
        s_ptr<Ogre::Entity>    pEntity_;
        s_ptr<Ogre::SceneNode> pNode_;
    };
}

#endif
