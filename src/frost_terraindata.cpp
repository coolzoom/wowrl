/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           TerrainData source           */
/*                                        */
/*                                        */

#include "frost_terraindata.h"

using namespace std;

namespace Frost
{
    const s_str TerrainData::CLASS_NAME = "TerrainData";

    // TODO : Terminer les terrains
    // # définir une specification (données requises)
    // # définir un format de fichier (données stoquées)
    // # créer un editeur de terrain capable de lire/écrire dans ce format
    // # écrire le code de loading de ce format

    TerrainData::TerrainData(const s_str& sFile)
    {
        /*File mFile(sFile, FILE_I, true);
        if (!mFile.IsValid())
        {
            mFile.Close();
            return;
        }

        s_uint uiLength = mFile.GetLength();

        if (uiLength < sizeof(TerrainHeader))
        {
            Error("Terrain", "Corrupted file.");
            mFile.Close();
            return;
        }

        char* sBuffer = new char[uiLength.Get()];
        mFile.Read(sBuffer, uiLength);
        mFile.Close();

        TerrainHeader* pHeader = (TerrainHeader*)sBuffer;

        if ((pHeader->sID[0] != 'F') ||
            (pHeader->sID[1] != 'T') ||
            (pHeader->sID[2] != 'E') ||
            (pHeader->sID[3] != 'R'))
        {
            Error("Terrain", "Wrong file type : \""+s_str(pHeader->sID)+"\".");
            return;
        }

        uiNumXPoint_ = pHeader->uiNumXPoint;
        uiNumYPoint_ = pHeader->uiNumYPoint;

        CreateArray_();

        TerrainPoint* pPoints = (TerrainPoint*)(sBuffer + pHeader->uiOffPoints);
        for (uint i = 0u; i < pHeader->uiNumXPoint*pHeader->uiNumYPoint; i++)
        {
            lPointList_[i] = TerrainPoint(pPoints[i].cHeight, pPoints[i].ucFlags);
            lOldHeight_[i] = pPoints[i].cHeight/127.0f;
        }

        delete[] sBuffer;*/
    }

    TerrainData::~TerrainData()
    {
    }
}
