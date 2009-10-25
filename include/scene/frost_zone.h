/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               Zone header              */
/*                                        */
/*                                        */


#ifndef FROST_ZONE_H
#define FROST_ZONE_H

#include "frost.h"

namespace Frost
{
    /// Contains displayable objects.
    class Zone
    {
    public :

        /// Constructor.
        /** \param sName This Zone's name
        */
        Zone(const s_str& sName);

        /// Destructor.
        ~Zone();

        /// Sets this Zone's default ambient color.
        /** \param mColor The ambient color
        */
        void SetAmbientColor(const Color& mColor);

        /// Returns this Zone's default ambient color.
        /** \return This Zone's default ambient color
        */
        const Color& GetAmbientColor() const;

        /// Sets this Zone's default sun color.
        /** \param mColor The sun color
        */
        void SetSunColor(const Color& mColor);

        /// Returns this Zone's default sun color.
        /** \return This Zone's default sun color
        */
        const Color& GetSunColor() const;

        /// Adds a new chunk to this Zone.
        /** \param pChunk The new chunk
        *   \return The ID of the chunk
        *   \note Automatically called by TerrainChunk.
        */
        s_uint AddChunk(s_ptr<TerrainChunk> pChunk);

        /// Adds a new light to this Zone.
        void AddLight(const s_str& sLightName, s_ptr<Light> pLight);

        /// Hides/Shows chunks for them to be seen by the camera.
        /** \param pCamera The rendering camera
        */
        void UpdateChunks(s_ptr<Camera> pCamera);

        /// Returns this Zone's name.
        /** \return This Zone's name
        */
        const s_str& GetName();

    private :

        s_str sName_;
        Color mAmbientColor_;
        Color mSunColor_;

        s_map< s_str, s_ptr<Light> >         lLightList_;
        s_map< s_uint, s_ptr<TerrainChunk> > lChunkList_;
    };
}

#endif
