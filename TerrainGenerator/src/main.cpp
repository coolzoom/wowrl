#include "generator.h"

#include <CImg.h>

using namespace Frost;

struct TerrainHeader
{
    /// Makes sure we are opening a terrain file
    char sID[2];
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

struct TerrainVertex
{
    /// Vertex height
    float fHeight;
    /// Vertex normal
    float fNormal[3];
    /// Texture coordinates
    float fUVs[2];
    /// Flags
    uchar ucFlags;
};

File mLog("TerrainGenerator.log", File::O);

void LogCout(const s_str& sMessage, const s_bool& bStamps, const s_uint& uiOffset)
{
    std::cout << sMessage.Get() << std::endl;
    mLog.WriteLine(sMessage.Get());
    mLog.Flush();
}

int main(int argc, char** argv)
{
    UtilsManager::GetSingleton()->SetLogFunction(&LogCout);

    if (argc > 1)
    {
        s_str sFile = argv[argc-1];
        Log("Generating terrain from : "+sFile+"...");

        if (!File::Exists(sFile))
        {
            Error("", "File doesn't exist or is not available.");
            return 0;
        }

        cimg_library::CImg<float> image(sFile.c_str());

        float fXDim, fYDim, fZDim;
        uint  uiNPX, uiNPZ;
        uint  uiNCX, uiNCZ;
        uint  uiBlur;

        std::cout << "Please fill in the parameters : " << std::endl;
        std::cout << "    num. X chunk : ";
        std::cin >> uiNCX;
        std::cout << "    num. Z chunk : ";
        std::cin >> uiNCZ;
        std::cout << "    num. X point per chunk : ";
        std::cin >> uiNPX;
        std::cout << "    num. Z point per chunk : ";
        std::cin >> uiNPZ;
        std::cout << "    X dimension : ";
        std::cin >> fXDim;
        std::cout << "    Y dimension : ";
        std::cin >> fYDim;
        std::cout << "    Z dimension : ";
        std::cin >> fZDim;
        std::cout << "    blur ammount : ";
        std::cin >> uiBlur;

        uint uiNTPX = uiNPX*uiNCX;
        uint uiNTPZ = uiNPZ*uiNCZ;

        float fXIncrement = image.width()/uiNTPX;
        float fZIncrement = image.height()/uiNTPZ;

        float fMaxY = -fYDim;
        float fMinY = fYDim;

        float* lHeightList = new float[uiNTPX*uiNTPZ+uiNTPZ];
        float* lVertexList = new float[(uiNTPX*uiNTPZ+uiNTPZ)*3];
        for (uint x = 0; x < uiNTPX; ++x)
        {
            for (uint z = 0; z < uiNTPZ; ++z)
            {
                float y = (image.atXY(
                    x*fXIncrement,
                    z*fZIncrement
                )-128)/255.0f;

                uint i = 1;
                for (uint r = 1; r <= uiBlur; ++r)
                {
                    uint tmax = 3*r + 4;
                    for (uint t = 0; t < tmax; ++t)
                    {
                        y += (image.atXY(
                            (x + r*cos(2*M_PI*t/((float)tmax)))*fXIncrement,
                            (z + r*sin(2*M_PI*t/((float)tmax)))*fZIncrement
                        )-128)/255.0f;
                        ++i;
                    }
                }

                y /= i;

                uint j = x*uiNTPZ+z;

                lHeightList[j] = y;

                lVertexList[j*3 + 0] = fXDim*x/(uiNTPX-1.0f);
                lVertexList[j*3 + 1] = fYDim*y;
                lVertexList[j*3 + 2] = fZDim*z/(uiNTPZ-1.0f);

                if (y < fMinY)
                    fMinY = y;
                if (y > fMaxY)
                    fMaxY = y;
            }
        }

        uint uiIndexCount = (uiNTPX-1)*(uiNTPZ-1)*6;
        uint* lIndices = new uint[uiIndexCount];
        for (uint x = 0; x < uiNTPX-1; ++x)
        {
            for (uint z = 0; z < uiNTPZ-1; ++z)
            {
                uint i = (x*(uiNTPZ-1) + z)*6;

                lIndices[i+0] = x*uiNTPZ+z;
                lIndices[i+1] = x*uiNTPZ+z+1;
                lIndices[i+2] = (x+1)*uiNTPZ+z+1;

                lIndices[i+3] = x*uiNTPZ+z;
                lIndices[i+4] = (x+1)*uiNTPZ+z+1;
                lIndices[i+5] = (x+1)*uiNTPZ+z;
            }
        }

        s_array<Vector> lNArray; lNArray.Resize(uiIndexCount);
        for (uint i = 0; i < uiIndexCount; i += 3)
        {
            ushort i1 = lIndices[i+0];
            ushort i2 = lIndices[i+1];
            ushort i3 = lIndices[i+2];

            Vector mP1 = Vector(lVertexList[i1*3+0], lVertexList[i1*3+1], lVertexList[i1*3+2]);
            Vector mP2 = Vector(lVertexList[i2*3+0], lVertexList[i2*3+1], lVertexList[i2*3+2]);
            Vector mP3 = Vector(lVertexList[i3*3+0], lVertexList[i3*3+1], lVertexList[i3*3+2]);

            Vector mNormal = (mP2 - mP1)^(mP3 - mP1);

            lNArray[i1] += mNormal;
            lNArray[i2] += mNormal;
            lNArray[i3] += mNormal;
        }

        for (uint x = 0; x < uiNTPX; ++x)
        {
            for (uint z = 0; z < uiNTPZ; ++z)
            {
                lNArray[x*uiNTPZ+z].Normalize();
            }
        }

        TerrainHeader mHeader;
        mHeader.sID[0] = 'F';
        mHeader.sID[1] = 'T';

        mHeader.sVersion[0] = '0';
        mHeader.sVersion[1] = '0';
        mHeader.sVersion[2] = '0';
        mHeader.sVersion[3] = '1';

        mHeader.fXSize = fXDim/uiNCX;
        mHeader.fYSize = fYDim;
        mHeader.fZSize = fZDim/uiNCZ;

        Log("\nStatistics :");
        Log("--------------------------");
        Log("    size of a chunk : ("+s_float(mHeader.fXSize)+", "+s_float(mHeader.fZSize)+")");
        Log("    total number of vertex : "+s_uint(uiNTPX*uiNTPZ+uiNTPZ)+" (density : "+s_float((uiNTPX*uiNTPZ+uiNTPZ)/(fXDim*fZDim))+")");
        Log("    highest point : "+s_float(fMaxY*fYDim));
        Log("    lowest point : "+s_float(fMinY*fYDim));
        Log("--------------------------\n");

        mHeader.uiNumXPoint = uiNPX;
        mHeader.uiNumZPoint = uiNPZ;

        mHeader.uiOffPoints = sizeof(TerrainHeader);

        s_int count = 1;
        for (uint x = 0; x < uiNCX; ++x)
        {
            for (uint z = 0; z < uiNCZ; ++z)
            {
                File mFile("Test"+s_str(count, 3)+".ft", File::O, true);

                mFile.Write(mHeader);

                TerrainVertex* lLocalVertexList = new TerrainVertex[uiNPX*uiNPZ+uiNPZ];
                for (uint i = 0; i < uiNPX; ++i)
                {
                    for (uint j = 0; j < uiNPZ; ++j)
                    {
                        uint ip = i + x*(uiNPX-1);
                        uint jp = j + z*(uiNPZ-1);
                        uint uiLocalID = i*uiNPZ+j;
                        uint uiID = ip*uiNTPZ+jp;
                        lLocalVertexList[uiLocalID].fHeight = lHeightList[uiID];
                        lLocalVertexList[uiLocalID].fUVs[0] = ip/0.3f;
                        lLocalVertexList[uiLocalID].fUVs[1] = jp/0.3f;
                        lLocalVertexList[uiLocalID].fNormal[0] = lNArray[uiID].X().Get();
                        lLocalVertexList[uiLocalID].fNormal[1] = lNArray[uiID].Y().Get();
                        lLocalVertexList[uiLocalID].fNormal[2] = lNArray[uiID].Z().Get();
                        lLocalVertexList[uiLocalID].ucFlags = 0;
                    }
                }

                mFile.Write(lLocalVertexList, (uiNPX*uiNPZ+uiNPZ)*sizeof(TerrainVertex));
                Log("Written : "+mFile.GetName());
                ++count;
            }
        }

        Log("\nTerrain generation successful !");

        delete[] lHeightList;
        delete[] lVertexList;
        delete[] lIndices;
    }
    else
    {
        std::cout << "Usage : <TerrainGenerator> <height map>" << std::endl;
    }

    UtilsManager::Delete();
    return 0;
}
