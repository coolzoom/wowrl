#include "converter.h"
#include "modeldata.h"
#include "fmloader.h"
#include "m2loader.h"
#include "xmlloader.h"

using namespace Frost;

File mLog("ModelConverter.log", File::O);

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
        Log("Converting : "+sFile+"...");

        s_str sExtension = sFile.Cut(".").Back();
        s_str sOutput;
        ModelData mData;
        if (sExtension == "m2")
        {
            Log("Extension says the file is an M2 model (from Blizzard's World of Warcraft).\nTrying to load it as such...");
            sOutput = "fm";
            try
            {
                mData = M2Loader::LoadModelData(sFile);
            }
            catch (Exception e)
            {
                Log(e.GetDescription());
                return 0;
            }
        }
        else if (sExtension == "xml")
        {
            Log("Extension says the file is an XML model (Frost's human readable model format).\nTrying to load it as such...");
            sOutput = "fm";
            try
            {
                mData = XMLLoader::LoadModelData(sFile);
            }
            catch (Exception e)
            {
                Log(e.GetDescription());
                return 0;
            }
        }
        else if (sExtension == "fm")
        {
            Log("Extension says the file is an FM model (Frost's binary model format).\nTrying to load it as such...");
            sOutput = "xml";
            try
            {
                mData = FMLoader::LoadModelData(sFile);
            }
            catch (Exception e)
            {
                Log(e.GetDescription());
                return 0;
            }
        }
        else
        {
            Log("ERROR : Unsupported extension : \""+s_str::ToUpper(sExtension)+"\".");
            return 0;
        }

        Log("Loading successful.");

        Log("\nStatistics : ");
        Log("------------------------");
        Log("    submeshes : "+mData.lSubMeshList.GetSize());
        s_uint uiVNbr, uiTNbr;
        s_array<SubMesh>::const_iterator iterSub;
        foreach (iterSub, mData.lSubMeshList)
        {
            Log("      mesh id : "+iterSub->uiID);
            Log("        vertices : "+iterSub->lVertexList.GetSize());
            Log("        triangles : "+iterSub->lTriangleList.GetSize());
            uiVNbr += iterSub->lVertexList.GetSize();
            uiTNbr += iterSub->lTriangleList.GetSize();
        }
        Log("    vertices : "+uiVNbr);
        Log("    triangles : "+uiTNbr);
        Log("    bones : "+mData.lBoneList.GetSize());
        Log("------------------------\n");

        Log("Converting to "+s_str::ToUpper(sOutput)+"...");

        s_str sNew = sFile;
        sNew.EraseFromEnd(sExtension.GetLength());
        sNew += sOutput;

        if (sOutput == "xml")
        {
            XMLLoader::WriteModelData(mData, sNew);
        }
        else if (sOutput == "fm")
        {
            FMLoader::WriteModelData(mData, sNew);
        }
        else
        {
            Log("ERROR : Unsupported output format : \""+s_str::ToUpper(sOutput)+"\"");
            return 0;
        }

        Log("Conversion successful !");
    }
    else
    {
        std::cout << "Usage : <ModelConverter> <file to convert>" << std::endl;
    }

    UtilsManager::Delete();
    return 0;
}
