/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Zone source               */
/*                                        */
/*                                        */

#include "scene/frost_zone.h"
#include "scene/frost_doodad.h"
#include "scene/frost_terrainchunk.h"

using namespace std;

namespace Frost
{
    s_str SerializeColorRGB( const Color& mColor, const s_str& sTag )
    {
        return "<"+sTag+" r=\""+s_float(mColor.GetR())/255.0f+"\""
                        " g=\""+s_float(mColor.GetG())/255.0f+"\""
                        " b=\""+s_float(mColor.GetB())/255.0f+"\"/>";
    }

    s_str SerializeColorARGB( const Color& mColor, const s_str& sTag )
    {
        return "<"+sTag+" a=\""+s_float(mColor.GetA())/255.0f+"\""
                        " r=\""+s_float(mColor.GetR())/255.0f+"\""
                        " g=\""+s_float(mColor.GetG())/255.0f+"\""
                        " b=\""+s_float(mColor.GetB())/255.0f+"\"/>";
    }

    s_str SerializeVector( const Vector& mVector, const s_str& sTag )
    {
        return "<"+sTag+" x=\""+mVector.X()+"\" y=\""+mVector.Y()+"\" z=\""+mVector.Z()+"\"/>";
    }

    s_str SerializeQuaternion( const Ogre::Quaternion& mQuat, const s_str& sTag )
    {
        return "<"+sTag+" x=\""+s_float(mQuat.x)+"\""
                        " y=\""+s_float(mQuat.y)+"\""
                        " z=\""+s_float(mQuat.z)+"\""
                        " w=\""+s_float(mQuat.w)+"\"/>";
    }

    void Zone::Serialize( const s_str& sFileName ) const
    {
        File mFile(sFileName, File::O);
        mFile.WriteLine("<Zone>");
        mFile.AddTab();

        mFile.WriteLine("");
        mFile.WriteLine(SerializeColorRGB(mAmbientColor_, "AmbientColor"));
        mFile.WriteLine(SerializeColorRGB(mSunColor_, "SunColor"));
        mFile.WriteLine("");

        /*mFile.WriteLine("<Fog>");
            mFile.AddTab();
            mFile.WriteLine("<Color />");
            mFile.WriteLine("<Distance />");
            mFile.RemoveTab();
        mFile.WriteLine("</Fog>");*/

        SerializeModels_(mFile);
        mFile.WriteLine("");
        SerializeDoodads_(mFile);
        mFile.WriteLine("");
        SerializeTerrain_(mFile);
        mFile.WriteLine("");

        /*mFile.WriteLine("<Lights>");
            mFile.AddTab();
            mFile.RemoveTab();
        mFile.WriteLine("</Lights>");*/

        mFile.RemoveTab();
        mFile.WriteLine("</Zone>");
    }

    void Zone::SerializeTerrain_( File& mFile ) const
    {
        mFile.WriteLine("<Terrain>");
        mFile.AddTab();
        mFile.WriteLine("<Chunks>");
        mFile.AddTab();

        s_map< s_uint, s_ptr<TerrainChunk> >::const_iterator iterChunk;
        foreach (iterChunk, lChunkList_)
        {
            s_ptr<TerrainChunk> pChunk = iterChunk->second;
            mFile.WriteLine("<Chunk>");
                mFile.AddTab();

                mFile.WriteLine(SerializeVector(pChunk->GetPosition(), "Position"));

                AxisAlignedBox mBBox = pChunk->GetVisibilityBox();
                if (mBBox.IsInfinite())
                    mFile.WriteLine("<BoundingBox alwaysVisible=\"true\"/>");
                else
                {
                    mFile.WriteLine("<BoundingBox>");
                        mFile.AddTab();
                        mFile.WriteLine(SerializeVector(mBBox.GetMax() - mBBox.GetMin(), "Size"));
                        mFile.RemoveTab();
                    mFile.WriteLine("</BoundingBox>");
                }

                mFile.WriteLine("<Data>");
                    mFile.AddTab();
                    if (pChunk->IsPlanar())
                    {
                        mFile.WriteLine("<Plane xSize=\""+pChunk->GetPlaneXSize()+"\""
                                               "zSize=\""+pChunk->GetPlaneZSize()+"\""
                                               "density=\""+pChunk->GetPlaneDensity()+"\"/>");
                    }
                    else
                    {
                        mFile.WriteLine("<TerrainFile file=\""+pChunk->GetTerrainFile()+"\"/>");
                    }
                    mFile.RemoveTab();
                mFile.WriteLine("</Data>");

                mFile.WriteLine("<Textures>");
                    mFile.AddTab();
                    const TerrainChunk::MaterialInfo& mMatInfo = pChunk->GetMaterialInfo();

                    if (!mMatInfo.sMaskFile.IsEmpty())
                        mFile.WriteLine("<Mask file=\""+mMatInfo.sMaskFile+"\"/>");

                    for (s_uint i; i < mMatInfo.uiLayerCount; ++i)
                    {
                        mFile.WriteLine("<Layer>");
                        mFile.AddTab();

                        const TerrainChunk::MaterialInfo::Layer& mLayer = mMatInfo.lLayerList[i];
                        mFile.WriteLine("<Diffuse file=\""+mLayer.sDiffuseFile+"\">");
                            mFile.AddTab();
                            mFile.WriteLine("<Tilling x=\""+mLayer.fXTilling+"\" z=\""+mLayer.fZTilling+"\"/>");
                            mFile.RemoveTab();
                        mFile.WriteLine("</Diffuse>");

                        if (!mLayer.sSpecularFile.IsEmpty())
                            mFile.WriteLine("<Specular file=\""+mLayer.sSpecularFile+"\"/>");

                        mFile.RemoveTab();
                        mFile.WriteLine("</Layer>");
                    }

                    mFile.RemoveTab();
                mFile.WriteLine("</Textures>");

                mFile.RemoveTab();
            mFile.WriteLine("</Chunk>");
        }

        mFile.RemoveTab();
        mFile.WriteLine("</Chunks>");
        mFile.RemoveTab();
        mFile.WriteLine("</Terrain>");
    }

    void Zone::SerializeDoodads_( File& mFile ) const
    {
        mFile.WriteLine("<Doodads>");
        mFile.AddTab();

        s_map< s_str, s_ptr<Doodad> >::const_iterator iterDoodad;
        foreach (iterDoodad, lDoodadList_)
        {
            s_ptr<Doodad> pDoodad = iterDoodad->second;
            mFile.WriteLine("<Doodad name=\""+pDoodad->GetName()+"\" model=\""+pDoodad->GetModelName()+"\""+
                            (pDoodad->IsShown() ? "" : "hidden=\"true\"")+">");
                mFile.AddTab();
                mFile.WriteLine(SerializeVector(pDoodad->GetPosition(false), "Position"));
                mFile.WriteLine(SerializeVector(pDoodad->GetScale(false), "Scale"));
                mFile.WriteLine(SerializeQuaternion(pDoodad->GetOrientation(false), "Orientation"));
                mFile.RemoveTab();
            mFile.WriteLine("</Doodad>");
        }

        mFile.RemoveTab();
        mFile.WriteLine("</Doodads>");
    }

    void Zone::SerializeModels_( File& mFile ) const
    {
        mFile.WriteLine("<Models>");
        mFile.AddTab();

        s_map<s_str, s_str>::const_iterator iterModel;
        s_map< s_str, s_map<s_int, s_map<s_int, MaterialInfo> > >::const_iterator iterMap = lMaterialInfoList_.Begin();
        foreach (iterModel, lModelList_)
        {
            if (iterModel->first != iterMap->first)
            {
                mFile.WriteLine("<Model name=\""+iterModel->first+"\" file=\""+iterModel->second+"\"/>");
                Warning(CLASS_NAME, "\""+sName_+"\" serialization : Model \""+iterModel->first+"\" doesn't have material info.");
                continue;
            }

            mFile.WriteLine("<Model name=\""+iterModel->first+"\" file=\""+iterModel->second+"\">");
                mFile.AddTab();
                const s_map< s_int, s_map<s_int, MaterialInfo> >& mInfoMap = iterMap->second;
                s_map< s_int, s_map<s_int, MaterialInfo> >::const_iterator iterSubMesh;
                mFile.WriteLine("<Materials>");
                    mFile.AddTab();
                    foreach (iterSubMesh, mInfoMap)
                    {
                        s_map<s_int, MaterialInfo>::const_iterator iterSubEntity;
                        foreach (iterSubEntity, iterSubMesh->second)
                        {
                            const MaterialInfo& mInfo = iterSubEntity->second;
                            mFile.WriteLine("<Material subMeshID=\""+iterSubMesh->first+
                                            "\" subEntityID=\""+iterSubEntity->first+"\">");
                            mFile.AddTab();
                            if (mInfo.bDiffuseColor)
                            {
                                mFile.WriteLine(SerializeColorRGB(mInfo.mDiffuseColor, "DiffuseColor"));
                            }
                            else
                            {
                                mFile.WriteLine("<DiffuseTexture file=\""+mInfo.sDiffuseFile+"\""
                                                +(mInfo.bAlphaReject ? " alphaReject=\"true\"" : "")+"/>");
                            }
                            mFile.RemoveTab();
                            mFile.WriteLine("</Material>");
                        }
                    }
                    mFile.RemoveTab();
                mFile.WriteLine("</Materials>");
                mFile.RemoveTab();
            mFile.WriteLine("</Model>");

            ++iterMap;
        }

        mFile.RemoveTab();
        mFile.WriteLine("</Models>");
    }
}
