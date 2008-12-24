#include "frost_te_terrain.h"

#include <iostream>
#include <math.h>

using namespace std;
using namespace Frost;

TE::Terrain::Terrain()
{
    uiNumXPoint_ = uiNumZPoint_ = 0u;

    fXSize_ = fYSize_ = fZSize_ = 0.0f;

    lPointList_ = NULL;

    bArrayCreated_ = false;
}


TE::Terrain::Terrain( const uint& uiNumXPoint, const uint& uiNumZPoint, const float& fXSize, const float& fYSize, const float& fZSize )
{
    SetNumPoint(uiNumXPoint, uiNumZPoint);
    SetSize(fXSize, fYSize, fZSize);

    lPointList_ = NULL;
    bArrayCreated_ = false;

    CreateArray_();

    for (uint i = 0u; i < uiNumXPoint*uiNumZPoint; i++)
    {
        lOldHeight_[i] = 0.0f;
    }
}


TE::Terrain::Terrain( const Frost::s_str& sFileName )
{
    uiNumXPoint_ = 0;
    uiNumZPoint_ = 0;
    lPointList_ = NULL;
    bArrayCreated_ = false;

    File mFile(sFileName, FILE_I, true);
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
        Error("Terrain", "Wrong file type : \""+s_str(pHeader->sID)+"\" (\"FTER\" expected).");
        return;
    }

    if ((pHeader->sVersion[0] != '1') ||
        (pHeader->sVersion[1] != '0') ||
        (pHeader->sVersion[2] != '0') ||
        (pHeader->sVersion[3] != '0'))
    {
        Error("Terrain", "Wrong version : \""+s_str(pHeader->sVersion)+"\" (\"1001\" expected).");
        return;
    }

    fXSize_ = pHeader->fXSize;
    fYSize_ = pHeader->fYSize;
    fZSize_ = pHeader->fZSize;

    uiNumXPoint_ = pHeader->uiNumXPoint;
    uiNumZPoint_ = pHeader->uiNumZPoint;

    CreateArray_();

    TerrainPoint* pPoints = (TerrainPoint*)(sBuffer + pHeader->uiOffPoints);
    for (uint i = 0u; i < pHeader->uiNumXPoint*pHeader->uiNumZPoint; i++)
    {
        lPointList_[i] = TerrainPoint(pPoints[i].fHeight, pPoints[i].ucFlags);
        lOldHeight_[i] = pPoints[i].fHeight/fYSize_;
    }

    delete[] sBuffer;
}

TE::Terrain::~Terrain()
{
    delete lPointList_;
}

void TE::Terrain::SetNumXPoint( const uint& uiNumXPoint )
{
    if (uiNumXPoint == 0)
    {
        Warning("Terrain", "Setting number of X points to zero.");
    }

    uiNumXPoint_ = uiNumXPoint;
}

void TE::Terrain::SetNumZPoint( const uint& uiNumZPoint )
{
    if (uiNumZPoint != 0)
    {
        Warning("Terrain", "Setting number of Z points to zero.");
    }

    uiNumZPoint_ = uiNumZPoint;
}

void TE::Terrain::SetNumPoint( const uint& uiNumXPoint, const uint& uiNumZPoint )
{
    if (uiNumXPoint == 0)
    {
        Warning("Terrain", "Setting number of X points to zero.");
    }

    if (uiNumZPoint == 0)
    {
        Warning("Terrain", "Setting number of Z points to zero.");
    }

    uiNumXPoint_ = uiNumXPoint;
    uiNumZPoint_ = uiNumZPoint;
}


const uint& TE::Terrain::GetNumXPoint() const
{
    return uiNumXPoint_;
}

const uint& TE::Terrain::GetNumZPoint() const
{
    return uiNumZPoint_;
}

void TE::Terrain::SetSize( const float& fXSize, const float& fYSize, const float& fZSize )
{
    fXSize_ = fXSize;
    fYSize_ = fYSize;
    fZSize_ = fZSize;
}

const float& TE::Terrain::GetXSize() const
{
    return fXSize_;
}

const float& TE::Terrain::GetYSize() const
{
    return fYSize_;
}

const float& TE::Terrain::GetZSize() const
{
    return fZSize_;
}

void TE::Terrain::SetPointHeight( const uint& uiX, const uint& uiZ, const float& fY )
{
    if (!bArrayCreated_)
        CreateArray_();

    bool bError = false;

    if ( (uiX >= uiNumXPoint_) || (uiZ >= uiNumZPoint_) )
    {
        Error("Terrain", "Trying to get a point that doesn't exist.");
        bError = true;
    }

    if (!bError && bArrayCreated_)
    {
        lOldHeight_[uiX*uiNumZPoint_+uiZ] = fY;
        lPointList_[uiX*uiNumZPoint_+uiZ].fHeight = fY*fYSize_;
    }
}

void TE::Terrain::SetPointFlags( const uint& uiX, const uint& uiZ, const uchar& ucFlags )
{
    if (!bArrayCreated_)
        CreateArray_();

    bool bError = false;

    if ( (uiX >= uiNumXPoint_) || (uiZ >= uiNumZPoint_) )
    {
        Error("Terrain", "Trying to get a point that doesn't exist.");
        bError = true;
    }

    if (!bError && bArrayCreated_)
    {
        lPointList_[uiX*uiNumZPoint_+uiZ].ucFlags = ucFlags;
    }
}

const float& TE::Terrain::GetLastHeight(const uint& uiX, const uint& uiZ)
{
    if (!bArrayCreated_)
        CreateArray_();

    if ( (uiX >= uiNumXPoint_) || (uiZ >= uiNumZPoint_) )
    {
        Error("Terrain", "Trying to get a point that doesn't exist.");
    }

    return lOldHeight_[uiX*uiNumZPoint_+uiZ];
}

float TE::Terrain::GetPointHeight(const uint& uiX, const uint& uiZ)
{
    if (!bArrayCreated_)
        CreateArray_();

    if ( (uiX >= uiNumXPoint_) || (uiZ >= uiNumZPoint_) )
    {
        Error("Terrain", "Trying to get a point that doesn't exist.");
    }

    return lPointList_[uiX*uiNumZPoint_+uiZ].fHeight/fYSize_;
}

float TE::Terrain::FastGetPointHeight_(const uint& uiX, const uint& uiZ)
{
    return lPointList_[uiX*uiNumZPoint_+uiZ].fHeight;
}

float TE::Terrain::CalcPointHeight(const float& fX, const float& fZ)
{
    float fNX = (fX/fXSize_ + 0.5f)*uiNumXPoint_;
    float fNZ = (fZ/fZSize_ + 0.5f)*uiNumZPoint_;
    Log("nx : "+s_float(fNX)+", nz : "+s_float(fNZ));

    float fXMin = floor(fNX);
    float fXMax = ceil(fNX);
    float fZMin = floor(fNZ);
    float fZMax = ceil(fNZ);

    float fH1 = FastGetPointHeight_((uint)fXMin, (uint)fZMin);
    float fH2 = FastGetPointHeight_((uint)fXMin, (uint)fZMax);
    float fMH1 = fH1 + (fH2-fH1)*(fNZ-fZMin)/(fZMax-fZMin);

    float fH3 = FastGetPointHeight_((uint)fXMax, (uint)fZMin);
    float fH4 = FastGetPointHeight_((uint)fXMax, (uint)fZMax);
    float fMH2 = fH3 + (fH4-fH3)*(fNZ-fZMin)/(fZMax-fZMin);

    return fMH1 + (fMH2-fMH1)*(fNX-fXMin)/(fXMax-fXMin);
}

const uchar& TE::Terrain::GetPointFlags(const uint& uiX, const uint& uiZ)
{
    if (!bArrayCreated_)
        CreateArray_();

    if ( (uiX >= uiNumXPoint_) || (uiZ >= uiNumZPoint_) )
    {
        Error("Terrain", "Trying to get a point that doesn't exist.");
    }

    return lPointList_[uiX*uiNumZPoint_+uiZ].ucFlags;
}

bool TE::Terrain::TestPoint(const float& fX, const float& fZ)
{
    return ((fX > -fXSize_/2.0f && fX < fXSize_/2.0f) &&
            (fZ > -fZSize_/2.0f && fZ < fZSize_/2.0f));
}

void TE::Terrain::WriteToFile( const Frost::s_str& sFileName )
{
    if (bArrayCreated_)
    {
        TerrainHeader mHeader;
        mHeader.sID[0] = 'F';
        mHeader.sID[1] = 'T';
        mHeader.sID[2] = 'E';
        mHeader.sID[3] = 'R';
        mHeader.sVersion[0] = '1';
        mHeader.sVersion[1] = '0';
        mHeader.sVersion[2] = '0';
        mHeader.sVersion[3] = '0';
        mHeader.fXSize = fXSize_;
        mHeader.fYSize = fYSize_;
        mHeader.fZSize = fZSize_;
        mHeader.uiNumXPoint = uiNumXPoint_;
        mHeader.uiNumZPoint = uiNumZPoint_;
        mHeader.uiOffPoints = sizeof(TerrainHeader);

        File mFile(sFileName, FILE_O, true);

        mFile.Write((char*)(&mHeader), sizeof(TerrainHeader));
        mFile.Write((char*)lPointList_, sizeof(TerrainPoint)*mHeader.uiNumXPoint*mHeader.uiNumZPoint);
    }
}

void TE::Terrain::CreateArray_()
{
    bool bError = false;
    if (uiNumXPoint_ == 0)
    {
        Error("Terrain", "Can't create point array with zero X points.");
        bError = true;
    }
    if (uiNumZPoint_ == 0)
    {
        Error("Terrain", "Can't create point array with zero Z points.");
        bError = true;
    }

    if (!bError)
    {
        DeleteArray_();

        lPointList_ = new TerrainPoint[uiNumXPoint_*uiNumZPoint_];
        lOldHeight_ = new float[uiNumXPoint_*uiNumZPoint_];
        bArrayCreated_ = true;
    }
}

void TE::Terrain::DeleteArray_()
{
    if (bArrayCreated_)
    {
        delete lPointList_;
        lPointList_ = NULL;
        delete lOldHeight_;
        lOldHeight_ = NULL;
        bArrayCreated_ = false;
    }
}

