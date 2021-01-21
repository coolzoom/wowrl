/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */

#include "ftloader.h"
#include "ftmodelstructs.h"

#include <frost_utils_file.h>

using namespace std;

namespace Frost
{
    const s_str FTLoader::CLASS_NAME = "FTLoader";

    ModelData FTLoader::LoadModelData( const s_str& sFile )
    {
        ModelData mData;

        File mFile(sFile, File::I, true);
        if (!mFile.IsValid())
        {
            throw Exception(CLASS_NAME, "Model file \""+sFile+"\" couldn't be loaded.");
        }

        if (mFile.GetLength() < sizeof(TerrainHeader))
        {
            throw Exception(CLASS_NAME, "Model file \""+sFile+"\" must be corrupted (incorrect size).");
        }

        TerrainHeader mHeader;
        mFile.Read(mHeader);

        if (mHeader.sID[0] != 'F' || mHeader.sID[1] != 'T')
        {
            throw Exception(CLASS_NAME,
                "\""+sFile+"\" has wrong model type (FT expected, got "+s_str(mHeader.sID)+")."
            );
        }

        s_str sVersion;
        for (int i = 0; i < 4; ++i)
            sVersion.PushBack(mHeader.sVersion[i]);

        if (sVersion > "0001")
        {
            throw Exception(CLASS_NAME,
                "\""+sFile+"\" has wrong model version (0001 expected, got "+sVersion+")."
            );
        }

        uint uiNX = mHeader.uiNumXPoint;
        uint uiNZ = mHeader.uiNumZPoint;
        float fXSize = mHeader.fXSize;
        float fYSize = mHeader.fYSize;
        float fZSize = mHeader.fZSize;
        uint uiIndexCount = (uiNX-1)*(uiNZ-1)*6;

        s_array<TerrainVertex> lPointList; lPointList.Resize(uiNX*uiNZ);
        mFile.Read(lPointList.GetClassicArray(), uiNX*uiNZ*sizeof(TerrainVertex));
        mFile.Close();

        SubMesh mSM;
        mSM.uiID = 0;

        float fNX = uiNX;
        float fNZ = uiNZ;

        float fOffX = (uiNX % 2 == 0) ? 0.5f - 1.0f/(2.0f*uiNX) : 0.5f;
        float fOffZ = (uiNZ % 2 == 0) ? 0.5f - 1.0f/(2.0f*uiNZ) : 0.5f;

        s_float fYMin = s_float::INFPLUS;
        s_float fYMax = s_float::INFMINUS;

        for (uint x = 0; x < uiNX; ++x)
        {
            for (uint z = 0; z < uiNZ; ++z)
            {
                Vertex mV;
                uint j = x*uiNZ + z;

                float fHeight = lPointList[j].fHeight*fYSize;

                if (lPointList[j].ucFlags != 1)
                {
                    if (fYMax < fHeight)
                        fYMax = fHeight;

                    if (fYMin > fHeight)
                        fYMin = fHeight;
                }

                // Position
                mV.mPos    = Vector(fXSize*(x/(fNX-1) - fOffX), fHeight, fZSize*(z/(fNZ-1) - fOffZ));
                mV.mNormal = Vector(lPointList[j].fNormal[0], lPointList[j].fNormal[1],lPointList[j].fNormal[2]);

                // Texture coordinates (constant scale)
                mV.fU = lPointList[j].fUVs[2];
                mV.fV = lPointList[j].fUVs[3];

                mSM.lVertexList.PushBack(mV);
            }
        }

        s_array<ushort> lIndices; lIndices.Resize(uiIndexCount);
        for (uint x = 0; x < uiNX-1; ++x)
        {
            for (uint z = 0; z < uiNZ-1; ++z)
            {
                uint i = (x*(uiNZ-1) + z)*6;

                // Indices for the first triangle
                lIndices[i+0] = x*uiNZ+z;
                lIndices[i+1] = x*uiNZ+z+1;
                lIndices[i+2] = (x+1)*uiNZ+z+1;

                // Indices for the second triangle
                lIndices[i+3] = x*uiNZ+z;
                lIndices[i+4] = (x+1)*uiNZ+z+1;
                lIndices[i+5] = (x+1)*uiNZ+z;
            }
        }

        mSM.lTriangleList.Reserve(uiIndexCount/3);
        for (uint i = 0; i < uiIndexCount; i += 3)
        {
            // Get the three vertices that make the faces
            mSM.lTriangleList.PushBack(Triangle(
                lIndices[i+0], lIndices[i+1], lIndices[i+2]
            ));
        }

        mData.lSubMeshList.PushBack(mSM);

        mData.mBoundingBox = AxisAlignedBox(
            Vector(-fXSize*fOffX, fYMin - 1.0f, -fZSize*fOffZ),
            Vector( fXSize*(1.0f-fOffX), fYMax + 1.0f, fZSize*(1.0f-fOffX))
        );

        return mData;
    }
}
