#include "converter.h"
#include "modeldata.h"
#include "fmloader.h"
#include "ftloader.h"
#include "m2loader.h"
#include "xmlloader.h"
#include "ogrexmlloader.h"

#include <frost_utils_file.h>
#include <frost_utilsmanager.h>

using namespace Frost;

File mLog("ModelConverter.log", File::O);

void LogCout(const s_str& sMessage, const s_bool& bStamps, const s_uint& uiOffset)
{
    std::cout << sMessage.Get() << std::endl;
    mLog.WriteLine(sMessage.Get());
    mLog.Flush();
}

s_bool LoadModel( ModelData& mData, const s_str& sFile )
{
    Log("Loading : "+sFile+"...");
    if (!sFile.Find("."))
    {
        Log("This file has no extension, and the model format cannot be determined. Aborting.");
        return false;
    }
    s_str sExtension = sFile.Cut(".").Back();
    if (sExtension == "m2")
    {
        Log("Extension says the file is an M2 model (from Blizzard's World of Warcraft).\nTrying to load it as such...");
        try
        {
            mData = M2Loader::LoadModelData(sFile);
        }
        catch (Exception e)
        {
            Log(e.GetDescription());
            return false;
        }
    }
    else if (sExtension == "oxml")
    {
        Log("Extension says the file is an OXML model (Ogre's human readable model format).\nTrying to load it as such...");
        try
        {
            mData = OgreXMLLoader::LoadModelData(sFile);
        }
        catch (Exception e)
        {
            Log(e.GetDescription());
            return false;
        }
    }
    else if (sExtension == "xml")
    {
        Log("Extension says the file is an XML model (Frost's human readable model format).\nTrying to load it as such...");
        try
        {
            mData = XMLLoader::LoadModelData(sFile);
        }
        catch (Exception e)
        {
            Log(e.GetDescription());
            return false;
        }
    }
    else if (sExtension == "fm")
    {
        Log("Extension says the file is an FM model (Frost's binary model format).\nTrying to load it as such...");
        try
        {
            mData = FMLoader::LoadModelData(sFile);
        }
        catch (Exception e)
        {
            Log(e.GetDescription());
            return false;
        }
    }
    else if (sExtension == "ft")
    {
        Log("Extension says the file is an FT model (Frost's terrain model format).\nTrying to load it as such...");
        try
        {
            mData = FTLoader::LoadModelData(sFile);
        }
        catch (Exception e)
        {
            Log(e.GetDescription());
            return false;
        }
    }
    else
    {
        Log("ERROR : Unsupported extension : \""+s_str::ToUpper(sExtension)+"\".");
        return false;
    }

    Log("Loading successful.");
    return true;
}

s_bool Convert(const ModelData& mData, const s_str& sOutputFile, const s_str& sFormat)
{
    Log("Converting to "+sOutputFile+"...");

    if (sFormat == "xml")
    {
        XMLLoader::WriteModelData(mData, sOutputFile);
    }
    else if (sFormat == "fm")
    {
        FMLoader::WriteModelData(mData, sOutputFile);
    }
    else
    {
        Log("ERROR : Unsupported output format : \""+s_str::ToUpper(sFormat)+"\"");
        return false;
    }

    Log("Conversion successful !");
    return true;
}

void ShowStatistics(const ModelData& mData)
{
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
}

void ReverseAxis(ModelData& mData, const char& cAxis)
{
    Log("Reversing "+s_char(cAxis)+" axis...");
    s_array<SubMesh>::iterator iterSubMesh;
    foreach (iterSubMesh, mData.lSubMeshList)
    {
        s_array<Vertex>::iterator iterVertex;
        foreach (iterVertex, iterSubMesh->lVertexList)
        {
            switch (cAxis)
            {
                case 'x' :
                    iterVertex->mPos.X() *= (-1.0f);
                    iterVertex->mNormal.X() *= (-1.0f);
                    break;
                case 'y' :
                    iterVertex->mPos.Y() *= (-1.0f);
                    iterVertex->mNormal.Y() *= (-1.0f);
                    break;
                case 'z' :
                    iterVertex->mPos.Z() *= (-1.0f);
                    iterVertex->mNormal.Z() *= (-1.0f);
                    break;
            }
        }
    }

    s_array<Bone>::iterator iterBone;
    foreach (iterBone, mData.lBoneList)
    {
        switch (cAxis)
        {
            case 'x' :
                iterBone->mPos.X() *= (-1.0f);
                break;
            case 'y' :
                iterBone->mPos.Y() *= (-1.0f);
                break;
            case 'z' :
                iterBone->mPos.Z() *= (-1.0f);
                break;
        }

        s_map<s_uint, Vector>::iterator iterTrans;
        foreach (iterTrans, iterBone->lTranslationAnimation)
        {
            switch (cAxis)
            {
                case 'x' :
                    iterTrans->second.X() *= (-1.0f);
                    break;
                case 'y' :
                    iterTrans->second.Y() *= (-1.0f);
                    break;
                case 'z' :
                    iterTrans->second.Z() *= (-1.0f);
                    break;
            }
        }

        s_map<s_uint, Quaternion>::iterator iterRot;
        foreach (iterRot, iterBone->lRotationAnimation)
        {
            switch (cAxis)
            {
                case 'x' :
                    iterRot->second.fY *= (-1.0f);
                    iterRot->second.fZ *= (-1.0f);
                    break;
                case 'y' :
                    iterRot->second.fX *= (-1.0f);
                    iterRot->second.fZ *= (-1.0f);
                    break;
                case 'z' :
                    iterRot->second.fX *= (-1.0f);
                    iterRot->second.fY *= (-1.0f);
                    break;
            }
        }
    }
    Log("Done.");
}

void RotateVector(Vector& mVec, const Quaternion& mQuat)
{
    Vector mAxis(mQuat.fX, mQuat.fY, mQuat.fZ);
    Vector mTemp1 = mAxis ^ mVec;
    Vector mTemp2 = mAxis ^ mTemp1;
    mTemp1 *= mQuat.fW;

    mVec += 2.0f*(mTemp1 + mTemp2);
}

void MulQuat(Quaternion& mQ1, const Quaternion& mQ2)
{
    mQ1 = Quaternion(
        mQ1.fW * mQ2.fW - mQ1.fX * mQ2.fX - mQ1.fY * mQ2.fY - mQ1.fZ * mQ2.fZ,
        mQ1.fW * mQ2.fX + mQ1.fX * mQ2.fW + mQ1.fY * mQ2.fZ - mQ1.fZ * mQ2.fY,
        mQ1.fW * mQ2.fY + mQ1.fY * mQ2.fW + mQ1.fZ * mQ2.fX - mQ1.fX * mQ2.fZ,
        mQ1.fW * mQ2.fZ + mQ1.fZ * mQ2.fW + mQ1.fX * mQ2.fY - mQ1.fY * mQ2.fX
    );
}

void Rotate(ModelData& mData, Vector mAxis, const s_float& fAngle)
{
    Log("Rotating of "+fAngle.GetDeg()+" degrees around "+mAxis+"...");
    Quaternion mQuat(mAxis, fAngle);

    mData.mBoundingBox = AxisAlignedBox();

    s_array<SubMesh>::iterator iterSubMesh;
    foreach (iterSubMesh, mData.lSubMeshList)
    {
        s_array<Vertex>::iterator iterVertex;
        foreach (iterVertex, iterSubMesh->lVertexList)
        {
            RotateVector(iterVertex->mPos, mQuat);

            if (mData.mBoundingBox.IsInfinite())
            {
                mData.mBoundingBox.SetMin(iterVertex->mPos);
                mData.mBoundingBox.SetMax(iterVertex->mPos);
            }
            else
            {
                mData.mBoundingBox.Include(iterVertex->mPos);
            }

            RotateVector(iterVertex->mNormal, mQuat);
        }
    }

    s_array<Bone>::iterator iterBone;
    foreach (iterBone, mData.lBoneList)
    {
        RotateVector(iterBone->mPos, mQuat);

        s_map<s_uint, Vector>::iterator iterTrans;
        foreach (iterTrans, iterBone->lTranslationAnimation)
        {
            RotateVector(iterTrans->second, mQuat);
        }

        s_map<s_uint, Quaternion>::iterator iterRot;
        foreach (iterRot, iterBone->lRotationAnimation)
        {
            MulQuat(iterRot->second, mQuat);
        }
    }
    Log("Done.");
}

void Scale(ModelData& mData, const s_float& fFactor)
{
    Log("Scaling of "+fFactor*100+"%...");

    s_array<SubMesh>::iterator iterSubMesh;
    foreach (iterSubMesh, mData.lSubMeshList)
    {
        s_array<Vertex>::iterator iterVertex;
        foreach (iterVertex, iterSubMesh->lVertexList)
        {
            iterVertex->mPos *= fFactor;
        }
    }

    s_array<Bone>::iterator iterBone;
    foreach (iterBone, mData.lBoneList)
    {
        iterBone->mPos *= fFactor;

        s_map<s_uint, Vector>::iterator iterTrans;
        foreach (iterTrans, iterBone->lTranslationAnimation)
        {
            iterTrans->second *= fFactor;
        }
    }

    mData.mBoundingBox.SetMin(mData.mBoundingBox.GetMin()*fFactor);
    mData.mBoundingBox.SetMax(mData.mBoundingBox.GetMax()*fFactor);

    Log("Done.");
}

void CalculateBoundingBox(ModelData& mData)
{
    Log("Recalculating bounding box...");
    Log("Old : "+mData.mBoundingBox);

    mData.mBoundingBox = AxisAlignedBox();

    s_array<SubMesh>::iterator iterSubMesh;
    foreach (iterSubMesh, mData.lSubMeshList)
    {
        s_array<Vertex>::iterator iterVertex;
        foreach (iterVertex, iterSubMesh->lVertexList)
        {
            if (mData.mBoundingBox.IsInfinite())
            {
                mData.mBoundingBox.SetMin(iterVertex->mPos);
                mData.mBoundingBox.SetMax(iterVertex->mPos);
            }
            else
            {
                mData.mBoundingBox.Include(iterVertex->mPos);
            }
        }
    }

    Log("New : "+mData.mBoundingBox);
}

void NormalizeNormals(ModelData& mData)
{
    Log("Normalizing normals...");

    s_array<SubMesh>::iterator iterSubMesh;
    foreach (iterSubMesh, mData.lSubMeshList)
    {
        s_array<Vertex>::iterator iterVertex;
        foreach (iterVertex, iterSubMesh->lVertexList)
        {
            iterVertex->mNormal.Normalize();
        }
    }

    Log("Done.");
}

int main(int argc, char** argv)
{
    UtilsManager::GetSingleton()->SetLogFunction(&LogCout);

    if (argc > 1)
    {
        s_str sArg1 = argv[1];
        if (sArg1.StartsWith("-"))
        {
            sArg1.EraseFromStart(1);
            if (sArg1 == "e")
            {
                if (argc == 2)
                {
                    std::cout << "No file provided. For help, run : ModelConverter -h" << std::endl;
                    return 0;
                }

                s_str sFile = argv[2];

                std::cout << "Entering editing mode.\n" << std::endl;

                ModelData mData;
                if (!LoadModel(mData, sFile))
                    return 0;

                s_bool bContinue = true;

                while (bContinue)
                {
                    std::cout << "\nAvailable actions :" << std::endl;
                    std::cout << " - 0 : exit" << std::endl;
                    std::cout << " - 1 : reverse axis" << std::endl;
                    std::cout << " - 2 : rotate model" << std::endl;
                    std::cout << " - 3 : scale model" << std::endl;
                    std::cout << " - 4 : reverse normals" << std::endl;
                    std::cout << " - 5 : normalize normals" << std::endl;
                    std::cout << " - 6 : recalculate bounding box" << std::endl;
                    std::cout << "What do you want to do ? ";
                    int iChoice;
                    std::cin >> iChoice;

                    if (iChoice == 0)
                        return 0;

                    if (iChoice == 1)
                    {
                        std::cout << "\nWhich axis do you want to reverse (x, y or z) ? ";
                        char cAxis;
                        std::cin >> cAxis;

                        ReverseAxis(mData, cAxis);
                    }
                    else if (iChoice == 2)
                    {
                        std::cout << "\nAround which axis do you want to rotate (x, y or z) ? ";
                        char cAxis;
                        Vector mAxis;
                        while (mAxis.IsNull())
                        {
                            std::cin >> cAxis;
                            switch (cAxis)
                            {
                                case 'x' :
                                    mAxis = Vector::UNIT_X;
                                    break;
                                case 'y' :
                                    mAxis = Vector::UNIT_Y;
                                    break;
                                case 'z' :
                                    mAxis = Vector::UNIT_Z;
                                    break;
                                default :
                                    std::cout << "Please enter either x, y or z (lowercase) : ";
                            }
                        }

                        std::cout << "\nBy how much (dXX : deg, rXX : rad, XX : ndim) ? ";
                        s_str sAngle;
                        std::cin >> sAngle.Get();
                        s_float fAngle;
                        if (sAngle.StartsWith("d"))
                        {
                            sAngle.EraseFromStart(1);
                            fAngle = s_float(sAngle)/360.0f;
                        }
                        else if (sAngle.StartsWith("r"))
                        {
                            sAngle.EraseFromStart(1);
                            fAngle = s_float(sAngle)/(2.0f*M_PI);
                        }
                        else
                            fAngle = s_float(sAngle);


                        Rotate(mData, mAxis, fAngle);
                    }
                    else if (iChoice == 3)
                    {
                        std::cout << "\nBy how much do you want to scale (1 : identity) ? ";
                        float fScale;
                        std::cin >> fScale;

                        Scale(mData, fScale);
                    }
                    else if (iChoice == 4)
                    {
                        std::cout << "Not yet implemented, sorry." << std::endl;
                        continue;
                    }
                    else if (iChoice == 5)
                    {
                        NormalizeNormals(mData);
                    }
                    else if (iChoice == 6)
                    {
                        CalculateBoundingBox(mData);
                    }
                    else
                    {
                        std::cout << "Unrecognised choice : " << iChoice << std::endl;
                        continue;
                    }

                    bool bSave;
                    std::cout << "\nSave and exit (0 : no, 1 : yes) ? ";
                    std::cin >> bSave;
                    if (bSave)
                    {
                        s_str sExtension = sFile.Cut(".").Back();
                        s_str sNew = sFile;
                        sNew.EraseFromEnd(sExtension.GetLength());

                        s_str sOutput;
                        if (sExtension == "fm")
                            sOutput = "fm";
                        else if (sExtension == "xml")
                            sOutput = "xml";
                        else if (sExtension == "m2")
                            sOutput = "fm";
                        else if (sExtension == "oxml")
                            sOutput = "fm";

                        sNew += sOutput;

                        std::cout << "The suggested format for saving is : "+s_str::ToUpper(sOutput).Get()+"." << std::endl;
                        std::cout << "The edited model will be saved to : "+sNew.Get() << std::endl;
                        std::cout << "If you want to change it, enter the new name or leave blank : ";
                        std::cin.ignore();
                        std::string sTemp;
                        getline(std::cin, sTemp);
                        while (!sTemp.empty())
                        {
                            sNew = sTemp;
                            sOutput = sNew.Cut(".").Back();
                            std::cout << "\nFormat for saving is now : "+s_str::ToUpper(sOutput).Get()+"." << std::endl;
                            std::cout << "The edited model will be saved to : "+sNew.Get() << std::endl;
                            std::cout << "Please confirm by pressing Enter, or type a new name : ";
                            getline(std::cin, sTemp);
                        }

                        Convert(mData, sNew, sOutput);

                        bContinue = false;
                    }
                }
            }
            else
                std::cout << "Unknown command : \"-"+sArg1.Get()+"\". For help, run : ModelConverter -h" << std::endl;
        }
        else
        {
            s_str sFile = sArg1;
            s_str sOutputFile;
            s_str sOutput;
            if (argc >= 3)
            {
                s_str sArg2 = argv[2];
                if (sArg2.StartsWith("-"))
                {
                    sArg2.EraseFromStart(1);
                    if (sArg2 == "o")
                    {
                        if (argc >= 4)
                        {
                            sOutputFile = argv[3];
                            if (sOutputFile.Find("."))
                            {
                                sOutput = sOutputFile.Cut(".").Back();
                                sOutputFile.EraseFromEnd(sOutput.GetLength()+1);
                            }
                            else
                            {
                                // Assume the user just provided the extension
                                sOutput = sOutputFile;
                                sOutputFile = "";
                            }
                        }
                        else
                            std::cout << "Command \"-o\" requires an output file. For help, run : ModelConverter -h" << std::endl;
                    }
                    else
                        std::cout << "Unknown command : \"-"+sArg2.Get()+"\". For help, run : ModelConverter -h" << std::endl;
                }
                else
                    std::cout << "Unknown command : \""+sArg2.Get()+"\". For help, run : ModelConverter -h" << std::endl;
            }

            if (sOutput != "fm" && sOutput != "xml" && sOutput != "")
            {
                Log("This program doesn't support exporting to formats other than \"fm\" and \"xml\". Sorry !");
                return 0;
            }

            ModelData mData;
            if (!LoadModel(mData, sFile))
                return 0;

            s_str sExtension = sFile.Cut(".").Back();
            if (sOutput.IsEmpty())
            {
                if (sExtension == "fm")
                    sOutput = "xml";
                else if (sExtension == "xml")
                    sOutput = "fm";
                else if (sExtension == "m2")
                    sOutput = "fm";
                else if (sExtension == "oxml")
                    sOutput = "fm";
            }

            ShowStatistics(mData);

            s_str sNew;
            if (sOutputFile.IsEmpty())
            {
                sNew = sFile;
                sNew.EraseFromEnd(sExtension.GetLength());
                sNew += sOutput;
            }
            else
            {
                sNew = sOutputFile;
                sNew += "." + sOutput;
            }

            Convert(mData, sNew, sOutput);
        }
    }
    else
    {
        std::cout << "Usage : ModelConverter <file to convert> [output format]" << std::endl;
    }

    UtilsManager::Delete();
    return 0;
}
