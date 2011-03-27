/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               Zone header              */
/*                                        */
/*                                        */


#ifndef FROST_ZONE_H
#define FROST_ZONE_H

#include "frost.h"
#include <frost_xml.h>
#include <frost_utils_color.h>

namespace Frost
{
    /// Contains displayable objects.
    class Zone
    {
    public :

        /// Constructor.
        Zone();

        /// Constructor.
        /** \param sName The name of the Zone
        */
        Zone(const s_str& sName);

        /// Destructor.
        ~Zone();

        /// Sets this Zone's default ambient color.
        /** \param mColor The ambient color
        */
        void         SetAmbientColor(const Color& mColor);

        /// Returns this Zone's default ambient color.
        /** \return This Zone's default ambient color
        */
        const Color& GetAmbientColor() const;

        /// Sets this Zone's default sun color.
        /** \param mColor The sun color
        */
        void         SetSunColor(const Color& mColor);

        /// Returns this Zone's default sun color.
        /** \return This Zone's default sun color
        */
        const Color& GetSunColor() const;

        /// Adds a new chunk to this Zone.
        /** \param pChunk The new chunk
        *   \return The ID of the chunk
        *   \note Automatically called by TerrainChunk.
        */
        s_uint       AddChunk(s_ptr<TerrainChunk> pChunk);

        /// Adds a new light to this Zone.
        void         AddLight(const s_str& sLightName, s_ptr<Light> pLight);

        /// Adds a new Decal on the ground.
        /** \param pDecal The decal to put on the ground
        */
        void         AddDecal(s_wptr<Decal> pDecal);

        /// Removes a Decal from the ground.
        /** \param pDecal The decal to remove
        */
        void         RemoveDecal(s_wptr<Decal> pDecal);

        /// Creates a model and loads it.
        /** \param sName The name of the model to create
        *   \param sFile The file from which to load the model
        *   \return 'true' if everything went fine, 'false' if the model
        *           couldn't be loaded or if the provided name was already in use.
        */
        s_bool       RegisterModel(const s_str& sName, const s_str& sFile);

        /// Sets a default material for a model.
        /** \param sModelName The name of the model
        *   \param mMatInfo   Basic informations about the material
        */
        void         SetMaterialInfo(const s_str& sModelName, const ModelMaterial& mMatInfo);

        /// Adds a Doodad to the zone.
        /** \param sName      The name of the Doodad
        *   \param sModelName The name of the model to give to this Doodad
        *   \return The created Doodad
        *   \note Returns nullptr if the model couldn't be found or if the
        *         provided name was already in use.
        */
        s_ptr<Doodad> AddDoodad(const s_str& sName, const s_str& sModelName);

        /// Adds a Doodad to the zone.
        /** \param uiID       The ID to give to this Doodad
        *   \param sName      The name of the Doodad
        *   \param sModelName The name of the model to give to this Doodad
        *   \return The created Doodad
        *   \note Returns nullptr if the model couldn't be found or if the
        *         provided name was already in use.
        *   \note This function should only be used while in Editor mode.
        *         The ID you provide shouldn't be already in use by another
        *         object of the scene.
        */
        s_ptr<Doodad> AddDoodad(const s_uint& uiID, const s_str& sName, const s_str& sModelName);

        /// Removes a Doodad from the zone and deletes it.
        /** \param pDoodad The Doodad to remove
        *   \note The provided pointer will then become invalid.
        */
        void DeleteDoodad(s_ptr<Doodad> pDoodad);

        /// Removes a Doodad from the zone and deletes it.
        /** \param sName The name of the Doodad to remove
        */
        void DeleteDoodad(const s_str& sName);

        /// Returns the Doodad associated with the provided name.
        /** \param sName The name of the Doodad
        *   \return The associated Doodad (nullptr if none)
        */
        s_ptr<Doodad> GetDoodadByName(const s_str& sName) const;

        /// Casts a ray to find the terrain position under the mouse.
        /** \param fX The horizontal coordinate of the cursor
        *   \param fY The vertical coordinate of the cursor
        *   \return The position
        */
        Vector       GetTerrainUnderMouse(const s_float& fX, const s_float& fY) const;

        /// Returns the height of the terrain at the provided coordinates.
        /** \param fX The X coordinate
        *   \param fZ The Z coordinate
        *   \return The height of the terrain at the provided coordinates
        */
        s_float      GetTerrainHeight(const s_float& fX, const s_float& fZ) const;

        /// Updates this Zone and its content.
        /** \param fDelta The time elapsed since the last call
        */
        void         Update(const s_float& fDelta);

        /// Hides/Shows chunks for them to be seen by the camera.
        /** \param pCamera The rendering camera
        */
        void         UpdateChunks(s_ptr<Camera> pCamera);

        /// Returns this Zone's name.
        /** \return This Zone's name
        */
        const s_str& GetName();

        /// Returns the list of all models registered to this Zone.
        const s_map<s_str,s_str>& GetModelList() const;

        /// Returns the list of ModelMaterials of this Zone.
        const s_map<s_str, ModelMaterial>& GetMaterialInfoList() const;

        /// Returns the list of Doodads in this Zone.
        const s_map< s_str, s_ptr<Doodad> >& GetDoodadList() const;

        /// Reads this Zone's data from an XML file.
        /** \param sFile The file to read
        */
        void         ParseXMLFile(const s_str& sFile);

        /// Saves this Zone's data into an XML file.
        /** \param sFileName The file to write to
        */
        void         Serialize(const s_str& sFileName) const;

        static const s_str CLASS_NAME;

    private :

        void ParseTerrain_(s_ptr<XML::Block> pTerrainBlock);
        void ParseModels_(s_ptr<XML::Block> pTerrainBlock);
        void ParseDoodads_(s_ptr<XML::Block> pTerrainBlock);

        void SerializeModels_(File& mFile) const;
        void SerializeDoodads_(File& mFile) const;
        void SerializeTerrain_(File& mFile) const;

        s_str sName_;
        Color mAmbientColor_;
        Color mSunColor_;

        s_map< s_str, s_ptr<Light> >         lLightList_;
        s_map< s_uint, s_ptr<TerrainChunk> > lChunkList_;
        s_map< s_str, s_str >                lModelList_;
        s_map< s_str, s_ptr<Doodad> >        lDoodadList_;

        s_map<s_str, ModelMaterial> lMaterialInfoList_;
    };
}

#endif
