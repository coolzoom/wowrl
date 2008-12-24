#ifndef FROST_TE_TERRAIN_H
#define FROST_TE_TERRAIN_H

#include "frost_te.h"

namespace Frost
{
namespace TE
{
    struct TerrainHeader
    {
        /// Makes sure we are opening a terrain file
        char sID[4];
        /// Makes sure the loading code supports this file
        char sVersion[4];
        /// The dimension of the terrain in world unit on the X axis
        float fXSize;
        /// The dimension of the terrain in world unit on the Y axis
        float fYSize;
        /// The dimension of the terrain in world unit on the Y axis
        float fZSize;
        /// The dimension of the terrain in number of points on the X axis
        uint uiNumXPoint;
        /// The dimension of the terrain in number of points on the Z axis
        uint uiNumZPoint;
        /// The offset at which the loader can get the first point
        uint uiOffPoints;
    };

    struct TerrainPoint
    {
        TerrainPoint()
        {
            fHeight = 0.0f;
            ucFlags = 0;
        }

        TerrainPoint(const float& height, const uchar& flags = 0)
        {
            fHeight = height;
            ucFlags = flags;
        }

        /// The height of this point
        float fHeight;
        /// Special flags : 11111111
        /** # : hole : if '1', this point is a hole
        */
        uchar ucFlags;
    };

    class Terrain
    {
    public:

        Terrain();

        Terrain(const uint& uiNumXPoint, const uint& uiNumYPoint, const float& fXSize, const float& fYSize, const float& fZSize);

        Terrain(const Frost::s_str& sFile);

        ~Terrain();

        void SetNumXPoint(const uint& uiNumXPoint);
        void SetNumZPoint(const uint& uiNumZPoint);
        void SetNumPoint(const uint& uiNumXPoint, const uint& uiNumZPoint);

        const uint& GetNumXPoint() const;
        const uint& GetNumZPoint() const;

        void SetSize(const float& fXSize, const float& fYSize, const float& fZSize);

        const float& GetXSize() const;
        const float& GetYSize() const;
        const float& GetZSize() const;

        void SetPointHeight(const uint& uiX, const uint& uiZ, const float& fY);
        void SetPointFlags(const uint& uiX, const uint& uiZ, const uchar& ucFlags);

        const float& GetLastHeight(const uint& uiX, const uint& uiZ);
        float        GetPointHeight(const uint& uiX, const uint& uiZ);
        float        CalcPointHeight(const float& fX, const float& fZ);
        const uchar& GetPointFlags(const uint& uiX, const uint& uiZ);

        bool TestPoint(const float& fX, const float& fZ);

        void WriteToFile(const Frost::s_str& sFileName);

    private :

        void CreateArray_();
        void DeleteArray_();

        float FastGetPointHeight_(const uint& uiX, const uint& uiZ);

        uint uiNumXPoint_;
        uint uiNumZPoint_;

        float fXSize_;
        float fYSize_;
        float fZSize_;

        TerrainPoint* lPointList_;
        float*        lOldHeight_;

        bool bArrayCreated_;

    };
}
}

#endif
