#include "generator.h"

#include <frost_utils_file.h>
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
    float fUVs[4];
    /// Flags
    uchar ucFlags;
};

File mLog("TerrainGenerator.log", File::O);

void LogImpl(const s_str& sMessage, const s_bool& bStamps, const s_uint& uiOffset)
{
    mLog.WriteLine(sMessage.Get());
    mLog.Flush();
}

void LogCout(const s_str& sMessage, const s_bool& bStamps=false, const s_uint& uiOffset=0)
{
    std::cout << sMessage.Get() << std::endl;
    Log(sMessage, bStamps, uiOffset);
}

namespace Frost
{
    s_double GetTime()
    {
        return 0.0;
    }
}

int main(int argc, char** argv)
{
    UtilsManager::GetSingleton()->SetLogFunction(&LogImpl);

    if (argc > 1)
    {
        s_str sFile = argv[argc-1];

        if (!File::Exists(sFile))
        {
            std::cout << "File doesn't exist or is not available." << std::endl;
            return 0;
        }

        if (!sFile.EndsWith(".png"))
        {
            std::cout << "This program can only work with PNG textures." << std::endl;
            return 0;
        }

        uint uiChoice;

        std::cout << "Available features :" << std::endl << std::endl;
        std::cout << "    0 - Exit" << std::endl;
        std::cout << "    1 - Generate .ft files from height map" << std::endl;
        std::cout << "    2 - Split mask file" << std::endl << std::endl;
        std::cout << "What do you want to do ? ";
        std::cin >> uiChoice;

        if (uiChoice == 0)
            return 0;

        if (uiChoice == 1)
        {
            LogCout("Reading height map file : "+sFile+"...");
            cimg_library::CImg<float> image(sFile.c_str());
            LogCout("Done.");

            float fXDim, fYDim, fZDim;
            uint  uiNPX, uiNPZ;
            uint  uiNCX, uiNCZ;
            uint  uiBlur;

            std::cout << std::endl << "Please fill in the following parameters : " << std::endl;
            std::cout << "    num. X chunk : ";
            std::cin >> uiNCX;
            std::cout << "    num. Z chunk : ";
            std::cin >> uiNCZ;
            std::cout << "    num. X point : ";
            std::cin >> uiNPX;
            std::cout << "    num. Z point : ";
            std::cin >> uiNPZ;
            std::cout << "    X dimension : ";
            std::cin >> fXDim;
            std::cout << "    Y dimension : ";
            std::cin >> fYDim;
            std::cout << "    Z dimension : ";
            std::cin >> fZDim;
            std::cout << "    blur ammount (integer) : ";
            std::cin >> uiBlur;

            Log("\nParameters : ");
            Log("--------------------------");
            Log("  XChunk : "+s_uint(uiNCX));
            Log("  ZChunk : "+s_uint(uiNCZ));
            Log("  XPoint : "+s_uint(uiNPX));
            Log("  ZPoint : "+s_uint(uiNPX));
            Log("  XDim   : "+s_float(fXDim));
            Log("  YDim   : "+s_float(fYDim));
            Log("  ZDim   : "+s_float(fZDim));
            Log("  Blur   : "+s_float(uiBlur));
            Log("--------------------------");

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

                    lVertexList[j*3 + 0] = fXDim*x;
                    lVertexList[j*3 + 1] = fYDim*y;
                    lVertexList[j*3 + 2] = fZDim*z;

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

            LogCout("\nStatistics :");
            LogCout("--------------------------");
            LogCout("    size of a chunk : ("+s_float(mHeader.fXSize)+", "+s_float(mHeader.fZSize)+")");
            LogCout("    total number of vertex : "+s_uint(uiNTPX*uiNTPZ+uiNTPZ)+" (density : "+s_float((uiNTPX*uiNTPZ+uiNTPZ)/(fXDim*fZDim))+")");
            LogCout("    highest point : "+s_float(fMaxY*fYDim));
            LogCout("    lowest point : "+s_float(fMinY*fYDim));
            LogCout("--------------------------\n");

            mHeader.uiNumXPoint = uiNPX;
            mHeader.uiNumZPoint = uiNPZ;

            mHeader.uiOffPoints = sizeof(TerrainHeader);

            s_int count = 1;
            for (uint x = 0; x < uiNCX; ++x)
            {
                for (uint z = 0; z < uiNCZ; ++z)
                {
                    File mFile("Test"+s_str::Convert(count, 3)+".ft", File::O, true);

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
                            lLocalVertexList[uiLocalID].fUVs[0] = i/(float)(uiNPX-1);
                            lLocalVertexList[uiLocalID].fUVs[1] = j/(float)(uiNPZ-1);
                            lLocalVertexList[uiLocalID].fUVs[2] = ip/0.3f;
                            lLocalVertexList[uiLocalID].fUVs[3] = jp/0.3f;
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

            LogCout("\nTerrain generation successful !");

            delete[] lHeightList;
            delete[] lVertexList;
            delete[] lIndices;
        }
        else if (uiChoice == 2)
        {
            LogCout("Reading mask file : "+sFile+"...");
            cimg_library::CImg<float> image(sFile.c_str());
            LogCout("Done.");

            uint  uiNCX, uiNCZ;

            std::cout << std::endl << "Please fill in the following parameters : " << std::endl;
            std::cout << "    num. X chunk : ";
            std::cin >> uiNCX;
            std::cout << "    num. Z chunk : ";
            std::cin >> uiNCZ;

            s_uint uiCount = 1;
            cimg_library::CImgList<float> list = image.get_split('x', uiNCX);
            cimg_library::CImgList<float>::iterator iter1, iter2;
            foreach (iter1, list)
            {
                cimg_library::CImgList<float> sublist = iter1->get_split('y', uiNCZ);
                foreach (iter2, sublist)
                {
                    iter2->save_png(("Test"+s_str::Convert(uiCount, 3)+".png").c_str());
                    ++uiCount;
                }
            }
        }
    }
    else
    {
        std::cout << "Usage : <TerrainGenerator> <png file>" << std::endl;
    }

    UtilsManager::Delete();
    return 0;
}
